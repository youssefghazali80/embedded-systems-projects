/*
 * gpio.c
 *
 *  Created on: Feb 18, 2024
 *      Author: youssef
 */

#include "gpio.h"
#include "common_macros.h"
#include <avr/io.h>




/* description :
 * setup  the direction of the required pin input/output.
 * if the input port number or pin number are not correct \
 * the function will not handle the request
 */
void GPIO_setupPinDirection(uint8 port_num,uint8 pin_num,GPIO_PinDirectionType direction){
	if((port_num >= NUM_OF_PORTS) || (pin_num >=NUM_OF_PINS)){
		/*     Do Nothing     */
	}
	else {
		switch(port_num)   // checking on the port to operate on it
		{
		case PORTA_ID:
			if(direction == PIN_OUTPUT){
				SET_BIT(DDRA,pin_num); // setting the required pin to make it output
			}
			else{
				CLEAR_BIT(DDRA,pin_num); // clearing the corresponding bit to make the bit input
			}
			break;

		case PORTB_ID:
			if(direction == PIN_OUTPUT){
				SET_BIT(DDRB,pin_num);
			}
			else{
				CLEAR_BIT(DDRB,pin_num);
			}
			break;
		case PORTC_ID:
			if(direction == PIN_OUTPUT){
				SET_BIT(DDRC,pin_num);
			}
			else{
				CLEAR_BIT(DDRC,pin_num);
			}
			break;
		case PORTD_ID:
			if(direction == PIN_OUTPUT){
				SET_BIT(DDRD,pin_num);
			}
			else{
				CLEAR_BIT(DDRD,pin_num);
			}
			break;
		}
	}



}



/* description :
 * write the value logic High or logic low on the required pin .
 * if the input port number or pin number are not correct \
 * the function will not handle the request.
 * if the pin is input, this function will enable/disable the internal pull \
 * up resistor.
 */
void GPIO_writePin(uint8 port_num , uint8 pin_num ,uint8 value){
	if((port_num >= NUM_OF_PORTS) || (pin_num >=NUM_OF_PINS)){
		/*     Do Nothing     */
	}
	else {
		switch(port_num)   // checking on the port to operate on it
		{
		case PORTA_ID:
			if(value == LOGIC_HIGH){
				SET_BIT(PORTA,pin_num); // setting the required pin to make it output
			}
			else{
				CLEAR_BIT(PORTA,pin_num); // clearing the corresponding bit to make the bit input
			}
			break;

		case PORTB_ID:
			if(value == LOGIC_HIGH){
				SET_BIT(PORTB,pin_num);
			}
			else{
				CLEAR_BIT(PORTB,pin_num);
			}
			break;
		case PORTC_ID:
			if(value == LOGIC_HIGH){
				SET_BIT(PORTC,pin_num);
			}
			else{
				CLEAR_BIT(PORTC,pin_num);
			}
			break;
		case PORTD_ID:
			if(value == LOGIC_HIGH){
				SET_BIT(PORTD,pin_num);
			}
			else{
				CLEAR_BIT(PORTD,pin_num);
			}
			break;
		}
	}

}


/*
 * Description :
 * Read and return the value for the required pin, it should be Logic High or Logic Low.
 * If the input port number or pin number are not correct, The function will return Logic Low.
 */
uint8 GPIO_readPin(uint8 port_num , uint8 pin_num){
	if((port_num >= NUM_OF_PORTS) || (pin_num >=NUM_OF_PINS)){
		return LOGIC_LOW;
	}
	else {
		switch(port_num)   // checking on the port to operate on it
		{
		case PORTA_ID:
			if(BIT_IS_SET(PINA,pin_num)){
				return LOGIC_HIGH; // setting the required pin to make it output
			}
			else{
				return LOGIC_LOW; // clearing the corresponding bit to make the bit input
			}
			break;

		case PORTB_ID:
			if(BIT_IS_SET(PINB,pin_num)){
				return LOGIC_HIGH;
			}
			else{
				return LOGIC_LOW;
			}
			break;
		case PORTC_ID:
			if(BIT_IS_SET(PINC,pin_num)){
				return LOGIC_HIGH;
			}
			else{
				return LOGIC_LOW;
			}
			break;
		case PORTD_ID:
			if(BIT_IS_SET(PIND,pin_num)){
				return LOGIC_HIGH;
			}
			else{
				return LOGIC_LOW;
			}
			break;
		}


	}
	return LOGIC_LOW ;
}

/*
 * Description :
 * Setup the direction of the required port all pins input/output.
 * If the direction value is PORT_INPUT all pins in this port should be input pins.
 * If the direction value is PORT_OUTPUT all pins in this port should be output pins.
 * If the input port number is not correct, The function will not handle the request.
 */
void GPIO_setupPortDirection(uint8 port_num ,GPIO_PortDirectionType direction){
	if(port_num >= NUM_OF_PORTS){
		/*     Do Nothing     */
	}
	else {
		switch(port_num)   // checking on the port to operate on it
		{
		case PORTA_ID:
			DDRA =direction;
			break;

		case PORTB_ID:
			DDRB = direction;
			break;
		case PORTC_ID:
			DDRC = direction;
			break;
		case PORTD_ID:
			DDRD =direction ;
			break;
		}
	}
}

/*
 * Description :
 * Write the value on the required port.
 * If any pin in the port is output pin the value will be written.
 * If any pin in the port is input pin this will activate/deactivate the internal pull-up resistor.
 * If the input port number is not correct, The function will not handle the request.
 */
void GPIO_writePort(uint8 port_num ,uint8 value){
	if(port_num >= NUM_OF_PORTS){
		/*     Do Nothing     */
	}
	else {
		switch(port_num)   //  checking on the port to operate on it
		{
		case PORTA_ID:
			PORTA = value;
			break;

		case PORTB_ID:
			PORTB = value;
			break;
		case PORTC_ID:
			PORTC = value;
			break;
		case PORTD_ID:
			PORTD = value;
			break;
		}
	}


}

/*
 * Description :
 * Read and return the value of the required port.
 * If the input port number is not correct, The function will return ZERO value.
 */
uint8 GPIO_readPort(uint8 port_num){
	if(port_num >= NUM_OF_PORTS){
		return 0;
	}
	else {
		switch(port_num)   //  checking on the port to operate on it
		{
		case PORTA_ID:
			return PINA;
		case PORTB_ID:
			return PINB;
		case PORTC_ID:
			return PINC;
		case PORTD_ID:
			return PIND;
		}
	}
	return 0;



}
