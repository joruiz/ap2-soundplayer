/*
  BasicPlayer
  Play the first song of the microSD memory
 
  This example code is in the public domain.
 */
 
#include "SOMO14D.h"

SOMO14D somo14d;

void setup() {
  
  somo14d.begin(A1, A2, A3);
  somo14d.play(0);
}

void loop() {

  delay(10);
}
