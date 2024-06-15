/*
 * timer1.h
 *
 *  Created on: Mar 21, 2024
 *      Author: youssef
 */

#ifndef TIMER1_H_
#define TIMER1_H_
#include "gpio.h"
#include "std_types.h"

/******************************DEFINITIONS *********************************/





/******************************** TYPE DECLARATIONS **************************/
typedef enum {
	NO_CLOCK ,F_CPU_1 , F_CPU_8 ,F_CPU_64 ,
	F_CPU_256 ,F_CPU_1024,
	EXTERNAL_FALLING,EXTERNAL__RISING

}Timer1_Prescaler;
typedef enum{
	NORMAL , CTC

}Timer1_Mode;

typedef struct {
 uint16 initial_value;
 uint16 compare_value; // it will be used in compare mode only.
 Timer1_Prescaler prescaler;
 Timer1_Mode mode;
} Timer1_ConfigType;

/***************************     Functions prototypes **************************/
void Timer1_init(const Timer1_ConfigType* Config_Ptr);
/*
 *  Description :
          Function to initialize the Timer driver
          Inputs: pointer to the configuration structure with type
          Timer1_ConfigType.
     Return: None
 */


void Timer1_deInit(void);
/*
 *  Description :
          Function to disable the Timer1.
    Inputs: None
    Return: None
 */

void Timer1_setCallBack(void(*a_ptr)(void));
/*
 *  Description :
           Function to set the Call Back function address.
    Inputs: pointer to Call Back function.
    Return: None
 */



#endif /* TIMER1_H_ */
