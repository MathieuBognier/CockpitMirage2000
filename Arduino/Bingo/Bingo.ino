/*
  Tell DCS-BIOS to use a serial connection and use interrupt-driven
  communication. The main program will be interrupted to prioritize
  processing incoming data.
  
  This should work on any Arduino that has an ATMega328 controller
  (Uno, Pro Mini, many others).
 */
#define DCSBIOS_IRQ_SERIAL

#include "DcsBios.h"
#include <Timer.h>

   Timer tm;

//Pin
   const int LDigitPin1 =  8;
   const int LDigitPin2 =  9;
   const int LDigitPin4 =  10;
   const int LDigitPin8 =  11;
   const int RDigitPin1 =  4;
   const int RDigitPin2 =  5;
   const int RDigitPin4 =  6;
   const int RDigitPin8 =  7;

 //Pin state
   bool LDigit1, LDigit2, LDigit4, LDigit8;
   bool RDigit1, RDigit2, RDigit4, RDigit8;

 //Real values
   const char* RDigit;
   const char* LDigit;

 //DCS tacan values
   const char* RDigitDCS;
   const char* LDigitDCS;

 //int to char conversion array
   char *ConvArray[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};

 //DCS reading

   void onBingoFuel1kKgSelChange(unsigned int newValue) {
    LDigitDCS = ConvArray[newValue];
   }
   DcsBios::IntegerBuffer bingoFuel1kKgSelBuffer(0x725c, 0x7800, 11, onBingoFuel1kKgSelChange);

   void onBingoFuel100KgSelChange(unsigned int newValue) {
    RDigitDCS = ConvArray[newValue];
   }
   DcsBios::IntegerBuffer bingoFuel100KgSelBuffer(0x7260, 0x000f, 0, onBingoFuel100KgSelChange);
  

/* paste code snippets from the reference documentation here */

void setup() {
  DcsBios::setup();
  pinMode(LDigitPin1, INPUT_PULLUP);
  pinMode(LDigitPin2, INPUT_PULLUP);
  pinMode(LDigitPin4, INPUT_PULLUP);
  pinMode(LDigitPin8, INPUT_PULLUP);
  pinMode(RDigitPin1, INPUT_PULLUP);
  pinMode(RDigitPin2, INPUT_PULLUP);
  pinMode(RDigitPin4, INPUT_PULLUP);
  pinMode(RDigitPin8, INPUT_PULLUP);

  tm.every(200, bingo);
}

void loop() {
   DcsBios::loop();
   tm.update();
}

void bingo(int timer){
  RDigit1 = digitalRead(RDigitPin1);
  RDigit2 = digitalRead(RDigitPin2);
  RDigit4 = digitalRead(RDigitPin4);
  RDigit8 = digitalRead(RDigitPin8);
  LDigit1 = digitalRead(LDigitPin1);
  LDigit2 = digitalRead(LDigitPin2);
  LDigit4 = digitalRead(LDigitPin4);
  LDigit8 = digitalRead(LDigitPin8);

  //Right digit decryption
  if (RDigit1 == 1 && RDigit2 == 1 && RDigit4 == 1 && RDigit8 == 1)
    {RDigit = "0";}
  if (RDigit1 == 0 && RDigit2 == 1 && RDigit4 == 1 && RDigit8 == 1)
    {RDigit = "1";}
  if (RDigit1 == 1 && RDigit2 == 0 && RDigit4 == 1 && RDigit8 == 1)
    {RDigit = "2";}
  if (RDigit1 == 0 && RDigit2 == 0 && RDigit4 == 1 && RDigit8 == 1)
    {RDigit = "3";}
  if (RDigit1 == 1 && RDigit2 == 1 && RDigit4 == 0 && RDigit8 == 1)
    {RDigit = "4";}
  if (RDigit1 == 0 && RDigit2 == 1 && RDigit4 == 0 && RDigit8 == 1)
    {RDigit = "5";}
  if (RDigit1 == 1 && RDigit2 == 0 && RDigit4 == 0 && RDigit8 == 1)
    {RDigit = "6";}
  if (RDigit1 == 0 && RDigit2 == 0 && RDigit4 == 0 && RDigit8 == 1)
    {RDigit = "7";}
  if (RDigit1 == 1 && RDigit2 == 1 && RDigit4 == 1 && RDigit8 == 0)
    {RDigit = "8";}
  if (RDigit1 == 0 && RDigit2 == 1 && RDigit4 == 1 && RDigit8 == 0)
    {RDigit = "9";}
  
  //Left digit decryption
  if (LDigit1 == 1 && LDigit2 == 1 && LDigit4 == 1 && LDigit8 == 1)
    {LDigit = "0";}
  if (LDigit1 == 0 && LDigit2 == 1 && LDigit4 == 1 && LDigit8 == 1)
    {LDigit = "1";}
  if (LDigit1 == 1 && LDigit2 == 0 && LDigit4 == 1 && LDigit8 == 1)
    {LDigit = "2";}
  if (LDigit1 == 0 && LDigit2 == 0 && LDigit4 == 1 && LDigit8 == 1)
    {LDigit = "3";}
  if (LDigit1 == 1 && LDigit2 == 1 && LDigit4 == 0 && LDigit8 == 1)
    {LDigit = "4";}
  if (LDigit1 == 0 && LDigit2 == 1 && LDigit4 == 0 && LDigit8 == 1)
    {LDigit = "5";}
  if (LDigit1 == 1 && LDigit2 == 0 && LDigit4 == 0 && LDigit8 == 1)
    {LDigit = "6";}
  if (LDigit1 == 0 && LDigit2 == 0 && LDigit4 == 0 && LDigit8 == 1)
    {LDigit = "7";}
  if (LDigit1 == 1 && LDigit2 == 1 && LDigit4 == 1 && LDigit8 == 0)
    {LDigit = "8";}
  if (LDigit1 == 0 && LDigit2 == 1 && LDigit4 == 1 && LDigit8 == 0)
    {LDigit = "9";}

  //Sending of the new values if they are different from the DCS one
  if (RDigitDCS != RDigit)
  {
    sendDcsBiosMessage("BINGO_FUEL_100_KG_SEL", RDigit);
  }

  if (LDigitDCS != LDigit)
  {
    sendDcsBiosMessage("BINGO_FUEL_1K_KG_SEL", LDigit);
  }
}
