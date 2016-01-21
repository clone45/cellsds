/*

TODO:
   
   * add more colors and inject into lua
	* reorganize directory
*/

// sox -V sound49.wav -c 1 -2 -r 22050 sound49.raw

// Includes
#include <PA9.h>
#include <fat.h>
#include <math.h>
#include <sys/dir.h>

#include "libdsmi.h"

// include background image
#include "cellbg.h"

#include <cctype>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <map>

using std::string;

using namespace std;

#include "../gfx.cpp"
GFX gfx;

// Grid dimensions
#define GRID_OFFSET_X 7
#define GRID_OFFSET_Y 7
#define GRID_WIDTH 225
#define GRID_HEIGHT 177
#define CELL_WIDTH 14
#define CELL_HEIGHT 11

// modes
#define NUMBER_OF_GLOBAL_MODES 5
#define NUMBER_OF_ENGINE_MODES 1

// - global
#define SEQUENCER_MODE 0
#define SNAPSHOT_MODE 1
#define CLOCK_MODE 2
#define VOLUME_MODE 3
#define MIDI_MODE 4

// - engine specific
#define ENGINE_SELECT_MODE 0

#define GLOBAL_MODE_STYLE 0
#define ENGINE_MODE_STYLE 1

// engine button positions
#define ENGINE_BUTTONS_X 192
#define ENGINE_BUTTONS_HEIGHT 12
#define ENGINE_BUTTON_1_Y 18

#define NUMBER_OF_ENGINES 6


// colors
#define MAX_COLORS 19
#define TRANSPARENT -1
#define BLACK 0
#define DARK_GRAY 1
#define MEDIUM_GRAY 2
#define LIGHT_GRAY 3
#define WHITE 4
#define DARK_BLUE 6
#define RED 7
#define GREEN 8
#define BLUE 9
#define YELLOW 10
#define CYAN 11
#define MAGENTA 12
#define DARK_RED 13
#define LIGHT_RED 14
#define DARK_GREEN 15
#define LIGHT_GREEN 16
#define LIGHT_BLUE 17
#define LIGHT_YELLOW 18
#define DARK_YELLOW 19

// screens
#define TOP_SCREEN 1
#define BOTTOM_SCREEN 0

// clock sources
#define CLOCK_SOURCE_INTERNAL 0
#define CLOCK_SOURCE_MIDI 1

// Sequencer Status
#define STOPPED 0
#define RUNNING 1

// globals
int clock_source = CLOCK_SOURCE_INTERNAL;
int bpm = 90;
int next_step = 0;
int clock_counter = 0;
int sequencer_status = RUNNING;
int active_engine = 0;
int mode_style = GLOBAL_MODE_STYLE;
int global_mode = SEQUENCER_MODE;
int engine_mode = ENGINE_SELECT_MODE;
int block = 1;


u8 message, data1, data2; // variables to hold midi-wifi messages

// grid keeps track of the colors of the cells in the grid.  Requests to draw cells can be 
// ignored if the cells are already set to that color.
int grid[16][16];

string engine_filenames[6];

//UPad uPad;
//UStylus uStylus;

void show_exception(string error_message)
{
	PA_Clear8bitBg(TOP_SCREEN);
	PA_OutputText(TOP_SCREEN,1,1,(char*)error_message.c_str());
   
	while(!Pad.Newpress.Select)
   {
      PA_WaitForVBL();
	}
}

void draw8bitRectEx(int x, int y, int width, int height, bool screen, int colour) 
{
	if ( ((x + width) <= 255) && ((y + height) <= 191) && (x >= 0) && (y >= 0))
	{
		// Draw top line
		u16 lastX = x + width;
		u16 i;	

		for (i = x; i < lastX; i++)
		{
			PA_Put8bitPixel(screen, i, y, colour);
		}

	
		// Calculate DMA copy values - we need to deal with even values only
		u16 dmaX = (x + 1) & ~1;
		s32 dmaWidth = width;
		bool drawRight = false;
	
		// Did we convert to an even x value?
		if (dmaX != x)
		{
			// Reduce the width to compensate
			dmaWidth--;
	
			// Remember that we need to draw an extra pixel
			drawRight = true;
		}
	
		// Pre-emptive bitshift to save calculations
		dmaX = dmaX >> 1;
		dmaWidth = (dmaWidth & 1 ? dmaWidth - 1 : dmaWidth) >> 1;
	
		// Calculate last line to draw
		u16 lastY = y + height;
		lastX = x + width - 1;
	
		// Precalculate line values for loop
		u16* line0 = PA_DrawBg[screen] + dmaX + (y << 7);
		u16* linei = line0 + (1 << 7);
	
		// Loop through all lines
		for (i = y + 1; i < lastY; i++)
		{
	
			// Perform DMA copy
			if (dmaWidth > 0) DMA_Copy(line0, linei, dmaWidth, DMA_16NOW);
	
			// Fill left gap
			if (x & 1) PA_Put8bitPixel(screen, x, i, colour);
	
			// Fill right gap
			if ((width & 1) || (drawRight)) PA_Put8bitPixel(screen, lastX, i, colour);
	
			// Move to next line
			linei += (1 << 7);
		}
	}	
} 


void initialize_pallet()
{
	PA_SetBgPalCol(BOTTOM_SCREEN, BLACK, PA_RGB(0, 0, 0));
	PA_SetBgPalCol(BOTTOM_SCREEN, DARK_GRAY, PA_RGB(7, 7, 7));  
	PA_SetBgPalCol(BOTTOM_SCREEN, MEDIUM_GRAY, PA_RGB(15, 15, 15));
	PA_SetBgPalCol(BOTTOM_SCREEN, LIGHT_GRAY, PA_RGB(25, 25, 25));
	PA_SetBgPalCol(BOTTOM_SCREEN, WHITE, PA_RGB(31, 31, 31));
	
	PA_SetBgPalCol(BOTTOM_SCREEN, DARK_BLUE, PA_RGB(0, 0, 5));
	PA_SetBgPalCol(BOTTOM_SCREEN, BLUE, PA_RGB(0, 0, 15));
	PA_SetBgPalCol(BOTTOM_SCREEN, LIGHT_BLUE, PA_RGB(0, 0, 26));
	
	PA_SetBgPalCol(BOTTOM_SCREEN, DARK_RED, PA_RGB(5, 0, 0));	
	PA_SetBgPalCol(BOTTOM_SCREEN, RED, PA_RGB(15, 0, 0));	
	PA_SetBgPalCol(BOTTOM_SCREEN, LIGHT_RED, PA_RGB(26, 0, 0));	
	
	PA_SetBgPalCol(BOTTOM_SCREEN, DARK_GREEN, PA_RGB(0, 5, 0));
	PA_SetBgPalCol(BOTTOM_SCREEN, GREEN, PA_RGB(0, 15, 0));
	PA_SetBgPalCol(BOTTOM_SCREEN, LIGHT_GREEN, PA_RGB(0, 26, 0));
	
	PA_SetBgPalCol(BOTTOM_SCREEN, YELLOW, PA_RGB(15, 0, 15));
	PA_SetBgPalCol(BOTTOM_SCREEN, CYAN, PA_RGB(0, 15, 15));
	PA_SetBgPalCol(BOTTOM_SCREEN, MAGENTA, PA_RGB(15, 0, 15));
		
   PA_SetBgPalCol(TOP_SCREEN, 1, PA_RGB(5, 5, 5));
   PA_SetBgPalCol(TOP_SCREEN, 2, PA_RGB(30, 5, 10));
   PA_SetBgPalCol(TOP_SCREEN, WHITE, PA_RGB(31, 31, 31));
}

void load_background()
{
	PA_LoadGif(BOTTOM_SCREEN,(void*)cellbg);
	initialize_pallet();
}

void do_interrupt()
{
	next_step = 1; 
}

// Includes.  Order is important, and was painful to get right

#include "../engine_swatches.cpp"
EngineSwatches engine_swatches;

#include "../voice.cpp" 
#include "../file_selector.cpp" 
#include "../clock_controller.cpp" 
#include "../volume_module.cpp" 
#include "../midi_module.cpp" 
#include "../snapshot_controller.cpp"

extern "C" {
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

// arrays to hold table information for notes and midi
int scale[16];
int midi_table[16];

// map to hold midi note-on/note-off information

map<int, int> midi_notes;

#include "../engine.h"

Engine* engines[NUMBER_OF_ENGINES]; 
VolumeModule volume_module;
MidiModule midi_module;

int sequencer_step_16 = 1; // counts from 1 to 16
int sequencer_step_32 = 1; // counts from 1 to 32

extern "C" {
	#include "../lua_callbacks.c"
}

#include "../engine.cpp"

void redraw_engine_screen(int active_engine)
{
   load_background();
	engines[active_engine]->activate();
	engines[active_engine]->redraw();
	engine_swatches.draw();
}



void load_scale()
{
	int i;
	string frequency;
		
	ifstream scale_file ("/cellsDS/scale.txt");
		
	if (scale_file.is_open())
	{
		for (i=0;i<16;i++)
		{
			getline(scale_file,frequency);		
			scale[i] = atoi((const char *)frequency.c_str());
		}			
    	scale_file.close();
  	}
  	else
  	{
  	   PA_OutputText(TOP_SCREEN,1,1,(char*)"Could not open scale.txt file.");
	}  	
}

void load_midi_table()
{
	int i;
	string midi_note;
		
	ifstream midi_table_file ("/cellsDS/midi.txt");
		
	if (midi_table_file.is_open())
	{
		for (i=0;i<16;i++)
		{
			getline(midi_table_file,midi_note);		
			midi_table[i] = atoi((const char *)midi_note.c_str());
		}			
    	midi_table_file.close();
  	}
  	else
  	{
		for (i=0;i<16;i++)
		{
			midi_table[i] = 0;
		}
  	   PA_OutputText(TOP_SCREEN,1,1,(char*)"Could not open midi.txt file.");
	}  	
}

void delete_snapshot(string filename)
{
 	
   string full_filename = "/cellsDS/snapshots/" + filename;
	string lua_script;
 
	remove(full_filename.c_str());
   
   lua_script = full_filename + ".0.lua";
	remove(lua_script.c_str());

   lua_script = full_filename + ".1.lua";
	remove(lua_script.c_str());

   lua_script = full_filename + ".2.lua";
	remove(lua_script.c_str());

   lua_script = full_filename + ".3.lua";
	remove(lua_script.c_str());

   lua_script = full_filename + ".4.lua";
	remove(lua_script.c_str());

   lua_script = full_filename + ".5.lua";
	remove(lua_script.c_str());
   

	PA_Clear8bitBg(BOTTOM_SCREEN);
	PA_8bitText(BOTTOM_SCREEN, 20, 80, 255, 40,(char*)"Removed:",1,3,0,130);
	PA_8bitText(BOTTOM_SCREEN, 20, 95, 255, 40,(char*)full_filename.c_str(),1,3,0,130);

}

void save_snapshot(string filename)
{
   int engine_id;
   char buffer[200];
   
   string full_filename = "/cellsDS/snapshots/" + filename;
   string bpm_string;
   
	// Open file for output
	ofstream snapshot_file (full_filename.c_str());

	PA_Clear8bitBg(BOTTOM_SCREEN);
	PA_8bitText(BOTTOM_SCREEN, 1, 20, 255, 40,(char*)"Saving...",WHITE,3,0,130);

	sprintf(buffer,"Saving %s...",(char *)full_filename.c_str());
	PA_8bitText(BOTTOM_SCREEN, 1, 50, 255, 40,buffer,LIGHT_GRAY,0,0,130);	   

	// store engine filenames
	for (engine_id = 0; engine_id < NUMBER_OF_ENGINES; engine_id++)
	{	
	   snapshot_file << engine_filenames[engine_id] << "\n";
	}
	
	// store engine basenames
	for (engine_id = 0; engine_id < NUMBER_OF_ENGINES; engine_id++)
	{	
	   snapshot_file << engines[engine_id]->basename << "\n";;
	}
	
	// store BPM
	snapshot_file << bpm << "\n";

	// store volumes
	for (engine_id = 0; engine_id < NUMBER_OF_ENGINES; engine_id++)
	{		   
		snapshot_file << volume_module.get_volume(engine_id) << "\n";
	}	

	snapshot_file.close();
			
	// Issue a "save" command to each of the engines.  They're in charge of saving their own data.	
	for (engine_id = 0; engine_id < NUMBER_OF_ENGINES; engine_id++)
	{		   
		engines[engine_id]->save(filename.c_str());
	}
	
	PA_8bitText(BOTTOM_SCREEN, 1, 180, 255, 40,(char*)"Done.",WHITE,3,0,130);

}

void load_snapshot(string filename)
{
   int engine_id;
	char buffer[100];
	
	string tmp;
	string full_filename = "/cellsDS/snapshots/" + filename;
      
	PA_Clear8bitBg(BOTTOM_SCREEN);
	PA_8bitText(BOTTOM_SCREEN, 20, 80, 255, 40,(char*)"Loading...",1,3,0,130);	   

   ifstream snapshot_file (full_filename.c_str());
			
	// load engine filenames			
	for (engine_id=0; engine_id < NUMBER_OF_ENGINES; engine_id++)
	{	   
		getline(snapshot_file,engine_filenames[engine_id]);
	}		
	
	// load engine basenames			
	for (engine_id=0; engine_id < NUMBER_OF_ENGINES; engine_id++)
	{	   
		getline(snapshot_file,engines[engine_id]->basename);
	}	
	
		
	// load beats per minute			
	getline(snapshot_file,tmp);
	bpm = atoi((const char *)tmp.c_str());
	TIMER_DATA(1) = (int)(-(0x2000000>>10)/ (float)((bpm * 4.0) / (float)60));

	// load volumes
	for (engine_id = 0; engine_id < NUMBER_OF_ENGINES; engine_id++)
	{		   
		getline(snapshot_file,tmp);
		volume_module.set_volume(engine_id,atoi((const char *)tmp.c_str()));
	}	

	snapshot_file.close();
	

	// Destroy the old engines and create new ones.  This is done
	// to wipe out existing Lua namespaces
	for (engine_id=0; engine_id < NUMBER_OF_ENGINES; engine_id++)
	{
		PA_Clear8bitBg(BOTTOM_SCREEN);
		sprintf(buffer,"Loading engine #%d...",engine_id);
		
		gfx.print(BOTTOM_SCREEN, 20, 80, buffer, WHITE, FONT_SIZE_LARGE);

		delete(engines[engine_id]);
		engines[engine_id] = new Engine(engine_id);
		engines[engine_id]->active = 0;
		
		// Load up the lua script for the engine
		engines[engine_id]->load_lua_script(engine_filenames[engine_id]);

		// Have each engine load up its saved state
		engines[engine_id]->load(filename);
	}
			
	global_mode = SEQUENCER_MODE; 
	
	redraw_engine_screen(active_engine);
}

void loading_message(string str)
{
  	gfx.print(BOTTOM_SCREEN, 20, 80,"                                   ", WHITE, FONT_SIZE_SMALL); 
	gfx.print(BOTTOM_SCREEN, 20, 80,str.c_str(), WHITE, FONT_SIZE_SMALL);
}


FileSelector file_selector;
ClockController clock_controller;


// Function: main()
int main(int argc, char ** argv)
{
 	string engine_filename;
 	string selected_engine;
	int engine_id = 0;
	int channel = 0;
	
	global_mode = SEQUENCER_MODE;
	engine_mode = ENGINE_SELECT_MODE;
	
	int snapshot_input;

	// Initialize global grid array
	
	for(int column=0; column<16; column++)
	{
	   for(int row=0; row<16; row++)
		{
		   grid[column][row] = WHITE;
		}
	}	
	
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL

	// Setup the internal interrupt
	if (clock_source == CLOCK_SOURCE_INTERNAL)
	{
		TIMER_CR(1) = TIMER_ENABLE | TIMER_DIV_1024 | TIMER_IRQ_REQ;
		TIMER_DATA(1) = (int)(-(0x2000000>>10)/ (float)((bpm * 4) / (float)60));
		irqSet(IRQ_TIMER1, do_interrupt);
		PA_VBLCounterStart(1);
	}

	// Initialize and load background image
	PA_Init8bitBg(BOTTOM_SCREEN, 3);
   PA_Init8bitBg(TOP_SCREEN, 3);

	// text initialization
	PA_InitText(1, 0);
	PA_InitText(0, 0);

	PA_SetTextCol(1, 31, 31, 31);
	PA_SetTextCol(0, 0, 0, 31);
	
	loading_message("initializing sounds...");
   
	PA_InitASLibForSounds(AS_MODE_SURROUND | AS_MODE_16CH);

	initialize_pallet();
	
	
	// <--- load_background was here

	loading_message("initializing fat libraries...");
	
	fatInitDefault();

	loading_message("loading scale.txt...");

	load_scale();
	
	loading_message("loading midi.txt...");
	
	load_midi_table();
	
	SnapshotController snapshot_controller;

	loading_message("loading default engines...");

	// Instantiate all engines
	for (engine_id = 0; engine_id < NUMBER_OF_ENGINES; engine_id++)
	{
		engines[engine_id] = new Engine(engine_id);	
		engines[engine_id]->load_lua_script("/cellsDS/scripts/step_sequencer.lua");
		engines[engine_id]->active = 0;
		_load_sound(engine_id,1); // load up first sound as a default
	}	
	
	for (engine_id = 0; engine_id < NUMBER_OF_ENGINES; engine_id++)
	{
		engine_filenames[engine_id].assign("/cellsDS/scripts/step_sequencer.lua");
		engines[engine_id]->set_basename("step_sequencer.lua");
	}		

	// Highlight first engine button by default
	
	engines[0]->activate();

	sequencer_step_16 = 0;
	sequencer_step_32 = 0;

	map<int, int>::iterator note_iterator, temp_iterator;

	loading_message("initializing snapshot list...");
	
	file_selector.initialize((char*)"/cellsDS/scripts", 0);

	PA_Clear8bitBg(BOTTOM_SCREEN);

	load_background();
	engine_swatches.draw();
	                
	while(1)
	{
	   
	   // Check for engine selection
		
		if ((mode_style == GLOBAL_MODE_STYLE) && (global_mode == SEQUENCER_MODE) && engine_swatches.poll())
		{
			PA_Clear8bitBg(TOP_SCREEN);
			
		   engines[active_engine]->deactivate();
	      engines[engine_swatches.selected_engine]->activate();
			engines[engine_swatches.selected_engine]->redraw();
			active_engine = engine_swatches.selected_engine;			
		}				   
		
	   
		// Check for next sequencer step
	   
	   if (next_step && (sequencer_status == RUNNING))
	   {
	      next_step = 0;

			// If midi is active, look at each note in the midi_notes map.  Decrement the durations of
			// each note in the map.  For any note with a duration of 0, remove it from the map and send
			// a midi-note-off message
			
			if (midi_module.get_status() == MIDI_STATUS_CONNECTED)
			{
			    for(note_iterator = midi_notes.begin(); note_iterator != midi_notes.end();)
			    {
			       if (note_iterator->second > 0)
			       {
						note_iterator->second--; // it->second is the duration
			   	
				   	if (note_iterator->second == 0) 
				   	{
							channel = 0;
							channel = note_iterator->first & 7;

							midi_module.send_note_off(channel, midi_table[note_iterator->first >> 3]);	

							midi_notes.erase(note_iterator++);

							// temp_iterator = note_iterator;
							// note_iterator--;
								
							// to-do: remove the entry from the map (make sure that this is safe first) 
							// midi_notes.erase(temp_iterator);
				 		}
				 		else
				 		{
				 		   ++note_iterator;
						} 		
					}
					else
					{
					   ++note_iterator;
					}					 	
				}
			}			

			// Maintain internal 16 beat counter to help syncronize lua engines that want to stay in sync
	      sequencer_step_16 ++;
	      if (sequencer_step_16 > 16) sequencer_step_16 = 1;

	      sequencer_step_32 ++;
	      if (sequencer_step_32 > 32) sequencer_step_32 = 1;
	      
	      // Call the clock method of the engine(s)
			for (engine_id = 0; engine_id < NUMBER_OF_ENGINES; engine_id++)
			{
	      	engines[engine_id]->clock();
			}			
		} 				    

		if (sequencer_status == RUNNING)
		{
			for (engine_id = 0; engine_id < NUMBER_OF_ENGINES; engine_id++)
			{   
				engines[engine_id]->run();
			}	
		}		
						
		if (Pad.Newpress.Select)
		{
		   if (mode_style == GLOBAL_MODE_STYLE)
		   {
		      mode_style = ENGINE_MODE_STYLE;
				engine_mode = ENGINE_SELECT_MODE;
			}
			else
			{
		      mode_style = GLOBAL_MODE_STYLE;
		      global_mode = SEQUENCER_MODE;			  
			}
		}					

		// GLOBAL MODE  **

		if (mode_style == GLOBAL_MODE_STYLE) 
	   {
			if (Pad.Newpress.R)
		   {
	      	global_mode++;
	      	if (global_mode >= NUMBER_OF_GLOBAL_MODES) global_mode = 0;
			}
				   
		   if (Pad.Newpress.L)
		   {
	      	global_mode--;
	      	if (global_mode < 0) global_mode = (NUMBER_OF_GLOBAL_MODES - 1);
			}
		
			// Check for select button press
			if (Pad.Newpress.L || Pad.Newpress.R || Pad.Newpress.Select)			      
		   {
	
				if (snapshot_controller.snapshot_mode == SNAPSHOT_NEW_KEYBOARD_ENTRY)
				{
					PA_KeyboardOut();
					snapshot_controller.snapshot_mode = 0;
				}
			   
			   switch(global_mode)
			   {
			      case CLOCK_MODE:
			         engines[active_engine]->active = 0;
						clock_controller.draw();
						break;
			         
			      case SEQUENCER_MODE:
			         redraw_engine_screen(active_engine);
			      break;
	
			      case SNAPSHOT_MODE:
			         engines[active_engine]->active = 0;
						snapshot_controller.draw();				
					break;
			      
					case VOLUME_MODE:
			         engines[active_engine]->active = 0;
						volume_module.draw();				
					break;
					
					case MIDI_MODE:
			         engines[active_engine]->active = 0;
						midi_module.draw();				
					break;
				}			
			}
			
			if (global_mode == CLOCK_MODE) clock_controller.handle_input();		
			if (global_mode == VOLUME_MODE) volume_module.handle_input();		
			if (global_mode == MIDI_MODE) midi_module.handle_input();		
			
			if (global_mode == SNAPSHOT_MODE)
			{
			   snapshot_input = snapshot_controller.handle_input();
			   
			   if (snapshot_input == SNAPSHOT_ACTION_SAVE)save_snapshot(snapshot_controller.action_filename);
			   if (snapshot_input == SNAPSHOT_ACTION_LOAD) load_snapshot(snapshot_controller.action_filename);
			   if (snapshot_input == SNAPSHOT_ACTION_DELETE) delete_snapshot(snapshot_controller.action_filename);
			   
			   if (snapshot_input != SNAPSHOT_ACTION_NONE) snapshot_controller.get_directory_listing();
			}
		}		



		// ENGINE MODE  **
		
		if (mode_style == ENGINE_MODE_STYLE)
	   {

			if (Pad.Newpress.R)
		   {
	      	engine_mode++;
	      	if (engine_mode >= NUMBER_OF_ENGINE_MODES) engine_mode = 0;
			}
				   
		   if (Pad.Newpress.L)
		   {
	      	engine_mode--;
	      	if (engine_mode < 0) engine_mode = (NUMBER_OF_ENGINE_MODES - 1);
			}
		
			if (Pad.Newpress.L || Pad.Newpress.R || Pad.Newpress.Select)			      
		   {	
			   
			   switch(engine_mode)
			   {
			      case ENGINE_SELECT_MODE:
			         engines[active_engine]->active = 0;
			         file_selector.center_on_filename(engines[active_engine]->basename);
						file_selector.draw();				
					break;
				}			
			}
		
			if (engine_mode == ENGINE_SELECT_MODE) 
			{
	
				if (engine_swatches.poll())
				{
					active_engine = engine_swatches.selected_engine;
		         file_selector.center_on_filename(engines[active_engine]->basename);
					file_selector.redraw();
				}
	
			   selected_engine.assign(file_selector.handle_input());
			   
			   if (! selected_engine.empty())
			   {
					engine_filename = "/cellsDS/scripts/" + selected_engine;
					
					delete(engines[active_engine]);
					engines[active_engine] = new Engine(active_engine);
					engines[active_engine]->load_lua_script(engine_filename);
					
					// Set engine basename, such as 'bouncer.lua'.  This basename
					// is used by the fileselector to default to the correct .lua file when being drawn
					engines[active_engine]->set_basename(selected_engine);
					
					// Store engine filename for later when we wish to save the setup
					engine_filenames[active_engine] = engine_filename;
					
					_load_sound(active_engine,1); // load up first sound as a default		 
					    
					// redraw engine 
					PA_Clear8bitBg(TOP_SCREEN);
	         	load_background();
	         	engines[active_engine]->activate();
	         	engines[active_engine]->redraw();
	        		engine_swatches.draw();
	         	
	         	// Exit back to the sequencer screen
	         	mode_style = GLOBAL_MODE_STYLE;
	         	global_mode = SEQUENCER_MODE;
				}   
			}			
		}		
		
		// Sequencer Start/Stop
			
		if (Pad.Newpress.Start)
		{
		   if (sequencer_status != RUNNING)
		   {
				sequencer_status = RUNNING;		      
				PA_VBLCounterStart(1);
			}  
			else
			{
				sequencer_status = STOPPED;
				PA_VBLCounterPause(1);  
				sequencer_step_16 = 0;
				sequencer_step_32 = 0;
			}			 
		}
		
		PA_WaitForVBL();
	}

}

