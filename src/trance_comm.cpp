#include <trance_comm.h>

#include <trance_debug.h>

#include <trance_conf.h>
#include <Trance.h>

#include <ArtnetWiFi.h>

// Create artnet instance
ArtnetWiFiReceiver artnet;

// prototype for artnet callback
void artnet_callback(const uint8_t *data, uint16_t size, const ArtDmxMetadata &metadata, const ArtNetRemoteInfo &remote);

// create artnet setting fields
Setting artnetSettings[] = {
    Setting("PortAddress", INT, "1"),
    Setting("Address", INT, "0"),
};

// handle setting changes
void artnet_settings_callback() {

    // Unsubscribe previous universes
    artnet.unsubscribeArtDmxUniverses();

    // Subscribe newly configured universe
    artnet.subscribeArtDmxUniverse(artnetSettings[0].value.toInt(), artnet_callback);

    DEBUG_PRINTLN("Trance Artnet: Settings changed!");
}

Section ArtnetSection("Artnet", artnet_settings_callback, 2, artnetSettings);

void TranceComm_::begin(callback_type callback, int channels) {

    // Register and retrieve Artnet settings
    TranceConf.register_section(&ArtnetSection);

    _callback = callback;
    _channels = channels;

    // Start Artnet service
    artnet.begin();

    // Add device specs to ArtPoll
    artnet.setArtPollReplyConfigShortName("TRANCE");
    artnet.setArtPollReplyConfigLongName("ubs-dmx rev 1.2");

    // Subscribe current congiguration
    artnet.subscribeArtDmxUniverse(artnetSettings[0].value.toInt(), artnet_callback);

}


// Callback wrapper
void artnet_callback(const uint8_t *data, uint16_t size, const ArtDmxMetadata &metadata, const ArtNetRemoteInfo &remote) {
    TranceComm.onArtnetCallback(data, size, metadata, remote);
}

// Real callback handler
void TranceComm_::onArtnetCallback(const uint8_t *data, uint16_t size, const ArtDmxMetadata &metadata, const ArtNetRemoteInfo &remote) {
    DEBUG_PRINTLN("Trance Artnet: Packet Recieved");

    uint8_t output[_channels];

    for (int i = 0; i < _channels; i++) {
        output[i] = data[i + artnetSettings[1].value.toInt() -1];
    }

    _callback(output);
}

void TranceComm_::handle() {
    artnet.parse();
}

TranceComm_ TranceComm;