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

#include "Arduino.h"
#include "SoundPlayer.h"
#include <Wire.h>

//
// begin
//
// Init I2C bus
void SoundPlayer::begin(uint8_t address){

	_address = address;
	Wire.begin();
}

//
// getVersion
//
// Get firmware verseion
uint8_t SoundPlayer::getVersion(){

	uint8_t volume;

	Wire.beginTransmission(_address);
    Wire.write(CMD_GETVERSION);
    Wire.endTransmission();
	//wait for operation
    delay(1);
    //request version value
    Wire.requestFrom((int)_address, 1);
    if (Wire.available() == 1) {
		return Wire.read();
	}
	return 0xFF;
}
//
// play
//
// Play an specific sound file; return true if operation is executed correctly
boolean SoundPlayer::play(unsigned int soundNumber){

	int reply;

	//send command and parameters
	Wire.beginTransmission(_address);
    Wire.write(CMD_PLAY);
    Wire.write(soundNumber >> 8);
    Wire.write(soundNumber & 0x00FF);
    Wire.endTransmission();
	//wait for operation
    delay(10);
    //request status
    Wire.requestFrom((int)_address, 1);
    if (Wire.available() == 1)
		reply = Wire.read();
	return (reply == STATE_PLAY);
}

//
// stop
//
// Stop present sound; return true if operation is executed correctly
boolean SoundPlayer::stop(){

	int reply;

	//send command
	Wire.beginTransmission(_address);
    Wire.write(CMD_STOP);
    Wire.endTransmission();
	//wait for operation
    delay(10);
    //request status
    Wire.requestFrom((int)_address, 1);
    if (Wire.available() == 1)
		reply = Wire.read();
	return (reply == STATE_STOP);
}

//
// pause
//
// Pause or resume present sound; return true if sound is resumed and false if it is paused
boolean SoundPlayer::pause(){
	
	Wire.beginTransmission(_address);
    Wire.write(CMD_PAUSE);
    Wire.endTransmission();
	//wait for operation
    delay(10);
    //request status
    Wire.requestFrom((int)_address, 1);
    if (Wire.available() == 1)
		return (Wire.read() == STATE_PLAY);
	else
		return false;
}

//
// setVolume
//
// Set a specific volume for sound player
void SoundPlayer::setVolume(uint8_t volume){

	Wire.beginTransmission(_address);
    Wire.write(CMD_SETVOLUME);
    Wire.write(volume);
    Wire.endTransmission();
	//wait for operation
    delay(10);
    //request status
    Wire.requestFrom((int)_address, 1);
    if (Wire.available() == 1)
		Wire.read();
}

//
// getVolume
//
// Get present volume for sound player
uint8_t SoundPlayer::getVolume(){

	uint8_t volume;

	Wire.beginTransmission(_address);
    Wire.write(CMD_GETVOLUME);
    Wire.endTransmission();
	//wait for operation
    delay(5);
    //request volume value
    Wire.requestFrom((int)_address, 1);
    if (Wire.available() == 1) {
		volume = Wire.read() - GETVOLUME_BASE;
		if (volume <= 7)
			return volume;
	}
	return 0xFF;
}

