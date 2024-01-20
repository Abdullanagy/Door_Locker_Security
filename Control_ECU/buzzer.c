/******************************************************************************
 *
 * Module: BUZZER
 *
 * File Name: buzzer.c
 *
 * Description: Source file for the BUZZER driver
 *
 * Author: Abdulla Nagy
 *
 *******************************************************************************/

#include"buzzer.h"
#include"avr/io.h"
/********** Buzzer Initialization function *************************/
void Buzzer_init(void){

	DDRD |=(1<<PD7);

	PORTD&=~(1<<PD7);

}
/************** Turn ON Buzzer ************/
void Buzzer_on(void){

	PORTD |=(1<<PD7);

}
/******************Turn OFF Buzzer  **********/
void Buzzer_off(){

	PORTD &=~(1<<PD7);
}
