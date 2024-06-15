/*
 * main.c
 *
 *  Created on: Mar 21, 2024
 *      Author: youssef
 */
#include "motor.h"
#include "gpio.h"
#include "buzzer.h"
#include "timer1.h"
#include <avr/io.h>
#include "uart.h"
#include "external_eeprom.h"
#include "twi.h"
#include <util/delay.h>

#define MC2_READY     0x10
#define F_CPU 8000000UL
#define PASS_ADDRESS   0x0311

/******************************* global variables ****************************/
volatile boolean wait = FALSE;


// flag that detects if the password is incorrect for three consecutive times
boolean error = FALSE;

volatile uint8 super_tick =0;

/****************************** function prototyping **************************/
;/*
 * function that is given two passwords and then check if they are both equal
 */
boolean check_password(uint8 * pass1 , uint8 * pass2);


/*
 * function that creates the password at the beginning of the
 * application by getting it and then checks the equality and then
 * saves it if it is a valid password
 */
void create_password(void);
/*
 * function that save the password to the EEPROM
 */
void save_password(uint8 * pass , uint16 adress);
/*
 * function that get the password from the user and then compare it with the
 * saved on in the EEEPROM and return the result
 */
boolean check_entered_password(void);

/*
 * function to extract the password from EEPROM
 */
void get_password(uint8 * pass , uint16 address);

/*
 * function that takes the password entered and checks on it then proceeds
 * the action that is entered by the user and return true if the action is done
 */
boolean do_action(void);

/*
 * function that displays the status of the door while unlocking
 * it
 *
 */
void unlock_door(void);
/*
 * function that displays the status of the door while unlocking
 * it
 *
 */
void lock_door(void);

/*
 * function that clears the screen indicating that the door is being
 * hold
 *
 */
void hold_door(void);
/*
 * function to control the door status messages timing
 */
void lock_unlock(void);

/*
 * function to handle the error that happened to the system beacuse the user
 * entered the password incorrectly for three consecutive times
 */
void handle_error(void);

int main(void)
{
	SREG |=(1<<7);  // enable global interrupt
	/*
	 * UART configuration :
	 * data bits = eight bits
	 * parity is even
	 * one stop bit mode
	 * baud rate = 9600
	 */
	UART_ConfigType uart_configuration={EIGHT_BITS , EVEN ,ONE_BIT ,9600};

	/*
	 * initialize the UART communication
	 */
	UART_init(&uart_configuration);

	/*
	 * I2C configurations :
	 * address =10
	 * TWBR =2 to make the baud rate = 400kbps
	 *
	 *
	 */
	TWI_ConfigType I2C_configuration ={10,2};
	/*
	 * initialize the I2C communication
	 */
	TWI_init(&I2C_configuration);

	/*
	 * initialize the motor
	 */
	DcMotor_Init();
	/*
	 * let the user creates the password and save it in the EEPROM
	 */
	Buzzer_init();


	create_password();
	uint8 count=0;
	boolean result;

	while (1){
		result=do_action();
		if(result){
			count=0;
		}
		else{
			count++;
		}
		if (count==3){
			handle_error();

		}


	}



}

boolean check_password(uint8 * pass1 , uint8 * pass2){
	uint8 i =0 ;
	while (pass1[i] != '\0' && pass2[i]!= '\0' ){
		if (pass1[i] != pass2[i]){
			return FALSE;
		}
		i++;
	}
	return TRUE;
}

void create_password(void){
	//flag to be raised when finishing the password
	boolean finish_password = FALSE;
	// create two strings to store the entered passwords
	uint8 created_password [6];
	uint8 confirming_password[6];
	uint8 result;


	while(finish_password == FALSE){
		// send a byte to tell the other controller that this controller is ready
		_delay_ms(500);
		UART_sendByte(MC2_READY);
		// receive the pass
		UART_receiveString(created_password);
		// Receive the second pass
		UART_receiveString(confirming_password);
		// check the equality of the two passwords
		result =check_password(created_password , confirming_password);
		// send the result to the other microcontroller 1
		UART_sendByte(result);
		if (result ==TRUE ){
			finish_password=TRUE;  // rasise the flag to indicate that we finsihed
			save_password(created_password, PASS_ADDRESS);
		}

	}

}


void save_password(uint8 *pass ,uint16 address){
	uint8 i =0;
	while (pass[i]!='\0'){
		EEPROM_writeByte(address, pass[i]);  // write the string byte after byte
		_delay_ms(10);
		i++;
		address++;
	}

}

boolean check_entered_password(void){
	/*create two strings to save the entered pass and the saved one */
	uint8  entered_pass [6];
	uint8  saved_pass[6] ;
	/* receive the entered pass from the first  microcontroller*/
	UART_receiveString(entered_pass);
	/* get the password saved from the EEPROM */
	get_password(saved_pass,PASS_ADDRESS);
	/* checks the equality of the passwords and return the result*/
	return (check_password(entered_pass , saved_pass));


}
void get_password(uint8 *pass , uint16 address){
	uint8 i =0;
	/* loop on the password bytes */
	while (i<5){
		EEPROM_readByte(address, pass+i);  // extract the password saved
		_delay_ms(10);
		i++;
		address++;
	}
	pass[5]='\0';

}
boolean do_action(void){
	// check on the password received from other controller
	boolean result =check_entered_password();
	// send the result to other controller to
	UART_sendByte(result);
	if (result ==TRUE){
		// if the result is True receive the action to be done from the other
		//controller
		uint8 option= UART_recieveByte();
		if (option=='+'){
			unlock_door();
			hold_door();
			lock_door();



		}
		else{
			create_password();
		}
	}
	else{

	}


	return result;
}

void unlock_door(void){
	DcMotor_Rotate(CW,100);
	Timer1_setCallBack(lock_unlock);
	/*
	 * timer1 configuration
	 * we will make the timer generate interrupt after
	 * 8 seconds then we change the TCNT1 value in the next ISR
	 * to generate the next interrupt after 7 seconds
	 * tick_time = 1.28*10**-4
	 * number of ticks required = 8/1.28*10**-4 = 62500
	 * initial_value = 65535 - 62500 =3035
	 * compare_value =0
	 * prescaler = 1024
	 * mode :normal_mode
	 */
	Timer1_ConfigType timer_configuration = {3035,0,F_CPU_1024,NORMAL};
	Timer1_init(&timer_configuration);
	// wait for the interrupt to change the value of wait to false
	wait = TRUE ;
	while(wait);

}


void lock_door(void){
	DcMotor_Rotate(A_CW,100);
	Timer1_setCallBack(lock_unlock);
	/*
	 * timer1 configuration
	 * we will make the timer generate interrupt after
	 * 8 seconds then we change the TCNT1 value in the next ISR
	 * to generate the next interrupt after 7 seconds
	 * tick_time = 1.28*10**-4
	 * number of ticks required = 8/1.28*10**-4 = 62500
	 * initial_value = 65535 - 62500 =3035
	 * compare_value =0
	 * prescaler = 1024
	 * mode :normal_mode
	 */
	Timer1_ConfigType timer_configuration = {3035,0,F_CPU_1024,NORMAL};
	Timer1_init(&timer_configuration);
	// wait for the interrupt to change the value of wait to false
	wait = TRUE ;
	while(wait);
	DcMotor_Rotate(STOP,0);
}


void lock_unlock(void){
	super_tick++;
	if  (super_tick == 2) {
		wait = FALSE ;
		super_tick = 0 ;
		Timer1_deInit() ;

	}
	else{
		/*
		 * update the initial value after the first interrupt to generate
		 * the next interrupt after 7 seconds
		 * c_freq = 8000000/1024 = 7812
		 * tick_time = 1.28 * 10**-4 seconds
		 * so to get 7 seconds we need 7/tick_time = 54688 ticks
		 * initial_value = 65535-54688 = 10874
		 */
		TCNT1 =10874;

	}

}
void hold_ISR(void){
	wait = FALSE ;
	super_tick = 0 ;
	Timer1_deInit() ;
}
void hold_door(void){
	DcMotor_Rotate(STOP,0);
	Timer1_setCallBack(hold_ISR);
	/*
	 * timer1 configuration
	 * we will make the timer generate interrupt after
	 * 3 seconds
	 * tick_time = 1.28*10**-4
	 * number of ticks required = 3/1.28*10**-4 = 23437
	 * initial_value = 65535 - 62500 =42097
	 * compare_value =0
	 * prescaler = 1024
	 * mode :normal_mode
	 */
	Timer1_ConfigType timer_configuration = {42097,0,F_CPU_1024,NORMAL};
	Timer1_init(&timer_configuration);
	// wait for the interrupt to change the value of wait to false
	wait = TRUE ;
	while(wait);

}
void error_ISR(void){
	super_tick++;
	if  (super_tick == 10) {
		error = FALSE ;
		super_tick = 0 ;
		Timer1_deInit() ;

	}
	else{
		TCNT1 =18660;
	}
}
void handle_error(void){
	Buzzer_on();   // display the wrong_pass
	error = TRUE ;
	Timer1_setCallBack(error_ISR);
	/*
	 * timer1 configuration
	 * we will make the timer generate interrupt every 6 seconds so we choose
	 * initial_value = 65535 - 46875 =18660
	 * compare_value = 0
	 * pre scaler = 1024
	 * mode :normal_mode
	 * time after each interrupt = 46875 *(1/(8000000/1024))
	 */
	Timer1_ConfigType timer_configurations = {18660,0,F_CPU_1024,NORMAL};
	Timer1_init(&timer_configurations);
	while(error);
	Buzzer_off();
}
