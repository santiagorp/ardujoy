#ifndef _ARDUJOY_H_
#define _ARDUJOY_H_

// Commands
#define AXIS_0         0x00
#define AXIS_1         0x01
#define AXIS_2         0x02
#define AXIS_3         0x03
#define AXIS_4         0x04
#define AXIS_5         0x05
#define AXIS_6         0x06
#define AXIS_7         0x07
#define BUTTON_RELEASE 0xA0
#define BUTTON_PRESSED 0xA0

struct ardujoy_data {
  int16_t axis[8];
  int16_t pressed_buttons;
  int16_t release_buttons;
};

#endif // _ARDUJOY_H_
