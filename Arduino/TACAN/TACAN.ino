/*
  Tell DCS-BIOS to use a serial connection and use the default Arduino Serial
  library. This will work on the vast majority of Arduino-compatible boards,
  but you can get corrupted data if you have too many or too slow outputs
  (e.g. when you have multiple character displays), because the receive
  buffer can fill up if the sketch spends too much time updating them.
  
  If you can, use the IRQ Serial connection instead.
*/
#define DCSBIOS_DEFAULT_SERIAL

#include "DcsBios.h"
#include <Timer.h>

   Timer tm;

   //Pin
   const int RDigit1Pin =  2;
   const int RDigit2Pin =  3;
   const int RDigit3Pin =  4;
   const int RDigit4Pin =  5;
   const int LDigit1Pin =  6;
   const int LDigit2Pin =  7;
   const int LDigit3Pin =  8;
   const int LDigit4Pin =  9;
   const int XYPin =  10;
   const int BackLightPin = 11;
   const int Mode1Pin =  A0;
   const int Mode2Pin =  A1;
   const int Mode3Pin =  A2;
   const int TestPin = A3;
   const int RedLightPin = 12;
   const int GreenLightPin = 13;

   //Pin state
   bool RDigit1, RDigit2, RDigit3, RDigit4;
   bool LDigit1, LDigit2, LDigit3, LDigit4;
   bool Mode1, Mode2, Mode3;
   bool XYval;
   bool Test;

   //Real Tacan values
   const char* RightDigit;
   const char* LeftDigit;
   const char* Mode; 
   const char* XY;

   //DCS tacan values
   const char* RightDigitDCS;
   const char* LeftDigitDCS;
   const char* ModeDCS;
   const char* XYDCS;

   //int to char conversion array
   char *ConvArray[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12"};

   //DCS tacan reading
   void onTacCh1SelChange(unsigned int newValue) {
    RightDigitDCS = ConvArray[newValue];
   }
   DcsBios::IntegerBuffer tacCh1SelBuffer(0x7322, 0x00f0, 4, onTacCh1SelChange);

   void onTacCh10SelChange(unsigned int newValue) {
    LeftDigitDCS = ConvArray[newValue];
   }
   DcsBios::IntegerBuffer tacCh10SelBuffer(0x7322, 0x000f, 0, onTacCh10SelChange);

   void onTacModeSelChange(unsigned int newValue) {
    ModeDCS = ConvArray[newValue];
   }
   DcsBios::IntegerBuffer tacModeSelBuffer(0x7316, 0x6000, 13, onTacModeSelChange);

   void onTacXYSelChange(unsigned int newValue) {
    XYDCS = ConvArray[newValue];
   }
   DcsBios::IntegerBuffer tacXYSelBuffer(0x7316, 0x8000, 15, onTacXYSelChange);

   void onConsPanelLgtKnobChange(unsigned int newValue) {
    analogWrite(BackLightPin, int(newValue/256));
   }
   DcsBios::IntegerBuffer consPanelLgtKnobBuffer(0x72a6, 0xffff, 0, onConsPanelLgtKnobChange);


void setup() {
  DcsBios::setup();

  pinMode(RDigit1Pin, INPUT_PULLUP);
  pinMode(RDigit2Pin, INPUT_PULLUP);
  pinMode(RDigit3Pin, INPUT_PULLUP);
  pinMode(RDigit4Pin, INPUT_PULLUP);
  pinMode(LDigit1Pin, INPUT_PULLUP);
  pinMode(LDigit2Pin, INPUT_PULLUP);
  pinMode(LDigit3Pin, INPUT_PULLUP);
  pinMode(LDigit4Pin, INPUT_PULLUP);
  pinMode(XYPin, INPUT_PULLUP);
  pinMode(Mode1Pin, INPUT_PULLUP);
  pinMode(Mode2Pin, INPUT_PULLUP);
  pinMode(Mode3Pin, INPUT_PULLUP);
  pinMode(TestPin, INPUT_PULLUP);
  pinMode(RedLightPin, OUTPUT);
  pinMode(GreenLightPin,OUTPUT);

  tm.every(200, setTacan);
}

void loop() {
  DcsBios::loop();
  tm.update();
}

void setTacan(){

  //Real Tacan reading
  RDigit1 = digitalRead(RDigit1Pin);
  RDigit2 = digitalRead(RDigit2Pin);
  RDigit3 = digitalRead(RDigit3Pin);
  RDigit4 = digitalRead(RDigit4Pin);
  LDigit1 = digitalRead(LDigit1Pin);
  LDigit2 = digitalRead(LDigit2Pin);
  LDigit3 = digitalRead(LDigit3Pin);
  LDigit4 = digitalRead(LDigit4Pin);
  XYval = digitalRead(XYPin);
  Mode1 = digitalRead(Mode1Pin);
  Mode2 = digitalRead(Mode2Pin);
  Mode3 = digitalRead(Mode3Pin);
  Test = digitalRead(TestPin);

  //Real Tacan right digit decryption
  if (RDigit1 == 1 && RDigit2 == 1 && RDigit3 == 1 && RDigit4 == 1)
    {RightDigit = "0";}
  if (RDigit1 == 0 && RDigit2 == 1 && RDigit3 == 1 && RDigit4 == 1)
    {RightDigit = "1";}
  if (RDigit1 == 1 && RDigit2 == 0 && RDigit3 == 1 && RDigit4 == 1)
    {RightDigit = "2";}
  if (RDigit1 == 0 && RDigit2 == 0 && RDigit3 == 1 && RDigit4 == 1)
    {RightDigit = "3";}
  if (RDigit1 == 1 && RDigit2 == 1 && RDigit3 == 0 && RDigit4 == 1)
    {RightDigit = "4";}
  if (RDigit1 == 0 && RDigit2 == 1 && RDigit3 == 0 && RDigit4 == 1)
    {RightDigit = "5";}
  if (RDigit1 == 1 && RDigit2 == 0 && RDigit3 == 0 && RDigit4 == 1)
    {RightDigit = "6";}
  if (RDigit1 == 0 && RDigit2 == 0 && RDigit3 == 0 && RDigit4 == 1)
    {RightDigit = "7";}
  if (RDigit1 == 1 && RDigit2 == 1 && RDigit3 == 1 && RDigit4 == 0)
    {RightDigit = "8";}
  if (RDigit1 == 0 && RDigit2 == 1 && RDigit3 == 1 && RDigit4 == 0)
    {RightDigit = "9";}

  //Real Tacan left digit decryption
  if (LDigit1 == 1 && LDigit2 == 1 && LDigit3 == 1 && LDigit4 == 1)
    {LeftDigit = "0";}
  if (LDigit1 == 0 && LDigit2 == 1 && LDigit3 == 1 && LDigit4 == 1)
    {LeftDigit = "1";}
  if (LDigit1 == 1 && LDigit2 == 0 && LDigit3 == 1 && LDigit4 == 1)
    {LeftDigit = "2";}
  if (LDigit1 == 0 && LDigit2 == 0 && LDigit3 == 1 && LDigit4 == 1)
    {LeftDigit = "3";}
  if (LDigit1 == 1 && LDigit2 == 1 && LDigit3 == 0 && LDigit4 == 1)
    {LeftDigit = "4";}
  if (LDigit1 == 0 && LDigit2 == 1 && LDigit3 == 0 && LDigit4 == 1)
    {LeftDigit = "5";}
  if (LDigit1 == 1 && LDigit2 == 0 && LDigit3 == 0 && LDigit4 == 1)
    {LeftDigit = "6";}
  if (LDigit1 == 0 && LDigit2 == 0 && LDigit3 == 0 && LDigit4 == 1)
    {LeftDigit = "7";}
  if (LDigit1 == 1 && LDigit2 == 1 && LDigit3 == 1 && LDigit4 == 0)
    {LeftDigit = "8";}
  if (LDigit1 == 0 && LDigit2 == 1 && LDigit3 == 1 && LDigit4 == 0)
    {LeftDigit = "9";}
  if (LDigit1 == 1 && LDigit2 == 0 && LDigit3 == 1 && LDigit4 == 0)
    {LeftDigit = "10";}
  if (LDigit1 == 0 && LDigit2 == 0 && LDigit3 == 1 && LDigit4 == 0)
    {LeftDigit = "11";}
  if (LDigit1 == 1 && LDigit2 == 1 && LDigit3 == 0 && LDigit4 == 0)
    {LeftDigit = "12";}

  //Real Tacan mode decryption
  if (Mode1 == 1 && Mode2 == 1 && Mode3 == 1)
    {Mode = "0";}
  if (Mode1 == 0 && Mode2 == 1 && Mode3 == 1)
    {Mode = "1";}
  if (Mode1 == 0 && Mode2 == 0 && Mode3 == 1)
    {Mode = "2";}
  if (Mode1 == 0 && Mode2 == 0 && Mode3 == 0)
    {Mode = "3";}

  XY = ConvArray[XYval];
  
  //Sending of the new values if they are different from the DCS one
  if (RightDigitDCS != RightDigit)
  {
    sendDcsBiosMessage("TAC_CH_1_SEL", RightDigit);
  }
  if (LeftDigitDCS != LeftDigit)
  {
    sendDcsBiosMessage("TAC_CH_10_SEL", LeftDigit);
  }
  if (ModeDCS != Mode)
  {
    sendDcsBiosMessage("TAC_MODE_SEL", Mode);
  }
  if (XYDCS != XY)
  {
    sendDcsBiosMessage("TAC_X_Y_SEL", XY);
  }

  if (Test == 0)
  {
      digitalWrite(GreenLightPin, HIGH);
      delay (500);
      digitalWrite(GreenLightPin, LOW);
      digitalWrite(RedLightPin, HIGH);
      delay (500);
      digitalWrite(RedLightPin, LOW);
      digitalWrite(GreenLightPin, HIGH);
      delay (500);
      digitalWrite(GreenLightPin, LOW);
      digitalWrite(RedLightPin, HIGH);
      delay (500);
      digitalWrite(RedLightPin, LOW);
      digitalWrite(GreenLightPin, HIGH);
      delay (500);
      digitalWrite(GreenLightPin, LOW);
      digitalWrite(RedLightPin, HIGH);
      delay (500);
      digitalWrite(RedLightPin, LOW);
      digitalWrite(GreenLightPin, HIGH);
      delay (2000);
      digitalWrite(GreenLightPin, LOW);
  }
}
