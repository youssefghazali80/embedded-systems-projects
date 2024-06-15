/*
 * motor.c
 *
 *  Created on: Feb 21, 2024
 *      Author: youssef
 */

#include "motor.h"
#include "gpio.h"
#include "pwm.h"


/*
 * Description
➢ The Function responsible for setup the direction for the two
   motor pins through the GPIO driver.
➢ Stop at the DC-Motor at the beginning through the GPIO driver
 */
void DcMotor_init(void){

	/*
	 * setup the direction of the two pins of the motor and stop the motor initially
	 */
	GPIO_setupPinDirection(MOTOR_PIN1_PORT_ID,MOTOR_PIN1_PIN_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(MOTOR_PIN2_PORT_ID,MOTOR_PIN2_PIN_ID,PIN_OUTPUT);
	GPIO_writePin(MOTOR_PIN1_PORT_ID,MOTOR_PIN1_PIN_ID,LOGIC_LOW);
	GPIO_writePin(MOTOR_PIN2_PORT_ID,MOTOR_PIN2_PIN_ID,LOGIC_LOW);



}


/*
 * Description:
➢ The function responsible for rotate the DC Motor CW/ or A-CW or
     stop the motor based on the state input state value.
➢ Send the required duty cycle to the PWM driver based on the
      required speed value.
 */


void DcMotor_Rotate(DcMotor_State state,uint8 speed){
	// initialize the enable pin of the motor that controls the speed to timer 0 pwm pin OC0
	PWM_Timer0_Start(speed);
	if  (state == STOP){

		// Write the two pins that are connected to the motor to logic zero to stop the motor
		GPIO_writePin(MOTOR_PIN1_PORT_ID,MOTOR_PIN1_PIN_ID,LOGIC_LOW);
		GPIO_writePin(MOTOR_PIN2_PORT_ID,MOTOR_PIN2_PIN_ID,LOGIC_LOW);

	}
	else if (state == CW){
		/*
		 * write the first pin to logic low and the second
		 * write the second pin to logic high to
		 * the motor rotates clock wise
		 */
		GPIO_writePin(MOTOR_PIN1_PORT_ID,MOTOR_PIN1_PIN_ID,LOGIC_LOW);
		GPIO_writePin(MOTOR_PIN2_PORT_ID,MOTOR_PIN2_PIN_ID,LOGIC_HIGH);

	}
	else if (state ==A_CW){
		/*
		 * write the first pin to logic high and the second
		 * write the second pin to logic low
		 * the  motor rotates anti clock wise
		*/
		GPIO_writePin(MOTOR_PIN1_PORT_ID,MOTOR_PIN1_PIN_ID,LOGIC_HIGH);
		GPIO_writePin(MOTOR_PIN2_PORT_ID,MOTOR_PIN2_PIN_ID,LOGIC_LOW);

	}


}
