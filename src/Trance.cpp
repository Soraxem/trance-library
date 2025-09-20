/*
  Trance.cpp Library for integrating your device with the Trance ecosystem
  License CC0 1.0 Universal
*/

#include "Arduino.h"
#include "Trance.h"

#include <TranceDebug.h>

#include "Configuration.h"
#include "Network.h"


void Trance_::begin(int channels, callback_type callback) {

  #ifdef DEBUG
  Serial.begin(115200);
  delay(2000);
  #endif

  DEBUG_PRINTLN("Trance: debugging enabled!");

  _channels = channels;
	if (callback) {
		_callback = callback;
	} 

  Configuration.begin();
  Network.begin();
}

void Trance_::handle() {
  Configuration.handle();
  Network.handle();
}

Trance_ Trance;

// 	_callback();