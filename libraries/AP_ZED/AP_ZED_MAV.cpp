/*
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <AP_HAL/AP_HAL.h>
#include "AP_ZED_MAV.h"
#include <AP_SerialManager/AP_SerialManager.h>

extern const AP_HAL::HAL& hal;

// constructor
AP_ZED_MAV::AP_ZED_MAV(AP_ZED &frontend) :
    AP_ZED_Backend(frontend)
{
}

// consume ZED data from MAVLink messages
void AP_ZED_MAV::handle_msg(mavlink_message_t *msg)
{
    // decode message
    mavlink_zed_vision_position_delta_t packet;
    mavlink_msg_zed_vision_position_delta_decode(msg, &packet);

    Vector3f angle_delta(packet.angle_delta[0], packet.angle_delta[1], packet.angle_delta[2]);
    Vector3f position_delta(packet.position_delta[0], packet.position_delta[1], packet.position_delta[2]);
    set_deltas(angle_delta, position_delta, packet.time_delta_usec, packet.confidence);
}
