#define MIDI_STATUS_DISCONNECTED 0
#define MIDI_STATUS_CONNECTED 1

class MidiModule
{
   public:

	int status;
	
	void draw();
	void handle_input();
	
	int get_status();
	void midi_connect();
	void send(u8 message, u8 data1, u8 data2);
	void send_note_on(int channel, int note, int velocity);
	void send_note_off(int channel, int note);

	MidiModule();
	// ~MidiModule();
};


void MidiModule::draw()
{
	PA_Clear8bitBg(BOTTOM_SCREEN);
	PA_8bitText(BOTTOM_SCREEN, 2, 2, 255, 40,(char*)"Midi Settings",WHITE,1,1,130);
	
	if (this->status == MIDI_STATUS_DISCONNECTED)
	{
		PA_8bitText(BOTTOM_SCREEN, 2, 15, 255, 40,(char*)"Press [A] to connect to MIDI...",WHITE,1,1,130);
	}
	else
	{
		PA_8bitText(BOTTOM_SCREEN, 2, 15, 255, 40,(char*)"Status: -- Connected --",WHITE,1,1,130);
	}		   	
}


void MidiModule::handle_input()
{
	if (Pad.Newpress.A && (this->status == MIDI_STATUS_DISCONNECTED))
	{
	   this->midi_connect();
	}	
}

int MidiModule::get_status()
{
	return(this->status);
}

void MidiModule::midi_connect()
{
	PA_Clear8bitBg(BOTTOM_SCREEN);
	   
	PA_8bitText(BOTTOM_SCREEN, 3, 3, 255, 40,"Searching for WIFI-MIDI...",1,1,0,100);

	dsmi_setup_wifi_support();

	int res = dsmi_connect();

	if (res == 0)
	{
		PA_8bitText(BOTTOM_SCREEN, 3, 15, 255, 40,"No WIFI-MIDI detected.",1,1,0,100);
		this->status = MIDI_STATUS_DISCONNECTED;
	}		
	else
	{
	   PA_8bitText(0, 3, 15, 255, 40,"Connected to WIFI-MIDI!",1,1,0,100);
		this->status = MIDI_STATUS_CONNECTED;
	}   
}

void MidiModule::send(u8 message, u8 data1, u8 data2)
{
   dsmi_write(message,data1,data2);
}

void MidiModule::send_note_on(int channel, int note, int velocity)
{
   channel--;
   dsmi_write(0x90 | channel, note, velocity);
}

void MidiModule::send_note_off(int channel, int note)
{
   channel--;
   dsmi_write(0x80 | channel, note, 0);
}

MidiModule::MidiModule()
{
	this->status = MIDI_STATUS_DISCONNECTED;
}

