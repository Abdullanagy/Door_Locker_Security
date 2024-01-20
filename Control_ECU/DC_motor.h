/******************************************************************************
 *
 * Module: DC_Motor
 *
 * File Name: DC_Motor.h
 *
 * Description: Header file for the DC_Motor driver
 *
 * Author: Abdulla Nagy
 *
 *******************************************************************************/

#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_
#include"std_types.h"
/* Static Configurations */
typedef enum{
	stop,CW,ACW
}DCMotor_state;
DCMotor_state state;

/* Functions Prototype */
void DCMotor_Init(void);

void DCMotor_Rotate(uint8 state);

void DCMotor_deInit(void);
#endif /* DC_MOTOR_H_ */
