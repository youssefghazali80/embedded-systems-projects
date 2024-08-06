/******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port.h
 *
 * Description: Header file for TM4C123GH6PM Microcontroller - Port Driver.
 *
 * Author: Youssef ghazali
 ******************************************************************************/

#ifndef PORT_H
#define PORT_H

#include "Common_Macros.h"
#include "Std_Types.h"
#include "Port_Cfg.h"
#include "Platform_Types.h"


/****************************************************************************************
 *                            Module Definitions
 *
 ****************************************************************************************/

#define PORT_VENDOR_ID   0xFFFF           // ID usef for educational and development purposes

#define PORT_MODULE_ID   (124U)              // Port module id as specified by AutoSar documents

#define PORT_INSTANCE_ID  (0U)

/*
 * Software module version 1.0.0
 */
#define PORT_SW_MAJOR_VERSION                (1U)
#define PORT_SW_MINOR_VERSION                (0U)
#define PORT_SW_PATCH_VERSION                (0U)

/*
 * AUTOSAR version is 4.0.3 compatible
 */
#define PORT_AR_RELEASE_MAJOR_VERSION         (4U)
#define PORT_AR_RELAESE_MINOR_VERSION         (0U)
#define PORT_AR_RELEASE_PATCH_VERSION         (3U)



/*************************************   Version check    ********************************/
#if ((STD_TYPES_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
        ||  (STD_TYPES_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
        ||  (STD_TYPES_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
#error "The AR version of Std_Types.h does not match the expected version"
#endif

/* AUTOSAR Version checking between PORT_Cfg.h and PORT.h files */

#if ((PORT_CFG_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
        ||  (PORT_CFG_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
        ||  (PORT_CFG_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
#error "The AR version of Port_Cfg.h does not match the expected version"
#endif

/* Software Version checking between PORT_Cfg.h and PORT.h files */
#if ((PORT_CFG_SW_MAJOR_VERSION != PORT_SW_MAJOR_VERSION)\
        ||  (PORT_CFG_SW_MINOR_VERSION != PORT_SW_MINOR_VERSION)\
        ||  (PORT_CFG_SW_PATCH_VERSION != PORT_SW_PATCH_VERSION))
#error "The SW version of Port_Cfg.h does not match the expected version"
#endif






/*******************************************************************************
 *                              MODULE REGISTERS                                *
 *******************************************************************************/

/* GPIO Registers base addresses */
#define GPIO_PORTA_BASE_ADDRESS           0x40004000
#define GPIO_PORTB_BASE_ADDRESS           0x40005000
#define GPIO_PORTC_BASE_ADDRESS           0x40006000
#define GPIO_PORTD_BASE_ADDRESS           0x40007000
#define GPIO_PORTE_BASE_ADDRESS           0x40024000
#define GPIO_PORTF_BASE_ADDRESS           0x40025000

/* GPIO Registers offset addresses */
#define PORT_DATA_REG_OFFSET              0x3FC
#define PORT_DIR_REG_OFFSET               0x400
#define PORT_ALT_FUNC_REG_OFFSET          0x420
#define PORT_PULL_UP_REG_OFFSET           0x510
#define PORT_PULL_DOWN_REG_OFFSET         0x514
#define PORT_DIGITAL_ENABLE_REG_OFFSET    0x51C
#define PORT_LOCK_REG_OFFSET              0x520
#define PORT_COMMIT_REG_OFFSET            0x524
#define PORT_ANALOG_MODE_SEL_REG_OFFSET   0x528
#define PORT_CTL_REG_OFFSET               0x52C

#define NUMBER_OF_PMCX_BITS               (4U)



/*************************************** Service Id******************************************/

/*
 * APi service Id for Port_Init function
 */
#define PORT_INIT_ID                     (uint8) 0x00


/*
 * APi service Id for Port_SetPin function
 */
#define PORT_SET_PIN_DIRECTION_ID         (uint8) 0x01


/*
 * APi service Id for Port_SetPin function
 */
#define PORT_REFRESH_PORT_DIRECTION_ID    (uint8) 0x02


/*
 * API service Id for port_GetVersionInfo function
 */

#define  PORT_GET_VERSION_INFO_ID         (uint8) 0x03


/*
 * API service ID for port_SetPinMode
 */
#define PORT_SET_PIN_MODE_ID              (uint8) 0x04


#define PORT_INITIALIZED                (1U)
#define PORT_NOT_INITIALIZED            (0U)




/**********************************************************************************
 *                              DET Error Codes
 ***********************************************************************************/

#define PORT_E_PARAM_PIN                   0x0A     // Invalid Port pin id requested

#define PORT_E_DIRECTION_UNCHANGEABLE      0x0B     // Port pin not configured as changeable

#define PORT_E_INIT_FAILED                 0x0C     /* API Port_Init service called with
                                                      wrong parameter*/


#define PORT_E_PARAM_INVALID_MODE          0x0D     /* API Port_SetPinMode service called
                                                        when mode is unchangeable */

#define PORT_E_MODE_UNCHANGEABLE           0X0E     /* API Port_SetPinMode service called
                                                        when mode is unchangeable */

#define PORT_E_UNINNIT                     0x0F     /* API service called without module
                                                        initialization*/

#define PORT_E_PARAM_POINTER               0x10     /* API called with NULL pointer*/









/*******************************************************************************
 *                              Module Data Types                              *
 *******************************************************************************/



typedef uint8 Port_PinType; //Data type for the symbolic name of a port pin

typedef uint8 Port_PinModeType;   // different port pin modes

/*
 * Enum to represent Possible directions of a port pin.
 */

typedef enum {
    PORT_PIN_IN , PORT_PIN_OUT
}Port_PinDirectionType;

/*
 * enum to represent the resistance connection of the pins that are designed to be I/O
 *
 */
typedef enum{
    PULL_UP , PULL_DOWN
}Resistance_Config;
/*
 * Data structure to contain the configuration of each pin.
 */
typedef struct {
    uint8 port_num ;    // port that each pin belong to
    uint8 pin_num ;     //pin number
    Port_PinModeType pin_mode ;  //mode that this pin will be
    Port_PinDirectionType direction ;  // the direction of this pin whether input or output
    Resistance_Config resistor ;      // the configuration of the internal resistor if its input pin
    uint8 inital_state;              // the initial state of this pin if its output pin

}Port_ConfigPins;


/*
 * Description Type of the external data structure
 * containing the initialization data for this module.
 */
typedef struct {
    Port_ConfigPins config_pins [PORT_NUMBER_OF_PORT_PINS];   // array of Port_ConfigPins structs


}Port_ConfigType;




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
void Port_Init(const Port_ConfigType *ConfigPtr );



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
void Port_SetPinDirection (Port_PinType Pin,Port_PinDirectionType Direction);
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
void Port_RefreshPortDirection (void);



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
void Port_GetVersionInfo (Std_VersionInfoType* versioninfo);
#endif


/************************************************************************************
 * Service Name: Port_SetPinMode
 * service ID 0x04
 * Sync/Async: Synchronous
 * Reentrancy:reentrant
 * Parameters (in): Pin   port pin ID number
 *                  Mode  New port pin mode to be set on port pin
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: Sets the port pin mode
 ************************************************************************************/
#if (PORT_SET_PIN_MODE_API == STD_ON)
void Port_SetPinMode (Port_PinType Pin,Port_PinModeType Mode);
#endif






















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
void Port_Init(const Port_ConfigType *ConfigPtr );



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
void Port_SetPinDirection (Port_PinType Pin,Port_PinDirectionType Direction);
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
void Port_RefreshPortDirection (void);



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
void Port_GetVersionInfo (Std_VersionInfoType* versioninfo);
#endif


/************************************************************************************
 * Service Name: Port_SetPinMode
 * service ID 0x04
 * Sync/Async: Synchronous
 * Reentrancy:reentrant
 * Parameters (in): Pin   port pin ID number
 *                  Mode  New port pin mode to be set on port pin
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: Sets the port pin mode
 ************************************************************************************/
#if (PORT_SET_PIN_MODE_API == STD_ON)
void Port_SetPinMode (Port_PinType Pin,Port_PinModeType Mode);
#endif




// include the
extern const Port_ConfigType Port_Configuration;



#endif /* PORT_H */
