/******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port.c
 *
 * Description: Source file for TM4C123GH6PM Microcontroller - Port Driver.
 *
 * Author: Youssef ghazali
 ******************************************************************************/

#include "Port.h"
#include "tm4c123gh6pm_registers.h"
#include "Det.h"
#include "Platform_Types.h"
#include "Common_Macros.h"


// global variables
STATIC const Port_ConfigPins * config_pins = NULL_PTR;
STATIC uint8 Port_Status = PORT_NOT_INITIALIZED;



/*******************************************************************************
 *                      Function Prototypes                                    *
 *******************************************************************************/

/************************************************************************************
 * Service Name: Port_Init
 * Service Id : 0x00
 * Sync/Async: Synchronous
 * Reentrancy: Non reentrant
 * Parameters (in): ConfigPtr - Pointer to  configuration data
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: Initialize the port Driver module
 ************************************************************************************/
void Port_Init(const Port_ConfigType *ConfigPtr ){

    config_pins = ConfigPtr->config_pins;
    uint8 pin_counter ;     // counter to loop on the config_pins array inside ConfigPtr
    volatile uint32 * PORT_GpioPtr = NULL_PTR;
    for (pin_counter=0 ;pin_counter < PORT_NUMBER_OF_PORT_PINS; pin_counter++){
        switch (config_pins[pin_counter].port_num){

        /*
         * address the value of the PORT_GPioPtr to the base address register of the w
         * wanted port
         */
        case PORT_A:
            PORT_GpioPtr = (uint32 *)(GPIO_PORTA_BASE_ADDRESS);
            break;
        case PORT_B:
            PORT_GpioPtr = (uint32 *)(GPIO_PORTB_BASE_ADDRESS);
            break;
        case PORT_C:
            PORT_GpioPtr = (uint32 *)(GPIO_PORTC_BASE_ADDRESS);
            break;
        case PORT_D:
            PORT_GpioPtr = (uint32 *)(GPIO_PORTD_BASE_ADDRESS);
            break;
        case PORT_E:
            PORT_GpioPtr = (uint32 *)(GPIO_PORTE_BASE_ADDRESS);
            break;
        case PORT_F:
            PORT_GpioPtr = (uint32 *)(GPIO_PORTF_BASE_ADDRESS);
            break;
        default :
            Det_ReportError( PORT_MODULE_ID,PORT_INSTANCE_ID,PORT_INIT_ID , PORT_E_INIT_FAILED);



        }
        // enable clock for the port
        SET_BIT(SYSCTL_RCGCGPIO_REG , config_pins[pin_counter].port_num);
        // check that this port is accessible after activating the clock for this port
        while (!(BIT_IS_SET(SYSCTL_PRGPIO_REG , config_pins[pin_counter].pin_num)));




        /*
         * check if the pin that is required to be configured needs locking and commiting
         *  or not
         * the pin that require locking in Tiva c are PC(0~3) PD7 PF0
         */
        if(pin_counter == PORTC_PIN0 || pin_counter == PORTC_PIN1  || pin_counter == PORTC_PIN2
                || pin_counter == PORTC_PIN3 || pin_counter == PORTD_PIN7 ||
                pin_counter == PORTF_PIN0){
            (*(uint32 *)(PORT_GpioPtr + PORT_LOCK_REG_OFFSET)) = 0x4C4F434B;   // Unlock the GPIOCR register

            //set the corresponding bit in GPIOCR register
            SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_COMMIT_REG_OFFSET) ,config_pins[pin_counter].pin_num);
        }
        /*
         * check the direction of the pin and adjusting the GPIODIR REGISTER by clearing
         *  the corresponding bit if input  and setting it if it is output
         * setting the bit value
         */

        if (config_pins[pin_counter].direction == PORT_PIN_IN){
            CLEAR_BIT(*(uint32 *)(PORT_GpioPtr +  PORT_DIR_REG_OFFSET) ,config_pins[pin_counter].pin_num);
            /*
             * adjust the internal resistor configuartions whether  it is pull_up or pull_down
             */
            if (config_pins[pin_counter].resistor == PULL_UP){
                SET_BIT(*(uint32 *)(PORT_GpioPtr +  PORT_PULL_UP_REG_OFFSET) ,config_pins[pin_counter].pin_num);
            }
            else if (config_pins[pin_counter].resistor == PULL_DOWN){
                SET_BIT(*(uint32 *)(PORT_GpioPtr +  PORT_PULL_DOWN_REG_OFFSET) ,config_pins[pin_counter].pin_num);


            }


        }
        else if(config_pins[pin_counter].direction == PORT_PIN_OUT){
            SET_BIT(*(uint32 *)(PORT_GpioPtr +  PORT_DIR_REG_OFFSET) ,config_pins[pin_counter].pin_num);


            /*
             * check if the initial value is high or low
             */

            if (config_pins[pin_counter].inital_state == STD_HIGH){
                /* SET the corresponding bit in the GPIO Data register the make initial
                 * state of the output pin HIGH
                 */

                SET_BIT(*(uint32 *)(PORT_GpioPtr +  PORT_DATA_REG_OFFSET) ,config_pins[pin_counter].pin_num);


            }
            else if  (config_pins[pin_counter].inital_state == STD_LOW){
                //clear the corresponding bit in the GPIO Data register the make initial
                // state of the output pin lOW

                CLEAR_BIT(*(uint32 *)(PORT_GpioPtr +  PORT_DATA_REG_OFFSET) ,config_pins[pin_counter].pin_num);


            }


        }


        switch (config_pins[pin_counter].pin_mode){
        case PORT_PIN_MODE_DIO:
            // CLEAR the corresponding bit in the analog mode select register to use bit as digital
            CLEAR_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,config_pins[pin_counter].pin_num);

            /*
             * Clear the corresponding 4 PMCx bits in GPIOPCTL register
             * to select regular I/O digital function
             */

            *(uint32 *)(PORT_GpioPtr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F <<(config_pins[pin_counter].pin_num * NUMBER_OF_PMCX_BITS));
            /*
             * clear the corresponding bit in the GPIOAFSEL register to disable alternate functionality
             */

            CLEAR_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ALT_FUNC_REG_OFFSET) ,config_pins[pin_counter].pin_num);

            /*
             * Enable the corresponding I/O pins by writing one to the corresponding biT
             * IN GPIODEN register
             */
            SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_DIGITAL_ENABLE_REG_OFFSET) ,config_pins[pin_counter].pin_num);
            break;
        case PORT_PIN_MODE_ADC:

            /*
             * Check if the pins that we want to configure can be configured as ADC by the harwdare
             */
            if(pin_counter == PORTE_PIN3 ||  pin_counter == PORTE_PIN2 || pin_counter == PORTE_PIN1 || pin_counter == PORTE_PIN0
                    || pin_counter == PORTD_PIN3 ||  pin_counter == PORTD_PIN2 || pin_counter == PORTD_PIN1 || pin_counter == PORTD_PIN0
                    ||  pin_counter == PORTE_PIN5 ||  pin_counter == PORTE_PIN4 ||  pin_counter == PORTB_PIN5 ||  pin_counter == PORTB_PIN4)
            {
                // clear the the corresponding bit in GPIODIR to configure pin as input
                CLEAR_BIT(*(uint32 *)(PORT_GpioPtr + PORT_DIR_REG_OFFSET) ,config_pins[pin_counter].pin_num);

                // clear the the corresponding bit in GPIODEN to disbale digital function of the pin
                CLEAR_BIT(*(uint32 *)(PORT_GpioPtr + PORT_DIGITAL_ENABLE_REG_OFFSET) ,config_pins[pin_counter].pin_num);

                // Set the  corresponding bit in GPIOAFSEL Register to enable alternate functionlaity
                SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ALT_FUNC_REG_OFFSET) ,config_pins[pin_counter].pin_num);

                // Set the corresponding bit in AMSEL REGISTER  to enable analog function for the pin
                SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,config_pins[pin_counter].pin_num);
            }

            else {
#if (PORT_DEV_ERROR_DETECT == STD_ON)
                //The user enetered an invalid pin to be configured as ADC
                Det_ReportError( PORT_MODULE_ID,PORT_INSTANCE_ID,PORT_INIT_ID , PORT_E_INIT_FAILED);
#endif
            }
            break;
        case PORT_PIN_MODE_UART :
            /*
             * Check if the pins that we want to configure can be configured as ADC by the hardware
             */

            if(pin_counter == PORTA_PIN0 ||  pin_counter == PORTA_PIN1 || pin_counter == PORTC_PIN5 || pin_counter == PORTF_PIN1
                    || pin_counter == PORTC_PIN4 ||  pin_counter == PORTF_PIN0 || pin_counter == PORTB_PIN0 || pin_counter == PORTC_PIN5
                    || pin_counter == PORTB_PIN1 ||  pin_counter == PORTD_PIN6 || pin_counter == PORTD_PIN7 || pin_counter == PORTC_PIN6
                    || pin_counter == PORTC_PIN7 ||  pin_counter == PORTE_PIN4 || pin_counter == PORTE_PIN5 || pin_counter == PORTD_PIN4
                    || pin_counter == PORTD_PIN5 || pin_counter == PORTE_PIN0  || pin_counter == PORTE_PIN1 )
            {
                // CLEAR the corresponding bit in the analog mode select register to use bit as digital
                CLEAR_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,config_pins[pin_counter].pin_num);

                /*
                 * Enable the corresponding I/O pins by writing one to the corresponding bit
                 * IN GPIODEN register
                 */


                SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_DIGITAL_ENABLE_REG_OFFSET) ,config_pins[pin_counter].pin_num);

                // Set the corresponding bit in GPIOAFSEL to enable alternate functionality

                SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ALT_FUNC_REG_OFFSET) ,config_pins[pin_counter].pin_num);

                /*
                 * Set the PMCx bits in thr GPIOCTL register for PE0 ,PE1 to 0001( enable
                 * UART functionality
                 */
                // clear the 4 corresponding bits

                *(uint32 *)(PORT_GpioPtr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F <<(config_pins[pin_counter].pin_num * NUMBER_OF_PMCX_BITS));

                /*
                 * Check which pin that we want to configure as UART in case of pin pc4 and pc5 different
                 * value is assigned in the GPIOCTL register
                 */
                if (pin_counter == PORTC_PIN4 || pin_counter == PORTC_PIN5 ){
                    // set the 4 corresponding bits to 0002 to enable UART functionality
                    *(uint32 *)(PORT_GpioPtr + PORT_CTL_REG_OFFSET) |= (0x00000002 <<(config_pins[pin_counter].pin_num *NUMBER_OF_PMCX_BITS));
                }

                else{
                    // set the 4 corresponding bits to 0001 to enable UART functionality
                    *(uint32 *)(PORT_GpioPtr + PORT_CTL_REG_OFFSET) |= (0x00000001 <<(config_pins[pin_counter].pin_num *NUMBER_OF_PMCX_BITS));
                }


            }
            else{
#if (PORT_DEV_ERROR_DETECT == STD_ON)
                //The user entered an invalid pin to be configured as UART
                Det_ReportError( PORT_MODULE_ID,PORT_INSTANCE_ID,PORT_INIT_ID , PORT_E_INIT_FAILED);
#endif
            }
            break;

        case PORT_PIN_MODE_SPI :
            /*
             * Check if the pins that we want to configure can be configured as SPI by the hardware
             */

            if(pin_counter == PORTA_PIN2 ||  pin_counter == PORTA_PIN3 || pin_counter == PORTA_PIN4 || pin_counter == PORTA_PIN5
                    || pin_counter == PORTF_PIN2 ||  pin_counter == PORTD_PIN0 || pin_counter == PORTF_PIN3 || pin_counter == PORTD_PIN1
                    || pin_counter == PORTF_PIN0 ||  pin_counter == PORTD_PIN2 || pin_counter == PORTF_PIN1 || pin_counter == PORTD_PIN3
                    || pin_counter == PORTB_PIN4 ||  pin_counter == PORTB_PIN5 || pin_counter == PORTB_PIN6 || pin_counter == PORTB_PIN7)
            {
                // CLEAR the corresponding bit in the analog mode select register to use bit as digital
                CLEAR_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,config_pins[pin_counter].pin_num);

                /*
                 * Enable the corresponding I/O pins by writing one to the corresponding bit
                 * IN GPIODEN register
                 */


                SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_DIGITAL_ENABLE_REG_OFFSET) ,config_pins[pin_counter].pin_num);

                // Set the corresponding bit in GPIOAFSEL to enable alternate functionality

                SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ALT_FUNC_REG_OFFSET) ,config_pins[pin_counter].pin_num);

                /*
                 * Set the PMCx bits in thr GPIOCTL register for PE0 ,PE1 to 0002( enable
                 * SPI (SSI) functionality
                 */
                // clear the 4 corresponding bits

                *(uint32 *)(PORT_GpioPtr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F <<(config_pins[pin_counter].pin_num * NUMBER_OF_PMCX_BITS));

                // SET the 4 corresponding bits to 0002 to enable SPI (SSI) functionality
                *(uint32 *)(PORT_GpioPtr + PORT_CTL_REG_OFFSET) |= (0x00000002 <<(config_pins[pin_counter].pin_num *NUMBER_OF_PMCX_BITS));


            }
            else {
#if (PORT_DEV_ERROR_DETECT == STD_ON)
                //The user entered an invalid pin to be configured as SPI
                Det_ReportError( PORT_MODULE_ID,PORT_INSTANCE_ID,PORT_INIT_ID , PORT_E_INIT_FAILED);
#endif
            }
            break ;


        case PORT_PIN_MODE_I2C :
            /*
             * Check if the pins that we want to configure can be configured as I2C by the hardware
             */

            if(pin_counter == PORTB_PIN2 ||  pin_counter == PORTB_PIN3 || pin_counter == PORTA_PIN6 || pin_counter == PORTA_PIN7
                    || pin_counter == PORTE_PIN4 ||  pin_counter == PORTE_PIN5 || pin_counter == PORTD_PIN0 || pin_counter == PORTD_PIN1)
            {
                // CLEAR the corresponding bit in the analog mode select register to use bit as digital
                CLEAR_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,config_pins[pin_counter].pin_num);

                /*
                 * Enable the corresponding I/O pins by writing one to the corresponding bit
                 * IN GPIODEN register
                 */


                SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_DIGITAL_ENABLE_REG_OFFSET) ,config_pins[pin_counter].pin_num);

                // Set the corresponding bit in GPIOAFSEL to enable alternate functionality

                SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ALT_FUNC_REG_OFFSET) ,config_pins[pin_counter].pin_num);

                /*
                 * Set the PMCx bits in thr GPIOCTL register for PE0 ,PE1 to 0003( enable
                 * SPI (SSI) functionality
                 */
                // clear the 4 corresponding bits

                *(uint32 *)(PORT_GpioPtr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F <<(config_pins[pin_counter].pin_num * NUMBER_OF_PMCX_BITS));

                // SET the 4 corresponding bits to 0003 to enable I2C  functionality
                *(uint32 *)(PORT_GpioPtr + PORT_CTL_REG_OFFSET) |= (0x00000003 <<(config_pins[pin_counter].pin_num *NUMBER_OF_PMCX_BITS));


            }
            else {
#if (PORT_DEV_ERROR_DETECT == STD_ON)
                //The user entered an invalid pin to be configured as I2X
                Det_ReportError( PORT_MODULE_ID,PORT_INSTANCE_ID,PORT_INIT_ID ,PORT_E_INIT_FAILED);
#endif
            }
            break ;
        case PORT_PIN_MODE_PWM :

            /*
             *  Check if the pin that we are configuring can
             *  be configured as PWM by the hardware and  if these bits  belong to module 0 or module 1
             */


            if(pin_counter == PORTF_PIN2 ||  pin_counter == PORTD_PIN6 || pin_counter == PORTD_PIN2 || pin_counter == PORTB_PIN6
                    || pin_counter == PORTB_PIN7 ||  pin_counter == PORTB_PIN4 || pin_counter == PORTB_PIN5 || pin_counter == PORTE_PIN4
                    || pin_counter == PORTE_PIN5 ||  pin_counter == PORTC_PIN4 || pin_counter == PORTD_PIN0 || pin_counter == PORTC_PIN5
                    || pin_counter == PORTD_PIN1)
                // pins that belong to module 0 PWM
            {
                // CLEAR the corresponding bit in the analog mode select register to use bit as digital
                CLEAR_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,config_pins[pin_counter].pin_num);

                /*
                 * Enable the corresponding I/O pins by writing one to the corresponding bit
                 * IN GPIODEN register
                 */


                SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_DIGITAL_ENABLE_REG_OFFSET) ,config_pins[pin_counter].pin_num);

                // Set the corresponding bit in GPIOAFSEL to enable alternate functionality

                SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ALT_FUNC_REG_OFFSET) ,config_pins[pin_counter].pin_num);

                /*
                 * Set the PMCx bits in the GPIOCTL register for PE0 ,PE1 to 0002( enable
                 * SPI (SSI) functionality
                 */
                // clear the 4 corresponding bits

                *(uint32 *)(PORT_GpioPtr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F <<(config_pins[pin_counter].pin_num * NUMBER_OF_PMCX_BITS));

                // SET the 4 corresponding bits to 0004 to enable PWM (SSI) functionality
                *(uint32 *)(PORT_GpioPtr + PORT_CTL_REG_OFFSET) |= (0x00000004 <<(config_pins[pin_counter].pin_num *NUMBER_OF_PMCX_BITS));


            }
            else if (pin_counter == PORTA_PIN6 || pin_counter == PORTA_PIN7 || pin_counter == PORTF_PIN0 || pin_counter == PORTF_PIN1
                    || pin_counter == PORTF_PIN3 || pin_counter == PORTF_PIN4) // pins that belong to module 1
            {
                // CLEAR the corresponding bit in the analog mode select register to use bit as digital
                CLEAR_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,config_pins[pin_counter].pin_num);

                /*
                 * Enable the corresponding I/O pins by writing one to the corresponding bit
                 * IN GPIODEN register
                 */


                SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_DIGITAL_ENABLE_REG_OFFSET) ,config_pins[pin_counter].pin_num);

                // Set the corresponding bit in GPIOAFSEL to enable alternate functionality

                SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ALT_FUNC_REG_OFFSET) ,config_pins[pin_counter].pin_num);

                /*
                 * Set the PMCx bits in the GPIOCTL register for PE0 ,PE1 to 0002( enable
                 * SPI (SSI) functionality
                 */
                // clear the 4 corresponding bits

                *(uint32 *)(PORT_GpioPtr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F <<(config_pins[pin_counter].pin_num * NUMBER_OF_PMCX_BITS));

                // SET the 4 corresponding bits to 0005 to enable PWM (SSI) functionality
                *(uint32 *)(PORT_GpioPtr + PORT_CTL_REG_OFFSET) |= (0x00000005 <<(config_pins[pin_counter].pin_num *NUMBER_OF_PMCX_BITS));

            }
            else {
#if (PORT_DEV_ERROR_DETECT == STD_ON)
                //The user entered an invalid pin to be configured as SPI
                Det_ReportError(PORT_MODULE_ID,PORT_INSTANCE_ID, PORT_INIT_ID ,PORT_E_INIT_FAILED);
#endif
            }
            break;
        case PORT_PIN_MODE_QEI :
            /*
             * Check if the pins that we want to configure can be configured as QEI by the hardware
             */

            if(pin_counter == PORTC_PIN4 ||  pin_counter == PORTC_PIN5 || pin_counter == PORTC_PIN6 || pin_counter == PORTD_PIN3
                    || pin_counter == PORTD_PIN6 ||  pin_counter == PORTD_PIN7 ||  pin_counter == PORTF_PIN0
                    || pin_counter == PORTF_PIN1  || pin_counter == PORTF_PIN4)
            {
                // CLEAR the corresponding bit in the analog mode select register to use bit as digital
                CLEAR_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,config_pins[pin_counter].pin_num);

                /*
                 * Enable the corresponding I/O pins by writing one to the corresponding bit
                 * IN GPIODEN register
                 */


                SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_DIGITAL_ENABLE_REG_OFFSET) ,config_pins[pin_counter].pin_num);

                // Set the corresponding bit in GPIOAFSEL to enable alternate functionality

                SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ALT_FUNC_REG_OFFSET) ,config_pins[pin_counter].pin_num);

                /*
                 * Set the PMCx bits in the GPIOCTL register  to 0006( enable
                 * QEI functionality
                 */
                // clear the 4 corresponding bits

                *(uint32 *)(PORT_GpioPtr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F <<(config_pins[pin_counter].pin_num * NUMBER_OF_PMCX_BITS));

                // SET the 4 corresponding bits to 0006 to enable QEI  functionality
                *(uint32 *)(PORT_GpioPtr + PORT_CTL_REG_OFFSET) |= (0x00000006 <<(config_pins[pin_counter].pin_num *NUMBER_OF_PMCX_BITS));


            }
            else {
#if (PORT_DEV_ERROR_DETECT == STD_ON)
                //The user entered an invalid pin to be configured as I2X
                Det_ReportError( PORT_MODULE_ID,PORT_INSTANCE_ID,PORT_INIT_ID ,PORT_E_INIT_FAILED);
#endif
            }
            break ;
        case PORT_PIN_MODE_GPT :
            /*
             * Check if the pins that we want to configure can be configured as GPT by the hardware
             */

            if(pin_counter == PORTB_PIN0 ||  pin_counter == PORTB_PIN1 || pin_counter == PORTB_PIN2 || pin_counter == PORTB_PIN3
                    || pin_counter == PORTB_PIN4 ||  pin_counter == PORTB_PIN5 ||  pin_counter == PORTB_PIN6
                    || pin_counter == PORTB_PIN7  || pin_counter == PORTF_PIN0 || pin_counter == PORTF_PIN1 ||
                    pin_counter == PORTF_PIN2  || pin_counter == PORTF_PIN3 || pin_counter == PORTF_PIN4 ||
                    pin_counter == PORTD_PIN0 || pin_counter == PORTD_PIN1  || pin_counter == PORTD_PIN2 || pin_counter == PORTD_PIN3 ||
                    pin_counter == PORTD_PIN4 || pin_counter == PORTD_PIN5 || pin_counter == PORTD_PIN6 || pin_counter == PORTD_PIN7
                    || pin_counter == PORTC_PIN0 || pin_counter == PORTC_PIN1  || pin_counter == PORTC_PIN2 || pin_counter == PORTC_PIN3
                    || pin_counter == PORTC_PIN4 || pin_counter == PORTC_PIN5 || pin_counter == PORTC_PIN6 || pin_counter == PORTC_PIN7)
            {
                // CLEAR the corresponding bit in the analog mode select register to use bit as digital
                CLEAR_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,config_pins[pin_counter].pin_num);

                /*
                 * Enable the corresponding I/O pins by writing one to the corresponding bit
                 * IN GPIODEN register
                 */


                SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_DIGITAL_ENABLE_REG_OFFSET) ,config_pins[pin_counter].pin_num);

                // Set the corresponding bit in GPIOAFSEL to enable alternate functionality

                SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ALT_FUNC_REG_OFFSET) ,config_pins[pin_counter].pin_num);

                /*
                 * Set the PMCx bits in thr GPIOCTL register 0007( enable
                 * GPT) functionality
                 */
                // clear the 4 corresponding bits

                *(uint32 *)(PORT_GpioPtr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F <<(config_pins[pin_counter].pin_num * NUMBER_OF_PMCX_BITS));

                // SET the 4 corresponding bits to 0007 to enable GPT  functionality
                *(uint32 *)(PORT_GpioPtr + PORT_CTL_REG_OFFSET) |= (0x00000007 <<(config_pins[pin_counter].pin_num *NUMBER_OF_PMCX_BITS));


            }
            else {
#if (PORT_DEV_ERROR_DETECT == STD_ON)
                //The user entered an invalid pin to be configured as GPT
                Det_ReportError( PORT_MODULE_ID,PORT_INSTANCE_ID,PORT_INIT_ID ,PORT_E_INIT_FAILED);
#endif
            }
            break ;
        case PORT_PIN_MODE_CAN :
            /*
             * Check if the pins that we want to configure can be configured as CAN pins by the hardware
             */

            if(pin_counter == PORTF_PIN0 ||  pin_counter == PORTB_PIN4 || pin_counter == PORTE_PIN4 || pin_counter == PORTF_PIN3
                    || pin_counter == PORTB_PIN5 ||  pin_counter == PORTE_PIN5 ||  pin_counter == PORTA_PIN0
                    || pin_counter == PORTA_PIN1)
            {
                // CLEAR the corresponding bit in the analog mode select register to use bit as digital
                CLEAR_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,config_pins[pin_counter].pin_num);

                /*
                 * Enable the corresponding I/O pins by writing one to the corresponding bit
                 * IN GPIODEN register
                 */


                SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_DIGITAL_ENABLE_REG_OFFSET) ,config_pins[pin_counter].pin_num);

                // Set the corresponding bit in GPIOAFSEL to enable alternate functionality

                SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ALT_FUNC_REG_OFFSET) ,config_pins[pin_counter].pin_num);

                /*
                 * Set the PMCx bits in thr GPIOCTL register  to 0008( enable
                 * CAN functionality
                 */
                // clear the 4 corresponding bits

                *(uint32 *)(PORT_GpioPtr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F <<(config_pins[pin_counter].pin_num * NUMBER_OF_PMCX_BITS));

                // SET the 4 corresponding bits to 0008 to enable CAN  functionality
                *(uint32 *)(PORT_GpioPtr + PORT_CTL_REG_OFFSET) |= (0x00000008 <<(config_pins[pin_counter].pin_num *NUMBER_OF_PMCX_BITS));


            }
            else {
#if (PORT_DEV_ERROR_DETECT == STD_ON)
                //The user entered an invalid pin to be configured as CAN
                Det_ReportError(PORT_MODULE_ID,PORT_INSTANCE_ID,PORT_INIT_ID ,PORT_E_INIT_FAILED);
#endif
            }
            break;
        case PORT_PIN_MODE_NMI :
            /*
             * Check if the pins that we want to configure can be configured as NMI pins by the hardware
             */

            if(pin_counter == PORTF_PIN0 ||  pin_counter == PORTD_PIN7 )

            {
                // CLEAR the corresponding bit in the analog mode select register to use bit as digital
                CLEAR_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,config_pins[pin_counter].pin_num);

                /*
                 * Enable the corresponding I/O pins by writing one to the corresponding bit
                 * IN GPIODEN register
                 */


                SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_DIGITAL_ENABLE_REG_OFFSET) ,config_pins[pin_counter].pin_num);

                // Set the corresponding bit in GPIOAFSEL to enable alternate functionality

                SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ALT_FUNC_REG_OFFSET) ,config_pins[pin_counter].pin_num);

                /*
                 * Set the PMCx bits in thr GPIOCTL register  to 0008( enable
                 * CAN functionality
                 */
                // clear the 4 corresponding bits

                *(uint32 *)(PORT_GpioPtr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F <<(config_pins[pin_counter].pin_num * NUMBER_OF_PMCX_BITS));

                // SET the 4 corresponding bits to 0008 to enable NMI  functionality
                *(uint32 *)(PORT_GpioPtr + PORT_CTL_REG_OFFSET) |= (0x00000008 <<(config_pins[pin_counter].pin_num *NUMBER_OF_PMCX_BITS));


            }
            else {
#if (PORT_DEV_ERROR_DETECT == STD_ON)
                //The user entered an invalid pin to be configured as NMI
                Det_ReportError( PORT_MODULE_ID,PORT_INSTANCE_ID,PORT_INIT_ID ,PORT_E_INIT_FAILED);
#endif
            }
            break;

        case PORT_PIN_MODE_USB :
            /*
             * Check if the pins that we want to configure can be configured as NMI pins by the hardware
             * and if it will be digital pin or analog
             */

            if (pin_counter == PORTC_PIN6 ||  pin_counter == PORTC_PIN7 || pin_counter == PORTD_PIN2 || pin_counter == PORTD_PIN3
                    || pin_counter == PORTF_PIN4)

            {
                // CLEAR the corresponding bit in the analog mode select register to use bit as digital
                CLEAR_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,config_pins[pin_counter].pin_num);

                /*
                 * Enable the corresponding I/O pins by writing one to the corresponding bit
                 * IN GPIODEN register
                 */


                SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_DIGITAL_ENABLE_REG_OFFSET) ,config_pins[pin_counter].pin_num);

                // Set the corresponding bit in GPIOAFSEL to enable alternate functionality

                SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ALT_FUNC_REG_OFFSET) ,config_pins[pin_counter].pin_num);

                /*
                 * Set the PMCx bits in thr GPIOCTL register  to 0008( enable
                 * USB functionality
                 */
                // clear the 4 corresponding bits

                *(uint32 *)(PORT_GpioPtr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F <<(config_pins[pin_counter].pin_num * NUMBER_OF_PMCX_BITS));

                // SET the 4 corresponding bits to 0008 to enable USB  functionality
                *(uint32 *)(PORT_GpioPtr + PORT_CTL_REG_OFFSET) |= (0x00000008 <<(config_pins[pin_counter].pin_num * NUMBER_OF_PMCX_BITS));


            }
            else if (pin_counter == PORTD_PIN4 || pin_counter == PORTD_PIN5 || pin_counter ==PORTB_PIN0 || pin_counter == PORTB_PIN1)
            {
                // clear the the corresponding bit in GPIODEN to disbale digital function of the pin
                CLEAR_BIT(*(uint32 *)(PORT_GpioPtr + PORT_DIGITAL_ENABLE_REG_OFFSET) ,config_pins[pin_counter].pin_num);

                // Set the  corresponding bit in GPIOAFSEL Register to enable alternate functionality
                SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ALT_FUNC_REG_OFFSET) ,config_pins[pin_counter].pin_num);

                // Set the corresponding bit in AMSEL REGISTER  to enable analog function for the pin
                SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,config_pins[pin_counter].pin_num);

            }
            else {
#if (PORT_DEV_ERROR_DETECT == STD_ON)
                //The user entered an invalid pin to be configured as USB
                Det_ReportError( PORT_MODULE_ID,PORT_INSTANCE_ID,PORT_INIT_ID ,PORT_E_INIT_FAILED);
#endif
            }
            break;
        case PORT_PIN_MODE_CORE :
            /*
             * Check if the pins that we want to configure can be configured as CORE pins by the hardware
             */

            if(pin_counter == PORTF_PIN1 ||  pin_counter == PORTF_PIN2 || pin_counter == PORTF_PIN3)

            {
                // CLEAR the corresponding bit in the analog mode select register to use bit as digital
                CLEAR_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,config_pins[pin_counter].pin_num);

                /*
                 * Enable the corresponding I/O pins by writing one to the corresponding bit
                 * IN GPIODEN register
                 */


                SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_DIGITAL_ENABLE_REG_OFFSET) ,config_pins[pin_counter].pin_num);

                // Set the corresponding bit in GPIOAFSEL to enable alternate functionality

                SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ALT_FUNC_REG_OFFSET) ,config_pins[pin_counter].pin_num);

                /*
                 * Set the PMCx bits in thr GPIOCTL register  to 000F( enable
                 * CORE functionality
                 */
                // clear the 4 corresponding bits

                *(uint32 *)(PORT_GpioPtr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F <<(config_pins[pin_counter].pin_num * NUMBER_OF_PMCX_BITS));

                // SET the 4 corresponding bits to 0008 to enable CORE  functionality
                *(uint32 *)(PORT_GpioPtr + PORT_CTL_REG_OFFSET) |= (0x0000000F <<(config_pins[pin_counter].pin_num *NUMBER_OF_PMCX_BITS));


            }
            else {
#if (PORT_DEV_ERROR_DETECT == STD_ON)
                //The user entered an invalid pin to be configured as CORE
                Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID ,PORT_INIT_ID, PORT_E_INIT_FAILED);
#endif
            }
            break;
        case PORT_PIN_MODE_ANALOG_COMP :
            /*
             * Check if the pins that we want to configure can be configured as Analog comparator pins by the hardware and if they will
             * work as analog or digital pins
             */

            if(pin_counter == PORTC_PIN7 ||   pin_counter == PORTC_PIN5 || pin_counter == PORTC_PIN4)

            {
                // SET the corresponding bit in the analog mode select register to use bit as digital
                SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,config_pins[pin_counter].pin_num);

                /*
                 * Diable the corresponding I/O pins by writing one to the corresponding bit
                 * IN GPIODEN register
                 */


                CLEAR_BIT(*(uint32 *)(PORT_GpioPtr + PORT_DIGITAL_ENABLE_REG_OFFSET) ,config_pins[pin_counter].pin_num);
            }
            else if (pin_counter == PORTF_PIN0 || pin_counter == PORTF_PIN1 ){
                // CLEAR the corresponding bit in the analog mode select register to use bit as digital
                CLEAR_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,config_pins[pin_counter].pin_num);

                /*
                 * Enable the corresponding I/O pins by writing one to the corresponding bit
                 * IN GPIODEN register
                 */

                SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_DIGITAL_ENABLE_REG_OFFSET) ,config_pins[pin_counter].pin_num);

                // Set the corresponding bit in GPIOAFSEL to enable alternate functionality

                SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ALT_FUNC_REG_OFFSET) ,config_pins[pin_counter].pin_num);

                /*
                 * Set the PMCx bits in thr GPIOCTL register  to 000F( enable
                 * CORE functionality
                 */
                // clear the 4 corresponding bits

                *(uint32 *)(PORT_GpioPtr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F <<(config_pins[pin_counter].pin_num * NUMBER_OF_PMCX_BITS));

                // SET the 4 corresponding bits to 0008 to enable Analog comparator  functionality
                *(uint32 *)(PORT_GpioPtr + PORT_CTL_REG_OFFSET) |= (0x00000009 <<(config_pins[pin_counter].pin_num * NUMBER_OF_PMCX_BITS));

            }



            else {
#if (PORT_DEV_ERROR_DETECT == STD_ON)
                //The user entered an invalid pin to be configured as Analog comparator
                Det_ReportError(PORT_MODULE_ID , PORT_INSTANCE_ID , PORT_INIT_ID ,PORT_E_INIT_FAILED);
#endif
            }
            break;
        case PORT_PIN_MODE_JTAG :
            /*
             * Check if the pins that we want to configure can be configured as CORE pins by the hardware
             */

            if(pin_counter == PORTC_PIN0 ||  pin_counter == PORTC_PIN1 || pin_counter == PORTC_PIN2 || pin_counter == PORTC_PIN3)

            {
                // CLEAR the corresponding bit in the analog mode select register to use bit as digital
                CLEAR_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,config_pins[pin_counter].pin_num);

                /*
                 * Enable the corresponding I/O pins by writing one to the corresponding bit
                 * IN GPIODEN register
                 */


                SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_DIGITAL_ENABLE_REG_OFFSET) ,config_pins[pin_counter].pin_num);

                // Set the corresponding bit in GPIOAFSEL to enable alternate functionality

                SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ALT_FUNC_REG_OFFSET) ,config_pins[pin_counter].pin_num);

                /*
                 * Set the PMCx bits in thr GPIOCTL register  to 0001( enable
                 * JTAG functionality
                 */
                // clear the 4 corresponding bits

                *(uint32 *)(PORT_GpioPtr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F <<(config_pins[pin_counter].pin_num * NUMBER_OF_PMCX_BITS));

                // SET the 4 corresponding bits to 0008 to enable CORE  functionality
                *(uint32 *)(PORT_GpioPtr + PORT_CTL_REG_OFFSET) |= (0x00000001 <<(config_pins[pin_counter].pin_num *NUMBER_OF_PMCX_BITS));


            }
            else {
#if (PORT_DEV_ERROR_DETECT == STD_ON)
                //The user entered an invalid pin to be configured as CORE
                Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID ,PORT_INIT_ID, PORT_E_INIT_FAILED);
#endif
            }
            break;
        default :
            //The user entered an invalid pin mode
#if (PORT_DEV_ERROR_DETECT == STD_ON)
            Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID ,PORT_INIT_ID, PORT_E_INIT_FAILED);
#endif

        }

    }
    Port_Status = PORT_INITIALIZED ;



}



/************************************************************************************
 * Service Name: Port_SetPinDirection
 * Servide ID : 0x01
 * Sync/Async: Synchronous
 * Reentrancy: reentrant
 * Parameters (in): Pin -    Port Pin Id number
 *                  Direction - Port Pin direction
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: Sets the port pin direction
 ************************************************************************************/

#if  (PORT_SET_PIN_DIRECTION_API == STD_ON)
void Port_SetPinDirection (Port_PinType Pin,Port_PinDirectionType Direction){
    // check if we can change the pin direction during runtime .
#if (PORT_DEV_ERROR_DETECT == STD_ON)
    if (Port_Status == PORT_NOT_INITIALIZED){
        // PORT IS NOT initialized yet.
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID ,PORT_SET_PIN_DIRECTION_ID, PORT_E_UNINNIT);
    }
#endif
    if (PORT_PIN_DIRECTION_CHANGEABLE == STD_ON) {
        volatile uint32 * PORT_GpioPtr = NULL_PTR;
        switch (config_pins[Pin].port_num){

        /*
         * address the value of the PORT_GPioPtr to the base address register of the w
         * wanted port
         */
        case PORT_A:
            PORT_GpioPtr = (uint32 *)(GPIO_PORTA_BASE_ADDRESS);
            break;
        case PORT_B:
            PORT_GpioPtr = (uint32 *)(GPIO_PORTB_BASE_ADDRESS);
            break;
        case PORT_C:
            PORT_GpioPtr = (uint32 *)(GPIO_PORTC_BASE_ADDRESS);
            break;
        case PORT_D:
            PORT_GpioPtr = (uint32 *)(GPIO_PORTD_BASE_ADDRESS);
            break;
        case PORT_E:
            PORT_GpioPtr = (uint32 *)(GPIO_PORTE_BASE_ADDRESS);
            break;
        case PORT_F:
            PORT_GpioPtr = (uint32 *)(GPIO_PORTF_BASE_ADDRESS);
            break;
        default :
            // invalid pin requested
#if (PORT_DEV_ERROR_DETECT == STD_ON)
            Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID ,PORT_SET_PIN_DIRECTION_ID, PORT_E_PARAM_PIN);
#endif


        }

        /*
         * check if the pin that is required to be configured needs locking and commiting
         *  or not
         * the pin that require locking in Tiva c are PC(0~3) PD7 PF0
         */
        if(Pin == PORTC_PIN0 || Pin == PORTC_PIN1  || Pin == PORTC_PIN2
                || Pin == PORTC_PIN3 || Pin == PORTD_PIN7 ||
                Pin == PORTF_PIN0){
            (*(uint32 *)(PORT_GpioPtr + PORT_LOCK_REG_OFFSET)) = 0x4C4F434B;   // Unlock the GPIOCR register

            //set the corresponding bit in GPIOCR register
            SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_COMMIT_REG_OFFSET) ,config_pins[Pin].pin_num);
        }
        /*
         * check the direction of the pin and adjusting the GPIODIR REGISTER by clearing
         *  the corresponding bit if input  and setting it if it is output
         * setting the bit value
         */

        if (Direction == PORT_PIN_IN){
            CLEAR_BIT(*(uint32 *)(PORT_GpioPtr +  PORT_DIR_REG_OFFSET) ,config_pins[Pin].pin_num);
        }



        else if(Direction == PORT_PIN_OUT){
            SET_BIT(*(uint32 *)(PORT_GpioPtr +  PORT_DIR_REG_OFFSET) ,config_pins[Pin].pin_num);

        }
    }
    else{
#if (PORT_DEV_ERROR_DETECT == STD_ON)
        // change the pin direction during runtime configuration parameter is set to STD_OFF
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID ,PORT_SET_PIN_DIRECTION_ID, PORT_E_DIRECTION_UNCHANGEABLE);
#endif
    }
}


#endif

/************************************************************************************
 * Service Name: Port_RefreshPortDirection
 * service ID 0x02
 * Sync/Async: Synchronous
 * Reentrancy: Non reentrant
 * Parameters (in): None
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: Refreshes Port direction
 ************************************************************************************/
void Port_RefreshPortDirection (void){
#if (PORT_DEV_ERROR_DETECT == STD_ON)
    /* check if the Port driver is initialized*/
    if (Port_Status == PORT_NOT_INITIALIZED)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_INIT_ID, PORT_E_UNINNIT);

    }
    else {}
#endif
    uint8 pin_counter ;     // counter to loop on the config_pins array inside ConfigPtr
    volatile uint32 * PORT_GpioPtr = NULL_PTR;

    for (pin_counter=0 ;pin_counter < PORT_NUMBER_OF_PORT_PINS; pin_counter++){
        switch (config_pins[pin_counter].port_num){

        /*
         * address the value of the PORT_GPioPtr to the base address register of the w
         * wanted port
         */
        case PORT_A:
            PORT_GpioPtr = (uint32 *)(GPIO_PORTA_BASE_ADDRESS);
            break;
        case PORT_B:
            PORT_GpioPtr = (uint32 *)(GPIO_PORTB_BASE_ADDRESS);
            break;
        case PORT_C:
            PORT_GpioPtr = (uint32 *)(GPIO_PORTC_BASE_ADDRESS);
            break;
        case PORT_D:
            PORT_GpioPtr = (uint32 *)(GPIO_PORTD_BASE_ADDRESS);
            break;
        case PORT_E:
            PORT_GpioPtr = (uint32 *)(GPIO_PORTE_BASE_ADDRESS);
            break;
        case PORT_F:
            PORT_GpioPtr = (uint32 *)(GPIO_PORTF_BASE_ADDRESS);
            break;
        default :
            Det_ReportError( PORT_MODULE_ID,PORT_INSTANCE_ID,PORT_INIT_ID , PORT_E_INIT_FAILED);



        }



        /*Read Pin Direction */
        if(BIT_IS_SET(*(volatile uint32 *)((volatile uint8 *)PORT_GpioPtr + PORT_DIR_REG_OFFSET) , config_pins[pin_counter].pin_num))
        {
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)PORT_GpioPtr + PORT_DIR_REG_OFFSET) , config_pins[pin_counter].pin_num);
        }
        else
            if(BIT_IS_CLEAR(*(volatile uint32 *)((volatile uint8 *)PORT_GpioPtr + PORT_DIR_REG_OFFSET) , config_pins[pin_counter].pin_num))
            {
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PORT_GpioPtr + PORT_DIR_REG_OFFSET) , config_pins[pin_counter].pin_num);
            }
            else {/* No Action Required */}

    }



}





/************************************************************************************
 * Service Name: Port_GetVersionInfo
 * service ID 0x03
 * Sync/Async: Synchronous
 * Reentrancy: reentrant
 * Parameters (in): None
 * Parameters (inout): None
 * Parameters (out): version info
 * Return value: None
 * Description: returns the version information of this module
 ************************************************************************************/
#if (PORT_VERSION_INFO_API == STD_ON)
void Port_GetVersionInfo (Std_VersionInfoType* versioninfo){
#if (PORT_DEV_ERROR_DETECT == STD_ON)
    if (versioninfo == NULL_PTR){
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID ,PORT_SET_PIN_DIRECTION_ID, PORT_E_PARAM_POINTER);
    }
#endif
    versioninfo ->vendorID = PORT_VENDOR_ID;
    versioninfo ->moduleID = PORT_MODULE_ID ;
    versioninfo ->sw_major_version = PORT_SW_MAJOR_VERSION ;
    versioninfo ->sw_minor_version = PORT_SW_MINOR_VERSION ;
    versioninfo ->sw_patch_version = PORT_SW_PATCH_VERSION ;
}
#endif


/************************************************************************************
 * Service Name: Port_SetPinMode
 * service ID 0x04
 * Sync/Async: Synchronous
 * Reentrancy:reentrant
 * Parameters (in): Pin   port pin ID number
 *                  Mode  New port pin mode to be set on port pin
 * Parameters (in out): None
 * Parameters (out): None
 * Return value: None
 * Description: Sets the port pin mode
 ************************************************************************************/
#if (PORT_SET_PIN_MODE_API == STD_ON)
void Port_SetPinMode (Port_PinType Pin,Port_PinModeType Mode){
    volatile uint32 * PORT_GpioPtr = NULL_PTR;


    switch (config_pins[Pin].port_num){

    /*
     * address the value of the PORT_GPioPtr to the base address register of the w
     * wanted port
     */
    case PORT_A:
        PORT_GpioPtr = (uint32 *)(GPIO_PORTA_BASE_ADDRESS);
        break;
    case PORT_B:
        PORT_GpioPtr = (uint32 *)(GPIO_PORTB_BASE_ADDRESS);
        break;
    case PORT_C:
        PORT_GpioPtr = (uint32 *)(GPIO_PORTC_BASE_ADDRESS);
        break;
    case PORT_D:
        PORT_GpioPtr = (uint32 *)(GPIO_PORTD_BASE_ADDRESS);
        break;
    case PORT_E:
        PORT_GpioPtr = (uint32 *)(GPIO_PORTE_BASE_ADDRESS);
        break;
    case PORT_F:
        PORT_GpioPtr = (uint32 *)(GPIO_PORTF_BASE_ADDRESS);
        break;

    }


    /*
     * check if the pin that is required to be configured needs locking and commiting
     *  or not
     * the pin that require locking in Tiva c are PC(0~3) PD7 PF0
     */
    if(Pin == PORTC_PIN0 || Pin == PORTC_PIN1  || Pin == PORTC_PIN2
            || Pin == PORTC_PIN3 || Pin == PORTD_PIN7 || Pin == PORTF_PIN0){
        (*(uint32 *)(PORT_GpioPtr + PORT_LOCK_REG_OFFSET)) = 0x4C4F434B;   // Unlock the GPIOCR register

        //set the corresponding bit in GPIOCR register
        SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_COMMIT_REG_OFFSET) ,config_pins[Pin].pin_num);
    }
    /*
     * check the direction of the pin and adjusting the GPIODIR REGISTER by clearing
     *  the corresponding bit if input  and setting it if it is output
     * setting the bit value
     */

    if (config_pins[Pin].direction == PORT_PIN_IN){
        CLEAR_BIT(*(uint32 *)(PORT_GpioPtr +  PORT_DIR_REG_OFFSET) ,config_pins[Pin].pin_num);
        /*
         * adjust the internal resistor configuartions whether  it is pull_up or pull_down
         */


    }
    else if(config_pins[Pin].direction == PORT_PIN_OUT){
        SET_BIT(*(uint32 *)(PORT_GpioPtr +  PORT_DIR_REG_OFFSET) ,config_pins[Pin].pin_num);


    }


    switch (Mode){
    case PORT_PIN_MODE_DIO:
        // CLEAR the corresponding bit in the analog mode select register to use bit as digital
        CLEAR_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,config_pins[Pin].pin_num);

        /*
         * Clear the corresponding 4 PMCx bits in GPIOPCTL register
         * to select regular I/O digital function
         */

        *(uint32 *)(PORT_GpioPtr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F <<(config_pins[Pin].pin_num * NUMBER_OF_PMCX_BITS));
        /*
         * clear the corresponding bit in the GPIOAFSEL register to disable alternate functionality
         */

        CLEAR_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ALT_FUNC_REG_OFFSET) ,config_pins[Pin].pin_num);

        /*
         * Enable the corresponding I/O pins by writing one to the corresponding biT
         * IN GPIODEN register
         */
        SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_DIGITAL_ENABLE_REG_OFFSET) ,config_pins[Pin].pin_num);
        break;
    case PORT_PIN_MODE_ADC:

        /*
         * Check if the pins that we want to configure can be configured as ADC by the harwdare
         */
        if(Pin == PORTE_PIN3 ||  Pin == PORTE_PIN2 || Pin == PORTE_PIN1 || Pin == PORTE_PIN0
                || Pin == PORTD_PIN3 ||  Pin == PORTD_PIN2 || Pin == PORTD_PIN1 || Pin == PORTD_PIN0
                ||  Pin == PORTE_PIN5 ||  Pin == PORTE_PIN4 ||  Pin == PORTB_PIN5 ||  Pin == PORTB_PIN4)
        {
            // clear the the corresponding bit in GPIODIR to configure pin as input
            CLEAR_BIT(*(uint32 *)(PORT_GpioPtr + PORT_DIR_REG_OFFSET) ,config_pins[Pin].pin_num);

            // clear the the corresponding bit in GPIODEN to disbale digital function of the pin
            CLEAR_BIT(*(uint32 *)(PORT_GpioPtr + PORT_DIGITAL_ENABLE_REG_OFFSET) ,config_pins[Pin].pin_num);

            // Set the  corresponding bit in GPIOAFSEL Register to enable alternate functionlaity
            SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ALT_FUNC_REG_OFFSET) ,config_pins[Pin].pin_num);

            // Set the corresponding bit in AMSEL REGISTER  to enable analog function for the pin
            SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,config_pins[Pin].pin_num);
        }

        else {
#if (PORT_DEV_ERROR_DETECT == STD_ON)
            //The user enetered an invalid pin to be configured as ADC
            Det_ReportError( PORT_MODULE_ID,PORT_INSTANCE_ID,PORT_INIT_ID , PORT_E_INIT_FAILED);
#endif
        }
        break;
    case PORT_PIN_MODE_UART :
        /*
         * Check if the pins that we want to configure can be configured as ADC by the hardware
         */

        if(Pin == PORTA_PIN0 ||  Pin == PORTA_PIN1 || Pin == PORTC_PIN5 || Pin == PORTF_PIN1
                || Pin == PORTC_PIN4 ||  Pin == PORTF_PIN0 || Pin == PORTB_PIN0 || Pin == PORTC_PIN5
                || Pin == PORTB_PIN1 ||  Pin == PORTD_PIN6 || Pin == PORTD_PIN7 || Pin == PORTC_PIN6
                || Pin == PORTC_PIN7 ||  Pin == PORTE_PIN4 || Pin == PORTE_PIN5 || Pin == PORTD_PIN4
                || Pin == PORTD_PIN5 || Pin == PORTE_PIN0  || Pin == PORTE_PIN1 )
        {
            // CLEAR the corresponding bit in the analog mode select register to use bit as digital
            CLEAR_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,config_pins[Pin].pin_num);

            /*
             * Enable the corresponding I/O pins by writing one to the corresponding bit
             * IN GPIODEN register
             */


            SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_DIGITAL_ENABLE_REG_OFFSET) ,config_pins[Pin].pin_num);

            // Set the corresponding bit in GPIOAFSEL to enable alternate functionality

            SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ALT_FUNC_REG_OFFSET) ,config_pins[Pin].pin_num);

            /*
             * Set the PMCx bits in thr GPIOCTL register for PE0 ,PE1 to 0001( enable
             * UART functionality
             */
            // clear the 4 corresponding bits

            *(uint32 *)(PORT_GpioPtr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F <<(config_pins[Pin].pin_num * NUMBER_OF_PMCX_BITS));

            /*
             * Check which pin that we want to configure as UART in case of pin pc4 and pc5 different
             * value is assigned in the GPIOCTL register
             */
            if (Pin == PORTC_PIN4 || Pin == PORTC_PIN5 ){
                // set the 4 corresponding bits to 0002 to enable UART functionality
                *(uint32 *)(PORT_GpioPtr + PORT_CTL_REG_OFFSET) |= (0x00000002 <<(config_pins[Pin].pin_num *NUMBER_OF_PMCX_BITS));
            }

            else{
                // set the 4 corresponding bits to 0001 to enable UART functionality
                *(uint32 *)(PORT_GpioPtr + PORT_CTL_REG_OFFSET) |= (0x00000001 <<(config_pins[Pin].pin_num *NUMBER_OF_PMCX_BITS));
            }


        }
        else{
#if (PORT_DEV_ERROR_DETECT == STD_ON)
            //The user entered an invalid pin to be configured as UART
            Det_ReportError( PORT_MODULE_ID,PORT_INSTANCE_ID,PORT_INIT_ID , PORT_E_INIT_FAILED);
#endif
        }
        break;

    case PORT_PIN_MODE_SPI :
        /*
         * Check if the pins that we want to configure can be configured as SPI by the hardware
         */

        if(Pin == PORTA_PIN2 ||  Pin == PORTA_PIN3 || Pin == PORTA_PIN4 || Pin == PORTA_PIN5
                || Pin == PORTF_PIN2 ||  Pin == PORTD_PIN0 || Pin == PORTF_PIN3 || Pin == PORTD_PIN1
                || Pin == PORTF_PIN0 ||  Pin == PORTD_PIN2 || Pin == PORTF_PIN1 || Pin == PORTD_PIN3
                || Pin == PORTB_PIN4 ||  Pin == PORTB_PIN5 || Pin == PORTB_PIN6 || Pin == PORTB_PIN7)
        {
            // CLEAR the corresponding bit in the analog mode select register to use bit as digital
            CLEAR_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,config_pins[Pin].pin_num);

            /*
             * Enable the corresponding I/O pins by writing one to the corresponding bit
             * IN GPIODEN register
             */


            SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_DIGITAL_ENABLE_REG_OFFSET) ,config_pins[Pin].pin_num);

            // Set the corresponding bit in GPIOAFSEL to enable alternate functionality

            SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ALT_FUNC_REG_OFFSET) ,config_pins[Pin].pin_num);

            /*
             * Set the PMCx bits in thr GPIOCTL register for PE0 ,PE1 to 0002( enable
             * SPI (SSI) functionality
             */
            // clear the 4 corresponding bits

            *(uint32 *)(PORT_GpioPtr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F <<(config_pins[Pin].pin_num * NUMBER_OF_PMCX_BITS));

            // SET the 4 corresponding bits to 0002 to enable SPI (SSI) functionality
            *(uint32 *)(PORT_GpioPtr + PORT_CTL_REG_OFFSET) |= (0x00000002 <<(config_pins[Pin].pin_num *NUMBER_OF_PMCX_BITS));


        }
        else {
#if (PORT_DEV_ERROR_DETECT == STD_ON)
            //The user entered an invalid pin to be configured as SPI
            Det_ReportError( PORT_MODULE_ID,PORT_INSTANCE_ID,PORT_INIT_ID , PORT_E_INIT_FAILED);
#endif
        }
        break ;


    case PORT_PIN_MODE_I2C :
        /*
         * Check if the pins that we want to configure can be configured as I2C by the hardware
         */

        if(Pin == PORTB_PIN2 ||  Pin == PORTB_PIN3 || Pin == PORTA_PIN6 || Pin == PORTA_PIN7
                || Pin == PORTE_PIN4 ||  Pin == PORTE_PIN5 || Pin == PORTD_PIN0 || Pin == PORTD_PIN1)
        {
            // CLEAR the corresponding bit in the analog mode select register to use bit as digital
            CLEAR_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,config_pins[Pin].pin_num);

            /*
             * Enable the corresponding I/O pins by writing one to the corresponding bit
             * IN GPIODEN register
             */


            SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_DIGITAL_ENABLE_REG_OFFSET) ,config_pins[Pin].pin_num);

            // Set the corresponding bit in GPIOAFSEL to enable alternate functionality

            SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ALT_FUNC_REG_OFFSET) ,config_pins[Pin].pin_num);

            /*
             * Set the PMCx bits in thr GPIOCTL register for PE0 ,PE1 to 0003( enable
             * SPI (SSI) functionality
             */
            // clear the 4 corresponding bits

            *(uint32 *)(PORT_GpioPtr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F <<(config_pins[Pin].pin_num * NUMBER_OF_PMCX_BITS));

            // SET the 4 corresponding bits to 0003 to enable I2C  functionality
            *(uint32 *)(PORT_GpioPtr + PORT_CTL_REG_OFFSET) |= (0x00000003 <<(config_pins[Pin].pin_num *NUMBER_OF_PMCX_BITS));


        }
        else {
#if (PORT_DEV_ERROR_DETECT == STD_ON)
            //The user entered an invalid pin to be configured as I2X
            Det_ReportError( PORT_MODULE_ID,PORT_INSTANCE_ID,PORT_INIT_ID ,PORT_E_INIT_FAILED);
#endif
        }
        break ;
    case PORT_PIN_MODE_PWM :

        /*
         *  Check if the pin that we are configuring can
         *  be configured as PWM by the hardware and  if these bits  belong to module 0 or module 1
         */


        if(Pin == PORTF_PIN2 ||  Pin == PORTD_PIN6 || Pin == PORTD_PIN2 || Pin == PORTB_PIN6
                || Pin == PORTB_PIN7 ||  Pin == PORTB_PIN4 || Pin == PORTB_PIN5 || Pin == PORTE_PIN4
                || Pin == PORTE_PIN5 ||  Pin == PORTC_PIN4 || Pin == PORTD_PIN0 || Pin == PORTC_PIN5
                || Pin == PORTD_PIN1)
            // pins that belong to module 0 PWM
        {
            // CLEAR the corresponding bit in the analog mode select register to use bit as digital
            CLEAR_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,config_pins[Pin].pin_num);

            /*
             * Enable the corresponding I/O pins by writing one to the corresponding bit
             * IN GPIODEN register
             */


            SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_DIGITAL_ENABLE_REG_OFFSET) ,config_pins[Pin].pin_num);

            // Set the corresponding bit in GPIOAFSEL to enable alternate functionality

            SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ALT_FUNC_REG_OFFSET) ,config_pins[Pin].pin_num);

            /*
             * Set the PMCx bits in the GPIOCTL register for PE0 ,PE1 to 0002( enable
             * SPI (SSI) functionality
             */
            // clear the 4 corresponding bits

            *(uint32 *)(PORT_GpioPtr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F <<(config_pins[Pin].pin_num * NUMBER_OF_PMCX_BITS));

            // SET the 4 corresponding bits to 0004 to enable PWM (SSI) functionality
            *(uint32 *)(PORT_GpioPtr + PORT_CTL_REG_OFFSET) |= (0x00000004 <<(config_pins[Pin].pin_num *NUMBER_OF_PMCX_BITS));


        }
        else if (Pin == PORTA_PIN6 || Pin == PORTA_PIN7 || Pin == PORTF_PIN0 || Pin == PORTF_PIN1
                || Pin == PORTF_PIN3 || Pin == PORTF_PIN4) // pins that belong to module 1
        {
            // CLEAR the corresponding bit in the analog mode select register to use bit as digital
            CLEAR_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,config_pins[Pin].pin_num);

            /*
             * Enable the corresponding I/O pins by writing one to the corresponding bit
             * IN GPIODEN register
             */


            SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_DIGITAL_ENABLE_REG_OFFSET) ,config_pins[Pin].pin_num);

            // Set the corresponding bit in GPIOAFSEL to enable alternate functionality

            SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ALT_FUNC_REG_OFFSET) ,config_pins[Pin].pin_num);

            /*
             * Set the PMCx bits in the GPIOCTL register for PE0 ,PE1 to 0002( enable
             * SPI (SSI) functionality
             */
            // clear the 4 corresponding bits

            *(uint32 *)(PORT_GpioPtr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F <<(config_pins[Pin].pin_num * NUMBER_OF_PMCX_BITS));

            // SET the 4 corresponding bits to 0005 to enable PWM (SSI) functionality
            *(uint32 *)(PORT_GpioPtr + PORT_CTL_REG_OFFSET) |= (0x00000005 <<(config_pins[Pin].pin_num *NUMBER_OF_PMCX_BITS));

        }
        else {
#if (PORT_DEV_ERROR_DETECT == STD_ON)
            //The user entered an invalid pin to be configured as SPI
            Det_ReportError(PORT_MODULE_ID,PORT_INSTANCE_ID, PORT_INIT_ID ,PORT_E_INIT_FAILED);
#endif
        }
        break;
    case PORT_PIN_MODE_QEI :
        /*
         * Check if the pins that we want to configure can be configured as QEI by the hardware
         */

        if(Pin == PORTC_PIN4 ||  Pin == PORTC_PIN5 || Pin == PORTC_PIN6 || Pin == PORTD_PIN3
                || Pin == PORTD_PIN6 ||  Pin == PORTD_PIN7 ||  Pin == PORTF_PIN0
                || Pin == PORTF_PIN1  || Pin == PORTF_PIN4)
        {
            // CLEAR the corresponding bit in the analog mode select register to use bit as digital
            CLEAR_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,config_pins[Pin].pin_num);

            /*
             * Enable the corresponding I/O pins by writing one to the corresponding bit
             * IN GPIODEN register
             */


            SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_DIGITAL_ENABLE_REG_OFFSET) ,config_pins[Pin].pin_num);

            // Set the corresponding bit in GPIOAFSEL to enable alternate functionality

            SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ALT_FUNC_REG_OFFSET) ,config_pins[Pin].pin_num);

            /*
             * Set the PMCx bits in the GPIOCTL register  to 0006( enable
             * QEI functionality
             */
            // clear the 4 corresponding bits

            *(uint32 *)(PORT_GpioPtr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F <<(config_pins[Pin].pin_num * NUMBER_OF_PMCX_BITS));

            // SET the 4 corresponding bits to 0006 to enable QEI  functionality
            *(uint32 *)(PORT_GpioPtr + PORT_CTL_REG_OFFSET) |= (0x00000006 <<(config_pins[Pin].pin_num *NUMBER_OF_PMCX_BITS));


        }
        else {
#if (PORT_DEV_ERROR_DETECT == STD_ON)
            //The user entered an invalid pin to be configured as I2X
            Det_ReportError( PORT_MODULE_ID,PORT_INSTANCE_ID,PORT_INIT_ID ,PORT_E_INIT_FAILED);
#endif
        }
        break ;
    case PORT_PIN_MODE_GPT :
        /*
         * Check if the pins that we want to configure can be configured as GPT by the hardware
         */

        if(Pin == PORTB_PIN0 ||  Pin == PORTB_PIN1 || Pin == PORTB_PIN2 || Pin == PORTB_PIN3
                || Pin == PORTB_PIN4 ||  Pin == PORTB_PIN5 ||  Pin == PORTB_PIN6
                || Pin == PORTB_PIN7  || Pin == PORTF_PIN0 || Pin == PORTF_PIN1 ||
                Pin == PORTF_PIN2  || Pin == PORTF_PIN3 || Pin == PORTF_PIN4 ||
                Pin == PORTD_PIN0 || Pin == PORTD_PIN1  || Pin == PORTD_PIN2 || Pin == PORTD_PIN3 ||
                Pin == PORTD_PIN4 || Pin == PORTD_PIN5 || Pin == PORTD_PIN6 || Pin == PORTD_PIN7
                || Pin == PORTC_PIN0 || Pin == PORTC_PIN1  || Pin == PORTC_PIN2 || Pin == PORTC_PIN3
                || Pin == PORTC_PIN4 || Pin == PORTC_PIN5 || Pin == PORTC_PIN6 || Pin == PORTC_PIN7)
        {
            // CLEAR the corresponding bit in the analog mode select register to use bit as digital
            CLEAR_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,config_pins[Pin].pin_num);

            /*
             * Enable the corresponding I/O pins by writing one to the corresponding bit
             * IN GPIODEN register
             */


            SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_DIGITAL_ENABLE_REG_OFFSET) ,config_pins[Pin].pin_num);

            // Set the corresponding bit in GPIOAFSEL to enable alternate functionality

            SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ALT_FUNC_REG_OFFSET) ,config_pins[Pin].pin_num);

            /*
             * Set the PMCx bits in thr GPIOCTL register 0007( enable
             * GPT) functionality
             */
            // clear the 4 corresponding bits

            *(uint32 *)(PORT_GpioPtr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F <<(config_pins[Pin].pin_num * NUMBER_OF_PMCX_BITS));

            // SET the 4 corresponding bits to 0007 to enable GPT  functionality
            *(uint32 *)(PORT_GpioPtr + PORT_CTL_REG_OFFSET) |= (0x00000007 <<(config_pins[Pin].pin_num *NUMBER_OF_PMCX_BITS));


        }
        else {
#if (PORT_DEV_ERROR_DETECT == STD_ON)
            //The user entered an invalid pin to be configured as GPT
            Det_ReportError( PORT_MODULE_ID,PORT_INSTANCE_ID,PORT_INIT_ID ,PORT_E_INIT_FAILED);
#endif
        }
        break ;
    case PORT_PIN_MODE_CAN :
        /*
         * Check if the pins that we want to configure can be configured as CAN pins by the hardware
         */

        if(Pin == PORTF_PIN0 ||  Pin == PORTB_PIN4 || Pin == PORTE_PIN4 || Pin == PORTF_PIN3
                || Pin == PORTB_PIN5 ||  Pin == PORTE_PIN5 ||  Pin == PORTA_PIN0
                || Pin == PORTA_PIN1)
        {
            // CLEAR the corresponding bit in the analog mode select register to use bit as digital
            CLEAR_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,config_pins[Pin].pin_num);

            /*
             * Enable the corresponding I/O pins by writing one to the corresponding bit
             * IN GPIODEN register
             */


            SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_DIGITAL_ENABLE_REG_OFFSET) ,config_pins[Pin].pin_num);

            // Set the corresponding bit in GPIOAFSEL to enable alternate functionality

            SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ALT_FUNC_REG_OFFSET) ,config_pins[Pin].pin_num);

            /*
             * Set the PMCx bits in thr GPIOCTL register  to 0008( enable
             * CAN functionality
             */
            // clear the 4 corresponding bits

            *(uint32 *)(PORT_GpioPtr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F <<(config_pins[Pin].pin_num * NUMBER_OF_PMCX_BITS));

            // SET the 4 corresponding bits to 0008 to enable CAN  functionality
            *(uint32 *)(PORT_GpioPtr + PORT_CTL_REG_OFFSET) |= (0x00000008 <<(config_pins[Pin].pin_num *NUMBER_OF_PMCX_BITS));


        }
        else {
#if (PORT_DEV_ERROR_DETECT == STD_ON)
            //The user entered an invalid pin to be configured as CAN
            Det_ReportError(PORT_MODULE_ID,PORT_INSTANCE_ID,PORT_INIT_ID ,PORT_E_INIT_FAILED);
#endif
        }
        break;
    case PORT_PIN_MODE_NMI :
        /*
         * Check if the pins that we want to configure can be configured as NMI pins by the hardware
         */

        if(Pin == PORTF_PIN0 ||  Pin == PORTD_PIN7 )

        {
            // CLEAR the corresponding bit in the analog mode select register to use bit as digital
            CLEAR_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,config_pins[Pin].pin_num);

            /*
             * Enable the corresponding I/O pins by writing one to the corresponding bit
             * IN GPIODEN register
             */


            SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_DIGITAL_ENABLE_REG_OFFSET) ,config_pins[Pin].pin_num);

            // Set the corresponding bit in GPIOAFSEL to enable alternate functionality

            SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ALT_FUNC_REG_OFFSET) ,config_pins[Pin].pin_num);

            /*
             * Set the PMCx bits in thr GPIOCTL register  to 0008( enable
             * CAN functionality
             */
            // clear the 4 corresponding bits

            *(uint32 *)(PORT_GpioPtr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F <<(config_pins[Pin].pin_num * NUMBER_OF_PMCX_BITS));

            // SET the 4 corresponding bits to 0008 to enable NMI  functionality
            *(uint32 *)(PORT_GpioPtr + PORT_CTL_REG_OFFSET) |= (0x00000008 <<(config_pins[Pin].pin_num *NUMBER_OF_PMCX_BITS));


        }
        else {
#if (PORT_DEV_ERROR_DETECT == STD_ON)
            //The user entered an invalid pin to be configured as NMI
            Det_ReportError( PORT_MODULE_ID,PORT_INSTANCE_ID,PORT_INIT_ID ,PORT_E_INIT_FAILED);
#endif
        }
        break;

    case PORT_PIN_MODE_USB :
        /*
         * Check if the pins that we want to configure can be configured as NMI pins by the hardware
         * and if it will be digital pin or analog
         */

        if (Pin == PORTC_PIN6 ||  Pin == PORTC_PIN7 || Pin == PORTD_PIN2 || Pin == PORTD_PIN3
                || Pin == PORTF_PIN4)

        {
            // CLEAR the corresponding bit in the analog mode select register to use bit as digital
            CLEAR_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,config_pins[Pin].pin_num);

            /*
             * Enable the corresponding I/O pins by writing one to the corresponding bit
             * IN GPIODEN register
             */


            SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_DIGITAL_ENABLE_REG_OFFSET) ,config_pins[Pin].pin_num);

            // Set the corresponding bit in GPIOAFSEL to enable alternate functionality

            SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ALT_FUNC_REG_OFFSET) ,config_pins[Pin].pin_num);

            /*
             * Set the PMCx bits in thr GPIOCTL register  to 0008( enable
             * USB functionality
             */
            // clear the 4 corresponding bits

            *(uint32 *)(PORT_GpioPtr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F <<(config_pins[Pin].pin_num * NUMBER_OF_PMCX_BITS));

            // SET the 4 corresponding bits to 0008 to enable USB  functionality
            *(uint32 *)(PORT_GpioPtr + PORT_CTL_REG_OFFSET) |= (0x00000008 <<(config_pins[Pin].pin_num * NUMBER_OF_PMCX_BITS));


        }
        else if (Pin == PORTD_PIN4 || Pin == PORTD_PIN5 || Pin ==PORTB_PIN0 || Pin == PORTB_PIN1)
        {
            // clear the the corresponding bit in GPIODEN to disbale digital function of the pin
            CLEAR_BIT(*(uint32 *)(PORT_GpioPtr + PORT_DIGITAL_ENABLE_REG_OFFSET) ,config_pins[Pin].pin_num);

            // Set the  corresponding bit in GPIOAFSEL Register to enable alternate functionality
            SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ALT_FUNC_REG_OFFSET) ,config_pins[Pin].pin_num);

            // Set the corresponding bit in AMSEL REGISTER  to enable analog function for the pin
            SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,config_pins[Pin].pin_num);

        }
        else {
#if (PORT_DEV_ERROR_DETECT == STD_ON)
            //The user entered an invalid pin to be configured as USB
            Det_ReportError( PORT_MODULE_ID,PORT_INSTANCE_ID,PORT_INIT_ID ,PORT_E_INIT_FAILED);
#endif
        }
        break;
    case PORT_PIN_MODE_CORE :
        /*
         * Check if the pins that we want to configure can be configured as CORE pins by the hardware
         */

        if(Pin == PORTF_PIN1 ||  Pin == PORTF_PIN2 || Pin == PORTF_PIN3)

        {
            // CLEAR the corresponding bit in the analog mode select register to use bit as digital
            CLEAR_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,config_pins[Pin].pin_num);

            /*
             * Enable the corresponding I/O pins by writing one to the corresponding bit
             * IN GPIODEN register
             */


            SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_DIGITAL_ENABLE_REG_OFFSET) ,config_pins[Pin].pin_num);

            // Set the corresponding bit in GPIOAFSEL to enable alternate functionality

            SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ALT_FUNC_REG_OFFSET) ,config_pins[Pin].pin_num);

            /*
             * Set the PMCx bits in thr GPIOCTL register  to 000F( enable
             * CORE functionality
             */
            // clear the 4 corresponding bits

            *(uint32 *)(PORT_GpioPtr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F <<(config_pins[Pin].pin_num * NUMBER_OF_PMCX_BITS));

            // SET the 4 corresponding bits to 0008 to enable CORE  functionality
            *(uint32 *)(PORT_GpioPtr + PORT_CTL_REG_OFFSET) |= (0x0000000F <<(config_pins[Pin].pin_num *NUMBER_OF_PMCX_BITS));


        }
        else {
#if (PORT_DEV_ERROR_DETECT == STD_ON)
            //The user entered an invalid pin to be configured as CORE
            Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID ,PORT_INIT_ID, PORT_E_INIT_FAILED);
#endif
        }
        break;
    case PORT_PIN_MODE_ANALOG_COMP :
        /*
         * Check if the pins that we want to configure can be configured as Analog comparator pins by the hardware and if they will
         * work as analog or digital pins
         */

        if( Pin == PORTC_PIN7 ||   Pin == PORTC_PIN5 || Pin == PORTC_PIN4 )

        {
            // SET the corresponding bit in the analog mode select register to use bit as digital
            SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,config_pins[Pin].pin_num);

            /*
             * Diable the corresponding I/O pins by writing one to the corresponding bit
             * IN GPIODEN register
             */


            CLEAR_BIT(*(uint32 *)(PORT_GpioPtr + PORT_DIGITAL_ENABLE_REG_OFFSET) ,config_pins[Pin].pin_num);
        }
        else if ( Pin == PORTF_PIN0 || Pin == PORTF_PIN1 ){
            // CLEAR the corresponding bit in the analog mode select register to use bit as digital
            CLEAR_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,config_pins[Pin].pin_num);

            /*
             * Enable the corresponding I/O pins by writing one to the corresponding bit
             * IN GPIODEN register
             */

            SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_DIGITAL_ENABLE_REG_OFFSET) ,config_pins[Pin].pin_num);

            // Set the corresponding bit in GPIOAFSEL to enable alternate functionality

            SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ALT_FUNC_REG_OFFSET) ,config_pins[Pin].pin_num);

            /*
             * Set the PMCx bits in thr GPIOCTL register  to 000F( enable
             * CORE functionality
             */
            // clear the 4 corresponding bits

            *(uint32 *)(PORT_GpioPtr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F <<(config_pins[Pin].pin_num * NUMBER_OF_PMCX_BITS));

            // SET the 4 corresponding bits to 0008 to enable Analog comparator  functionality
            *(uint32 *)(PORT_GpioPtr + PORT_CTL_REG_OFFSET) |= (0x00000009 <<(config_pins[Pin].pin_num * NUMBER_OF_PMCX_BITS));

        }



        else {
#if ( PORT_DEV_ERROR_DETECT == STD_ON )
            //The user entered an invalid pin to be configured as Analog comparator
            Det_ReportError(PORT_MODULE_ID , PORT_INSTANCE_ID , PORT_INIT_ID ,PORT_E_INIT_FAILED);
#endif
        }
        break;
    case PORT_PIN_MODE_JTAG :
        /*
         * Check if the pins that we want to configure can be configured as CORE pins by the hardware
         */

        if(Pin == PORTC_PIN0 ||  Pin == PORTC_PIN1 || Pin == PORTC_PIN2 || Pin == PORTC_PIN3)

        {
            // CLEAR the corresponding bit in the analog mode select register to use bit as digital
            CLEAR_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,config_pins[Pin].pin_num);

            /*
             * Enable the corresponding I/O pins by writing one to the corresponding bit
             * IN GPIODEN register
             */


            SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_DIGITAL_ENABLE_REG_OFFSET) ,config_pins[Pin].pin_num);

            // Set the corresponding bit in GPIOAFSEL to enable alternate functionality

            SET_BIT(*(uint32 *)(PORT_GpioPtr + PORT_ALT_FUNC_REG_OFFSET) ,config_pins[Pin].pin_num);

            /*
             * Set the PMCx bits in thr GPIOCTL register  to 0001( enable
             * JTAG functionality
             */
            // clear the 4 corresponding bits

            *(uint32 *)(PORT_GpioPtr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F <<(config_pins[Pin].pin_num * NUMBER_OF_PMCX_BITS));

            // SET the 4 corresponding bits to 0008 to enable CORE  functionality
            *(uint32 *)(PORT_GpioPtr + PORT_CTL_REG_OFFSET) |= (0x00000001 <<(config_pins[Pin].pin_num *NUMBER_OF_PMCX_BITS));


        }
        else {
#if ( PORT_DEV_ERROR_DETECT == STD_ON )
            //The user entered an invalid pin to be configured as JTAG
            Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID ,PORT_INIT_ID, PORT_E_INIT_FAILED);
#endif
        }
        break;
    default :
        //The user entered an invalid pin mode
#if ( PORT_DEV_ERROR_DETECT == STD_ON )
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID ,PORT_INIT_ID, PORT_E_INIT_FAILED);
#endif

    }

}


#endif
