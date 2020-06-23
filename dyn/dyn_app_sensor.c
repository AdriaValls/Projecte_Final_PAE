/*
 * dyn_sensor.c
 *
 *  Created on: 18 mar. 2020
 *      Author: droma
 *
 * TODO: High-level functions like "distance_wall_front", etc
 * TODO: Generate another file for motors, with functions like "move_forward", etc
 */
#include <stdint.h>
#include "dyn_app_sensor.h"
#include "dyn_instr.h"
#include "dyn_frames.h"

int redObsDistance(byte ID, byte position){

    uint8_t distance;
    dyn_read_byte(ID, position, &distance); /** Llegim el valor del sensor i actualitzem el valor de distance **/

    return distance; /** Retornem el valor actualitzat **/
}


int getObstacleFlag(byte ID){
    byte flags;
    byte param[1];
    param [0] = 1;
    flags = dyn_read_byte(ID, 0x20, param);

    return flags;
}


