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
JoyState_t joySt;

static int buttons[] = {
  0,   1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
  16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
  0,   1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
  16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
};

void setup() {
#ifdef DEBUG
  Serial1.begin(57600);
  Serial1.println("Initializing...");
#endif

  keyboard1.begin(PS2_1_DAT, PS2_1_IRQ);
  keyboard2.begin(PS2_2_DAT, PS2_2_IRQ);

  resetJoystick();
}

void printAxis(int val) {
  for (int i = -100; i <= 100; i = i + 5) {
    if ( i == 0) {
      Serial1.print("O");
    } else if (i < 0 && i <= val || i > 0 && i >= val) {
      Serial1.print("-");
    } else if (i < 0 && i >= val || i > 0 && i <= val) {
      Serial1.print("*");
    }
  }
}

void resetJoystick() {
  // Reset joystick buttons
  joySt.buttons = 0;
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
  resetJoystick();

  char c;
  if (keyboard1.available()) {
    c = keyboard1.read();
    processKeyInput(c);
  }
  if (keyboard2.available()) {
    c = keyboard2.read();
    processKeyInput(c);
  }
  delay(5);
  
  float x = getXAxis();
  float y = getYAxis();
  
#ifdef DEBUG
  /*
  printAxis(x);
  Serial1.print("    ");
  printAxis(y);
  Serial1.println();
  */
#endif

  // Call Joystick.move
  Joystick.setState(&joySt);

  delay(5);
}

void processKeyInput(char c) {
  int index = getButtonIndex(c);
  if (index < 0)
    return;
  
  uint32_t btn = ((uint32_t) 0x01) << buttons[index];
  joySt.buttons += btn;  
  
#ifdef DEBUG
  Serial1.print("Button: ");
  Serial1.println(btn, HEX);
  Serial1.println(index);
#endif
}


// Get the button number from the keyobard code
int getButtonIndex(char c) {
  switch(c) {
  case '1':
    return 0;
  case '2':
    return 1;
  case '@':
    return 2;
  case '3':
    return 3;
  case '4':
    return 4;
  case '$':
    return 5;
  case '5':
    return 6;
  case '%':
    return 7;
  case '6':
    return 8;
  case '^':
   return 9;
  case '7':
    return 10;
  case '&':
    return 11;
  case '8':
    return 12;
  case '*':
    return 13;
  case '9':
    return 14;
  case '(':
    return 15;
  case 'q':
    return 16;
  case 'w':
    return 17;
  case 'W':
    return 18;
  case 'e':
    return 19;
  case 'r':
    return 20;
  case 'R':
    return 21;
  case 't':
    return 22;
  case 'T':
    return 23;
  case 'y':
    return 24;
  case 'Y':
    return 25;
  case 'u':
    return 26;
  case 'U':
    return 27;
  case 'i':
    return 28;
  case 'I':
    return 29;
  case 'o':
    return 30;
  case 'O':
    return 31;
  case 'a':
    return 32;
  case 's':
    return 33;
  case 'S':
    return 34;
  case 'd':
    return 35;
  case 'f':
    return 36;
  case 'F':
    return 37;
  case 'g':
    return 38;
  case 'G':
    return 39;
  case 'h':
    return 40;
  case 'H':
    return 41;
  case 'j':
    return 42;
  case 'J':
    return 43;
  case 'k':
    return 44;
  case 'K':
    return 45;
  case 'l':
    return 46;
  case 'L':
    return 47;
  case 'z':
    return 48;
  case 'x':
    return 49;
  case 'X':
    return 50;
  case 'c':
    return 51;
  case 'v':
    return 52;
  case 'V':
    return 53;
  case 'b':
    return 54;
  case 'B':
    return 55;
  case 'n':
    return 56;
  case 'N':
    return 57;
  case 'm':
    return 58;
  case 'M':
    return 59;
  case ',':
    return 60;
  case '<':
    return 61;
  case '.':
    return 62;
  case '>':
    return 63;
  default:
    return -1;
  }  
}
