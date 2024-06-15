/*
 * buzzer.c
 *
 *  Created on: Mar 21, 2024
 *      Author: youssef
 */

#include "buzzer.h"
#include "std_types.h"


/*
 * Description:
    Setup the direction for the buzzer pin as output pin
    through the GPIO driver.
   Inputs: None
   Return: None
 */
void Buzzer_init(){
	GPIO_setupPinDirection(BUZZER_PORT_ID,BUZZER_PIN_ID,PIN_OUTPUT);

}

/*
 *  Description :
        Function to enable the Buzzer through the GPIO.
    Inputs: None
    Return: None
 */
void Buzzer_on(void){
	GPIO_writePin(BUZZER_PORT_ID , BUZZER_PIN_ID ,LOGIC_HIGH);
}

/*
 * Description :
   	   Function to disable the Buzzer through the GPIO.
   Inputs: None
   Return: No
 *
 */
void Buzzer_off(void){
	GPIO_writePin(BUZZER_PORT_ID , BUZZER_PIN_ID ,LOGIC_LOW);
}






