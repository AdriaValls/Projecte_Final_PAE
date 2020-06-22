//
// Created by Usuario on 22/06/2020.
//

#ifndef P4_PC_JOYSTICK2_HABITACION_DYN_APP_MOTOR_H
#define P4_PC_JOYSTICK2_HABITACION_DYN_APP_MOTOR_H

#include <stdint.h>
#include <stdbool.h>
typedef uint8_t byte;
static byte BROADCASTING = 0xFE;
static byte WRITE_DATA = 0x03;
static byte CW_ANGLE_LIMIT_L = 0x06;

void endlessTurn(byte idMotor);//to rotate wheels

void moveWheel(byte ID, bool rotation,unsigned int speed);

void stop(void);

void turnLeft(unsigned int speed);

void turnOnItselfLeft(unsigned int speed);

void turnRight(unsigned int speed);

void turnOnItselfRight(unsigned int speed);

void forward(unsigned int speed);

void backward(unsigned int speed);

#endif //P4_PC_JOYSTICK2_HABITACION_DYN_APP_MOTOR_H
