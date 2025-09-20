/*

Network managing module for the trance firmware

*/

#include "Network.h"

#include <TranceDebug.h>

#include "Configuration.h"

// Library for connecting to Wifi networks
//#include <WiFi.h>

void settings_callback(Section* WifiSection) {
  DEBUG_PRINTLN("Trance Wifi: Settings changed!");
}

Section WifiSection = Section(
  "Wifi",
  settings_callback,
  {
    Setting("SSID", STRING, "changeme"),
    Setting("PASSWORD", STRING, "1234")
  }
);


void Network_::begin() {
  Configuration.register_section(&WifiSection);
  //WiFi.mode(WIFI_STA);
  //WiFi.begin("SSID", "PASSWORD");
}

void Network_::handle() {
  //if (WiFi.status() != WL_CONNECTED) {
  //  WiFi.begin("SSID", "PASSWORD");
  //}
}

Network_ Network;