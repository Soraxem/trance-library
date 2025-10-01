/*
  Trance.cpp Library for integrating your device with the Trance ecosystem
  License CC0 1.0 Universal

  Dependencies: Preferences v2.1.0,
*/

#include "Arduino.h"
#include <trance.h>

#include <trance_debug.h>

#include <trance_conf.h>
#include <trance_network.h>


void Trance_::begin(int channels, callback_type callback, int reset_button) {

  #ifdef DEBUG
  Serial.begin(115200);
  delay(2000);
  #endif

  DEBUG_PRINTLN("Trance: debugging enabled!");

  _channels = channels;
	if (callback) {
		_callback = callback;
	} 

  TranceNetwork.begin();
  TranceConf.begin(reset_button);
}

void Trance_::handle() {
  TranceNetwork.handle();
  TranceConf.handle();
}

Trance_ Trance;

// 	_callback();