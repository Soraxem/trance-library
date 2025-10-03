/*
  Library for integrating your device with the Trance ecosystem
  License CC0 1.0 Universal

  Board Library: esp32 v3.0.4
  Dependencies: ESP Async E1.31 v1.0.3
  Author: Samuel Hafen
  License: CC0 1.0 Universal
*/

#ifndef Trance_h
#define Trance_h

#include "Arduino.h"

typedef void (*callback_type)(uint8_t[]);

class Trance_
{
  public:
    Trance_() {};
    void begin(int channels, callback_type callback, int reset_button);
    void handle();
};

extern Trance_ Trance;

#endif
