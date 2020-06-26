/*
 * hal_dyn.c
 *
 *  Created on: 18 mar. 2020
 *      Author: droma
 *
 * This file implements the different dynamixel instructions
 */

#include "dyn/dyn_instr.h"
#include "dyn/dyn_frames.h"

/**
 * Single byte write instruction
 *
 * This function sends a write instruction for a single address position
 * to a given dynamixel module.
 *
 * @param[in] module_id Id of the dynamixel module
 * @param[in] reg_addr Address where the write is performed
 * @param[in] reg_write_val Value written to the previous address
 * @return Error code to be treated at higher levels.
 */
int dyn_write_byte(uint8_t module_id, DYN_REG_t reg_addr, uint8_t reg_write_val) {
    uint8_t parameters[2];
    struct RxReturn reply;

    parameters[0] = reg_addr;
    parameters[1] = reg_write_val;
    reply = RxTxPacket(module_id, 2, DYN_INSTR__WRITE, parameters);
    return (reply.tx_err << 1) | reply.time_out;
}

/**
 * Single byte read instruction
 *
 * This function sends a read instruction for a single address position
 * to a given dynamixel module.
 *
 * @param[in] module_id Id of the dynamixel module
 * @param[in] reg_addr Address where the read is performed
 * @param[out] reg_read_val Pointer where the read value is stored
 * @return Error code to be treated at higher levels.
 */
int dyn_read_byte(uint8_t module_id, DYN_REG_t reg_addr, uint8_t *reg_read_val) {
    uint8_t parameters[2];
    struct RxReturn reply;

    parameters[0] = reg_addr;
    parameters[1] = 1;
    reply = RxTxPacket(module_id, 2, DYN_INSTR__READ, parameters);
    *reg_read_val = reply.StatusPacket[5];

    return (reply.tx_err << 1) | reply.time_out;
}

/**
 * Multi-byte write instruction
 *
 * This function sends a write instruction starting at a given address position
 * with a given length for a dynamixel module.
 *
 * @param[in] module_id Id of the dynamixel module
 * @param[in] reg_addr Address where the write is performed
 * @param[in] val Pointer to the byte array to be written
 * @param[in] len Number of position to be written
 * @return Error code to be treated at higher levels.
 */
int dyn_write(uint8_t module_id, DYN_REG_t reg_addr, uint8_t *val, uint8_t len) {

    uint8_t new_pack[len+1];
    struct RxReturn ret_Rx; /** Creem un nou paquet de dades i una variable de retorn del Rx packet**/

    uint8_t new_len = len+1;
    uint8_t new_ins = DYN_INSTR__WRITE;
    new_pack[0] = reg_addr;

    int i; /** El primer valor del nou packet és la adreça, per tant ara que li passem el paràmetres de la instrucció els
               guardarem a partir de la posició 1**/
    for (i = 1; i < len + 1; i++){
        new_pack[i] = val[i-1];
    }

    ret_Rx = RxTxPacket(module_id, new_len, new_ins, new_pack);

    return (ret_Rx.tx_err < 1) | ret_Rx.time_out; /** Vigilem que no hi hagi cap error**/
}

