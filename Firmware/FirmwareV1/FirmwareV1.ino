#include <Wire.h>
#include "SOMO14D.h"

//Firmware version
#define VERSION        0
//I2C address
#define I2C_ADDRESS    0x61
//IO pins
#define I2C_TRANSFER   5
#define PLAYER_STATE   6
#define PLAYER_SCK     A1
#define PLAYER_DATA    A2
#define PLAYER_BUSY    A3

//#define DEBUG
#ifdef DEBUG
//#define COMMAND_DEBUG
#define OPERATION_TIME
#endif

//SOMO14D sound player
SOMO14D player;
byte command = 0xFF;
byte reply = 0xFF;

//
// setup
//
void setup() {

#ifdef DEBUG
  Serial.begin(9600);
  Serial.print("Start I2C Sound player program");
#endif

  //setup state LED pins
  pinMode(I2C_TRANSFER, OUTPUT);
  digitalWrite(I2C_TRANSFER, LOW);
  pinMode(PLAYER_STATE, OUTPUT);
  digitalWrite(PLAYER_STATE, LOW);
  //init I2C bus
  Wire.begin(I2C_ADDRESS);
  Wire.onReceive(receiveEvent);   //reception event
  Wire.onRequest(requestEvent);   //request event
  //init sound player
  player.begin(PLAYER_SCK, PLAYER_DATA, PLAYER_BUSY);
  //stop any sound
  player.stop();
}

//
// loop
//
void loop() {

  //update player state LED
  if (player.getState())
    digitalWrite(PLAYER_STATE, HIGH);
  else
    digitalWrite(PLAYER_STATE, LOW);
  delay(10);
}

//
// receiveEvent
//
// This function is executed when the I2C master sends data
//
void receiveEvent(int howMany) {

#ifdef OPERATION_TIME
  long opTime = micros();
#endif
  //I2C LED on
  digitalWrite(I2C_TRANSFER, HIGH);
  //get commmand and set default state for request
  command = Wire.read();
  reply = 0x4F;
#ifdef COMMAND_DEBUG
  Serial.print("Nuevo comando=");
  Serial.println(command, HEX);
#endif
  //esecute command
  switch (command)
  {
  case 0x10:  //get version
    //no operation, request set firmware version
#ifdef COMMAND_DEBUG
    Serial.print("  Solicitar version");
#endif
    break;
  case 0x40:  //play sound
    if (howMany == 3) {
      //get song number and notify to sound player
      unsigned int songNumber = (Wire.read() << 8) + Wire.read();
      if ((songNumber <=0x1FF) && player.play(songNumber)) {
        reply = 0x4E;  //update sate for request
#ifdef COMMAND_DEBUG
        Serial.print("  Reproducir sonido numero ");
        Serial.println(songNumber, HEX);
#endif
      }
      else
        reply = 0xFF;    //song number incorrect
    }
    else
      reply = 0xFF;    //song number not specified
    break;
  case 0x41:  //stop sound
    player.stop();
#ifdef COMMAND_DEBUG
    Serial.println("  Detener sonido");
#endif
    break;
  case 0x42:  //pause or resume sound
    if (player.pause())
      reply = 0x4E;
    break;
#ifdef COMMAND_DEBUG
    Serial.println("  Pausar sonido");
#endif
  case 0x50:  //set volume
    if (howMany == 2) {
      //get volume, validate and notify to sound player
      byte volume = Wire.read();
      if (volume <= 7) {
        player.setVolume(volume);
        reply = 0x5F;
#ifdef COMMAND_DEBUG
        Serial.print("  Establecer volumen a ");
        Serial.println(volume, HEX);
#endif
      }
      else
        reply = 0xFF;  //volume incorrect
    }
    else
      reply = 0xFF;  //volume not specified
    break;
  case 0x51:  //getVolume
    //no operation, request set volume
#ifdef COMMAND_DEBUG
    Serial.print("  Solicitar volumen");
#endif
    break;
  }
  //clear I2C buffer
  while (Wire.available() != 0)
    Wire.read();
#ifdef OPERATION_TIME
  opTime = micros() - opTime;
  Serial.print("Tiempo de operación: ");
  Serial.print(opTime, DEC);
  Serial.println(" microsegundos");
#endif
}

//
// requestEvent
//
// This function is executed when the I2C master request for data
// Request depend on the last command received
//
void requestEvent() {

  int volume;

#ifdef COMMAND_DEBUG
  Serial.print("Respuesta al comando ");
  Serial.print(command, HEX);
  Serial.print(": ");
#endif
  switch (command)
  {
  case 0x10:  //get version
    //return firmware version
    Wire.write(0x10 + VERSION);
#ifdef COMMAND_DEBUG
    Serial.println(0x10 + VERSION, HEX);
#endif
    break;
  case 0x40:  //play sound
  case 0x41:  //stop sound
  case 0x42:  //pause o resume sound
    Wire.write(reply);    //return sound player state
#ifdef COMMAND_DEBUG
    Serial.println(reply, HEX);
#endif
    break;
  case 0x50:  //set volume
    Wire.write(reply);    //return reply from receive event
#ifdef COMMAND_DEBUG
    Serial.println(reply, HEX);
#endif
    break;
  case 0x51:  //get volume
    //return sound player volume
    volume = player.getVolume();
    Wire.write(0x50 + volume);
#ifdef COMMAND_DEBUG
    Serial.println(0x50 + volume, HEX);
#endif
    break;
  default:    //other
    Wire.write(0xFF);  //error
#ifdef COMMAND_DEBUG
    Serial.println(0xFF, HEX);
#endif
    break;
  }
  //reset command
  command = 0xFF;
  //I2C LED off
  digitalWrite(I2C_TRANSFER, LOW);
}



