/*
 * gpio.h
 *
 *  Created on: Feb 18, 2024
 *      Author: youssef
 */

#ifndef GPIO_H_
#define GPIO_H_
#include "std_types.h"
/*
 * *********************   Definitions  ***************************
 */
/*
 * define the num_of_ports and the number of pins in the atmega32
 */
#define NUM_OF_PORTS  4
#define NUM_OF_PINS   8


/*
 * define each port in the atmega32 to be able to check on them in the driver
 */


#define PORTA_ID     0
#define PORTB_ID     1
#define PORTC_ID     2
#define PORTD_ID     3

/*
 * Define the pins in the atmega32
 */

#define PIN0_ID       0
#define PIN1_ID       1
#define PIN2_ID       2
#define PIN3_ID       3
#define PIN4_ID       4
#define PIN5_ID       5
#define PIN6_ID       6
#define PIN7_ID       7
/*
 * *********************   TYPES DECLARATIONS ***************************
 */



/*
 * creating an enum to define the directions of the pin
 */
typedef enum {
	PIN_INPUT  , PIN_OUTPUT
}GPIO_PinDirectionType;



/*
 * creating an enum to define the directions of the ports
 */

typedef enum{
	PORT_INPUT , PORT_OUTPUT=0xFF
}GPIO_PortDirectionType;



/*
 * ***********************   FUNCTION_PROTOTYPES  **********************
 */

/* description :
 * setup  the direction of the required pin input/output.
 * if the input port number or pin number are not correct \
 * the function will not handle the request
 */
void GPIO_setupPinDirection(uint8 port_num,uint8 pin_num,GPIO_PinDirectionType direction);



/* description :
 * write the value logic High or logic low on the required pin .
 * if the input port number or pin number are not correct \
 * the function will not handle the request.
 * if the pin is input, this function will enable/disable the internal pull \
 * up resistor.
 */
void GPIO_writePin(uint8 port_num , uint8 pin_num ,uint8 value);


/*
 * Description :
 * Read and return the value for the required pin, it should be Logic High or Logic Low.
 * If the input port number or pin number are not correct, The function will return Logic Low.
 */
uint8 GPIO_readPin(uint8 port_num , uint8 pin_num);

/*
 * Description :
 * Setup the direction of the required port all pins input/output.
 * If the direction value is PORT_INPUT all pins in this port should be input pins.
 * If the direction value is PORT_OUTPUT all pins in this port should be output pins.
 * If the input port number is not correct, The function will not handle the request.
 */
void GPIO_setupPortDirection(uint8 port_num ,GPIO_PortDirectionType direction);

/*
 * Description :
 * Write the value on the required port.
 * If any pin in the port is output pin the value will be written.
 * If any pin in the port is input pin this will activate/deactivate the internal pull-up resistor.
 * If the input port number is not correct, The function will not handle the request.
 */
void GPIO_writePort(uint8 port_num ,uint8 value);

/*
 * Description :
 * Read and return the value of the required port.
 * If the input port number is not correct, The function will return ZERO value.
 */
uint8 GPIO_readPort(uint8 port_num);





#endif /* GPIO_H_ */
