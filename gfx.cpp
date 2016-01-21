#define FONT_SIZE_SMALL 1
#define FONT_SIZE_LARGE 3

class GFX
{
   public:

	void print(int screen, int x, int y, const char* text, int color, int font_size);
	void print(int screen, int x, int y, const char* text, int color, int font_size, int transparency);
};


void GFX::print(int screen, int x, int y, const char* text, int color, int font_size)
{
	// PA_8bitText (u8 screen, s16 basex, s16 basey, s16 maxx, s16 maxy, char *text, u8 color, u8 size, u8 transp, s32 limit)
	PA_8bitText(screen, x, y, 255, 40, (char *)text, color, font_size, 0, 100);
}

void GFX::print(int screen, int x, int y, const char* text, int color, int font_size, int transparency)
{
	// PA_8bitText (u8 screen, s16 basex, s16 basey, s16 maxx, s16 maxy, char *text, u8 color, u8 size, u8 transp, s32 limit)
	PA_8bitText(screen, x, y, 255, 40, (char *)text, color, font_size, transparency, 100);
}
