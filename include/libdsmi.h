/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//                DSMI - Nintendo DS Music Interface Library                   //
//                                                                             //
// Version 12-03-2007                                                          //
// by 0xtob (Tobias Weyand) & TheRain (Collin Meyer)                           //
// Licensed under LGPL                                                         //
//                                                                             //
// Created with devKitPro (http://www.devkitpro.org) and libnds                //
// Special thanks to sgstair for WiFi lib and all his assistance               //
// and to natrium42 for creating the DSerial and MIDI adapter                  //
// Thanks also to the patrons of GBAdev.org forums, and the #dsdev channel     //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////


// Message types must be sent with a MIDI Channel # (see MIDI spec for more details)
// Usage like so:  write_MIDI(NOTE_ON|0x01, 60, 127);
//    where the MIDI channel (0-15) is being OR'd with the note on message,
//    60 is the note number, and 127 is the keypress velocity.

// note on is 0x9n where n is the MIDI Channel
// data1 is note number, data2 is key velocity 
#define NOTE_ON 0x90

// note off is 0x8n where n is the MIDI channel
// data1 is note number, data2 is release velocity
#define NOTE_OFF 0x80

// midi CC (control change) is 0xBn where n is the MIDI channel
// data1 is the control number, data2 is the control value 0-127
#define MIDI_CC 0xB0

// midi PC (pitch change) is 0xEn where n is the MIDI channel
// the message is a 14 bit value, where
// data1 is the least significant 7 bits and data2 is the most significant 7 bits
#define MIDI_PC	0xE0

#define DSMI_SERIAL	0
#define DSMI_WIFI	1


#ifdef __cplusplus
extern "C" {
#endif

// ------------ SETUP ------------ //

// Initializes Wifi (call this at the beginning
// of your app if you want to use wifi!)
extern void dsmi_setup_wifi_support(void);



// ------------ CONNECT ------------ //

// If a DSerial is inserted, this sets up the connection to the DSerial.
// Else, it connects to the default access point stored in Nintendo WFC
// memory (use an official game, e.g. mario kart to set this up)
// The initialized interface is set as the default interface.
//
// Returns 1 if connected, and 0 if failed.
extern int dsmi_connect(void);

// Using these you can force a wifi connection even if a DSerial is
// inserted or set up both connections for forwarding.
extern int dsmi_connect_dserial(void);
extern int dsmi_connect_wifi(void);



// ------------ WRITE ------------ //

// Send a MIDI message over the default interface, see MIDI spec for more details
extern void dsmi_write(u8 message,u8 data1, u8 data2);

// Force a MIDI message to be sent over DSerial
extern void dsmi_write_dserial(u8 message,u8 data1, u8 data2);

// Force a MIDI message to be sent over Wifi
extern void dsmi_write_wifi(u8 message,u8 data1, u8 data2);



// ------------ READ ------------ //

// Checks if a new message arrived at the default interface and returns it by
// filling the given pointers
//
// Returns 1, if a message was received, 0 if not
extern int dsmi_read(u8* message, u8* data1, u8* data2);

// Force receiving over DSerial
// extern int dsmi_read_dserial(u8* message, u8* data1, u8* data2); // TODO

// Force receiving over Wifi
extern int dsmi_read_wifi(u8* message, u8* data1, u8* data2);



// ------------ MISC ------------ //

// Returns the default interface (DSMI_SERIAL or DSMI_WIFI)
extern int dsmi_get_default_interface(void);



#ifdef __cplusplus
};
#endif
