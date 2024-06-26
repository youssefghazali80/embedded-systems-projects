/*
 * buzzer.h
 *
 *  Created on: Mar 21, 2024
 *      Author: youssef
 */

#ifndef BUZZER_H_
#define BUZZER_H_

#include "gpio.h"
/******************************* Definitions *******************************/
#define BUZZER_PORT_ID  PORTA_ID
#define BUZZER_PIN_ID   PIN6_ID


/*
 * Description:
    Setup the direction for the buzzer pin as output pin
    through the GPIO driver.
   Inputs: None
   Return: None
 */
void Buzzer_init(void);

/*
 *  Description :
        Function to enable the Buzzer through the GPIO.
    Inputs: None
    Return: None
 */
void Buzzer_on(void);

/*
 * Description :
   	   Function to disable the Buzzer through the GPIO.
   Inputs: None
   Return: No
 *
 */
void Buzzer_off(void);



#endif /* BUZZER_H_ */
