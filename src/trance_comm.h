
#include <Arduino.h>

#include <trance.h>

#include <ESPAsyncE131.h>

class TranceComm_ {
    public:
        TranceComm_() {};
        void begin(callback_type callback, int channels);
        void handle();
    private:
        callback_type _callback;
        int _channels;
};

extern TranceComm_ TranceComm;