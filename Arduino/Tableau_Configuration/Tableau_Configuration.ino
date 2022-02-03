/*
  Tell DCS-BIOS to use a serial connection and use interrupt-driven
  communication. The main program will be interrupted to prioritize
  processing incoming data.
  
  This should work on any Arduino that has an ATMega328 controller
  (Uno, Pro Mini, many others).
 */
#define DCSBIOS_IRQ_SERIAL

#include "DcsBios.h"

/* paste code snippets from the reference documentation here */
DcsBios::LED confF(0x7246, 0x0008, 13);
DcsBios::LED confA(0x7246, 0x0004, 12);
DcsBios::LED confFrein(0x7246, 0x0020, 11);
DcsBios::LED confDirav(0x7246, 0x0010, 10);
DcsBios::LED confSpad(0x7246, 0x0080, 9);
DcsBios::LED confRouge(0x7246, 0x0100, 8);
DcsBios::LED confAux(0x7246, 0x0400, 7);
DcsBios::LED confG(0x7246, 0x0200, 6);
DcsBios::LED confD(0x7246, 0x0800, 5);

void setup() {
  DcsBios::setup();
}

void loop() {
  DcsBios::loop();
}
