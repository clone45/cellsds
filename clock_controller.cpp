// clock sources

#define CLOCK_SOURCE_INTERNAL 0
#define CLOCK_SOURCE_MIDI 1

class ClockController
{
   public:

	void draw();
	void draw_clock_source_midi();
	void draw_bpm();
	void handle_input();

	int old_bpm;
	int clock_source;
	int clock_division;
	
	ClockController();
	// ~ClockController();
};

ClockController::ClockController()
{
   clock_source = CLOCK_SOURCE_INTERNAL;
   old_bpm = 0;
	clock_division = 6;
}

void ClockController::draw()
{
   if (clock_source == CLOCK_SOURCE_MIDI)
   {
      this->draw_clock_source_midi();
	}
	if (clock_source == CLOCK_SOURCE_INTERNAL)
	{
	   this->draw_bpm();
	}	
}

void ClockController::draw_clock_source_midi()
{
   char buffer[100];
	int i;
	
	// PA_8bitText (u8 screen, s16 basex, s16 basey, s16 maxx, s16 maxy, char *text, u8 color, u8 size, u8 transp, s32 limit)	
	// example: PA_8bitText(0, 10, 20, 255, 40, text, 1, 3, 0, 100);
   
	PA_Clear8bitBg(0);    
	PA_8bitText(0, 3, 3, 255, 40,(char*)"clock control: MIDI",1,1,0,100); 
	
   sprintf(buffer, "Clock Division: %d", clock_division);
   PA_8bitText(0, 40, 80, 255, 40,(char*)buffer,1,4,0,130);	
   
   // Draw shortcut buttons
   
   
   for (i=1;i<7;i++)
   {
		draw8bitRectEx(32 * i, 140, 16, 16, 0, 7);	
		draw8bitRectEx(32 * i, 170, 16, 16, 0, 7);
	} 

   PA_8bitText(0, 32, 140, 255, 40,(char*)"1",1,1,0,100);
   PA_8bitText(0, 64, 140, 255, 40,(char*)"2",1,1,0,100);
   PA_8bitText(0, 96, 140, 255, 40,(char*)"3",1,1,0,100);
   PA_8bitText(0, 128, 140, 255, 40,(char*)"4",1,1,0,100);  
   PA_8bitText(0, 160, 140, 255, 40,(char*)"5",1,1,0,100);
   PA_8bitText(0, 192, 140, 255, 40,(char*)"6",1,1,0,100); 
	 
   PA_8bitText(0, 32, 170, 255, 40,(char*)"9",1,1,0,100);
   PA_8bitText(0, 64, 170, 255, 40,(char*)"12",1,1,0,100);
   PA_8bitText(0, 96, 170, 255, 40,(char*)"24",1,1,0,100);
   PA_8bitText(0, 128, 170, 255, 40,(char*)"36",1,1,0,100);  
   PA_8bitText(0, 160, 170, 255, 40,(char*)"48",1,1,0,100);
   PA_8bitText(0, 192, 170, 255, 40,(char*)"64",1,1,0,100); 

}

void ClockController::draw_bpm()
{
   char buffer[10];

	// PA_8bitText (u8 screen, s16 basex, s16 basey, s16 maxx, s16 maxy, char *text, u8 color, u8 size, u8 transp, s32 limit)	
	// example: PA_8bitText(0, 10, 20, 255, 40, text, 1, 3, 0, 100);
   
	PA_Clear8bitBg(0);    
	PA_8bitText(0, 3, 3, 255, 40,(char*)"clock control:BPM",WHITE,1,0,100);   
   sprintf(buffer, "%d BPM", bpm);
   PA_8bitText(0, 90, 80, 255, 40,buffer,WHITE,4,0,130);
	PA_8bitText(0, 30, 105, 255, 40,(char*)"Use up/down keypad for +/- 1 BPM",MEDIUM_GRAY,2,0,130);
	PA_8bitText(0, 25, 120, 255, 40,(char*)"Use right/left keypad for +/- 10 BPM",MEDIUM_GRAY,2,0,130);
	PA_8bitText(0, 65, 180, 255, 40,(char*)"BPM is not 100% accurate",MEDIUM_GRAY,1,0,130);
}

void ClockController::handle_input()
{
   if (clock_source == CLOCK_SOURCE_INTERNAL)
   {
	   old_bpm = bpm;
		
		if (Pad.Newpress.Up) if (bpm < 300) bpm += 1;  
		if (Pad.Newpress.Down) if (bpm > 49) bpm -= 1;
		if (Pad.Newpress.Right) if (bpm < 291) bpm += 10;  
		if (Pad.Newpress.Left) if (bpm > 60) bpm -= 10;  
	
		if (old_bpm != bpm)
		{
		   old_bpm = bpm;
		   TIMER_DATA(1) = (int)(-(0x2000000>>10)/ (float)((bpm * 4.0) / (float)60));	   
		   draw_bpm();
		}	
	}

	if (clock_source == CLOCK_SOURCE_MIDI)
	{
	   if (Stylus.Newpress && PA_StylusInZone(32, 140, 32 + 16, 140 + 16)) clock_division = 1;
	   if (Stylus.Newpress && PA_StylusInZone(64, 140, 64 + 16, 140 + 16)) clock_division = 2;
	   if (Stylus.Newpress && PA_StylusInZone(96, 140, 96 + 16, 140 + 16)) clock_division = 3;
	   if (Stylus.Newpress && PA_StylusInZone(128, 140, 128 + 16, 140 + 16)) clock_division = 4;
	   if (Stylus.Newpress && PA_StylusInZone(160, 140, 160 + 16, 140 + 16)) clock_division = 5;
	   if (Stylus.Newpress && PA_StylusInZone(192, 140, 192 + 16, 140 + 16)) clock_division = 6;

	   if (Stylus.Newpress && PA_StylusInZone(32, 170, 32 + 16, 170 + 16)) clock_division = 9;
	   if (Stylus.Newpress && PA_StylusInZone(64, 170, 64 + 16, 170 + 16)) clock_division = 12;
	   if (Stylus.Newpress && PA_StylusInZone(96, 170, 96 + 16, 170 + 16)) clock_division = 24;
	   if (Stylus.Newpress && PA_StylusInZone(128, 170, 128 + 16, 170 + 16)) clock_division = 36;
	   if (Stylus.Newpress && PA_StylusInZone(160, 170, 160 + 16, 170 + 16)) clock_division = 48;
	   if (Stylus.Newpress && PA_StylusInZone(192, 170, 192 + 16, 170 + 16)) clock_division = 64;
			   
	   if (Stylus.Newpress) draw_clock_source_midi();
	}	
				
}

