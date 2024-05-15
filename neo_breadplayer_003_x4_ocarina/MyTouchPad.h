//MyTouchPad class for capacitive touch pins on e.g. ESP32
//by Tue Brisson Mosich

#ifndef MyTouchPad_h
#define MyTouchPad_h

#include <Arduino.h>

class MyTouchPad {
public:
  MyTouchPad(int pin, int threshold, unsigned long updateInterval);

  bool update();
  int readTouchAverage();
  bool touched();
  bool touchedFor(unsigned long duration_ms);

  int lastValue();
  int getBase();
  void setBase(int base);

private:
  int _pin;
  int m_base;
  int m_threshold;
  int m_value;
  int m_lastValue;
  bool m_touched;
  bool m_touchedNow;
  unsigned long m_touchStart;
  unsigned long m_touchLength;
  unsigned long m_lastUpdateTime;
  unsigned long m_updateInterval;
};

#endif
