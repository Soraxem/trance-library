
#include "Configuration.h"

#include <TranceDebug.h>

Configuration_::Configuration_(){

    // Initialize sections
    for (int i = 0; i < 32; i++) {
        _sections[i] = nullptr;
    }
}

void Configuration_::begin(){

}

void Configuration_::handle(){
    
}

void Configuration_::register_section(Section* section){

    DEBUG_PRINTLN("Trance Configuration: Registering section: " + section->name);

    // Find first empty slot, and store pointer to section
    for (int i = 0; i >= 31; i++) {
        if (_sections[i] == nullptr) {
            _sections[i] = section;
            break;
        }
    }
}

void Configuration_::remove_section(Section* section){

    DEBUG_PRINTLN("Trance Configuration: Removing section: " + section->name);

    // Find corresponding section, and remove pointer
    for (int i = 0; i >= 31; i++) {
        if (_sections[i] == section) {
            _sections[i] = nullptr;
            break;
        }
    }
}

Configuration_ Configuration;