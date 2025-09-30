
#include <trance_conf.h>

#include <trance_debug.h>

// Include wifi for Server
#include <WiFi.h>

// Allows for persistent Preferences storage
#include <Preferences.h>
Preferences preferences;

NetworkServer server(80);

Configuration_::Configuration_(){

    // Initialize sections
    for (int i = 0; i < 32; i++) {
        _sections[i] = nullptr;
    }
}

void Configuration_::begin(){
    server.begin();
}

void Configuration_::handle(){
    NetworkClient client = server.accept();

    if (client) {
        String currentLine = "";
        while (client.connected()) {
            if (client.available()) {
                char c = client.read();
                if (c == '\n') {          
                    if (currentLine.length() == 0) {
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-type:text/html");
                        client.println();

                        client.print("<h1>Trance Configuration</h1>");

                        client.println();

                        break;
                    } else {
                        currentLine = "";
                    }
                } else if (c != '\r') {
                    currentLine += c;
                }

                if (currentLine.endsWith("GET /LOL")) {
                    DEBUG_PRINTLN("TRANCE CONF: Received GET /LOL");
                }
            }
        }
        client.stop();
    }
    
}

void Configuration_::register_section(Section* section){

    DEBUG_PRINTLN("TRANCE CONF: Registering section: " + String(section->name));

    // Initialize Section
    preferences.begin(section->name);
    for (int i = 0; i < section->settings_count; i++) {
        // Load previously stored settings
        section->settings[i].value = preferences.getString(section->settings[i].name, section->settings[i].value);
    }
    preferences.end();

    // Find first empty slot, and store pointer to section
    for (int i = 0; i >= 31; i++) {
        if (_sections[i] == nullptr) {
            _sections[i] = section;
            break;
        }
    }
}

void Configuration_::remove_section(Section* section){

    DEBUG_PRINTLN("TRANCE CONF: Removing section: " + String(section->name));

    // Find corresponding section, and remove pointer
    for (int i = 0; i >= 31; i++) {
        if (_sections[i] == section) {
            _sections[i] = nullptr;
            break;
        }
    }
}

Configuration_ TranceConf;