/*

Network managing module for the trance firmware

*/

#include <trance_network.h>

#include <trance_debug.h>

#include <trance_conf.h>
#include <DNSServer.h>
#include <WiFi.h>

// Library for connecting to Wifi networks
//#include <WiFi.h>

static DNSServer dnsServer;



// Open a AP when wifi connection is interrupted.
void wifi_fail(arduino_event_id_t wifi_event, WiFiEventInfo_t wifi_info) {
  DEBUG_PRINTLN("TRANCE NETWORK: Not able to connect, switching to AP STA mode");
  WiFi.softAP("trance-config");
  WiFi.mode(WIFI_AP_STA);
}

// Close AP when connection is successfull
void wifi_connected(arduino_event_id_t wifi_event, WiFiEventInfo_t wifi_info) {
  DEBUG_PRINTLN("TRANCE NETWORK: Connected to Wifi, siwtching to STA mode");
  WiFi.mode(WIFI_STA);
}

void ap_start(arduino_event_id_t wifi_event, WiFiEventInfo_t wifi_info) {
  DEBUG_PRINTLN("TRANCE NETWORK: Soft AP started, Starting DNS");
  //DEBUG_PRINTLN("WIFI: Soft AP IP is: %s", WiFi.softAPIP());
  dnsServer.start(53, "*", WiFi.softAPIP());
}

Setting wifiSettings[] = {
  Setting("SSID", STRING, "changeme"),
  Setting("PASSWORD", STRING, "1234")
};

void settings_callback() {
  DEBUG_PRINTLN("Trance Wifi: Settings changed!");

  WiFi.disconnect();
  WiFi.begin(wifiSettings[0].value, wifiSettings[1].value);
}


Section WifiSection("Wifi", settings_callback, 2, wifiSettings);

void Network_::begin() {
  // Register wifi configuration settings
  TranceConf.register_section(&WifiSection);

  WiFi.onEvent(wifi_connected, ARDUINO_EVENT_WIFI_STA_CONNECTED);
  WiFi.onEvent(wifi_fail, ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
  WiFi.onEvent(ap_start, ARDUINO_EVENT_WIFI_AP_START);

  WiFi.mode(WIFI_STA);
  WiFi.begin(wifiSettings[0].value, wifiSettings[1].value);
}

void Network_::handle() {
  //if (WiFi.status() != WL_CONNECTED) {
  //  WiFi.begin("SSID", "PASSWORD");
  //}
  dnsServer.processNextRequest();
}

Network_ TranceNetwork;
