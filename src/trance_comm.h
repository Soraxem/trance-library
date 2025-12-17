
#include <Arduino.h>

#include <Trance.h>

#include <ESPAsyncE131.h>

#include <ArtnetWiFi.h>

class TranceComm_ {
    public:
        TranceComm_() {};
        void begin(callback_type callback, int channels);
        void handle();
        void onArtnetCallback(const uint8_t *data, uint16_t size, const ArtDmxMetadata &metadata, const ArtNetRemoteInfo &remote);
    private:
        callback_type _callback;
        int _channels;
};

extern TranceComm_ TranceComm;