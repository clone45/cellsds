class ds_pad_buttons
{
   public:
      
	bool A;
	bool B;
	bool X;
	bool Y;

	bool L; // Left shoulder button
	bool R; // Right shoulder button
	
	bool Right; // Right directional pad
	bool Left;
	bool Up;
	bool Down;
	
	bool Select;
	bool Start;
		
};


class UPad
{
   public:
      
	ds_pad_buttons Newpress;
	ds_pad_buttons Released;		
	ds_pad_buttons Held;
	
	void update();
	
	UPad();
};

class UStylus
{
   public:
      
	bool Newpress;
	bool Released;		
	bool Held;
	
	void update();

	UStylus();
};



void UPad::update()
{

	// Calculate new values for .Newpress

   Pad.Held.Start && !Held.Start ? Newpress.Start = 1 : Newpress.Start = 0;
   Pad.Held.Select && !Held.Select ? Newpress.Select = 1 : Newpress.Select = 0;

   Pad.Held.A && !Held.A ? Newpress.A = 1 : Newpress.A = 0;
   Pad.Held.B && !Held.B ? Newpress.B = 1 : Newpress.B = 0;
   Pad.Held.X && !Held.X ? Newpress.X = 1 : Newpress.X = 0;
   Pad.Held.Y && !Held.Y ? Newpress.Y = 1 : Newpress.Y = 0;

   Pad.Held.L && !Held.L ? Newpress.L = 1 : Newpress.L = 0;
   Pad.Held.R && !Held.R ? Newpress.R = 1 : Newpress.R = 0;

   Pad.Held.Up && !Held.Up ? Newpress.Up = 1 : Newpress.Up = 0;
   Pad.Held.Down && !Held.Down ? Newpress.Down = 1 : Newpress.Down = 0;
   Pad.Held.Left && !Held.Left ? Newpress.Left = 1 : Newpress.Left = 0;
   Pad.Held.Right && !Held.Right ? Newpress.Right = 1 : Newpress.Right = 0;

	// Calculate new values for .Held

	Held.Start = Pad.Held.Start;
	Held.Select = Pad.Held.Select;

   Held.A = Pad.Held.A;  
   Held.B = Pad.Held.B;
   Held.X = Pad.Held.X;
   Held.Y = Pad.Held.Y;

   Held.L = Pad.Held.L;
   Held.R = Pad.Held.R;

   Held.Up = Pad.Held.Up;  
   Held.Down = Pad.Held.Down;
   Held.Left = Pad.Held.Left;
   Held.Right = Pad.Held.Right;

	// TODO: same thing for released
	
   Released.A = Pad.Released.A;  
   Released.B = Pad.Released.B;
   Released.X = Pad.Released.X;
   Released.Y = Pad.Released.Y;

   Released.L = Pad.Released.L;
   Released.R = Pad.Released.R;

   Released.Up = Pad.Released.Up;  
   Released.Down = Pad.Released.Down;
   Released.Left = Pad.Released.Left;
   Released.Right = Pad.Released.Right;

}

void UStylus::update()
{
	uint16 specialKeysPressed = ~IPC->buttons;

	// pen down (see http://www.double.co.nz/nintendo_ds/nds_develop3.html)
	(specialKeysPressed & (1 << 6)) && !Held ? Newpress = 1 : Newpress = 0;
   
	
	Held = (specialKeysPressed & (1 << 6));
	Released = Stylus.Released;
}


UPad::UPad()
{
   Newpress.Select = 0;
   Newpress.Start = 0;

   Newpress.A = 0;  
   Newpress.B = 0;
   Newpress.X = 0;
   Newpress.Y = 0;

   Newpress.L = 0;
   Newpress.R = 0;

   Newpress.Up = 0;  
   Newpress.Down = 0;
   Newpress.Left = 0;
   Newpress.Right = 0;

   Released.Select = 0;
   Released.Start = 0;
   
   Released.A = 0;  
   Released.B = 0;
   Released.X = 0;
   Released.Y = 0;

   Released.L = 0;
   Released.R = 0;

   Released.Up = 0;  
   Released.Down = 0;
   Released.Left = 0;
   Released.Right = 0;
 
	Held.Select = 0;
	Held.Start = 0; 
   
   Held.A = 0;  
   Held.B = 0;
   Held.X = 0;
   Held.Y = 0;

   Held.L = 0;
   Held.R = 0;

   Held.Up = 0;  
   Held.Down = 0;
   Held.Left = 0;
   Held.Right = 0;
}

UStylus::UStylus()
{
   Held = 0;
	Newpress = 0;
	Released = 0;
}


