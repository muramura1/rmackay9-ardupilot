// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-
#pragma once

#include "AP_Beacon_Backend.h"

#define AP_BEACON_POZYX_MSG_LEN_MAX         20      // messages from uno/pozyx are no more than 20bytes
#define AP_BEACON_POZYX_HEADER              0x01    // messages start with this character
#define AP_BEACON_POZYX_MSGID_BEACON_CONFIG 0x02    // message contains anchor config information
#define AP_BEACON_POZYX_MSGID_BEACON_DIST   0x03    // message contains individual beacon distance
#define AP_BEACON_POZYX_MSGID_POSITION      0x04    // message contains vehicle position information

class AP_Beacon_Pozyx : public AP_Beacon_Backend
{

public:
    // constructor
    AP_Beacon_Pozyx(AP_Beacon &frontend, AP_SerialManager &serial_manager);

    // update
    void update();

private:

    enum ParseState{
        ParseState_WaitingForHeader = 0,
        ParseState_WaitingForMsgId = 1,
        ParseState_WaitingForLen = 2,
        ParseState_WaitingForContents = 3
    } parse_state;

    // parse buffer
    void parse_buffer();

    uint8_t parse_msg_id;
    uint8_t parse_msg_len;

    AP_HAL::UARTDriver *uart = nullptr;
    uint8_t linebuf[AP_BEACON_POZYX_MSG_LEN_MAX];
    uint8_t linebuf_len = 0;
};
