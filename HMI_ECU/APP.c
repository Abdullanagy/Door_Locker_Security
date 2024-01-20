/**********************************************************************************
 *Description: Source File For Application Layer Code
 *
 *HMI_ECU (Human Interface)
 *
 *Author: Abdulla Nagy
 **********************************************************************************/
#include "APP.h"

#include"util/delay.h"
#include"timer1.h"
#include "avr/io.h"
#include"uart.h"
#include"lcd.h"
#include"keypad.h"

/*********************************************************************************
 *                            Important Macros                                   *
 ********************************************************************************/
#define creat_password 0x01
#define required_operation 0x02
#define check_reentered_pass 0x03
#define open_dooor 0x04
#define changee_pass 0x05
#define ERROR 0x06
#define MC2_Ready 0x02
/*********************************************************************************
 *                            Global Variables                                   *
 ********************************************************************************/
uint8 counter1=1;
uint8 operation=0;
uint8 g_count = 0 ;

/*********************************************************************************
 *                            Users Function                                  *
 ********************************************************************************/

void Timer(void) /* Every 1 Second Timer ISR Call This Function */
{
	g_count++ ;
}

/* Description: Delay Function using Timer1*/
void delay(uint8 second)
{
	/* Configure Timer With Desired Specifications */
	Timer1_ConfigType t_configure = {0 , 7812 , Fcpu1024 , CTC };

	Timer1_init(&t_configure); /* Start Timer */
	while(g_count != second); /* Wait until g_count=input seconds  */
	Timer1_deInit(); /* Stop Timer */
	g_count = 0 ;
}

/*********** Creat New Password ************************/
void creat_pass(void){
	int check=1;
	uint8 pass1[5]={0};
	uint8 pass2[5]={0};

	/*********************** To save first Password entering from user to HMI_ECU ************************/
	LCD_clearScreen();//clear the LCD
	LCD_displayString("PLZ enter Pass :");//Display on LCD
	while(KEYPAD_getPressedKey()!=61){//wait to save all first input password

		for (int i=0;i<5;i++){
			pass1[i]=KEYPAD_getPressedKey();
			_delay_ms(100);
			LCD_moveCursor(1,i);
			LCD_displayString("*");
			_delay_ms(200);
		}
	}
	_delay_ms(100);
	/********************** To Send The Entered Password to Control_ECU ************************************/

	/**************** TO Save the  Re-entered  Password from user *******************/

	LCD_clearScreen();
	LCD_moveCursor(0,0);
	LCD_displayString("PLZ Re-enter The");
	LCD_moveCursor(1,0);
	LCD_displayString("Same Pass :");

	while(KEYPAD_getPressedKey()!=61){//wait to save all first input password

		for (int x=0;x<5;x++){
			pass2[x]=KEYPAD_getPressedKey();
			_delay_ms(100);
			LCD_moveCursor(1,x+11);
			LCD_displayString("*");
			_delay_ms(200);
		}
	}
	_delay_ms(100);

	/********************** To Send The Entered Password to Control_ECU ************************************/

	UART_sendByte(creat_password);
	_delay_ms(10);
	for(int i=0;i<5;i++){
		UART_sendByte(pass1[i]);
		_delay_ms(10);
	}

	for(int i=0;i<5;i++){
		UART_sendByte(pass2[i]);
		_delay_ms(10);
	}
	_delay_ms(50);
/******************* Receive the result of the check operation from Control_ECU  */
	check=UART_recieveByte();
	_delay_ms(50);

	if(check==1){
		UART_sendByte(required_operation ); //send the required operation to Control_ECU
		_delay_ms(10);
		main_option();//call the main options
	}
	else{
		creat_pass();//re-call the creat new password function
	}

}
void reenter_pass(void){
	int check1;

	uint8 pass3[5]={0};
	/*********************** To save first Password entering from user to HMI_ECU ************************/

	LCD_clearScreen();//clear LCD
	LCD_displayString("PLZ enter YOUR");//display on LCD
	LCD_moveCursor(1,0);
	LCD_displayString("Pass :");

	while(KEYPAD_getPressedKey()!=61){/*wait to save re-input password to check it is true or false compared with
		                                        the first one saved in external eeprom */
		for (int i=0;i<5;i++){

			pass3[i]=KEYPAD_getPressedKey();
			_delay_ms(100);

			LCD_moveCursor(1,i+7);
			LCD_displayString("*");
			_delay_ms(300);

		}
	}
	_delay_ms(100);
	/********************** To Send The Re-Entered Password to Control_ECU ************************************/
	UART_sendByte(check_reentered_pass);
	_delay_ms(20);
	for(int i=0;i<5;i++){
		UART_sendByte(pass3[i]);
		_delay_ms(20);
	}
	_delay_ms(250);
/******************** Receive the Result of the check operation from the Control_ECU */
	check1=UART_recieveByte();
	_delay_ms(20);

	if(check1==1){

		if(operation==43){

			open_door();//call the open-door function
			main_option();//then re-call the main-option function
		}

		else if(operation==45){
			UART_sendByte(changee_pass);//send the required operation to the Control_ECU
			_delay_ms(10);
			change_pass();//call the change password function
		}
	}


	else if(check1==0){

		counter1++;
		if(counter1<4){

			reenter_pass();// re-call the re-entered password function again
		}

		else{
			counter1=1;

			Error_display();//call the error function

			main_option();//then call the main option function after the error function it is done

		}
	}

}

void main_option(void){

	/**************************** To Display The Main Options on LCD ****************************/

	LCD_clearScreen();//clear the LCD
	LCD_moveCursor(0,0);//move the cursor of LCD
	LCD_displayString("+ : OPEN DOOR");//display on LCD
	LCD_moveCursor(1,0);
	LCD_displayString("- : CHANGE PASS");
	_delay_ms(50);

	operation=KEYPAD_getPressedKey();
	_delay_ms(20);
	/*********** send the required operation to the Control_ECU */
	UART_sendByte(required_operation);
	_delay_ms(20);
	/***************** send what the user operation wanted ***************/
	UART_sendByte(operation);
	_delay_ms(20);

	reenter_pass();//call the re-entered function




}


void open_door(void){
	UART_sendByte(open_dooor);
	_delay_ms(10);

	LCD_clearScreen();//clear LCD
	LCD_displayStringRowColumn(0, 1, "Door Unlocking");//display on LCD
	delay(15); /* Waiting For 15 Seconds */

	/* Door Holding */
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 1, "Door Holding");
	delay(15); /* Waiting For 3 Seconds */

	/* Door locking */
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 1, "Door Locking");
	delay(15); /* Waiting For 15 Seconds */
}

void change_pass(void){

	creat_pass();//re-call creat new password function

}
void Error_display(void){
/************* send ERROR to the Control_ECU to Turn ON the Buzzer ***********/
	UART_sendByte(ERROR);
	_delay_ms(10);
	LCD_clearScreen();
	LCD_moveCursor(0,0);
	LCD_displayString("SYSTEM IS LOCKED");
	LCD_moveCursor(1,0);
	LCD_displayString("for :");

	//count down 60 seconds delay
	for(int i=60;i>=0;i--){

		LCD_moveCursor(1,7);
		LCD_intgerToString(i);
		LCD_displayString(" ");
		delay(1);

	}
}


int main(void){

	SREG |= (1<<7); /* Enable global interrupts in MC */
	LCD_init();
	Timer1_setCallBack(Timer); /* Setup CallBAck Function For Timer */
	UART_configType  configurations ={eight_bits,disable,one_bit,9600};
	UART_init(&configurations);
	UART_recieveByte(); /* Waiting For MCU2 To be Ready */

	while(1){

		creat_pass();

	}
}
