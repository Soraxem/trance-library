/*
  Trance.cpp Library for integrating your device with the Trance ecosystem
  License CC0 1.0 Universal

  Dependencies: 
*/

#include "Arduino.h"
#include <trance.h>

#include <trance_debug.h>

#include <trance_conf.h>
#include <trance_network.h>
#include <trance_comm.h>


void Trance_::begin(int channels, callback_type callback, int reset_button) {

  #ifdef DEBUG
  Serial.begin(115200);
  delay(2000);
  #endif

  DEBUG_PRINTLN("Trance: debugging enabled!");

  TranceNetwork.begin();
  TranceConf.begin(reset_button);
  TranceComm.begin(callback, channels);
}

void Trance_::handle() {
  TranceNetwork.handle();
  TranceConf.handle();
  TranceComm.handle();
}

Trance_ Trance;

// 	_callback();