 /******************************************************************************
 *
 * Module: TIMER 1
 *
 * File Name: timerOne.h
 *
 * Description: Header file for the Timer 1 AVR driver
 *
 * Author: Abdulla Nagy
 *
 *******************************************************************************/
#ifndef TIMERONE_H_
#define TIMERONE_H_

#include "std_types.h"

/****************************** Modes Configure **************************************/
#define COMPARE        /* For Normal Mode OR Compare Mode */
#define COMPARE1A      /* (COMPARE1A OR COMPARE1B(Compare Mode)) OR (NULL(Normal Mode)) */


/********************************* Data Types **************************************/
typedef enum
{
	No_CLK , Fcpu , Fcpu8 , Fcpu64 , Fcpu256 , Fcpu1024
}Timer1_Prescaler;

typedef enum
{
	Normal , CTC = 4
}Timer1_Mode;

typedef struct {
uint16 initial_value;
#ifdef COMPARE
uint16 compare_value; /* it will be used in compare mode only. */
#endif
Timer1_Prescaler prescaler;
Timer1_Mode mode;
} Timer1_ConfigType;

/****************************** Functions Prototype *********************************/
void Timer1_init(const Timer1_ConfigType * Config_Ptr);
void Timer1_deInit(void);
void Timer1_setCallBack(void (*a_ptr)(void));

#endif /* TIMERONE_H_ */
