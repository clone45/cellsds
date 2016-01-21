#define VOLUME_FADER_WIDTH 42 
#define VOLUME_FADER_BOTTOM 165

class VolumeModule
{
   public:

	int volume[6];
	
	void draw();
	void handle_input();
	void _draw_volume_fader(int i);
	
	int get_volume(int engine_id);
	void set_volume(int engine_id, int volume);
	
	VolumeModule();
	// ~VolumeModule();
};

int VolumeModule::get_volume(int engine_id)
{
   return(this->volume[engine_id]);
}

void VolumeModule::set_volume(int engine_id, int volume)
{
   this->volume[engine_id] = volume;
}

void VolumeModule::_draw_volume_fader(int i)
{
   draw8bitRectEx(i*VOLUME_FADER_WIDTH, (VOLUME_FADER_BOTTOM - 127), VOLUME_FADER_WIDTH - 3, 127, 0, BLACK); // draw black box  
   draw8bitRectEx(i*VOLUME_FADER_WIDTH, VOLUME_FADER_BOTTOM - volume[i], VOLUME_FADER_WIDTH - 3, volume[i], 0, WHITE);
}

void VolumeModule::draw()
{
	PA_Clear8bitBg(BOTTOM_SCREEN);
	PA_8bitText(BOTTOM_SCREEN, 2, 2, 255, 40,(char*)"Volume",WHITE,1,1,130);

   for (int engine_id=0; engine_id < NUMBER_OF_ENGINES; engine_id++)
   {
		this->_draw_volume_fader(engine_id);
	}
}


void VolumeModule::handle_input()
{
   int stylus_x;
   int stylus_y;
   
	// Allow user to change faders using the stylus
	if (Stylus.Held)
	{
		stylus_x = Stylus.X / VOLUME_FADER_WIDTH;
		stylus_y = Stylus.Y;
		
		// The -10 and +10 on the next line makes it easier to hit the extemes with the sylus
		if ((stylus_y >= (VOLUME_FADER_BOTTOM - 127 - 20)) && (stylus_y < VOLUME_FADER_BOTTOM + 10) && (stylus_x < 6))
		{
			volume[stylus_x] = VOLUME_FADER_BOTTOM - stylus_y;
			
			if (volume[stylus_x] > 127) volume[stylus_x] = 127;
			if (volume[stylus_x] < 0) volume[stylus_x] = 0;
			
			this->_draw_volume_fader(stylus_x);

			// sound_slot[stylus_x].volume = volume[stylus_x];			
		}			
	}
}

VolumeModule::VolumeModule()
{
   int engine_id;
   for (engine_id=0; engine_id < NUMBER_OF_ENGINES; engine_id++)
   {
      volume[engine_id] = 127;
	}
}

