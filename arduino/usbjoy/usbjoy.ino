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
  /*  printAxis(x);
  Serial.print("    ");
  printAxis(y);
  Serial.println();*/
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

    Serial.print(": ");
    int code = getButtonIndex(c);
    Serial.println(code);
  }
}


// Get the button number from the keyobard code
int getButtonIndex(char c) {
  switch(c) {
  case '1':
    return 1;
  case '2':
    return 2;
  case '@':
    return 3;
  case '3':
    return 4;
  case '4':
    return 5;
  case '$':
    return 6;
  case '5':
    return 7;
  case '%':
    return 8;
  case '6':
    return 9;
  case '^':
   return 10;
  case '7':
    return 11;
  case '&':
    return 12;
  case '8':
    return 13;
  case '*':
    return 14;
  case '9':
    return 15;
  case '(':
    return 16;
  case 'q':
    return 17;
  case 'w':
    return 18;
  case 'W':
    return 19;
  case 'e':
    return 20;
  case 'r':
    return 21;
  case 'R':
    return 22;
  case 't':
    return 23;
  case 'T':
    return 24;
  case 'y':
    return 25;
  case 'Y':
    return 26;
  case 'u':
    return 27;
  case 'U':
    return 28;
  case 'i':
    return 29;
  case 'I':
    return 30;
  case 'o':
    return 31;
  case 'O':
    return 32;
  case 'a':
    return 33;
  case 's':
    return 34;
  case 'S':
    return 35;
  case 'd':
    return 36;
  case 'f':
    return 37;
  case 'F':
    return 38;
  case 'g':
    return 39;
  case 'G':
    return 40;
  case 'h':
    return 41;
  case 'H':
    return 42;
  case 'j':
    return 43;
  case 'J':
    return 44;
  case 'k':
    return 45;
  case 'K':
    return 46;
  case 'l':
    return 47;
  case 'L':
    return 48;
  case 'z':
    return 49;
  case 'x':
    return 50;
  case 'X':
    return 51;
  case 'c':
    return 52;
  case 'v':
    return 53;
  case 'V':
    return 54;
  case 'b':
    return 55;
  case 'B':
    return 56;
  case 'n':
    return 57;
  case 'N':
    return 58;
  case 'm':
    return 59;
  case 'M':
    return 60;
  case ',':
    return 61;
  case '<':
    return 62;
  case '.':
    return 63;
  case '>':
    return 64;
  default:
    return -1;
  }  
}
