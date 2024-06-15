/*
 * main.c
 *
 *  Created on: Mar 21, 2024
 *      Author: youssef mohamed ghazaly
 */



#include "gpio.h"
#include <util/delay.h>
#include "lcd.h"
#include "keypad.h"
#include "uart.h"
#include "timer1.h"
#include <avr/io.h>

/***************************** DEFINITIONS ************************************/
#define F_CPU 8000000UL

#define ENTER '='
#define MC2_READY 0x10
#define TIMER_INITIAL_VALUE 18661    // initial_value = 65535 - 46875 =18660

/**************************** Global variables ********************************/

// flag that detects if the password is incorrect for three consecutive times
boolean error = FALSE;

// variable to store how many times that timer reach its maximum value
volatile uint8 super_tick = 0;

// flag to wait  till the timer finishes
volatile boolean wait = FALSE ;
/***************************** function prototyping  **************************/


/*
 * function to get a password of five digits and save it in the string given
      to it
 */
void enter_password(uint8* pass);



/*
 * function that sends the password by the UART
 */
void send_password(uint8*pass) ;
/*
 * function to get the result from the other controller whether the two passwords
 * are equal or not
 */
uint8 get_result(void);


/*
 * function that creates that creates the password at the beginning of the
 * application
 */
void create_password(void);
/*
 * Function to display the main options of the system
 */

void display_main_options(void);


/*
 * function that choose the action to be done  after checking the password
 *  entered by the user by sending it to the other controller and waits
 *  for the result and then update the screen according to the action entered
 *
 */
boolean do_action(uint8 option);

/* function to display error message on the lcd and set the flag of error
 * and initialize the timer to waits for the interrupt of the timer to set it to false to remove
 * the error message
 */
void handle_error(void);



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
 * function to control the error message timing
 */
void error_ISR(void);

/*
 * function to control the the hold timing on the screen
 *
 */
void hold_ISR(void);



int main(void)
{
	SREG |=(1<<7); // enable global interrupt of the microcontroller
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


	LCD_init();  // initialize the LED to be used


	create_password();   // create password at the beginnig of the app

	while(1){
		display_main_options(); // display the options message
		uint8 option =  KEYPAD_getPressedKey();  //variable to save option chosen
		if (option == '+' || option=='-'){
			// flag to detect if the user entered the password correctly
			boolean correct_password = FALSE ;
			 /*
			  * variable to count how many times did the user entered the
			  * password
			  */
			uint8 count =0;
			while (count < 3 && correct_password == FALSE){
				correct_password  = do_action(option);
				count++;
			}
			if(count ==3 && correct_password==FALSE){
				/* this condition will be applied if the user
				 * entered the password incorrectly for three consecutive times
				 */
				handle_error();
			}
			// wait for the timer to finish counting and update the error flag
			while(error==TRUE);

		}




	}

}

void enter_password(uint8* pass){
	uint8 counter = 0;    // counter to count the digits entered by the user
	uint8 digit ;         // variable to save the input from the user
	while (counter < 5){
		digit = KEYPAD_getPressedKey();  // get the digit pressed from the user
		if (digit >=0 && digit <=9){
			// convert the integer to character example : 9 to '9'
			digit = digit +'0';
			pass[counter]=digit;     // save the digit in the password
			LCD_displayCharacter('*');
			counter++;
		}
		/*
		 * This delay to give the user an opportunity  to release his hands
		 *      and press another digit
		 */

		_delay_ms(450);


	}
	// wait for the user to press the enter button and ignore any other actions
	while (digit != ENTER){
		digit = KEYPAD_getPressedKey();

	}
	pass[5]='#' ;  // add this character to denote the end of the string
	pass[6]= '\0' ; //  add the null at the end of the string
	LCD_clearScreen() ;


}


void send_password(uint8 * pass){
	UART_sendString(pass);
}

uint8 get_result(){
	return UART_recieveByte();
}

void create_password(void){
	// create two strings to store the entered passwords
	uint8 created_password [7];
	uint8 confirming_password [7];
	// flag to be raised when we finish creating password
	boolean finish_password = FALSE;
	while (finish_password == FALSE){

		LCD_displayString("Plz enter pass: "); // display this message on the lcd
		LCD_moveCursor(1,0);           // move the cursor to display password on lcd


		// let the user create password and save it
		enter_password(created_password);

		// display re-entering the password message on the screen
		LCD_displayString("Plz re-enter the");


		// adjust the cursor
		LCD_moveCursor(1,0);


		// display the remaining message
		LCD_displayString("same pass:");

		//enter the password again to save it
		enter_password(confirming_password);


		// Wait until the second micro controller sends that its ready to get the info
		while(UART_recieveByte() != MC2_READY);
		// send password to the second microcontroller
		send_password(created_password);
		// send the confirming password to the second microcontroller
		send_password(confirming_password);
		uint8 result = get_result();

		if (result ==TRUE ){
			finish_password=TRUE;
		}

	}

}
void display_main_options(void){
	LCD_clearScreen();
	LCD_displayString("+ : Open Door ");
	LCD_moveCursor(1,0);
	LCD_displayString("- : Change Pass ");
}

boolean do_action(uint8 option){
	uint8 password[7];
	boolean result;
	LCD_clearScreen();
	LCD_displayString("Plz enter pass: ");
	LCD_moveCursor(1,0);
	// get the password from the user
	enter_password(password);
	// send the password to the other controller to be checked
	UART_sendString(password);
	// receive the result of the checking
	result =UART_recieveByte();
	if (result == TRUE){
		// send the action required to the other controller
		UART_sendByte(option);
		if(option == '+'){
			//unlock the door for 15 seconds
			unlock_door();
			//hold the door for 3 seconds
			hold_door();
			// lock the door for 15 seconds
			lock_door();



		}
		else{
			LCD_displayString("new_pass");
			_delay_ms(1000);
			LCD_clearScreen();
			create_password();
		}
		return TRUE ;
	}
	else{
		return FALSE;
	}

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
	LCD_clearScreen();
	LCD_displayString("wrong pass");  // display the wrong_pass
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


void unlock_door(void){
	LCD_displayString("Door is");
	LCD_moveCursor(1,0);
	LCD_displayString("unlocking");
	// adjust the interrupt call back function
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
	LCD_displayString("DOOR is locking");
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

void hold_ISR(void){
	wait = FALSE ;
	super_tick = 0 ;
	Timer1_deInit() ;

}
void hold_door(void){
	LCD_clearScreen();
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
