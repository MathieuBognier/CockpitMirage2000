/*
  Tell DCS-BIOS to use a serial connection and use interrupt-driven
  communication. The main program will be interrupted to prioritize
  processing incoming data.
  
  This should work on any Arduino that has an ATMega328 controller
  (Uno, Pro Mini, many others).
 */
#define DCSBIOS_DEFAULT_SERIAL

#include "DcsBios.h"
#include <Timer.h>


   Timer tm;

//Pin
   const int PitotPin = 10;
 //Pin state
   bool PitotPinState;
 //Real values
   const char* PitotReal; 
 //DCS values
   const char* PitotDCS;
 //int to char conversion array
   char *ConvArray[] = {"0", "1"};
 //DCS reading
  void onPitotHeatSwChange(unsigned int newValue) {
    PitotDCS = ConvArray[newValue];
  }
  DcsBios::IntegerBuffer pitotHeatSwBuffer(0x7302, 0x8000, 15, onPitotHeatSwChange);


/* paste code snippets from the reference documentation here */


void setup() {
  DcsBios::setup();
  pinMode(PitotPin, INPUT_PULLUP);
  tm.every(200, pitot);
  tm.every(200, cutoff);
}

void loop() {
  DcsBios::loop();
   tm.update();
}

void pitot(int timer){

  PitotPinState = digitalRead(PitotPin);

  PitotReal = ConvArray[PitotPinState];

  if (PitotDCS == PitotReal && PitotReal == "1")
  {
    sendDcsBiosMessage("PITOT_HEAT_SW", "0");
    sendDcsBiosMessage("PITOT_HEAT_COV", "0");
  }
  if (PitotDCS == PitotReal && PitotReal == "0")
  {
    sendDcsBiosMessage("PITOT_HEAT_COV", "1");
    sendDcsBiosMessage("PITOT_HEAT_SW", "1");
  }

  
}

void cutoff(int timer){
  
}
