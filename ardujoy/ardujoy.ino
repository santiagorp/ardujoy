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

  keyboard1.begin(PS2_1_DAT, PS2_1_IRQ, &onKeyEvent);
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
  uint8_t keyDown;
  if (keyboard1.available()) {    
    c = keyboard1.read(&keyDown);
  }
  if (keyboard2.available()) {
    c = keyboard2.read();
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

// Callback on key up
void onKeyEvent(uint8_t keycode, uint8_t down) {
  int index = getButtonIndex(keycode);
#ifdef DEBUG  
  Serial1.print("Button: ");
  Serial1.print(index);
  if (down)
    Serial1.println(" DOWN");
  else
    Serial1.println(" UP");
#endif
}

/*
void processKeyInput(char c, uint8_t keyDown) {
  int index = getButtonIndex(c);
  if (index < 0 || index > 63)
    return;
  
  uint32_t btn = ((uint32_t) keyDown) << buttons[index];
  joySt.buttons = joySt.buttons | btn;  
}
*/

// Get the button number from the keyobard code
int getButtonIndex(uint8_t keycode) {
  switch(keycode) {
  case 0x1C: // A
    return 0;
  case 0x32: // B
    return 1;
  case 0x21: // C
    return 2;
  case 0x23: // D
    return 3;
  case 0x24: // E
    return 4;
  case 0x2B: // F
    return 5;
  case 0x34: // G
    return 6;
  case 0x33: // H
    return 7;
  case 0x43: // I
    return 8;
  case 0x3B: // J
   return 9;
  case 0x42: // K
    return 10;
  case 0x4B: // L
    return 11;
  case 0x3A: // M
    return 12;
  case 0x31: // N
    return 13;
  case 0x44: // O
    return 14;
  case 0x4D: // P
    return 15;

  case 0x15: // Q
    return 16;
  case 0x2D: // R
    return 17;
  case 0x1B: // S
    return 18;
  case 0x2C: // T
    return 19;
  case 0x3C: // U
    return 20;
  case 0x2A: // V
    return 21;
  case 0x1D: // W
    return 22;
  case 0x22: // X
    return 23;
  case 0x35: // Y
    return 24;
  case 0x1A: // Z
    return 25;
  case 0x45: // 0
    return 26;
  case 0x16: // 1
    return 27;
  case 0x1E: // 2
    return 28;
  case 0x26: // 3
    return 29;
  case 0x25: // 4
    return 30;
  case 0x2E: // 5
    return 31;

  case 0x36: // 6
    return 32;
  case 0x3D: // 7
    return 33;
  case 0x3E: // 8
    return 34;
  case 0x46: // 9
    return 35;
  case 0x4E: // -
    return 36;
  case 0x55: // =
    return 37;
  case 0x5B: // ]
    return 38;
  case 0x29: // SPACE
    return 39;
  case 0x0D: // TAB
    return 40;
  case 0x05: // F1
    return 41;
  case 0x06: // F2
    return 42;
  case 0x04: // F3
    return 43;
  case 0x0C: // F4
    return 44;
  case 0x03: // F5
    return 45;
  case 0x0B:  // F6
    return 46;
  case 0x83:  // F7
    return 47;

  case 0x0A:  // F8
    return 48;
  case 0x01:  // F9
    return 49;
  case 0x09:  // F10
    return 50;
  case 0x78:  // F11
    return 51;
  case 0x07:  // F12
    return 52;
  case 0x54:  // [
    return 53;
  case 0x7C:  // KP *
    return 54;
  case 0x7B:  // KP -
    return 55;
  case 0x79:  // KP +
    return 56;
  case 0x71:  // KP .
    return 57;
  case 0x70:  // KP 0
    return 58;
  case 0x69:  // KP 1
    return 59;
  case 0x72:  // KP 2
    return 60;
  case 0x7A:  // KP 3
    return 61;
  case 0x6B:  // KP 4
    return 62;
  case 0x73 : // KP 5
    return 63;
  default: 
    return -1;
  }  
}
