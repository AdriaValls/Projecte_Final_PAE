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
    byte params[4]={0,0,0,0};  /** Activem l'endless turn **/
    dyn_write(idMotor, P_CW_ANGLE_LIMIT_L, params, 4);
}

void moveWheel(uint8_t ID, bool rotation, unsigned int speed)
{
    uint8_t speed_H,speed_L;  /** Tenim dos registres a escriure per tant creem dos speeds **/
    speed_L = speed;

    if(speed<1024){  /** Comprovem que no es passa del maxim de velocitat**/

        if(rotation){                     /** Segons la rotació fem que el valor d'speed_H s'ajusti a la direcció que volem **/
            speed_H = (speed >> 8)+4;
        }else{
            speed_H = speed >> 8;
        }
        uint8_t bParameter[2]={speed_L,speed_H};  /** Ajuntem les dues velocitats i les enviem **/
        dyn_write(ID,MOV_SPEED_L,bParameter,2);
    }
}

void stop(void){
    /** Posem la velocitat a 0 per que no es mogui **/
    moveWheel(RIGHT_WHEEL, 0, 0);
    moveWheel(LEFT_WHEEL, 0, 0);
}


void turnLeft(unsigned int speed){
    /** Si volem que giri a l'esquerra li traiem velocitat a la roda esquerra i li donem a la dreta així gira sobre l'eix de la roda esquerra**/
    if(speed < 1024){
        moveWheel(RIGHT_WHEEL, LEFT, speed);
        moveWheel(LEFT_WHEEL, RIGHT, 0);
    }
}

void turnOnItselfLeft(unsigned int speed){
    /** Per fer un gir més tancat li donem la mateixa dirrecció a les dues rodes per tal de que giri sobre l'eix del robot **/
    if(speed < 1024){
        moveWheel(RIGHT_WHEEL, RIGHT, speed);
        moveWheel(LEFT_WHEEL, RIGHT, speed);
    }
}

void turnRight(unsigned int speed){
    /** El mateix pero cap a la dreta **/
    if(speed < 1024){
        moveWheel(RIGHT_WHEEL, LEFT, 0);
        moveWheel(LEFT_WHEEL, RIGHT, speed);
    }
}

void turnOnItselfRight(unsigned int speed){
    /** El mateix pero cap a la dreta **/
    if(speed < 1024){
        moveWheel(RIGHT_WHEEL, LEFT, speed);
        moveWheel(LEFT_WHEEL, LEFT, speed);
    }
}

void forward(unsigned int speed){
    /** Li donem la mateixa speed a les dues rodes i direccións oposades ja que els motors estan posats de forma inversa un de l'altre **/
    if(speed < 1024){
        moveWheel(RIGHT_WHEEL, LEFT , speed);
        moveWheel(LEFT_WHEEL, LEFT, speed);
    }
}

void backward(unsigned int speed) {
    /** El mateix pero Inverint les direccións per tal de que el robot camini cap enrere **/
    if (speed < 1024) {
        moveWheel(RIGHT_WHEEL, RIGHT, speed);
        moveWheel(LEFT_WHEEL, RIGHT, speed);
    }
}

