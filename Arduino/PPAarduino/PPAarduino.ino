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
#include <Wire.h>
#include <Timer.h>

Timer tm;

/*********** Common variables ***********/

uint8_t convTable[] = {
        B00111111,  //  0
        B00000110,  //  1
        B01011011,  //  2
        B01001111,  //  3
        B01100110,  //  4
        B01101101,  //  5
        B01111100,  //  6
        B00000111,  //  7
        B01111111,  //  8
        B01100111,  //  9
        B00000000,  //  NULL       
    };
    
/*********** PPA variables ***********/

const int addrPPA = 0x70;
uint8_t ppaS530PLight = 0;
uint8_t ppaMISLight = 0;
uint8_t ppaAUTLight = 0;
uint8_t ppaMANLight = 0;
uint8_t ppaMagicPLight = 0;
uint8_t ppaMAGLight = 0;
uint8_t ppaTOTLight = 0;
uint8_t ppaPARLight = 0;
uint8_t ppa7segDatas[4];

uint8_t ks40h, ks41h, ks42h, ks43h, ks44h, ks45h;

const uint8_t bombRelQtySelP = B00000001;
const uint8_t bombRelQtySelM = B00000010;

/*********** DCS snippets ***********/

void onPpaS530PChange(unsigned int newValue) {
  
  if (newValue == 1) {
    ppaS530PLight = 1; //1 = b00000001
  }
  else{
    ppaS530PLight = 0;
  }
  updateLights();
}
DcsBios::IntegerBuffer ppaS530PBuffer(0x72e0, 0x0400, 10, onPpaS530PChange);

void onPpaS530MisChange(unsigned int newValue) {

  if (newValue == 1) {
    ppaMISLight = 2; //2 = b00000010
  }
  else{
    ppaMISLight = 0;
  }
  updateLights();
}
DcsBios::IntegerBuffer ppaS530MisBuffer(0x72e0, 0x0800, 11, onPpaS530MisChange);

void onPpaAutoChange(unsigned int newValue) {
  
  if (newValue == 1) {
    ppaAUTLight = 4; //4 = b00000100
  }
  else{
    ppaAUTLight = 0;
  }
  updateLights();
}
DcsBios::IntegerBuffer ppaAutoBuffer(0x72e0, 0x1000, 12, onPpaAutoChange);

void onPpaManChange(unsigned int newValue) {
  
  if (newValue == 1) {
    ppaMANLight = 8; //8 = b00001000
  }
  else{
    ppaMANLight = 0;
  }
  updateLights();
}
DcsBios::IntegerBuffer ppaManBuffer(0x72e0, 0x2000, 13, onPpaManChange);

void onPpaMagicPChange(unsigned int newValue) {
  
  if (newValue == 1) {
    ppaMagicPLight = 16; //16 = b00010000
  }
  else{
    ppaMagicPLight = 0;
  }
  updateLights();
}
DcsBios::IntegerBuffer ppaMagicPBuffer(0x72e0, 0x4000, 14, onPpaMagicPChange);

void onPpaMagicMisChange(unsigned int newValue) {
  
   if (newValue == 1) {
    ppaMAGLight = 32; //32 = b00100000
  }
  else{
    ppaMAGLight = 0;
  }
  updateLights();
}
DcsBios::IntegerBuffer ppaMagicMisBuffer(0x72e0, 0x8000, 15, onPpaMagicMisChange);

void onPpaGunRocketTotChange(unsigned int newValue) {
  
   if (newValue == 1) {
    ppaTOTLight = 64; //64 = b01000000
  }
  else{
    ppaTOTLight = 0;
  }
  updateLights();
}
DcsBios::IntegerBuffer ppaGunRocketTotBuffer(0x72e4, 0x0200, 9, onPpaGunRocketTotChange);

void onPpaGunRocketPapChange(unsigned int newValue) {
  
   if (newValue == 1) {
    ppaPARLight = 128; //128 = b10000000
  }
  else{
    ppaPARLight = 0;
  }
  updateLights();
}
DcsBios::IntegerBuffer ppaGunRocketPapBuffer(0x72e4, 0x0100, 8, onPpaGunRocketPapChange);

void onPpaQtyDispChange(char* newValue) {
    ppa7segDatas[0] = convTable[newValue[0]-48]; //ASCII code for '0' is 48
    ppa7segDatas[1] = convTable[newValue[1]-48];
    update7Seg();
}
DcsBios::StringBuffer<2> ppaQtyDispBuffer(0x7308, onPpaQtyDispChange);

void onPpaIntDispChange(char* newValue) {
    ppa7segDatas[2] = convTable[newValue[0]-48];//ASCII code for '0' is 48
    ppa7segDatas[3] = convTable[newValue[1]-48];
    update7Seg();
}
DcsBios::StringBuffer<2> ppaIntDispBuffer(0x730a, onPpaIntDispChange);

/*********** setup and loop ***********/

void setup() {

  DcsBios::setup();
  Wire.begin();

    /* Initiate PPA HT16K33 */
  Wire.beginTransmission(addrPPA);
  Wire.write(0x20 | 1); // Turn oscillator on
  Wire.endTransmission();

  Wire.beginTransmission(addrPPA);
  Wire.write(0x80 | 1); // Turn display on
  Wire.endTransmission();

  setBrightness(15);

  tm.every(200, setPPA);
}

void loop() {
  DcsBios::loop();
  tm.update();
}

/*********** Functions definitions ***********/

void updateLights(){
  
  Wire.beginTransmission(addrPPA);
  Wire.write(0x00); //Start at adress 0x00 (Lights are connected to COM0)
  Wire.write(ppaS530PLight + ppaMISLight + ppaAUTLight + ppaMANLight + ppaMagicPLight + ppaMAGLight + ppaTOTLight + ppaPARLight);//Setting ROW0 to ROW7
  Wire.endTransmission(); 
}

void update7Seg(){
  
  Wire.beginTransmission(addrPPA);
  Wire.write(0x08); // Start at address 0x08 (First 7 segments display is connected to COM4)
    for (int i = 0; i < 4; i++) {
      Wire.write(ppa7segDatas[i]);//Setting ROW0 to ROW7
      Wire.write(0x00); //Setting ROW8 to ROW15 at 0
    } 
  Wire.endTransmission();  
}


void setBrightness(uint8_t b){
  
  if(b > 15) return;
  
  Wire.beginTransmission(addrPPA);
  Wire.write(0xE0 | b); // Dimming command
  Wire.endTransmission();
}

void setPPA(){

   Wire.beginTransmission(addrPPA);
   Wire.write(0x40);
   Wire.endTransmission();
   Wire.requestFrom(addrPPA,6);
   
   if(Wire.available ()<=6)
    {
      ks40h = Wire.read();
      ks41h = Wire.read();
      ks42h = Wire.read();
      ks43h = Wire.read(); 
      ks44h = Wire.read();
      ks45h = Wire.read();
    }

  if (ks41h&bombRelQtySelP){sendDcsBiosMessage("HUD_RALT_SEL_SW", "1");}
  else {sendDcsBiosMessage("HUD_RALT_SEL_SW", "0");}

  if (ks43h&bombRelQtySelM){sendDcsBiosMessage("HUD_RALT_SEL_SW", "1");}
  else {sendDcsBiosMessage("HUD_RALT_SEL_SW", "0");}
  
}
