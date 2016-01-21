// Snapshot Modes
#define SNAPSHOT_NONE 0
#define SNAPSHOT_LOAD 1
#define SNAPSHOT_SAVE 2
#define SNAPSHOT_NEW 3
#define SNAPSHOT_DELETE 4
#define SNAPSHOT_NEW_KEYBOARD_ENTRY 10

// Snapshot actions

#define SNAPSHOT_ACTION_NONE 0
#define SNAPSHOT_ACTION_LOAD 1
#define SNAPSHOT_ACTION_SAVE 2
#define SNAPSHOT_ACTION_DELETE 3

class SnapshotController
{
   public:

	string snapshot_filenames[200];
	
	int number_of_snapshot_files;
	int snapshot_file_offset;
	int snapshot_mode;
	char text[200];
	s32 nletter;
	char letter;
	
	string action_filename;
	
	void get_directory_listing();
	void draw();
	int handle_input();
	
	SnapshotController();
	// ~SnapshotController();
};

SnapshotController::SnapshotController()
{
	number_of_snapshot_files = 1;
	snapshot_file_offset = -4;
	snapshot_mode = SNAPSHOT_NONE;
	nletter = 0;
	letter = 0;
	
	this->get_directory_listing();
}


int SnapshotController::handle_input()
{
 	if (snapshot_mode < 10)
	{
	   if (number_of_snapshot_files > 0)
	   {
			if (Pad.Newpress.Down)
			{
			   if ((snapshot_file_offset + 4) < (number_of_snapshot_files-1)) snapshot_file_offset ++;
			   this->draw();
			}			

			if (Pad.Newpress.Up)
			{
			   if ((snapshot_file_offset + 4) > 0) snapshot_file_offset--;
			   this->draw();
			}
		
			if (Pad.Newpress.A) // LOAD snapshot
			{ 
			   action_filename = snapshot_filenames[snapshot_file_offset + 4];			   
			   snapshot_mode = SNAPSHOT_NONE;
				return(SNAPSHOT_ACTION_LOAD);
			}			

			if (Pad.Newpress.X) // SAVE over existing snapshot
			{
				action_filename = snapshot_filenames[snapshot_file_offset + 4];					   
			   snapshot_mode = SNAPSHOT_NONE;
				return(SNAPSHOT_ACTION_SAVE);
			}
						
			if (Pad.Newpress.Y) // DELETE snapshot
			{	
				action_filename = snapshot_filenames[snapshot_file_offset + 4];					   
			   snapshot_mode = SNAPSHOT_NONE;
		   	snapshot_file_offset = -4;
				return(SNAPSHOT_ACTION_DELETE);		
			}
			
			if (Pad.Newpress.Right)
			{
			   /*
			   next_version_filename = next_version(snapshot_filenames[snapshot_file_offset + 4]);
			   save_snapshot(next_version_filename);
			   free(next_version_filename);
			   draw_screen_save_load_mode();
			   */
			}			
			
		}
					
		if (Pad.Newpress.B) 
		{ 
		   snapshot_mode = SNAPSHOT_NEW_KEYBOARD_ENTRY; 
  	
	  		PA_Clear8bitBg(0);
  			PA_InitKeyboard(2); 
			PA_KeyboardIn(20, 100);
	
			PA_8bitText(0, 20, 40, 255, 40,(char*)"ENTER FILENAME, THEN PRESS (A) TO SAVE:",1,1,0,130);
	
			PA_8bitText(0, 20, 60, 255, 40,text,1,1,0,130);
		}			
	}	
	
	else if (snapshot_mode == SNAPSHOT_NEW_KEYBOARD_ENTRY)
	{
			
		letter = PA_CheckKeyboard();
			
		if (letter > 31) // there is a new letter
		{ 
			text[nletter] = letter;
			nletter++;
			PA_8bitText(0, 20, 60, 255, 40,text,1,1,0,130);
		}
		else if ((letter == PA_BACKSPACE)&&nletter)  // Backspace pressed
		{ 
			if(nletter > 0)
			{
				nletter--;
				text[nletter] = ' '; // Erase the last letter
				PA_8bitText(0, 20, 60, 255, 40,text,1,1,0,130);
			}			
		}

		if (Pad.Newpress.A || (letter == '\n'))
		{
		   action_filename.assign(text);
		   
		   PA_KeyboardOut();
		   snapshot_mode = SNAPSHOT_NONE;
		   this->draw();
	
			return(SNAPSHOT_ACTION_SAVE);
		}	
	}			

	return(SNAPSHOT_ACTION_NONE);
}

void SnapshotController::get_directory_listing()
{
	// get directory listing

	int file_number = 0;
	struct stat st;
	char filename[256]; // to hold a full filename and string terminator

	string fileext;
	
	DIR_ITER* dir = diropen("/cellsDS/snapshots");

	if (dir == NULL) 
	{
		PA_OutputText(0,2,2,(char*)"Unable to open the directory.");
		PA_OutputText(0,2,3,(char*)"Please make sure that /cellsDS/snapshots exists");
	}
	else
	{
	   dirnext(dir, filename, &st); // eat up .
	   dirnext(dir, filename, &st); // eat up ..
	   
		while (dirnext(dir, filename, &st) == 0)
		{
		   if (strlen(filename) > 4)
		   {
		   	fileext.assign(filename);
		   	fileext = fileext.substr(fileext.length()-4);
			}
			else
			{
			   fileext.assign("");
			}			
			   
			if (fileext != ".lua") 
			{		     
			   snapshot_filenames[file_number].assign(filename);
			   file_number++;
			}			
		}
	} 
	
	number_of_snapshot_files = file_number; 
}


void SnapshotController::draw()
{
	int i;
	int y_pos = 0;
	
	PA_Clear8bitBg(0);
	PA_8bitText(0, 3, 3, 255, 40,(char*)"snapshot files",WHITE,1,0,100);
	PA_8bitText(0, 10, 180, 255, 40,(char*)"A - Load    |    B -  Save As...    |    X -  Save    |    Y -  Delete",WHITE,1,0,130); 		

	for(i = snapshot_file_offset; i < (snapshot_file_offset + 9); i++)
	{
		if ((y_pos < 4) && (i>=0) && (i < number_of_snapshot_files))
		{   
	   	PA_8bitText(0, 3, (y_pos * 12)+35, 255, 40,(char*)snapshot_filenames[i].c_str(),DARK_GRAY,0,0,100);
		} 
		
	   if (y_pos == 4)
	   {
	      PA_8bitText(0, 3, 90, 255, 40,(char*)snapshot_filenames[i].c_str(),WHITE,3,0,100);
		} 

		if ((y_pos > 4) && (i>=0) && (i < number_of_snapshot_files))
		{   
	   	PA_8bitText(0, 3, (y_pos * 12)+55, 255, 40,(char*)snapshot_filenames[i].c_str(),DARK_GRAY,0,0,100);
		}		
		 
		y_pos++;
	}
}
