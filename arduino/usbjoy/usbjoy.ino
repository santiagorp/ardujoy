/* Blink without Delay

   Turns on and off a light emitting diode(LED) connected to a digital
   pin, without using the delay() function.  This means that other code
   can run at the same time without being interrupted by the LED code.
*/

#include "Arduino.h"
#include "PS2Keyboard.h"

const int PS2_1_IRQ = 2; // clk
const int PS2_1_DAT = 4;

const int PS2_2_IRQ = 3; // clk
const int PS2_2_DAT = 5;

PS2Keyboard1 keyboard1;
PS2Keyboard2 keyboard2;

void setup() {
  Serial.begin(57600);

  keyboard1.begin(PS2_1_DAT, PS2_1_IRQ);
  keyboard2.begin(PS2_2_DAT, PS2_2_IRQ);
}

void printAxis(int val) {
  for (int i = -100; i <= 100; i = i + 5) {
    if ( i == 0) {
      Serial.print("O");
    } else if (i < 0 && i <= val || i > 0 && i >= val) {
      Serial.print("-");
    } else if (i < 0 && i >= val || i > 0 && i <= val) {
      Serial.print("*");
    }
  }
}

float getXAxis() {
  // Resistencia de 100K en la salida
  float r1 = (1023.0 / (float) analogRead(A1) - 1) * 100000;
  // Measured 47000 in extreme
  float val = (r1 - 76000) / 470;
  float dz = 5;
  int sign = val >= 0 ? 1 : -1;

  if (abs(val) < dz) {
    val = 0;
  } else if (sign > 0 && val > dz) {
    val = val * .9 - dz;
  } else if (sign < 0 && val < -dz) {
    val = val * 1.05 + dz;
  }

  val = val > 100 ? 100 : val;
  val = val < -100 ? -100 : val;

  return val;
}

float getYAxis() {
  // Resistencia de 100K en la salida
  float r1 = (1023.0 / (float) analogRead(A0) - 1) * 100000;
  // Measured 47000 in extreme
  float val = (r1 - 76000) / -470;
  float dz = 5;
  int sign = val >= 0 ? 1 : -1;

  if (abs(val) < dz) {
    val = 0;
  } else if (sign > 0 && val > dz) {
    val = val * .9 - dz;
  } else if (sign < 0 && val < -dz) {
    val = val * 1.05 + dz;
  }

  val = val > 100 ? 100 : val;
  val = val < -100 ? -100 : val;

  return val;
}


void loop() {
  char c;
  if (keyboard1.available()) {
    c = keyboard1.read();
    processKeyInput(c);
  }
  if (keyboard2.available()) {
    c = keyboard2.read();
    processKeyInput(c);
  }
  delay(50);
  
  float x = getXAxis();
  float y = getYAxis();
  printAxis(x);
  Serial.print("    ");
  printAxis(y);
  Serial.println();
}

void processKeyInput(char c) {
  // check for some of the special keys
  if (c == PS2_ENTER) {
    Serial.println();
  } else if (c == PS2_TAB) {
    Serial.print("[Tab]");
  } else if (c == PS2_ESC) {
    Serial.print("[ESC]");
  } else if (c == PS2_PAGEDOWN) {
    Serial.print("[PgDn]");
  } else if (c == PS2_PAGEUP) {
    Serial.print("[PgUp]");
  } else if (c == PS2_LEFTARROW) {
    Serial.print("[Left]");
  } else if (c == PS2_RIGHTARROW) {
    Serial.print("[Right]");
  } else if (c == PS2_UPARROW) {
    Serial.print("[Up]");
  } else if (c == PS2_DOWNARROW) {
    Serial.print("[Down]");
  } else if (c == PS2_DELETE) {
    Serial.print("[Del]");
  } else {

    // otherwise, just print all normal characters
    Serial.print(c);
  }
}
