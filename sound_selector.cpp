#define PALLET_BACKGROUND 1

class SoundSelector
{
   public:
 
 	int key_hold_time;
 	int file_offset;
 	int number_of_files;
	char file_assignment[100];
	char *sound_filenames[800];
	char* subdirectory;
	char * original_sound_buffer;

	SoundInfo sound_slot;  
   
   SoundSelector(void);
   
   void init(void);
   void draw(void);
	void redraw(void);
	void run(void);	
	void close(void);
	
	void load_sound(char* filename);
};

SoundSelector::SoundSelector()
{
   this->key_hold_time = 0;
	this->file_offset = 0;
	this->original_sound_buffer = NULL;
	
	// get directory listing
	
	int file_number = 0;
	int i;
	struct stat st;
	char filename[256]; // to hold a full filename and string terminator
	
	char* path_to_sound_files = "/cellsDS/sounds";
	
	DIR_ITER* dir = diropen(path_to_sound_files);

	if (dir == NULL) 
	{
		PA_OutputText(0,2,2,"Unable to open the directory.");
		PA_OutputText(0,2,3,path_to_sound_files);
	}
	else
	{
	   dirnext(dir, filename, &st); // eat up .
	   dirnext(dir, filename, &st); // eat up ..
	   
		while (dirnext(dir, filename, &st) == 0)
		{  
		   if (st.st_mode & S_IFDIR)
		   {
	   		// do nothing with directories
			}
			else
			{
			   this->sound_filenames[file_number] = (char*)malloc(strlen(filename) + 1);
			   strcpy(this->sound_filenames[file_number],filename);
				file_number++;	 
			}
		}
	} 
	
	this->number_of_files = file_number - 1;  	
}

void SoundSelector::init()
{

}

void SoundSelector::draw()
{
	int i;
	int row = 0;
	
	PA_Clear8bitBg(PALLET_BACKGROUND);
	PA_8bitText(PALLET_BACKGROUND, 3, 3, 255, 40,"choose sounds",1,1,0,100);
	PA_8bitText(PALLET_BACKGROUND, 35, 180, 255, 40,"Press up/down to select sound, A to load",1,1,0,130);		         

	// I do need the next line.  I'm remming it out temporarily just to get this to compile
	// ^^^
	// this->file_offset = find_position_of_filename_in_directory(file_assignment) - 4;

	for(i = this->file_offset; i < (this->file_offset + 9); i++)
	{
		if ((row < 4) && (i>=0) && (i <= this->number_of_files))
		{   
	   	PA_8bitText(PALLET_BACKGROUND, 3, (row * 12)+35, 255, 40, this->sound_filenames[i],1,0,0,100);
		} 
		
	   if (row == 4)
	   {
	      PA_8bitText(PALLET_BACKGROUND, 3, 90, 255, 40, this->sound_filenames[i],1,3,0,100);
		} 

		if ((row > 4) && (i>=0) && (i<= this->number_of_files))
		{   
	   	PA_8bitText(PALLET_BACKGROUND, 3, (row * 12)+55, 255, 40, this->sound_filenames[i],1,0,0,100);
		}		
		 
		row++;
	}		
}

void SoundSelector::redraw()
{

	int i;
	int row = 0;
	
	PA_Clear8bitBg(PALLET_BACKGROUND);
	PA_8bitText(PALLET_BACKGROUND, 3, 3, 255, 40,"choose sounds",1,1,0,100);
	PA_8bitText(PALLET_BACKGROUND, 35, 180, 255, 40,"Press up/down to select sound, A to load",1,1,0,130);		         

	for(i = this->file_offset; i < (this->file_offset + 9); i++)
	{
		if ((row < 4) && (i>=0) && (i <= this->number_of_files))
		{   
	   	PA_8bitText(PALLET_BACKGROUND, 3, (row * 12)+35, 255, 40,this->sound_filenames[i],1,0,0,100);
		} 
		
	   if (row == 4)
	   {
	      PA_8bitText(PALLET_BACKGROUND, 3, 90, 255, 40,this->sound_filenames[i],1,3,0,100);
		} 

		if ((row > 4) && (i>=0) && (i <= this->number_of_files))
		{   
	   	PA_8bitText(PALLET_BACKGROUND, 3, (row * 12)+55, 255, 40,this->sound_filenames[i],1,0,0,100);
		}		
		 
		row++;
	}	  

} 


void SoundSelector::run()
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
		this->load_sound(sound_filenames[this->file_offset + 4]);
	   AS_SoundPlay(this->sound_slot);		
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
	   
	   if ((this->key_hold_time > 10) && (this->key_hold_time % 10 == 0)) // the modulus slows down the scroll
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
	   
	   if ((this->key_hold_time > 10) && (this->key_hold_time % 10 == 0)) // the modulus slows down the scroll
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
}

void SoundSelector::load_sound(char* filename)
{
   FILE* fp;
 	u32 buffer_size;
   char * buffer;
	char *full_filename;
	u8 *old_buffer;
	char tmp_filename[100];
	u32 original_sound_buffer_size;

	char *path = "/cellsDS/sounds/";


	old_buffer = NULL;	
	
	strcpy(tmp_filename, filename); // just in case 
	
	full_filename = (char *)calloc(strlen(filename) + strlen(path) + 1, sizeof(char));
  
	strcat(full_filename, path);
	strcat(full_filename, filename);  
   	   
	fp = fopen (full_filename, "rb");
		
	if (fp == NULL) 
	{
		PA_Clear8bitBg(1);
	   PA_OutputSimpleText(1, 1, 5, "unable to read file");
	   PA_OutputSimpleText(1, 1, 6, full_filename);
	   
	   sound_slot.data = NULL;
	}
	else
	{

	   fseek(fp , 0 , SEEK_END);
	   buffer_size = ftell(fp);
	   rewind(fp);

		if (original_sound_buffer != NULL) free(original_sound_buffer);
	 
		buffer = (char*) malloc (sizeof(char)*buffer_size);
		original_sound_buffer = (char*) malloc (sizeof(char)*buffer_size);		
		
		original_sound_buffer_size = (sizeof(char)*buffer_size); 
		
	   fread (buffer,buffer_size,1,fp);
	   fclose (fp); 
	   
	   // copy sound data into new buffer
	  	memcpy((void *)original_sound_buffer,(void *)buffer,original_sound_buffer_size);
	   
	   old_buffer = sound_slot.data;
	   
		this->sound_slot.data = (u8*)buffer;
		this->sound_slot.size = (u32)buffer_size;
		this->sound_slot.format = (u32)1;
		this->sound_slot.volume = (u8)128;
		this->sound_slot.rate = (s32)22050;
		this->sound_slot.loop = (u8)0;
		this->sound_slot.delay = AS_REVERB;
	}
	
	if (old_buffer != NULL) free(old_buffer); // free up old memory
	free(full_filename);

	strcpy(this->file_assignment,tmp_filename);

}

