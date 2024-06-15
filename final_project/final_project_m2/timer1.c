/*
 * timer1.c
 *
 *  Created on: Mar 22, 2024
 *      Author: youssef
 */

#include "timer1.h"
#include <avr/io.h>
#include "common_macros.h"
#include <avr/interrupt.h>

/************************  Global variables *********************************/

static volatile void (*g_callBackPtr)(void) = NULL_PTR;
/***********************     functions      ************************************/

void Timer1_init(const Timer1_ConfigType* Config_Ptr){

	/* Timer1 always operates in Normal Mode */
	TCCR1A = (1<<FOC1A) | (1<<FOC1B);

	TCCR1B =Config_Ptr -> prescaler  ;  // configure the prescaler of the timer
	if (Config_Ptr -> mode == NORMAL){

		// Modify The Bits of The normal mode  WGM10=0 , WGM11=0, WGM12=0 , WGM13=0
		CLEAR_BIT(TCCR1A,WGM10);
		CLEAR_BIT(TCCR1A,WGM11);
		CLEAR_BIT(TCCR1B,WGM12);
		CLEAR_BIT(TCCR1B,WGM13);
		// enable the interrupt of the overflow mode
		SET_BIT(TIMSK ,TOIE1);
	}
	else if (Config_Ptr -> mode == CTC){
		// Modify The Bits of the normal mode  WGM10=0 ,WGM11=0 ,WGM12=1 , WGM13=0
		CLEAR_BIT(TCCR1A,WGM10);
		CLEAR_BIT(TCCR1A,WGM11);
		SET_BIT(TCCR1B,WGM12);
		CLEAR_BIT(TCCR1B,WGM13);
		// enable the interrupt of the compare on outpput compare register A
		SET_BIT (TIMSK ,OCIE1A);
		OCR1A = Config_Ptr -> compare_value;

	}


	TCNT1 = Config_Ptr ->initial_value; // insert the initial value in the timer counter register




}
/*
 *  Description :
          Function to initialize the Timer driver
          Inputs: pointer to the configuration structure with type
          Timer1_ConfigType.
     Return: None
 */


void Timer1_deInit(void){
	/* Clear All Timer1/ICU Registers */
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1 = 0;
	ICR1 = 0;
	TIMSK=0;
	/* Reset the global pointer value */
	g_callBackPtr = NULL_PTR;




}

/*
 *  Description :
          Function to disable the Timer1.
    Inputs: None
    Return: None
 */

void Timer1_setCallBack(void(*a_ptr)(void)){

	/* Save the address of the Call back function in a global variable */
	g_callBackPtr = a_ptr  ;


}
/*
 *  Description :
           Function to set the Call Back function address.
    Inputs: pointer to Call Back function.
    Return: None
 */


ISR (TIMER1_OVF_vect){
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}



ISR (TIMER1_COMPA_vect){
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}
