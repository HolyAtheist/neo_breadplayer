//MyTouchPad class for capacitive touch pins on e.g. ESP32
//by Tue Brisson Mosich

#include "MyTouchPad.h"
#include <math.h>
#include <Arduino.h>

MyTouchPad::MyTouchPad(int pin, int threshold, unsigned long updateInterval) {
  _pin = pin;
  m_threshold = threshold;
  m_updateInterval = updateInterval;
  m_lastUpdateTime = 0;
}

bool MyTouchPad::update() {
  m_lastValue = m_value;

  unsigned long currentMillis = millis();
  // only do the actual read every UPDATE_INTERVAL millis
  if (currentMillis - m_lastUpdateTime >= m_updateInterval) {
    m_lastUpdateTime = currentMillis;
    m_value = readTouchAverage();
  };
  // Check for a new touch
  if (m_base - m_value > m_threshold && !m_touched) {
    m_touched = true;
    m_touchedNow = true;
    m_touchStart = millis();
    Serial.println("new touch");
    return true;
  }
  // Check for a continued touch
  else if (m_base - m_value > m_threshold && m_touched) {
    m_touchedNow = true;
    m_touchLength = millis() - m_touchStart;
    //Serial.println("cont");
    return false;
  }
  // Check for a released touch
  else if (m_base - m_value < m_threshold && m_touched) {
    m_touched = false;
    m_touchedNow = false;
    m_touchLength = millis() - m_touchStart;
    Serial.println("release");
    return false;
  }
  // No touch detected
  else {
    m_touchedNow = false;
    return false;
  }
}

bool MyTouchPad::touchedFor(unsigned long duration_ms) {
  if (!m_touched) {
    return false;
  }

  unsigned long current_time = millis();
  if (!m_touchedNow) {
    // The touch has ended, reset the start time
    m_touchStart = current_time;
    return false;
  }

  // Check if the touch has been continuously touched for the specified duration
  if (current_time - m_touchStart >= duration_ms) {
    return true;
  }

  return false;
}


int MyTouchPad::readTouchAverage() {
  //this function reads the pin 3 times,
  //tries to compare the 3 readings, then discards the outlier,
  //then returns the average of the 2 remaining readings
  int p1 = 0, p2 = 0, p3 = 0;
  int a = 0, b = 0, c = 0;
  int x = 0, y = 0;
  p1 = touchRead(_pin);
  p2 = touchRead(_pin);
  p3 = touchRead(_pin);

  a = abs(p1 - p2);
  b = abs(p1 - p3);
  c = abs(p2 - p3);

  if (a >= b && b >= c) {
    x = p2;
    y = p3;
  } else if (a >= b && c >= b) {
    x = p1;
    y = p3;
  } else if (b >= a && c >= b) {
    x = p1;
    y = p2;
  } else if (b >= a && a >= c) {
    x = p2;
    y = p3;
  } else if (b >= a && c >= a) {
    x = p1;
    y = p2;
  } else Serial.println("uncaught" + String(p1));
  return ((y + x) / 2);
}



bool MyTouchPad::touched() {
  return m_touched;
}

int MyTouchPad::lastValue() {
  return m_lastValue;
}

// Getter for m_base
int MyTouchPad::getBase() {
  return m_base;
}

// Setter for m_base
void MyTouchPad::setBase(int base) {
  m_base = base;
}
