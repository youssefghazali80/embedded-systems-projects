/*
 * pwm.c
 *
 *  Created on: Feb 20, 2024
 *      Author: youssef
 */


#include "pwm.h"
#include "std_types.h"



/*************************************  Definitions  ********************************************/
#define PWM_PORT_ID            PORTB_ID
#define PWM_PIN_ID             PIN3_ID
/*
 * Description:
 * ➢ The function responsible for trigger the Timer0 with the PWM Mode.
   ➢ Setup the PWM mode with Non-Inverting.
   ➢ Setup the prescaler with F_CPU/8.
   ➢ Setup the compare value based on the required input duty cycle
   ➢ Setup the direction for OC0 as output pin through the GPIO driver.
   ➢ The generated PWM signal frequency will be 500Hz to control the DC
       Motor speed
 */
void PWM_Timer0_Start(uint8 duty_cycle){
	/*
	 * setup the pwm_mode  and the frequency of the timer
	 */
	TCCR0 = (1<<WGM01)|(1<<WGM00)|(1<<COM01)|(1<<CS01);

	/*
	 * setup the pin OC0 as output to put the pwm on it
	 */
	GPIO_setupPinDirection(PWM_PORT_ID , PWM_PIN_ID,PIN_OUTPUT);
	TCNT0 = 0;
	OCR0  = 255 * duty_cycle / 100  ;





}
