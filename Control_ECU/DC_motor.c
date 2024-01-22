/******************************************************************************
 *
 * Module: DC_Motor
 *
 * File Name: dc_motor.c
 *
 * Description: Source file for the DC_Motor driver
 *
 * Author: Abdulla Nagy
 *
 *******************************************************************************/


#include"avr/io.h"
#include"DC_motor.h"
#include"gpio.h"



/************* Initialization The DC_motor Pins ********************/
void DCMotor_Init(void){

	DDRA |= 0x70;
	/* Motor is stop at the beginning */
	PORTA &=~(1<<PA4);
	PORTA &=~(1<<PA5);
	PORTA |=(1<<PA6);
}
/************ set the wanted operations of DC-Motor *********************/
void DCMotor_Rotate(uint8 state){

	switch(state)
	{

	case CW://Turn_ON the DC_motor with clock wise rotation
		GPIO_writePin(PORTA_ID, PIN4_ID, LOGIC_LOW);
		GPIO_writePin(PORTA_ID,PIN5_ID, LOGIC_HIGH);
		break;
	case ACW://Turn_ON the DC_motor with clock  Anti wise rotation
		GPIO_writePin(PORTA_ID, PIN4_ID, LOGIC_HIGH);
		GPIO_writePin(PORTA_ID, PIN5_ID, LOGIC_LOW);
		break;
	case stop://Turn_OFF DC-Motor
		GPIO_writePin(PORTA_ID, PIN4_ID, LOGIC_LOW);
		GPIO_writePin(PORTA_ID, PIN5_ID, LOGIC_LOW);
		break;
	}

}
/************* DE_Initialization DC_Motor *************/
void DCMotor_deInit(void){
	PORTA &=~(1<<PA4);
	PORTA &=~(1<<PA5);
	PORTA &=~(1<<PA6);
}
