
class DistortionModule
{
   public:

	int step;
	int distortion_amount;
	
	void draw();
	void handle_input();
	int distort(int frequency);
		
	DistortionModule();
	// ~DistortionModule();
};

int distort(int frequency)
{
   if (step == 0)
   {
    	step = 1;
		return (frequency + distortion_amount);
	}
	else
	{
	   step = 0;
		return (frequency - distortion_amount);
	}		
}


void DistortionModule::draw()
{
	PA_Clear8bitBg(BOTTOM_SCREEN);
	
	// Draw grid border
	PA_Draw8bitLine(BOTTOM_SCREEN,7,7,231,7,WHITE);
	PA_Draw8bitLine(BOTTOM_SCREEN,7,183,231,183,WHITE);
	PA_Draw8bitLine(BOTTOM_SCREEN,7,7,7,183,WHITE);	
	PA_Draw8bitLine(BOTTOM_SCREEN,231,7,231,183,WHITE);
	
	// Draw white background box on bottom
	draw8bitRectEx(7, 168, 225, 15, BOTTOM_SCREEN, WHITE);	
	
	PA_8bitText(BOTTOM_SCREEN, 10, 10, 255, 40,(char*)"Distortion",WHITE,1,1,130);
}


void DistortionModule::handle_input()
{
	distortion_amount = (185 - Stylus.Y) * 300;
				
}

DistortionModule::DistortionModule()
{
	step = 0;
	distortion_amount = 0;
}

