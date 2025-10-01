
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

void Configuration_::begin(int reset_button){
    _reset_button = reset_button;
    pinMode(_reset_button, INPUT_PULLUP);
    server.begin();
}

void Configuration_::handle(){
    
    if (!digitalRead(_reset_button) && !_reset_state) {
        _reset_timer = millis();
        _reset_state = true;
    } else if (!digitalRead(_reset_button) && _reset_state) {
        if (_reset_timer + 5000 < millis()) {
            DEBUG_PRINTLN("TRANCE CONF: Resetting configuration!");
            _reset_state = false;
            for (int i = 0; i < 32; i++) {
                if (_sections[i] != nullptr) {
                    preferences.begin(_sections[i]->name);
                    preferences.clear();
                    preferences.end();
                }
            }
            ESP.restart();
        }
    } else {
        _reset_state = false;
    }
    
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

                        for (int i = 0; i < 32; i++) {
                            if (_sections[i] != nullptr) {
                                client.print("<h2>" + String(_sections[i]->name) + "</h2>");
                                client.print("<form action=\"" +  String(_sections[i]->name) + "\">");
                                for (int j = 0; j < _sections[i]->settings_count; j++) {
                                    client.print("<label>" + String(_sections[i]->settings[j].name) + "</label><br>");
                                    if (_sections[i]->settings[j].type == SettingType::STRING) {
                                        client.print("<input type=\"text\" name=\"" + String(_sections[i]->settings[j].name) + "\" value=\"" + _sections[i]->settings[j].value + "\"><br>");
                                    } else if (_sections[i]->settings[j].type == SettingType::INT) {
                                        client.print("<input type=\"number\" name=\"" + String(_sections[i]->settings[j].name) + "\" value=\"" + _sections[i]->settings[j].value + "\"><br>");
                                    } else if (_sections[i]->settings[j].type == SettingType::BOOL_CHECK) {
                                        client.print("<input type=\"checkbox\" name=\"" + String(_sections[i]->settings[j].name) + "\" value=\"" + _sections[i]->settings[j].value + "\"><br>");
                                    } else if (_sections[i]->settings[j].type == SettingType::BOOL_RADIO) {
                                        client.print("<input type=\"radio\" name=\"" + String(_sections[i]->settings[j].name) + "\" value=\"" + _sections[i]->settings[j].value + "\"><br>");
                                    }
                                }
                                client.print("<input type=\"submit\" value=\"Submit\">");
                                client.print("</form>");
                            }
                        }

                        client.println();
                        break;

                    } else {

                        if (currentLine.startsWith("GET /")) {

                            DEBUG_PRINTLN("TRANCE CONF: got request: " + currentLine);

                            String params = currentLine.substring(5, currentLine.lastIndexOf(" "));
                            
                            if (params.indexOf("?") == -1) {
                                DEBUG_PRINTLN("TRANCE CONF: Section not found");
                            } else {
                                String section = params.substring(0, params.indexOf("?"));
                                params = params.substring(section.length() + 1);
                                DEBUG_PRINTLN("TRANCE CONF: found Section:" + section);

                                Section* found_section = nullptr;
                                for (int i = 0; i < 32; i++) {
                                    if (_sections[i] != nullptr && String(_sections[i]->name) == section) {
                                        found_section = _sections[i];
                                        break;
                                    }
                                }

                                preferences.begin(found_section->name);
                                while (params != "") {
                                    String param = params.substring(0, params.indexOf("&"));
                                    params = params.substring(param.length() + 1);
                                    DEBUG_PRINTLN("TRANCE CONF: found Parameter:" + param);

                                    String name = param.substring(0, param.indexOf("="));
                                    String value = param.substring(name.length() + 1);
                                    DEBUG_PRINTLN("TRANCE CONF: found Name:" + name + " Value:" + value);
                                    
                                    for (int i = 0; i < found_section->settings_count; i++) {
                                        if (String(found_section->settings[i].name) == name) {
                                            found_section->settings[i].value = value;
                                            preferences.putString(found_section->settings[i].name, value);
                                        }
                                    }
                                }
                                preferences.end();

                                _callback_section = found_section;
                            }

                            
                        }   

                        currentLine = "";
                    }
                } else if (c != '\r') {
                    currentLine += c;
                }
            }
        }
        client.stop();
    }

    if (_callback_section != nullptr) {
        _callback_section->callback();
        _callback_section = nullptr;
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
    for (int i = 0; i < 32; i++) {
        if (_sections[i] == nullptr) {
            _sections[i] = section;
            break;
        }
    }
}

void Configuration_::remove_section(Section* section){

    DEBUG_PRINTLN("TRANCE CONF: Removing section: " + String(section->name));

    // Find corresponding section, and remove pointer
    for (int i = 0; i < 32; i++) {
        if (_sections[i] == section) {
            _sections[i] = nullptr;
            break;
        }
    }
}

Configuration_ TranceConf;