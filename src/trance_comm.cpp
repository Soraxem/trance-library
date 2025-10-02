

#include <trance_comm.h>

#include <trance_debug.h>

#include <trance_conf.h>
#include <trance.h>

#include <ESPAsyncE131.h>

ESPAsyncE131 e131(8);

void TranceComm_::begin(callback_type callback, int channels) {

    _callback = callback;
    _channels = channels;

    e131.begin(E131_MULTICAST, 1, 1);
}


void TranceComm_::handle() {
    if (!e131.isEmpty()) {
        e131_packet_t packet;
        e131.pull(&packet);     // Pull packet from ring buffer

        _callback(packet.property_values);

        DEBUG_PRINTLN("TRANCE COMM: Packet received!");
    }
}

TranceComm_ TranceComm;