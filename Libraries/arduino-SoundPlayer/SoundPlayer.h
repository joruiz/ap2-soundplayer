/*
 Arduino Library for I2C sound player peripheral.
 It is based on the 4D Systems SOMO-14D Embedded Audio-Sound Module
 go to http://www.4dsystems.com.au/product/10/117/Development/SOMO_14D/ for more information
 
 License: CC BY-SA 3.0: Creative Commons Share-alike 3.0. Feel free 
 to use and abuse this code however you'd like. If you find it useful
 please attribute, and SHARE-ALIKE!
 
 Created February 2014
 by Jonathan Ruiz de Garibay
*/ 

#ifndef SOUNDPLAYER_h
#define SOUNDPLAYER_h

#include "Arduino.h"

#define CMD_GETVERSION	0x10
#define CMD_PLAY		0x40
#define	CMD_STOP		0x41
#define CMD_PAUSE		0x42
#define CMD_SETVOLUME	0x50
#define CMD_GETVOLUME	0x51
#define STATE_PLAY		0x4E
#define STATE_STOP		0x4F
#define SETVOLUME_OK	0x5F
#define GETVOLUME_BASE	0x50

class SoundPlayer
{
	public:
		void begin(uint8_t address);
		uint8_t getVersion();
		boolean play(unsigned int song);
		boolean stop();
		boolean pause();
		void setVolume(uint8_t volume);
		uint8_t getVolume();

	private:
		uint8_t _address;
};

#endif