/*
 * dyn_sensor.h
 *
 *  Created on: 18 mar. 2020
 *      Author: droma
 */

#ifndef DYN_SENSOR_H_
#define DYN_SENSOR_H_

typedef uint8_t byte;

int redObsDistance(byte ID, byte position);
int sensorRead(byte ID, byte sensor);
int getObstacleFlag(byte ID);


#endif /* DYN_SENSOR_H_ */
