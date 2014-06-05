#include <Wire.h> //Incluimos la librería para comunicación I2C
#include <SoundPlayer.h>

SoundPlayer soundPlayer;

void setup() {

  Serial.begin(9600);
  Serial.println("Master test program");

  soundPlayer.begin(0x61);

  int ver = soundPlayer.getVersion();  
  switch(ver)
  {
  case 0x10:
    Serial.println("Version: 1.0)");
    break;
  }
  
  soundPlayer.play(0x0);
    Serial.println("Playing song 0...");
}

void loop()
{
   int volume;
   
  soundPlayer.setVolume(2);
  volume = soundPlayer.getVolume();
  Serial.print("Volume=");
  Serial.println(volume,DEC);

  delay(1000);

  soundPlayer.setVolume(7);
  volume = soundPlayer.getVolume();
  Serial.print("Volume=");
  Serial.println(volume,DEC);
  delay(1000);
}

