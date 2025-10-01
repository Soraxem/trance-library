/*
  Trance.h Library for integrating your device with the Trance ecosystem
  License CC0 1.0 Universal
*/

#ifndef Trance_h
#define Trance_h

// ToDo: Add support for Ethernet

#ifndef TRANCE_ETHERNET  
#define TRANCE_WIFI
#endif

#include "Arduino.h"

typedef void (*callback_type)(uint8_t[]);

class Trance_
{
  public:
    Trance_() {};
    void begin(int channels, callback_type callback, int reset_button);
    void handle();
  private:
    int _channels;
    callback_type _callback;
};

extern Trance_ Trance;

#endif
