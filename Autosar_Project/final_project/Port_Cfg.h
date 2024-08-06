/*
 * Port_Cfg.h
 *
 *  Created on: Jul 21, 2024
 *  Author: youssef
 */

#ifndef PORT_CFG_H_
#define PORT_CFG_H_




/*******************************************************************************************
 *                                 Module Definitions
 ******************************************************************************************/

#define PORT_CFG_SW_MAJOR_VERSION              (1U)
#define PORT_CFG_SW_MINOR_VERSION              (0U)
#define PORT_CFG_SW_PATCH_VERSION              (0U)

/*
 * AUTOSAR Version 4.0.2
 */
#define PORT_CFG_AR_RELEASE_MAJOR_VERSION     (4U)
#define PORT_CFG_AR_RELEASE_MINOR_VERSION     (0U)
#define PORT_CFG_AR_RELEASE_PATCH_VERSION     (3U)




/******************************** Configuration Parameters **************************/


/*
 * The number of specified PortPins in this PortContainer.
 */
#define PORT_NUMBER_OF_PORT_PINS       (43U)



/*
 *Switches the development error detection and notification on or off.
        • true: detection and notification is enabled.
        • false: detection and notification is disabled.
 */
#define PORT_DEV_ERROR_DETECT          (STD_ON)


/*
 * Pre-processor switch to enable / disable the use of the function Port_SetPinDirection().
    TRUE: Enabled - Function Port_SetPinDirection() is available. FALSE: Disabled -
    Function Port_SetPinDirection() is not available.
 */

#define PORT_SET_PIN_DIRECTION_API       (STD_ON)

/*
 * Pre-processor switch to enable / disable the use of the function Port_SetPinMode().
   true: Enabled - Function Port_SetPinMode() is available. false: Disabled - Function
   Port_SetPinMode() is not available.
 */
#define  PORT_SET_PIN_MODE_API              (STD_ON)


/*
 * Pre-processor switch to enable / disable the API to read out the modules version
   information. true: Version info API enabled. false: Version info API disabled.
 */

#define PORT_VERSION_INFO_API          (STD_ON)


/*
 *The initial direction of the pin (IN or OUT). If the direction is not changeable, the value
    configured here is fixed.
  The direction must match the pin mode. E.g. a pin used for an ADC must be configured
  to be an in port.
  Implementation Type: Port_PinDirectionType
 */

#define PORT_PIN_DIRECTTION           (PORT_PIN_IN)



/*
 * Parameter to indicate if the direction is changeable on a port pin during runtime. true:
   Port Pin direction changeable enabled. false: Port Pin direction changeable disabled.
 */


#define PORT_PIN_DIRECTION_CHANGEABLE            (STD_ON)





/*
 * *************************  port pin id  **********************************************/

//PORTA
#define PORT_PINS_OF_PORTA    (uint8) 8
#define PORTA_PIN0            (Port_PinType) 0
#define PORTA_PIN1            (Port_PinType) 1
#define PORTA_PIN2            (Port_PinType) 2
#define PORTA_PIN3            (Port_PinType) 3
#define PORTA_PIN4            (Port_PinType) 4
#define PORTA_PIN5            (Port_PinType) 5
#define PORTA_PIN6            (Port_PinType) 6
#define PORTA_PIN7            (Port_PinType) 7
//PORTB
#define PORT_PINS_OF_PORTB   (uint8) 8
#define PORTB_PIN0            (Port_PinType) 8
#define PORTB_PIN1            (Port_PinType) 9
#define PORTB_PIN2            (Port_PinType) 10
#define PORTB_PIN3            (Port_PinType) 11
#define PORTB_PIN4            (Port_PinType) 12
#define PORTB_PIN5            (Port_PinType) 13
#define PORTB_PIN6            (Port_PinType) 14
#define PORTB_PIN7            (Port_PinType) 15
//PORTC
#define PORT_PINS_OF_PORTC   (uint8) 8
#define PORTC_PIN0            (Port_PinType) 16
#define PORTC_PIN1            (Port_PinType) 17
#define PORTC_PIN2            (Port_PinType) 18
#define PORTC_PIN3            (Port_PinType) 19
#define PORTC_PIN4            (Port_PinType) 20
#define PORTC_PIN5            (Port_PinType) 21
#define PORTC_PIN6            (Port_PinType) 22
#define PORTC_PIN7            (Port_PinType) 23
//PORTD
#define PORT_PINS_OF_PORTD   (uint8) 8
#define PORTD_PIN0            (Port_PinType) 24
#define PORTD_PIN1            (Port_PinType) 25
#define PORTD_PIN2            (Port_PinType) 26
#define PORTD_PIN3            (Port_PinType) 27
#define PORTD_PIN4            (Port_PinType) 28
#define PORTD_PIN5            (Port_PinType) 29
#define PORTD_PIN6            (Port_PinType) 30
#define PORTD_PIN7            (Port_PinType) 31

//PORTE
#define PORT_PINS_OF_PORTE   (uint8) 6
#define PORTE_PIN0            (Port_PinType) 32
#define PORTE_PIN1            (Port_PinType) 33
#define PORTE_PIN2            (Port_PinType) 34
#define PORTE_PIN3            (Port_PinType) 35
#define PORTE_PIN4            (Port_PinType) 36
#define PORTE_PIN5            (Port_PinType) 37

//PORTF
#define PORT_PINS_OF_PORTF   (uint8) 5
#define PORTF_PIN0            (Port_PinType) 38
#define PORTF_PIN1            (Port_PinType) 39
#define PORTF_PIN2            (Port_PinType) 40
#define PORTF_PIN3            (Port_PinType) 41
#define PORTF_PIN4            (Port_PinType) 42







// Port Pin Initial  Modes
#define PORT_PIN_MODE_DIO            (Port_PinModeType)0     // digital I/O mode
#define PORT_PIN_MODE_ADC            (Port_PinModeType)1     // Analog to digital mode
#define PORT_PIN_MODE_ANALOG_COMP    (Port_PinModeType)2     // Analog comparator mode
#define PORT_PIN_MODE_CAN            (Port_PinModeType)3     // CAN mode
#define PORT_PIN_MODE_CORE           (Port_PinModeType)4     // CORE mode
#define PORT_PIN_MODE_GPT            (Port_PinModeType)5     // general purpose timer mode
#define PORT_PIN_MODE_I2C            (Port_PinModeType)6     // mode I2C
#define PORT_PIN_MODE_JTAG           (Port_PinModeType)7     // JTAG(debugging interface)
#define PORT_PIN_MODE_PWM            (Port_PinModeType)8     // PWM Mode
#define PORT_PIN_MODE_QEI            (Port_PinModeType)9     // QEI Mode
#define PORT_PIN_MODE_SPI            (Port_PinModeType)10    // SPI mode
#define PORT_PIN_MODE_HIBERNATE      (Port_PinModeType)11     // HIBERNATE MODE
#define PORT_PIN_MODE_UART           (Port_PinModeType)12    // UART mode
#define PORT_PIN_MODE_NMI            (Port_PinModeType)13    // Non maskable interrupt mode
#define PORT_PIN_MODE_USB            (Port_PinModeType)14    // USB mode




//port A
#define PORTA_PIN0_INIT_MODE      PORT_PIN_MODE_DIO
#define PORTA_PIN1_INIT_MODE      PORT_PIN_MODE_DIO
#define PORTA_PIN2_INIT_MODE      PORT_PIN_MODE_DIO
#define PORTA_PIN3_INIT_MODE      PORT_PIN_MODE_DIO
#define PORTA_PIN4_INIT_MODE      PORT_PIN_MODE_DIO
#define PORTA_PIN5_INIT_MODE      PORT_PIN_MODE_DIO
#define PORTA_PIN6_INIT_MODE      PORT_PIN_MODE_DIO
#define PORTA_PIN7_INIT_MODE      PORT_PIN_MODE_DIO


//Port B
#define PORTB_PIN0_INIT_MODE      PORT_PIN_MODE_DIO
#define PORTB_PIN1_INIT_MODE      PORT_PIN_MODE_DIO
#define PORTB_PIN2_INIT_MODE      PORT_PIN_MODE_DIO
#define PORTB_PIN3_INIT_MODE      PORT_PIN_MODE_DIO
#define PORTB_PIN4_INIT_MODE      PORT_PIN_MODE_DIO
#define PORTB_PIN5_INIT_MODE      PORT_PIN_MODE_DIO
#define PORTB_PIN6_INIT_MODE      PORT_PIN_MODE_DIO
#define PORTB_PIN7_INIT_MODE      PORT_PIN_MODE_DIO

//Port c
#define PORTC_PIN0_INIT_MODE      PORT_PIN_MODE_DIO
#define PORTC_PIN1_INIT_MODE      PORT_PIN_MODE_DIO
#define PORTC_PIN2_INIT_MODE      PORT_PIN_MODE_DIO
#define PORTC_PIN3_INIT_MODE      PORT_PIN_MODE_DIO
#define PORTC_PIN4_INIT_MODE      PORT_PIN_MODE_DIO
#define PORTC_PIN5_INIT_MODE      PORT_PIN_MODE_DIO
#define PORTC_PIN6_INIT_MODE      PORT_PIN_MODE_DIO
#define PORTC_PIN7_INIT_MODE      PORT_PIN_MODE_DIO

//Port d
#define PORTD_PIN0_INIT_MODE      PORT_PIN_MODE_DIO
#define PORTD_PIN1_INIT_MODE      PORT_PIN_MODE_DIO
#define PORTD_PIN2_INIT_MODE      PORT_PIN_MODE_DIO
#define PORTD_PIN3_INIT_MODE      PORT_PIN_MODE_DIO
#define PORTD_PIN4_INIT_MODE      PORT_PIN_MODE_DIO
#define PORTD_PIN5_INIT_MODE      PORT_PIN_MODE_DIO
#define PORTD_PIN6_INIT_MODE      PORT_PIN_MODE_DIO
#define PORTD_PIN7_INIT_MODE      PORT_PIN_MODE_DIO

// Port e
#define PORTE_PIN0_INIT_MODE      PORT_PIN_MODE_DIO
#define PORTE_PIN1_INIT_MODE      PORT_PIN_MODE_DIO
#define PORTE_PIN2_INIT_MODE      PORT_PIN_MODE_DIO
#define PORTE_PIN3_INIT_MODE      PORT_PIN_MODE_DIO
#define PORTE_PIN4_INIT_MODE      PORT_PIN_MODE_DIO
#define PORTE_PIN5_INIT_MODE      PORT_PIN_MODE_DIO

//Port f
#define PORTF_PIN0_INIT_MODE      PORT_PIN_MODE_DIO
#define PORTF_PIN1_INIT_MODE      PORT_PIN_MODE_DIO
#define PORTF_PIN2_INIT_MODE      PORT_PIN_MODE_DIO
#define PORTF_PIN3_INIT_MODE      PORT_PIN_MODE_DIO
#define PORTF_PIN4_INIT_MODE      PORT_PIN_MODE_DIO




//port indices
#define PORT_A                    (uint8) 0
#define PORT_B                    (uint8) 1
#define PORT_C                    (uint8) 2
#define PORT_D                    (uint8) 3
#define PORT_E                    (uint8) 4
#define PORT_F                    (uint8) 5

//pin indices
#define PIN0                      (uint8) 0
#define PIN1                      (uint8) 1
#define PIN2                      (uint8) 2
#define PIN3                      (uint8) 3
#define PIN4                      (uint8) 4
#define PIN5                      (uint8) 5
#define PIN6                      (uint8) 6
#define PIN7                      (uint8) 7








#endif /* PORT_CFG_H_ */
