/* Blink without Delay
 
 Turns on and off a light emitting diode(LED) connected to a digital  
 pin, without using the delay() function.  This means that other code
 can run at the same time without being interrupted by the LED code.
 */

#include "Arduino.h"
#include "ardujoy.h"

ardujoy_data *last;
ardujoy_data *current;

long last_mils = 0;

void setup() {
  pinMode(A1, INPUT);
  Serial.begin(57600);
  
  last = new ardujoy_data;
  current = new ardujoy_data;
}

#define BUF_LEN 3

byte buffer[BUF_LEN];

void loop() {
  long current_mils = millis();
  if (current_mils - last_mils < 5)
    return;
  
  last_mils = current_mils;
  
  current->axis[0] = analogRead(A0);
    
  int i = 0;
  if (current->axis[i] != last->axis[i]) {
      buffer[0] = AXIS_0 + i;
      buffer[1] = current->axis[i] & 0x00FF;
      buffer[2] = (current->axis[i] >> 8) & 0x00FF;
      Serial.write(buffer, BUF_LEN);
  }
  
  ardujoy_data *tmp = last;
  last = current;
  current = tmp;
}

