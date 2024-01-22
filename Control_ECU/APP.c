/**********************************************************************************
 *Description: Source File For Application Layer Code
 *
 *Control_ECU
 *
 *Author: Abdulla Nagy
 **********************************************************************************/

#include"buzzer.h"
#include"timer1.h"
#include "util/delay.h"
#include"uart.h"
#include"avr/io.h"
#include"DC_motor.h"
#include"Control_ECU.h"


/*********************************************************************************
 *                            Global Variables                                   *
 ********************************************************************************/

uint8 operation=0;
uint8 check=1;
uint8 counter1=1;
uint8 value1[5];
uint8 value2[5];
uint8 g_count = 0 ;
uint8 flagg=0;
/*********************************************************************************
 *                            Users Function                                  *
 ********************************************************************************/



void Timer(void) /* Every 1 Second Timer ISR Call This Function */
{
	g_count++ ;
}

/*Description: Delay Function using Timer 1*/
void delay(uint8 second)
{
	/* Configure Timer With Desired Specifications */
	Timer1_ConfigType t_configure = {0 , 7812 , Fcpu1024 , CTC };

	Timer1_init(&t_configure); /* Start Timer */
	while(g_count != second); /* Waiting */
	Timer1_deInit(); /* Stop Timer */
	g_count = 0 ;
}


void creat_pass(void){
	/********************* RECEIVE THE PASSWORD FROM HMI_ECU    */

	for(int i=0;i<5;i++){
		value1[i]=UART_recieveByte();
		_delay_ms(10);
	}
	for(int i=0;i<5;i++){
		value2[i]=UART_recieveByte();
		_delay_ms(10);
	}

	/****************check if the entered passwords are same or not *************/
	check=1;
	for(int i=0;i<5;i++){
		if(value1[i]!=value2[i]){
			check=0;
			_delay_ms(10);
		}

	}
	_delay_ms(50);
	/************** Send the result of check process  *************/
	UART_sendByte(check);
	_delay_ms(100);
	/************** SAVE the password at the external EEPROM if check==1   */
	if (check==1){
		flagg=0;
		/* Initialize the TWI/I2C Driver */
		TWI_init();
		EEPROM_writeArray((0x0311),value1,5); /* Write 0x0F in the external EEPROM */
		_delay_ms(100);

	}
}

void main_option(void){
	/****** RECEIVE the required operation from the HMI_ECU */
	operation=UART_recieveByte();
	_delay_ms(50);
}

void check_reenter_pass(void){

	uint8 value3[5];
	/************* RECEIVE the re-entered password from HMI_ECU ****************/
	for(int i=0;i<5;i++){
		value3[i]=UART_recieveByte();
		_delay_ms(20);
	}
	_delay_ms(50);
	/************ read the password already saved at external eeprom to compare it with the received re-entered password */
	TWI_init();
	EEPROM_readArray((0x0311),&value1,5); /* Write 0x0F in the external EEPROM */
	_delay_ms(100);


	/****************check if the entered passwords are the same *************/
	check=1;

	for(int i=0;i<5;i++){

		if(value3[i]!=value1[i]){
			check=0;
			_delay_ms(20);
		}

	}
	_delay_ms(50);
	/********** send the result of check process for HMI_ECU */
	UART_sendByte(check);
	_delay_ms(100);

}


void open_Door(void){


	/****** To Turn ON Timer1 to rotate aand hold the DC Motor
	 * "Rotate_CW_15_Seconds" or "Stop_15_Seconds" or "Rotate_ACW_15_Seconds" ***************/

	/*********** DC_motor  "Rotate_Clock_Wise_15_Seconds" *************/
	DCMotor_Rotate(CW);
	delay(15);
	/*********** DC_motor  "stop_15_Seconds" *************/
	DCMotor_Rotate(stop);
	delay(5);
	/*********** DC_motor  "Rotate_Anti_Clock_Wise_15_Seconds" *************/
	DCMotor_Rotate(ACW);
	delay(15);
	/************* De_intialization for DC Motor *************/
	DCMotor_deInit();
	/*************** call the main_option function *************/
	main_option();

}

void change_password(void){
	/************** call the creat_pass function ***************/
	creat_pass();
}
void buzzer_error(void){
	/****************** Turn ON Buzzer ****************/
	Buzzer_on();
	//delay 62 seconds
	for(int i=0;i<11;i++){
		delay(1);
	}
	/*********** Turn OFF Buzzer *****************/
	Buzzer_off();
	/********* re-call the main_option function */
	main_option();
}

int main(void)
{

	Buzzer_init(); //initialization Buzzer
	DCMotor_Init();//initialization DC_MOTOR

	SREG |=(1<<7);//SET the Global interrupt
	Timer1_setCallBack(Timer); /* Setup CallBAck Function For Timer */
	UART_configType  configurations ={eight_bits,disable,one_bit,9600};//set the value of configuration
	UART_init(&configurations);//initialization the UART
	_delay_ms(50);
	UART_sendByte(MC2_Ready); /* Send to 1st MCU That MCU2 is Ready To Receive */

	while(1)
	{

		int required=UART_recieveByte();//receive the required operation from HMI_ECU

		switch (required){
		case creat_password : creat_pass();
		break;
		case required_operation : main_option();
		break;
		case check_reentered_pass : check_reenter_pass();
		break;
		case open_dooor: open_Door();
		break;
		case ERROR: buzzer_error();
		break;

		}
	}
}
