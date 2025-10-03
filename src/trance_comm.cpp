

#include <trance_comm.h>

#include <trance_debug.h>

#include <trance_conf.h>
#include <Trance.h>

#include <ESPAsyncE131.h>

ESPAsyncE131 e131(8);


Setting sacnSettings[] = {
    Setting("Universe", INT, "1"),
    Setting("Address", INT, "1"),
};

void sacn_settings_callback() {
    DEBUG_PRINTLN("Trance ASCN: Settings changed!");

    //e131.begin(E131_MULTICAST, sacnSettings[0].value.toInt(), 1);

}

Section AscnSection("SACN", sacn_settings_callback, 2, sacnSettings);

void TranceComm_::begin(callback_type callback, int channels) {

    // Register ASCN settings
    TranceConf.register_section(&AscnSection);

    _callback = callback;
    _channels = channels;

    

    //e131.begin(E131_MULTICAST, sacnSettings[0].value.toInt(), 1);
    e131.begin(E131_MULTICAST, 1, 1);
}


void TranceComm_::handle() {
    if (!e131.isEmpty()) {
        DEBUG_PRINTLN("Trance ASCN: got Packet");
        e131_packet_t packet;
        e131.pull(&packet);     // Pull packet from ring buffer

        uint8_t channels[_channels];

        DEBUG_PRINTLN("Trance ASCN: adress: " + sacnSettings[1].value);

        int address = sacnSettings[1].value.toInt();

        for (int i = address; i < address + _channels; i++) {
            channels[i - address] = packet.property_values[i];
        }

        _callback(channels);

        
    }
}

TranceComm_ TranceComm;