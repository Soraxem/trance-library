

#include <trance_comm.h>

#include <trance_debug.h>

#include <trance_conf.h>
#include <Trance.h>

#include <ESPAsyncE131.h>

#include <ArtnetWiFi.h>


ESPAsyncE131 e131(8);

ArtnetWiFiReceiver artnet;


Setting sacnSettings[] = {
    Setting("Universe", INT, "1"),
    Setting("Address", INT, "1"),
};

void sacn_settings_callback() {
    DEBUG_PRINTLN("Trance sACN: Settings changed!");

    //e131.begin(E131_MULTICAST, sacnSettings[0].value.toInt(), 1);

}

Section AscnSection("sACN", sacn_settings_callback, 2, sacnSettings);


Setting artnetSettings[] = {
    Setting("Universe", INT, "0"),
    Setting("Address", INT, "1"),
};

void artnet_settings_callback() {
    DEBUG_PRINTLN("Trance Artnet: Settings changed!");
}

Section ArtnetSection("Artnet", artnet_settings_callback, 2, artnetSettings);

void artnet_callback(const uint8_t *data, uint16_t size, const ArtDmxMetadata &metadata, const ArtNetRemoteInfo &remote);

void TranceComm_::begin(callback_type callback, int channels) {

    // Register ASCN settings
    TranceConf.register_section(&AscnSection);

    // Register Artnet settings
    TranceConf.register_section(&ArtnetSection);

    _callback = callback;
    _channels = channels;

    

    //e131.begin(E131_MULTICAST, sacnSettings[0].value.toInt(), 1);
    //e131.begin(E131_MULTICAST, 1, 1);

    artnet.begin();

    artnet.setArtPollReplyConfigShortName("TRANCE");
    artnet.setArtPollReplyConfigLongName("ubs-dmx rev 1.2");
    artnet.subscribeArtDmxUniverse(artnetSettings[0].value.toInt(), artnet_callback);

}


// Callback wrapper
void artnet_callback(const uint8_t *data, uint16_t size, const ArtDmxMetadata &metadata, const ArtNetRemoteInfo &remote) {
    TranceComm.onArtnetCallback(data, size, metadata, remote);
}

// Real callback handler
void TranceComm_::onArtnetCallback(const uint8_t *data, uint16_t size, const ArtDmxMetadata &metadata, const ArtNetRemoteInfo &remote) {
    DEBUG_PRINTLN("Trance Artnet: Packet Recieved");
    _callback(data);
}


void TranceComm_::handle() {
    artnet.parse();
    /*
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

        
    }*/
}

TranceComm_ TranceComm;