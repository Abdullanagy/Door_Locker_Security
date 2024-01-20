/*
 * Control_ECU.h
 *
 *  Created on: Jan 20, 2024
 *      Author: Macbook Pro
 */

#ifndef CONTROL_ECU_H_
#define CONTROL_ECU_H_


/*********************************************************************************
 *                            Important Macros                                   *
 ********************************************************************************/

#define creat_password 0x01
#define required_operation 0x02
#define check_reentered_pass 0x03
#define open_dooor 0x04
#define changee_pass 0x05
#define ERROR 0x06
#define MC2_Ready 0x20

void change_password(void);
void buzzer_error(void);
void open_Door(void);
void check_reenter_pass(void);


#endif /* CONTROL_ECU_H_ */
