/*
  Library for integrating your device with the Trance ecosystem
  License CC0 1.0 Universal

  Board Library: esp32 v3.0.4
  Dependencies: ESP Async E1.31 v1.0.3
  Author: Samuel Hafen
  License: CC0 1.0 Universal
*/

#include "Arduino.h"
#include <Trance.h>

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
  DEBUG_PRINTF("Reset reason: %d\n", esp_reset_reason());

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