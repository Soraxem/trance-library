

#include <trance_comm.h>

#include <trance_debug.h>

#include <trance_conf.h>
#include <trance.h>


void TranceComm_::begin(callback_type callback, int channels) {

    _callback = callback;
    _channels = channels;

}

void TranceComm_::handle() {

}

TranceComm_ TranceComm;