/*
 * motor.h
 *
 *  Created on: Feb 21, 2024
 *      Author: youssef
 */

#ifndef MOTOR_H_
#define MOTOR_H_
#include "std_types.h"
/******************************************* DEFINITIONS  ***************************************/

#define MOTOR_PIN1_PORT_ID        PORTB_ID
#define MOTOR_PIN1_PIN_ID         PIN0_ID
#define MOTOR_PIN2_PORT_ID        PORTB_ID
#define MOTOR_PIN2_PIN_ID         PIN1_ID
#define MOTOR_SPEED_PIN_PORT_ID   PORTB_ID
#define MOTOR_SPEED_PIN_PIN_ID    PIN3_ID




/*******************************************  TYPES DECLARATION ************************************/


typedef enum {
	CW ,A_CW ,STOP
}DcMotor_State;





/************************************  FUNCTION DECLARATION ****************************************/


/*
 * Description
➢ The Function responsible for setup the direction for the two
   motor pins through the GPIO driver.
➢ Stop at the DC-Motor at the beginning through the GPIO driver
 */
void DcMotor_init(void);


/*
 * Description:
➢ The function responsible for rotate the DC Motor CW/ or A-CW or
     stop the motor based on the state input state value.
➢ Send the required duty cycle to the PWM driver based on the
      required speed value.
 */


void DcMotor_Rotate(DcMotor_State state,uint8 speed);
#endif /* MOTOR_H_ */
