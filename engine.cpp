// _call_lua(function_name) is syntatic sugar for keeping the code terse
void Engine::_call_lua(string function_name)
{   
	lua_getglobal(this->L, function_name.c_str()); // push the function name on to the stack

	// this->display_stack_length();

	
	// If the function name on the stack is an actual function in the LUA script, then call it
	if (lua_isfunction(this->L,-1))
	{	
		if (lua_pcall(this->L, 0, 0, 0) != 0) // Call the function with no parameters
		{
		   string additional_info = "In call to function: " + function_name;
		   this->display_lua_error(additional_info);
		}		
	}
	else
	{
	   lua_pop(this->L,1); // pop the function name off of the stack
	}			
}

void Engine::_set_lua_global(const char* variable_name, int value)
{
	lua_pushnumber(this->L, value);	
	lua_setglobal(this->L, variable_name);
}

Engine::Engine(int engine_id)
{
	// Set engine id based on the integer passed in through the constructor
	this->engine_id = engine_id;

	// initialize Lua state
	this->L = lua_open();
	
	// load Lua base libraries
	luaL_openlibs(this->L);

	// register callback functions
	lua_register(this->L, "set_block", set_block);
	lua_register(this->L, "set_global_variable", set_global_variable);
	lua_register(this->L, "send_midi_note_on", send_midi_note_on);
	lua_register(this->L, "send_midi_note_off", send_midi_note_off);
	lua_register(this->L, "send_midi_message", send_midi_message);
	lua_register(this->L, "set_scale_frequency", set_scale_frequency);
	lua_register(this->L, "get_scale_frequency", get_scale_frequency);
			
	switch(engine_id)
	{
	   case 0: {
			lua_register(this->L, "set_cell", set_cell0);	
			lua_register(this->L, "set_all_cells", set_all_cells0);	
			lua_register(this->L, "set_column", set_column0);
			lua_register(this->L, "set_row", set_row0);	
			lua_register(this->L, "play_note", play_note0);
			lua_register(this->L, "play_frequency", play_frequency0);	
			lua_register(this->L, "load_sound", load_sound0);
			lua_register(this->L, "set_pan", set_pan0);
			lua_register(this->L, "display_text", display_text0);
			lua_register(this->L, "midi_note", midi_note0);
			lua_register(this->L, "clear_top_screen", clear_top_screen0);
		}
		break;

	   case 1: {
			lua_register(this->L, "set_cell", set_cell1);	
			lua_register(this->L, "set_all_cells", set_all_cells1);	
			lua_register(this->L, "set_column", set_column1);
			lua_register(this->L, "set_row", set_row1);	
			lua_register(this->L, "play_note", play_note1);
			lua_register(this->L, "play_frequency", play_frequency1);	
			lua_register(this->L, "load_sound", load_sound1);
			lua_register(this->L, "set_pan", set_pan1);
			lua_register(this->L, "display_text", display_text1);
			lua_register(this->L, "midi_note", midi_note1);
			lua_register(this->L, "clear_top_screen", clear_top_screen1);
		}
		break;

	   case 2: {
			lua_register(this->L, "set_cell", set_cell2);	
			lua_register(this->L, "set_all_cells", set_all_cells2);	
			lua_register(this->L, "set_column", set_column2);
			lua_register(this->L, "set_row", set_row2);	
			lua_register(this->L, "play_note", play_note2);
			lua_register(this->L, "play_frequency", play_frequency2);	
			lua_register(this->L, "load_sound", load_sound2);
			lua_register(this->L, "set_pan", set_pan2);
			lua_register(this->L, "display_text", display_text2);
			lua_register(this->L, "midi_note", midi_note2);
			lua_register(this->L, "clear_top_screen", clear_top_screen2);
		}
		break;

	   case 3: {
			lua_register(this->L, "set_cell", set_cell3);	
			lua_register(this->L, "set_all_cells", set_all_cells3);	
			lua_register(this->L, "set_column", set_column3);
			lua_register(this->L, "set_row", set_row3);	
			lua_register(this->L, "play_note", play_note3);	
			lua_register(this->L, "play_frequency", play_frequency3);
			lua_register(this->L, "load_sound", load_sound3);
			lua_register(this->L, "set_pan", set_pan3);
			lua_register(this->L, "display_text", display_text3);
			lua_register(this->L, "midi_note", midi_note3);
			lua_register(this->L, "clear_top_screen", clear_top_screen3);
		}
		break;

	   case 4: {
			lua_register(this->L, "set_cell", set_cell4);	
			lua_register(this->L, "set_all_cells", set_all_cells4);	
			lua_register(this->L, "set_column", set_column4);
			lua_register(this->L, "set_row", set_row4);	
			lua_register(this->L, "play_note", play_note4);
			lua_register(this->L, "play_frequency", play_frequency4);	
			lua_register(this->L, "load_sound", load_sound4);
			lua_register(this->L, "set_pan", set_pan4);
			lua_register(this->L, "display_text", display_text4);
			lua_register(this->L, "midi_note", midi_note4);
			lua_register(this->L, "clear_top_screen", clear_top_screen4);
		}
		break;	
		
	   case 5: {
			lua_register(this->L, "set_cell", set_cell5);	
			lua_register(this->L, "set_all_cells", set_all_cells5);	
			lua_register(this->L, "set_column", set_column5);
			lua_register(this->L, "set_row", set_row5);	
			lua_register(this->L, "play_note", play_note5);
			lua_register(this->L, "play_frequency", play_frequency5);	
			lua_register(this->L, "load_sound", load_sound5);
			lua_register(this->L, "set_pan", set_pan5);
			lua_register(this->L, "display_text", display_text5);
			lua_register(this->L, "midi_note", midi_note5);
			lua_register(this->L, "clear_top_screen", clear_top_screen5);
		}
		break;
	}				
}

Engine::~Engine()
{
   lua_close(this->L);
}

void Engine::set_basename(string basename)
{
   this->basename = basename;
}

void Engine::activate()
{
   _set_lua_global("ACTIVE",1);
   this->active = 1;
}

void Engine::deactivate()
{
	_set_lua_global("ACTIVE",0);
   this->active = 0;
}


void Engine::load_lua_script(string filename)
{
   // char additional_info[100];

	string additional_info;

	this->loaded_script = filename;

   // The only thing inside of the lua file should be function definitions.  This piece of code
   // loads up the .lua file, parses it, and that's it.  The actual lua code is run later on by
   // calls to the functions.
   
	if (luaL_dofile(this->L, filename.c_str()) != 0)
	{
	   // sprintf(additional_info,"While parsing %s", filename.c_str()); 
	   additional_info = "While parsing " + filename;
	   this->display_lua_error(additional_info);
	}	

	// Set the sequencer step.  Notice that this might be any value between 1 and 16.  The goal
	// is to sync up the sequencer with all other sequencers currently running.
	_set_lua_global("SEQUENCER_STEP_16",sequencer_step_16);
	_set_lua_global("SEQUENCER_STEP_32",sequencer_step_32);
	
	// Register color constants in lua
	_set_lua_global("BLANK",BLACK);
	_set_lua_global("MEDIUM_GRAY",MEDIUM_GRAY);
	_set_lua_global("LIGHT_GRAY",LIGHT_GRAY);
	_set_lua_global("WHITE",WHITE);
	
	_set_lua_global("LIGHT_BLUE",LIGHT_BLUE);
	_set_lua_global("BLUE",BLUE);
	_set_lua_global("DARK_BLUE",DARK_BLUE);

	_set_lua_global("LIGHT_RED",LIGHT_RED);
	_set_lua_global("RED",RED);
	_set_lua_global("DARK_RED",DARK_RED);
	
	_set_lua_global("LIGHT_GREEN",LIGHT_GREEN);
	_set_lua_global("GREEN",GREEN);
	_set_lua_global("DARK_GREEN",DARK_GREEN);

	_set_lua_global("YELLOW",YELLOW);
	_set_lua_global("CYAN",CYAN);
	_set_lua_global("MAGENTA",MAGENTA);

	// set engine_id as a global variable in lua
	_set_lua_global("ENGINE_ID",this->engine_id);
	
	// set block
	_set_lua_global("BLOCK",block);
	
	// set default stylus positions
	_set_lua_global("X",1);
	_set_lua_global("Y",1);

	_call_lua("on_load");
}

void Engine::run()
{
   int grid_x;
   int grid_y;
   
   if (this->active)
   {
	   // inject button, pad, and stylus global variables into lua
	
		_set_lua_global("PAD_HELD_UP",Pad.Held.Up);
		_set_lua_global("PAD_HELD_DOWN",Pad.Held.Down);
		_set_lua_global("PAD_HELD_LEFT",Pad.Held.Left);
		_set_lua_global("PAD_HELD_RIGHT",Pad.Held.Right);
		_set_lua_global("PAD_HELD_X",Pad.Held.X);
		_set_lua_global("PAD_HELD_Y",Pad.Held.Y);
		_set_lua_global("PAD_HELD_A",Pad.Held.A);
		_set_lua_global("PAD_HELD_B",Pad.Held.B);
		_set_lua_global("STYLUS_HELD",Stylus.Held);
		
		// user definable actions
	   	   
	   if (Pad.Newpress.Left) _call_lua("pad_newpress_left");
	   if (Pad.Newpress.Right) _call_lua("pad_newpress_right");
	   if (Pad.Newpress.Up) _call_lua("pad_newpress_up");
	   if (Pad.Newpress.Down) _call_lua("pad_newpress_down");

	   if (Pad.Newpress.A) _call_lua("pad_newpress_a");	
	   if (Pad.Newpress.B) _call_lua("pad_newpress_b");
	   if (Pad.Newpress.X) _call_lua("pad_newpress_x");
	   if (Pad.Newpress.Y) _call_lua("pad_newpress_y");

		if (Pad.Released.Left) _call_lua("pad_released_left");
		if (Pad.Released.Right) _call_lua("pad_released_right");
		if (Pad.Released.Up) _call_lua("pad_released_up");
		if (Pad.Released.Down) _call_lua("pad_released_down");

		if (Pad.Released.A) _call_lua("pad_released_a");
		if (Pad.Released.B) _call_lua("pad_released_b");
		if (Pad.Released.X) _call_lua("pad_released_x");
		if (Pad.Released.Y) _call_lua("pad_released_y");
	
	
		if (Stylus.Newpress)
		{
		   if (PA_StylusInZone(GRID_OFFSET_X, GRID_OFFSET_Y, GRID_OFFSET_X + GRID_WIDTH, GRID_OFFSET_Y + GRID_HEIGHT))
			{
			   // Compute X,Y coordinates from 1 to 16
			   grid_x = ((Stylus.X - (GRID_OFFSET_X)) / CELL_WIDTH) + 1;
				grid_y = ((Stylus.Y - (GRID_OFFSET_Y)) / CELL_HEIGHT) + 1; 

				if ((grid_x >= 1) && (grid_x <= 16) && (grid_y >= 1) && (grid_y <= 16))
				{ 	
					// Inject X,Y as global variables into Lua
					_set_lua_global("X",grid_x);
					_set_lua_global("Y",grid_y);
		
					// push the function on to the stack
					_call_lua("stylus_newpress");
		
					this->old_grid_x = grid_x;
					this->old_grid_y = grid_y;
				}			
			}
		}

		if (Stylus.Released)
		{
		   if (PA_StylusInZone(GRID_OFFSET_X, GRID_OFFSET_Y, GRID_OFFSET_X + GRID_WIDTH, GRID_OFFSET_Y + GRID_HEIGHT))
			{
			   // Compute X,Y coordinates from 1 to 16
			   grid_x = ((Stylus.X - (GRID_OFFSET_X)) / CELL_WIDTH) + 1;
				grid_y = ((Stylus.Y - (GRID_OFFSET_Y)) / CELL_HEIGHT) + 1; 

				if ((grid_x >= 1) && (grid_x <= 16) && (grid_y >= 1) && (grid_y <= 16))
				{ 
				
					// Inject X,Y as global variables into Lua
					_set_lua_global("X",grid_x);
					_set_lua_global("Y",grid_y);
		
					// push the function on to the stack
					_call_lua("stylus_released");
				}			
			}
		}

		if (Stylus.Held)
		{
		   if (PA_StylusInZone(GRID_OFFSET_X, GRID_OFFSET_Y, GRID_OFFSET_X + GRID_WIDTH, GRID_OFFSET_Y + GRID_HEIGHT))
			{
			   grid_x = ((Stylus.X - (GRID_OFFSET_X)) / CELL_WIDTH) + 1;
				grid_y = ((Stylus.Y - (GRID_OFFSET_Y)) / CELL_HEIGHT) + 1; 

				if ((grid_x >= 1) && (grid_x <= 16) && (grid_y >= 1) && (grid_y <= 16))
				{ 

					// Inject X,Y as global variables into Lua
					_set_lua_global("X",grid_x);
					_set_lua_global("Y",grid_y);
					
					if ((grid_x != this->old_grid_x) || (grid_y != this->old_grid_y))
					{						
						_call_lua("stylus_drag");
		
						this->old_grid_x = grid_x;
						this->old_grid_y = grid_y;
					}
				}						
			}	   
		}	

	}	
}

void Engine::clock()
{
   // Inject sequencer_step_16 into lua global namespace
	_set_lua_global("SEQUENCER_STEP_16",sequencer_step_16);
	_set_lua_global("SEQUENCER_STEP_32",sequencer_step_32);
	
	_call_lua("clock");
}

void Engine::display_lua_error(string additional_info)
{
	char buffer[200];
	PA_Clear8bitBg(1);
	sprintf(buffer, "%s;engine %d;script %s;error %s", additional_info.c_str(), this->engine_id, this->loaded_script.c_str(), lua_tostring(L, -1));
   
	PA_OutputText(1,1,1,buffer);
	lua_pop(this->L, 1);
   
   while(!Pad.Newpress.Select)
   {
      PA_WaitForVBL();
	}   
}

void Engine::redraw()
{
   _call_lua("redraw");
}

void Engine::set_block_en(int block)
{
   if (block >= 1 && block <= 8)
   {
   	_set_lua_global("BLOCK", block);
	}	
}

void Engine::save(string filename)
{
   char file_and_path[200];
  	char buffer[300];
	   
   sprintf(file_and_path,"/cellsDS/snapshots/%s.%d.lua",filename.c_str(),this->engine_id);

	sprintf(buffer,"%s...",file_and_path);
	PA_8bitText(BOTTOM_SCREEN, 1, 60 + (this->engine_id * 10), 255, 40,buffer,LIGHT_GRAY,0,0,130);	   
	   
   lua_pushlstring(this->L, file_and_path, strlen(file_and_path));
	lua_setglobal(this->L, "FILENAME");
							
   _call_lua("save_snapshot");
}

void Engine::load(string filename)
{
	char file_and_path[200];
   
   sprintf(file_and_path,"/cellsDS/snapshots/%s.%d.lua",filename.c_str(),this->engine_id);
	   
   lua_pushlstring(this->L, file_and_path, strlen(file_and_path));
	lua_setglobal(this->L, "FILENAME");
	
   _call_lua("load_snapshot");
}

void Engine::play_note(int note, int volume)
{
   int multisample_part = 0;
 	int scaled_volume;
 	
 	int global_volume = volume_module.get_volume(this->engine_id);
 	
	// scale volume according to global setting
 	scaled_volume = volume * global_volume / 127.0;
 
   this->voice[multisample_part].play((scale[note]/2),scaled_volume);
}

void Engine::play_frequency(int frequency, int volume)
{
   int multisample_part = 0;
 	int scaled_volume;
 	
 	int global_volume = volume_module.get_volume(this->engine_id);
 	
	// scale volume according to global setting
 	scaled_volume = volume * global_volume / 127.0;
 
   this->voice[multisample_part].play(frequency,scaled_volume);
}

void Engine::display_stack_length()
{
	char buffer[200];
	int stack_top;

	stack_top = lua_gettop(this->L);

	PA_Clear8bitBg(1);
	sprintf(buffer, "s%d",stack_top);
   
	PA_OutputText(1,4 * (this->engine_id + 1),1,buffer);
}
