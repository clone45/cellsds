#define MAXFILES 800

class FileSelector
{
   public:

	void draw();
	void redraw();
	void center_on_filename(string filename);
	void initialize(char* path, int screen);
	
	string handle_input();

	string filenames[MAXFILES];
	int number_of_files;
	int screen;
	int selected_file_position;
	int key_hold_time;
	int file_offset;
		
	FileSelector();
	~FileSelector();
};


string FileSelector::handle_input()
{
	if (!Pad.Held.Down && !Pad.Held.Up & !Pad.Held.X & !Pad.Held.B) this->key_hold_time = 0;
   	
	if (Pad.Newpress.Up)
	{
	   if ((this->file_offset + 4) > 0) this->file_offset--;
		this->redraw();
	}

	if (Pad.Held.Up)
	{
	   this->key_hold_time++;
	   
	   if ((this->key_hold_time > 10) && (this->key_hold_time%3 == 0)) // the modulus slows down the scroll
	   {
		   if ((this->file_offset + 4) > 0) 
			{
			   this->file_offset--;
		   	this->redraw();
			}   
		}					   
	}

	if (Pad.Newpress.Down)
	{
	   if ((this->file_offset + 4) < this->number_of_files) this->file_offset ++;
	   this->redraw();
	}
	
	if (Pad.Held.Down)
	{
	   this->key_hold_time++;
	   
	   if ((this->key_hold_time > 10)  && (this->key_hold_time%3 == 0)) // the modulus slows down the scroll
	   {
	   	if ((this->file_offset + 4) < this->number_of_files) 
			{	
			   this->file_offset ++;
	   		this->redraw();	
	 		}  	
		}					   
	}		
	
	if (Pad.Newpress.A)
	{
   	return(this->filenames[this->file_offset+4]);
	}							
	   
	if (Pad.Newpress.B)
	{
   	if ((this->file_offset + 4 + 9) < this->number_of_files) 
		{	
		   this->file_offset += 9;	
 		} 
		else
		{
		   this->file_offset = this->number_of_files - 4;
		}
		this->redraw();	
	}
	
	if (Pad.Held.B)
	{
	   this->key_hold_time++;
	   
	   if ((this->key_hold_time > 10) && (this->key_hold_time%10 == 0)) // the modulus slows down the scroll
	   {
	   	if ((this->file_offset + 4 + 9) < this->number_of_files) 
			{	
			   this->file_offset += 9;	
	 		}
			else
			{
			   this->file_offset = this->number_of_files - 4;
			}	
			this->redraw();			 
		}					   
	}	
	
	if (Pad.Newpress.X)
	{
	   if ((this->file_offset + 4 - 9) > 0) 
		{
		   this->file_offset -= 9;
		} 
		else
		{
		   this->file_offset = -4;
		}
	   this->redraw();		
	}
	
	if (Pad.Held.X)
	{
	   this->key_hold_time++;
	   
	   if ((this->key_hold_time > 10) && (this->key_hold_time%10 == 0)) // the modulus slows down the scroll
	   {
		   if ((this->file_offset + 4 - 9) > 0) 
			{
			   this->file_offset -= 9;	
			}    
			else
			{
			   this->file_offset = -4;
			}
			this->redraw();
		}				   					   
	}	  
	
	
	return("");
}

void FileSelector::center_on_filename(string filename)
{
     
   int i;
	
	for(i=0; i < this->number_of_files; i++)
	{
	   if (filename == this->filenames[i])
	   {
			this->file_offset = i - 4;
		}   
	}		
}

void FileSelector::draw()
{
	int i;
	int y_pos = 0;
	
	PA_Clear8bitBg(this->screen);

	// Draw grid border
	PA_Draw8bitLine(this->screen,7,7,231,7,WHITE);
	PA_Draw8bitLine(this->screen,7,183,231,183,WHITE);
	PA_Draw8bitLine(this->screen,7,7,7,183,WHITE);	
	PA_Draw8bitLine(this->screen,231,7,231,183,WHITE);

	// Draw white background box on bottom
   draw8bitRectEx(7, 168, 225, 15, 0, WHITE);	

	PA_8bitText(this->screen, 14, 173, 255, 40,(char*)"UP/DOWN - select engine   |   A - load",DARK_BLUE,1,1,130);		         

	int text_x_offset = 14;

	for(i = this->file_offset; i < (this->file_offset + 9); i++)
	{
		if ((y_pos < 4) && (i>=0) && (i<=number_of_files))
		{   
	   	PA_8bitText(this->screen, text_x_offset, (y_pos * 12)+35, 255, 40,(char*)this->filenames[i].c_str(),1,0,0,100);
		} 
		
	   if (y_pos == 4)
	   {
	      PA_8bitText(this->screen, text_x_offset, 90, 255, 40,(char*)this->filenames[i].c_str(),WHITE,2,0,100);
		} 

		if ((y_pos > 4) && (i >= 0) && (i <= this->number_of_files))
		{   
	   	PA_8bitText(this->screen, text_x_offset, (y_pos * 12)+55, 255, 40,(char*)this->filenames[i].c_str(),1,0,0,100);
		}		
		 
		y_pos++;
	}

	engine_swatches.draw();
  
}

void FileSelector::redraw()
{
	int i;
	int y_pos = 0;
	
   draw8bitRectEx(8, 8, 223, 160, 0, BLACK);	

	int text_x_offset = 14;

	for(i = this->file_offset; i < (this->file_offset + 9); i++)
	{
		if ((y_pos < 4) && (i>=0) && (i<=number_of_files))
		{   
	   	PA_8bitText(this->screen, text_x_offset, (y_pos * 12)+35, 255, 40,(char*)this->filenames[i].c_str(),1,0,0,100);
		} 
		
	   if (y_pos == 4)
	   {
	      PA_8bitText(this->screen, text_x_offset, 90, 255, 40,(char*)this->filenames[i].c_str(),WHITE,2,0,100);
		} 

		if ((y_pos > 4) && (i >= 0) && (i <= this->number_of_files))
		{   
	   	PA_8bitText(this->screen, text_x_offset, (y_pos * 12)+55, 255, 40,(char*)this->filenames[i].c_str(),1,0,0,100);
		}		
		 
		y_pos++;
	}
}

FileSelector::FileSelector()
{
}

void FileSelector::initialize(char* path, int screen)
{
	int file_number = 0;
	struct stat st;
	char filename[256]; // to hold a full filename and string terminator

	string fileext;
	string s;

	this->screen = screen;
	this->selected_file_position = 0;
	
	DIR_ITER* dir = diropen(path);

	if (dir == NULL) 
	{
		PA_OutputText(screen,2,2,(char*)"Unable to open the directory.");
		PA_OutputText(screen,2,3,path);
	}
	else
	{
	   dirnext(dir, filename, &st); // eat up .
	   dirnext(dir, filename, &st); // eat up ..
	   
		while (dirnext(dir, filename, &st) == 0)
		{  
		   // fileext.assign(filename);
		   //fileext = fileext.substr(fileext.length()-4);

		   //if (fileext == ".lua") 
			//{
				s.assign(filename);
			   
				for (int j=0; j < (int)s.length(); ++j)
				{
					s[j]=tolower(s[j]);
				}
			   
			   this->filenames[file_number].assign(s);
				file_number++;	 
			//}			
		}
	} 
	
	this->number_of_files = file_number - 1;
	this->key_hold_time = 0;
	this->file_offset = -4;
}

FileSelector::~FileSelector()
{
}

