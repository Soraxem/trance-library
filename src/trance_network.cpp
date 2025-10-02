/*

Network managing module for the trance firmware

*/

#include <trance_network.h>

#include <trance_debug.h>

#include <trance_conf.h>

#include <WiFi.h>
#include <DNSServer.h>
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

void wifi_settings_callback() {
  DEBUG_PRINTLN("Trance Wifi: Settings changed!");

  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  //WiFi.setHostname(networkSettings[0].value.c_str());
  WiFi.begin(wifiSettings[0].value, wifiSettings[1].value);
}

Section WifiSection("Wifi", wifi_settings_callback, 2, wifiSettings);


Setting networkSettings[] = {
  Setting("hostname", STRING, "trance"),
};

void network_settings_callback() {
  DEBUG_PRINTLN("Trance Network: Settings changed!");

  WiFi.setHostname(networkSettings[0].value.c_str());
}

Section NetworkSection("Network", network_settings_callback, 1, networkSettings);

void Network_::begin() {
  // Register wifi configuration settings
  TranceConf.register_section(&WifiSection);

  // Register network configuration settings
  TranceConf.register_section(&NetworkSection);

  WiFi.onEvent(wifi_connected, ARDUINO_EVENT_WIFI_STA_CONNECTED);
  WiFi.onEvent(wifi_fail, ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
  WiFi.onEvent(ap_start, ARDUINO_EVENT_WIFI_AP_START);

  WiFi.mode(WIFI_STA);
  WiFi.setHostname(networkSettings[0].value.c_str());
  WiFi.begin(wifiSettings[0].value, wifiSettings[1].value);
}

void Network_::handle() {
  //if (WiFi.status() != WL_CONNECTED) {
  //  WiFi.begin("SSID", "PASSWORD");
  //}
  dnsServer.processNextRequest();
}

Network_ TranceNetwork;
