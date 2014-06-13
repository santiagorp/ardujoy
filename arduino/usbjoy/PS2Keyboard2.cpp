/*
  PS2Keyboard.cpp - PS2Keyboard library
  Copyright (c) 2007 Free Software Foundation.  All right reserved.
  Written by Christian Weichel <info@32leaves.net>

  ** Mostly rewritten Paul Stoffregen <paul@pjrc.com> 2010, 2011
  ** Modified for use beginning with Arduino 13 by L. Abraham Smith, <n3bah@microcompdesign.com> * 
  ** Modified for easy interrup pin assignement on method begin(datapin,irq_pin). Cuningan <cuninganreset@gmail.com> **

  for more information you can read the original wiki in arduino.cc
  at http://www.arduino.cc/playground/Main/PS2Keyboard
  or http://www.pjrc.com/teensy/td_libs_PS2Keyboard.html

  Version 2.3-Ctrl-Enter (September 2012)
  - Add Ctrl+Enter (Ctrl+J) as PS2_LINEFEED, code 10
  - Move PS2_DOWNARROW to code 12

  Version 2.3-Ctrl (June 2012)
  - Reintroduce Ctrl

  Version 2.3 (October 2011)
  - Minor bugs fixed

  Version 2.2 (August 2011)
  - Support non-US keyboards - thanks to Rainer Bruch for a German keyboard :)

  Version 2.1 (May 2011)
  - timeout to recover from misaligned input
  - compatibility with Arduino "new-extension" branch
  - TODO: send function, proposed by Scott Penrose, scooterda at me dot com

  Version 2.0 (June 2010)
  - Buffering added, many scan codes can be captured without data loss
    if your sketch is busy doing other work
  - Shift keys supported, completely rewritten scan code to ascii
  - Slow linear search replaced with fast indexed table lookups
  - Support for Teensy, Arduino Mega, and Sanguino added

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "PS2Keyboard.h"

#define BUFFER_SIZE 45
static volatile uint8_t buffer[BUFFER_SIZE];
static volatile uint8_t head, tail;
static uint8_t DataPin;
static uint8_t CharBuffer=0;
static uint8_t UTF8next=0;
static const PS2Keymap_t *keymap=NULL;

// The ISR for the external interrupt
void ps2interrupt2(void)
{
	static uint8_t bitcount=0;
	static uint8_t incoming=0;
	static uint32_t prev_ms=0;
	uint32_t now_ms;
	uint8_t n, val;

	val = digitalRead(DataPin);
	now_ms = millis();
	if (now_ms - prev_ms > 250) {
		bitcount = 0;
		incoming = 0;
	}
	prev_ms = now_ms;
	n = bitcount - 1;
	if (n <= 7) {
		incoming |= (val << n);
	}
	bitcount++;
	if (bitcount == 11) {
		uint8_t i = head + 1;
		if (i >= BUFFER_SIZE) i = 0;
		if (i != tail) {
			buffer[i] = incoming;
			head = i;
		}
		bitcount = 0;
		incoming = 0;
	}
}

static inline uint8_t get_scan_code(void)
{
	uint8_t c, i;

	i = tail;
	if (i == head) return 0;
	i++;
	if (i >= BUFFER_SIZE) i = 0;
	c = buffer[i];
	tail = i;
	return c;
}


#define BREAK     0x01
#define MODIFIER  0x02
#define SHIFT_L   0x04
#define SHIFT_R   0x08
#define ALTGR     0x10
#define CTRL      0x20

static char get_iso8859_code(void)
{
	static uint8_t state=0;
	uint8_t s;
	char c;

	while (1) {
		s = get_scan_code();
		if (!s) return 0;
		if (s == 0xF0) {
			state |= BREAK;
		} else if (s == 0xE0) {
			state |= MODIFIER;
		} else {
			if (state & BREAK) {
				if (s == 0x12) {
					state &= ~SHIFT_L;
				} else if (s == 0x59) {
					state &= ~SHIFT_R;
				} else if (s == 0x14) {
					state &= ~CTRL;
				} else if (s == 0x11 && (state & MODIFIER)) {
					state &= ~ALTGR;
				}
				// CTRL, ALT & WIN keys could be added
				// but is that really worth the overhead?
				state &= ~(BREAK | MODIFIER);
				continue;
			}
			if (s == 0x12) {
				state |= SHIFT_L;
				continue;
			} else if (s == 0x59) {
				state |= SHIFT_R;
				continue;
			} else if (s == 0x14) {
				state |= CTRL;
				continue;
			} else if (s == 0x11 && (state & MODIFIER)) {
				state |= ALTGR;
			}
			c = 0;
			if (state & MODIFIER) {
				switch (s) {
				  case 0x70: c = PS2_INSERT;      break;
				  case 0x6C: c = PS2_HOME;        break;
				  case 0x7D: c = PS2_PAGEUP;      break;
				  case 0x71: c = PS2_DELETE;      break;
				  case 0x69: c = PS2_END;         break;
				  case 0x7A: c = PS2_PAGEDOWN;    break;
				  case 0x75: c = PS2_UPARROW;     break;
				  case 0x6B: c = PS2_LEFTARROW;   break;
				  case 0x72: c = PS2_DOWNARROW;   break;
				  case 0x74: c = PS2_RIGHTARROW;  break;
				  case 0x4A: c = '/';             break;
				  case 0x5A: c = PS2_ENTER;       break;
				  default: break;
				}
			} else if ((state & ALTGR) && keymap->uses_altgr) {
				if (s < PS2_KEYMAP_SIZE)
					c = pgm_read_byte(keymap->altgr + s);
			} else if (state & (SHIFT_L | SHIFT_R)) {
				if (s < PS2_KEYMAP_SIZE)
					c = pgm_read_byte(keymap->shift + s);
			} else {
				if (s < PS2_KEYMAP_SIZE)
					c = pgm_read_byte(keymap->noshift + s);
			}
			if (state & CTRL) {
				if (c >= 'A' && c <= 'Z')
					c = c - 'A' + 1;
				else if (c >= 'a' && c <= 'z')
					c = c - 'a' + 1;
				else if (c == PS2_ENTER)
					c = PS2_LINEFEED;
			}
			state &= ~(BREAK | MODIFIER);
			if (c) return c;
		}
	}
}

bool PS2Keyboard2::available() {
	if (CharBuffer || UTF8next) return true;
	CharBuffer = get_iso8859_code();
	if (CharBuffer) return true;
	return false;
}

int PS2Keyboard2::read() {
	uint8_t result;

	result = UTF8next;
	if (result) {
		UTF8next = 0;
	} else {
		result = CharBuffer;
		if (result) {
			CharBuffer = 0;
		} else {
			result = get_iso8859_code();
		}
		if (result >= 128) {
			UTF8next = (result & 0x3F) | 0x80;
			result = ((result >> 6) & 0x1F) | 0xC0;
		}
	}
	if (!result) return -1;
	return result;
}

PS2Keyboard2::PS2Keyboard2() {
  // nothing to do here, begin() does it all
}

void PS2Keyboard2::begin(uint8_t data_pin, uint8_t irq_pin, const PS2Keymap_t &map) {
  uint8_t irq_num=0;

  DataPin = data_pin;
  keymap = &map;

  // initialize the pins
#ifdef INPUT_PULLUP
  pinMode(irq_pin, INPUT_PULLUP);
  pinMode(data_pin, INPUT_PULLUP);
#else
  pinMode(irq_pin, INPUT);
  digitalWrite(irq_pin, HIGH);
  pinMode(data_pin, INPUT);
  digitalWrite(data_pin, HIGH);
#endif
  
  switch(irq_pin) {
    #ifdef CORE_INT0_PIN
    case CORE_INT0_PIN:
      irq_num = 0;
      break;
    #endif
    #ifdef CORE_INT1_PIN
    case CORE_INT1_PIN:
      irq_num = 1;
      break;
    #endif
    #ifdef CORE_INT2_PIN
    case CORE_INT2_PIN:
      irq_num = 2;
      break;
    #endif
    #ifdef CORE_INT3_PIN
    case CORE_INT3_PIN:
      irq_num = 3;
      break;
    #endif
    #ifdef CORE_INT4_PIN
    case CORE_INT4_PIN:
      irq_num = 4;
      break;
    #endif
    #ifdef CORE_INT5_PIN
    case CORE_INT5_PIN:
      irq_num = 5;
      break;
    #endif
    #ifdef CORE_INT6_PIN
    case CORE_INT6_PIN:
      irq_num = 6;
      break;
    #endif
    #ifdef CORE_INT7_PIN
    case CORE_INT7_PIN:
      irq_num = 7;
      break;
    #endif
    default:
      irq_num = 0;
      break;
  }
  head = 0;
  tail = 0;
  attachInterrupt(irq_num, ps2interrupt2, FALLING);
}


