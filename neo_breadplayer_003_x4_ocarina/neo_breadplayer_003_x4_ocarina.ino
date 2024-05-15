//music player for ESP32 demonstrating touch pins by Tue Brisson Mosich
//uses XT_DAC_Audio library to output audio directly from ESP32

#include "SoundData.h"     // sample data
#include "XT_DAC_Audio.h"  // DAC library to output sound on DAC pin
#include "MyTouchPad.h"    // MyTouchPad class

#include "variables.h"  // global variables and constants etc


//  setup begin (this runs only once)

void setup() {
  Serial.begin(115200);
  delay(2000);  // time to bring up serial monitor
  // and get hands off touch-wires

  current_melody = spongebob;
  melody_notes = sizeof(spongebob) / sizeof(spongebob[0]);

  float touch1base = 0.0, touch2base = 0.0, touch3base = 0.0, touch4base = 0.0;
  //  get baseline for touchpads --
  //read each touch pin BASELINE_READS times, then get the average
  for (int i = 0; i < BASELINE_READS; i++) {
    touch1base += touch1.readTouchAverage();
    touch2base += touch2.readTouchAverage();
    touch3base += touch3.readTouchAverage();
    touch4base += touch4.readTouchAverage();
    delay(UPDATE_INTERVAL+10); // +10 just to make sure the timer is over
  }
  touch1.setBase(touch1base / BASELINE_READS);
  touch2.setBase(touch2base / BASELINE_READS);
  touch3.setBase(touch3base / BASELINE_READS);
  touch4.setBase(touch4base / BASELINE_READS);

  play_note_by_name("B5");
}  

// end setup
