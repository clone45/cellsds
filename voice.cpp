class Voice
{
   public:

	SoundInfo sound_info;
	
	// .wav specific settings
	u32 riff_size;
	u32 fmt_chunk_size;
	u16 compression_code;	
	u32 avg_bytes_per_sec; 
	u16 block_align;
	u16 bit_per_sample;
	u32 data_chunk_size;
	
	// sound settings
	int pan;
	
	// Functions
	void load_raw(char* filename);
	void load_wav(char* filename);
	
	void load(char* filename);
	void play(int frequency, int volume);
	void set_pan(int pan);
	int is_playable(void);
	
	Voice();
	~Voice();
};

void Voice::set_pan(int pan)
{
   this->pan = pan;
}

void Voice::play(int frequency, int volume)
{
	this->sound_info.rate = (s32)frequency;
	this->sound_info.pan = (s8)64;
   
   // panning of a sound (0=left, 64=center, 127=right) 
   
   // AS_SetDefaultSettings (u8 format, s32 rate, u8 delay)
   AS_SetDefaultSettings(1, this->sound_info.rate, 0);

   //AS_SoundDefaultPlay (u8 *data, u32 size, u8 volume, u8 pan, u8 loop, u8 prio)
   AS_SoundDefaultPlay(this->sound_info.data, this->sound_info.size, volume, this->pan, 0, 1);
}

/*
PA_8bitText (u8 screen, s16 basex, s16 basey, s16 maxx, s16 maxy, char *text, u8 color, u8 size, u8 transp, s32 limit)	
u8 = unsigned char (8 bits range 0 <-> 255)
u16 = unsigned short (16 bits range 0 <-> 65535)
u32 = unsigned long (32 bits range 0 <-> 4294967295)
s8 = char (7 bits + 1 sign bit range -128 <-> 127)
s16 = short (15 bits + 1 sign bit range -32768 <-> 32767)
s32 = long (31 bits + 1 sign bit range -2147483648 <-> 2147483647)
*/

int Voice::is_playable(void)
{
   if (this->sound_info.data == NULL) return(0);
   return(1);
}

void Voice::load(char* filename)
{
   this->load_raw(filename);
}

void Voice::load_raw(char* filename)
{
   FILE* fp;
 	u32 buffer_size;
   char *buffer;
	
	fp = fopen(filename, "rb");
		
	if (fp == NULL) 
	{
		// PA_Clear8bitBg(1);
	   // PA_OutputSimpleText(1, 1, 5, "unable to find file");
	   // PA_OutputSimpleText(1, 1, 6, filename);
	   
	   this->sound_info.data = NULL;
	}
	else
	{
	   fseek(fp , 0 , SEEK_END);
	   buffer_size = ftell(fp);
	   rewind(fp);

		if (this->sound_info.data != NULL) free(this->sound_info.data);
	 
		buffer = (char*) malloc (sizeof(char)*buffer_size);
	   fread (buffer,buffer_size,1,fp);
	   fclose (fp); 
	   
		this->sound_info.data = (u8*)buffer;
		this->sound_info.size = (u32)buffer_size;
		this->sound_info.format = (u32)1;
		this->sound_info.volume = (u8)126;
		this->sound_info.rate = (s32)22050;
		this->sound_info.loop = (u8)0;
		this->sound_info.pan = (s8)0;
	}
}

void Voice::load_wav(char* filename) // doesn't work yet
{
	char *buf = (char*)malloc(5);   
	FILE *fp;	
	
	fp = fopen(filename, "r");
	
	if (fp == NULL) 
	{
		PA_Clear8bitBg(1);
	   PA_OutputSimpleText(1, 1, 5, "unable to read file");
	   PA_OutputSimpleText(1, 1, 6, filename);
	   
	   this->sound_info.data = NULL;
	}
	
	memset(buf,0,5);
	
	fread(buf, 1, 4, fp); // RIFF header
	fread(&this->riff_size, 4, 1, fp);
	fread(buf, 1, 4, fp); // WAVE header
	fread(buf, 1, 4, fp); // fmt chunk
	fread(&this->fmt_chunk_size, 4, 1, fp);	
	fread(&this->compression_code, 2, 1, fp);
	fread(&this->sound_info.format, 2, 1, fp);
	this->sound_info.format = 2;
	fread(&this->sound_info.rate, 4, 1, fp);
	fread(&this->avg_bytes_per_sec, 4, 1, fp);
	fread(&this->block_align, 2, 1, fp);
	fread(&this->bit_per_sample, 2, 1, fp);
	
	// Skip extra bytes
	fseek(fp, this->fmt_chunk_size - 16, SEEK_CUR);
	
	fread(buf, 1, 4, fp);
	
	// Skip forward to the data chunk
	while((!feof(fp))&&(strcmp(buf,"data")!=0)) {
	    u32 chunk_size;
	    fread(&chunk_size, 4, 1, fp);
	    fseek(fp, chunk_size, 1);
	    fread(buf, 1, 4, fp);
	}
	
	// data chunk
	fread(&this->data_chunk_size, 4, 1, fp);
	
	// free up this->sound_info->data if it is not null
	if (this->sound_info.data != NULL) free(this->sound_info.data);
	
	this->sound_info.data = (u8*)malloc(data_chunk_size);
	memset(this->sound_info.data, 0, data_chunk_size);
	
	// Read the data
	fread(this->sound_info.data, data_chunk_size, 1, fp);
	
	// Convert 8 bit samples from unsigned to signed
	if(bit_per_sample == 8) {
	    s8* signed_data = (s8*)this->sound_info.data;
	    for(u32 i=0; i<data_chunk_size; ++i) {
	            signed_data[i] = this->sound_info.data[i] - 128;
	    }
	}
	
	// Finish up
	free(buf);
	
	fclose(fp);
}

Voice::Voice()
{
   this->pan = 64;
   
   this->sound_info.data = NULL;
	this->sound_info.format = (u32)1;
	this->sound_info.volume = (u8)126;
	this->sound_info.rate = (s32)22050;
	this->sound_info.loop = (u8)0;
	this->sound_info.pan = (s8)0;
}

Voice::~Voice()
{
   free(this->sound_info.data);
}

