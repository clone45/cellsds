#define ENGINE_SWATCH_WIDTH 14
#define ENGINE_SWATCH_HEIGHT 23
#define ENGINE_SWATCH_MARGIN 7
#define ENGINE_SWATCHES_X_OFFSET 237
#define ENGINE_SWATCHES_Y_OFFSET 9

class EngineSwatches
{
   public:
	void draw();
	int poll();
	void draw_active_button(int engine_number);
	
	int selected_engine;
	
	EngineSwatches();
};


EngineSwatches::EngineSwatches()
{
	selected_engine = 0;
}

void EngineSwatches::draw_active_button(int engine_number)
{
	draw8bitRectEx(ENGINE_SWATCHES_X_OFFSET+1, ENGINE_SWATCHES_Y_OFFSET+(engine_number*(ENGINE_SWATCH_MARGIN + ENGINE_SWATCH_HEIGHT))+1, ENGINE_SWATCH_WIDTH-2, ENGINE_SWATCH_HEIGHT-2, 0, DARK_GRAY);   
}

int EngineSwatches::poll()
{
   int i;
   int j;
   
   if (Stylus.Newpress)
   {
	   for (i=0; i < NUMBER_OF_ENGINES; i++)
	   {
		   if (PA_StylusInZone(ENGINE_SWATCHES_X_OFFSET, ENGINE_SWATCHES_Y_OFFSET+(i*(ENGINE_SWATCH_MARGIN + ENGINE_SWATCH_HEIGHT)), ENGINE_SWATCHES_X_OFFSET + ENGINE_SWATCH_WIDTH, ENGINE_SWATCHES_Y_OFFSET+(i*(ENGINE_SWATCH_MARGIN + ENGINE_SWATCH_HEIGHT)) + ENGINE_SWATCH_HEIGHT))
		   {
		      // clear all engine selection buttons
			   for (j=0; j < NUMBER_OF_ENGINES; j++)
			   {
			      draw8bitRectEx(ENGINE_SWATCHES_X_OFFSET, ENGINE_SWATCHES_Y_OFFSET+(j*(ENGINE_SWATCH_MARGIN + ENGINE_SWATCH_HEIGHT)), ENGINE_SWATCH_WIDTH, ENGINE_SWATCH_HEIGHT, 0, WHITE); 
				}
				
				this->draw_active_button(i);	

				this->selected_engine = i;

				return(1);
			}   
		}		
	}

	return(0);
}

void EngineSwatches::draw()
{
   int i;
   
   for(i=0;i<NUMBER_OF_ENGINES;i++)
   {
   	draw8bitRectEx(ENGINE_SWATCHES_X_OFFSET, ENGINE_SWATCHES_Y_OFFSET+(i*(ENGINE_SWATCH_MARGIN + ENGINE_SWATCH_HEIGHT)), ENGINE_SWATCH_WIDTH, ENGINE_SWATCH_HEIGHT, 0, WHITE);
	} 
	
	this->draw_active_button(this->selected_engine);
}
