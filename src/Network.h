
#include <Arduino.h>

class Network_ {
    public:
        Network_() {};
        void begin();
        void handle();
};

extern Network_ Network;