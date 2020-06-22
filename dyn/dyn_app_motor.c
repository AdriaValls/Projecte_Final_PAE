//
// Created by Usuario on 22/06/2020.
//

#include "dyn_app_motor.h"
#include <stdint.h>
#include "dyn_instr.h"
#include "dyn_frames.h"
#define P_CW_ANGLE_LIMIT_L 0x06
#define P_CW_ANGLE_LIMIT_H 0x07
#define P_CCW_ANGLE_LIMIT_L 0x08
#define P_CCW_ANGLE_LIMIT_H 0x09
#define LEFT 0
#define RIGHT 1
#define RIGHT_WHEEL 2
#define LEFT_WHEEL 1
static byte MOV_SPEED_L = 0x20;
static byte MOV_SPEED_H = 0x21;

// Funció
void endlessTurn(byte idMotor)
{
    byte params[4]={0,0,0,0};
    dyn_write(idMotor, P_CW_ANGLE_LIMIT_L, params, 4);
}

void moveWheel(byte ID, bool rotation, unsigned int speed)
{
    byte speed_H,speed_L;
    speed_L = speed;

    if(speed<1024){ // Velocidad max. 1023

        if(rotation){ // Rotation == 1
            speed_H = (speed >> 8)+4;   // Mover a la derecha (CW)
        }else{
            speed_H = speed >> 8;       // Mover a la izquierda (CCW)
        }
        byte bParameter[2]={speed_L,speed_H};
        dyn_write(ID,MOV_SPEED_L,bParameter,2);
    }
}

void stop(void)
{
    moveWheel(RIGHT_WHEEL, 0, 0);
    moveWheel(LEFT_WHEEL, 0, 0);
}


void turnLeft(unsigned int speed)
{
    // Girar a la izquierda - Mover a la derecha todas las ruedas
    if(speed < 1024){
        moveWheel(RIGHT_WHEEL, RIGHT, speed);
        moveWheel(LEFT_WHEEL, RIGHT, 0);
    }
}

void turnOnItselfLeft(unsigned int speed)
{
    // Girar a la izquierda - Mover a la derecha todas las ruedas
    if(speed < 1024){
        moveWheel(RIGHT_WHEEL, RIGHT, speed);
        moveWheel(LEFT_WHEEL, RIGHT, speed);
    }
}

void turnRight(unsigned int speed)
{
    // Girar a la derecha - Mover a la izquierda todas las ruedas
    if(speed < 1024){
        moveWheel(RIGHT_WHEEL, LEFT, 0);
        moveWheel(LEFT_WHEEL, LEFT, speed);
    }
}

void turnOnItselfRight(unsigned int speed)
{
    // Girar a la derecha - Mover a la izquierda todas las ruedas
    if(speed < 1024){
        moveWheel(RIGHT_WHEEL, LEFT, speed);
        moveWheel(LEFT_WHEEL, LEFT, speed);
    }
}

void forward(unsigned int speed)
{
    // Mover hacia delante
    if(speed < 1024){
        moveWheel(RIGHT_WHEEL, LEFT , speed);
        moveWheel(LEFT_WHEEL, RIGHT, speed);
    }
}

void backward(unsigned int speed) {
    // Mover hacia atr·s
    if (speed < 1024) {
        moveWheel(RIGHT_WHEEL, LEFT, speed);
        moveWheel(LEFT_WHEEL, RIGHT, speed);
    }
}