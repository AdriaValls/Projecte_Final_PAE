#include <pthread.h>
#include <signal.h>
#include <assert.h>
#include <posicion.h>
#include <stdio.h>

#include "main.h"
#include "dyn/dyn_app_common.h"
#include "dyn_app_motor.h"
#include "dyn_instr.h"
#include "dyn_app_sensor.h"
#include "dyn_frames.h"

#include "dyn_test/dyn_emu.h"
#include "dyn_test/b_queue.h"
#include "joystick_emu/joystick.h"
#include "habitacion_001.h"


uint8_t estado = Ninguno, estado_anterior = Ninguno, finalizar = 0;
uint32_t indice;

bool follow_wall; //true esquerra, false dreta
bool got_wall = false;
int dist_min = 2,dist_mid = 5, dist_max = 10, dist_seguretat = 12;
int sen_center, sen_left, sen_right;
/**
 * main.c
 */
int main(void) {
    pthread_t tid, jid;
    uint8_t tmp;

    //Init queue for TX/RX data
    init_queue(&q_tx);
    init_queue(&q_rx);

    //Start thread for dynamixel module emulation
    // Passing the room information to the dyn_emu thread
    pthread_create(&tid, NULL, dyn_emu, (void *) datos_habitacion);
    pthread_create(&jid, NULL, joystick_emu, (void *) &jid);

    //Testing some high level function
    printf("\nSetting LED to 0 \n");
    dyn_led_control(1, 0);
    printf("\nGetting LED value \n");
    dyn_led_read(1, &tmp);
    assert(tmp == 0);
    printf("\nSetting LED to 1 \n");
    dyn_led_control(1, 1);
    printf("\nGetting LED value \n");
    dyn_led_read(1, &tmp);
    assert(tmp == 1);

    printf("\n************************\n");
    printf("Test passed successfully\n");

    printf("\nDimensiones habitacion %d ancho x %d largo mm2\n", ANCHO, LARGO);
    printf("En memoria: %I64u B = %I64u MiB\n", sizeof(datos_habitacion), sizeof(datos_habitacion) >> 20);

    printf("Pulsar 'q' para terminar, qualquier tecla para seguir\n");
    fflush(stdout);//	return 0;

    endlessTurn(1);
    endlessTurn(2);

    while (estado != Quit) {
        if (simulator_finished) {
            break;
        }

        sen_center = redObsDistance(3, 0x1b);
        sen_left =  redObsDistance(3, 0x1a);
        sen_right = redObsDistance(3, 0x1c);
        /** Llegim els valors dels sensors per saber les distàncies de cad un**/

        /** L'algorisme comença comprovant que no tenim cap paret (got_wall = false al començar ja que al principi hem de buscar una paret) **/

        if(got_wall == false){

            /** Ja que no hem trobat cap paret n'hem de buscar una, aixó ho fem mesurant i comparant els tres sensors
             * Busquem la distància més petita dels sensors esquerra, dreta i centre, si el valor més petit és el del centre vol dir que anem ben encaminats i per tant anem cap endavant
             * en canvi si la distància més petita és la d'un sensor lateral, girem cap a aquella direcció per tal d'encararnos bé cap a la paret**/
            if (sen_left < sen_center && sen_left < sen_right){
                turnLeft(150);
            }else if (sen_right < sen_center && sen_left > sen_right){
                turnRight(150);
            }
            forward(150);

            /** Ara mirem el sensor centre, si veu que està molt a prop de la paret busca quin costat està més a prop de la paret i el segueix
             * Agafant el costat ara sabem quina paret hem de segir i per tant tenim una paret (got_wall = true)
             * I també marquem el costat en el que està la paret amb el follow wall (true = esquerra, false=dreta)**/
            if(sen_center <= dist_max){
                stop();
                if (sen_left < sen_right){
                    follow_wall = true;
                    got_wall = true;
                }else if (sen_left > sen_right){
                    follow_wall = false;
                    got_wall = true;
                }
            }


        }else if(got_wall == true){
            /** Ara ja hem trobat una paret i per tant la podem seguir. l'algorisme està dividit en dos parts utilitzant l'if per saber a quin costat està la paret**/
            if(follow_wall){

                /** Si seguint la paret ens trobem amb un obstacle al davant el motor girarà cap al costat contrari de la paret
                  fins que el sesor del centre ja no estigui detectant l'obstacle, així ens assegurem que hem deixat l'obstacle al mateix
                  costat que la paret que estavem seguint, fent així que l'obstacle sigui la paret que hem de seguir**/
                if(sen_center <= dist_max){ //girar
                    while (sen_center < dist_seguretat){
                        printf("-------------------------------------------------Drift-----------------------------------");
                        turnOnItselfRight(100);
                        printf("-------------------------------------------------Drift-----------------------------------");
                        sen_center = redObsDistance(3, 0x1b);
                    }
                }
                /** Si no troba cap obstacle, comprovem que no ens estem acostant a la paret, si estem massa a prop girem cap a la dreta
                 * i si ens allunyem massa girem cap a l'esquerra**/
                else if(sen_left <= dist_min){
                    turnRight(150);
                }
                else if(sen_left > dist_max){
                    turnLeft(150);
                }
                forward(150);
            }
            else if(follow_wall == false){
                /** Aquesta part és la mateixa que l'anterior pero amb els valors canviats per seguir la paret a la dreta**/
                if(sen_center <= dist_max){
                    while (sen_center < dist_seguretat){
                        printf("-------------------------------------------------Drift-----------------------------------");
                        turnOnItselfLeft(100);
                        printf("-------------------------------------------------Drift-----------------------------------");
                        sen_center = redObsDistance(3, 0x1b);
                    }
                }
                else if(sen_right <= dist_min){
                    turnRight(150);
                }
                else if(sen_right > dist_max){
                    turnLeft(150);
                }
                forward(150);
            }
        }



        Get_estado(&estado, &estado_anterior);
        if (estado != estado_anterior) {
            Set_estado_anterior(estado);
            printf("estado = %d\n", estado);
            switch (estado) {
                case Sw1:
                    printf("Boton Sw1 ('a') apretado\n");
                    dyn_led_control(1, 1); //Probaremos de encender el led del motor 2
                    printf("\n");
                    break;
                case Sw2:
                    printf("Boton Sw2 ('s') apretado\n");
                    dyn_led_control(1, 0); //Probaremos de apagar el led del motor 2
                    printf("\n");
                    break;
                case Up:

                    break;
                case Down:

                    break;
                case Left:
                    //Comprobaremos si detectamos las esquinas de la pared izquierda:
                    printf("Esquina inferior izquierda:\n");
                    printf("(1, 1): %d (fuera pared)\n", obstaculo(1, 1, datos_habitacion));
                    printf("(0, 1): %d (pared izq.)\n", obstaculo(0, 1, datos_habitacion));
                    printf("(1, 0): %d (pared del.)\n", obstaculo(1, 0, datos_habitacion));
                    printf("(0, 0): %d (esquina)\n", obstaculo(0, 0, datos_habitacion));
                    printf("Esquina superior izquierda:\n");
                    printf("(1, 4094): %d (fuera pared)\n", obstaculo(1, 4094, datos_habitacion));
                    printf("(0, 4094): %d (pared izq.)\n", obstaculo(0, 4094, datos_habitacion));
                    printf("(1, 4095): %d (pared fondo.)\n", obstaculo(1, 4095, datos_habitacion));
                    printf("(0, 4095): %d (esquina)\n", obstaculo(0, 4095, datos_habitacion));
                    break;
                case Right:
                    //Comprobaremos si detectamos las esquinas de la pared derecha:
                    printf("Esquina inferior derecha:\n");
                    printf("(4094, 1): %d (fuera pared)\n", obstaculo(4094, 1, datos_habitacion));
                    printf("(4094, 0): %d (pared del.)\n", obstaculo(4094, 0, datos_habitacion));
                    printf("(4095, 1): %d (pared der.)\n", obstaculo(4095, 1, datos_habitacion));
                    printf("(4095, 0): %d (esquina)\n", obstaculo(4095, 0, datos_habitacion));
                    printf("Esquina superior derecha:\n");
                    printf("(4094, 4094): %d (fuera pared)\n", obstaculo(4094, 4094, datos_habitacion));
                    printf("(4094, 4095): %d (pared fondo)\n", obstaculo(4094, 4095, datos_habitacion));
                    printf("(4095, 4094): %d (pared der.)\n", obstaculo(4095, 4094, datos_habitacion));
                    printf("(4095, 4095): %d (esquina)\n", obstaculo(4095, 4095, datos_habitacion));
                    break;
                case Center:

                    break;
                case Quit:
                    printf("Adios!\n");
                    break;
                    //etc, etc...
            }
            fflush(stdout);
        }
    }
    //Signal the emulation thread to stop
    pthread_kill(tid, SIGTERM);
    pthread_kill(jid, SIGTERM);
    printf("Programa terminado\n");
    fflush(stdout);
}
