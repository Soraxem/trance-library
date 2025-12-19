/*
  Library for integrating your device with the Trance ecosystem

  Board Library: esp32 v3.0.4
  Dependencies: ArtNet v0.9.2
  Author: Samuel Hafen
  License: MIT
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
