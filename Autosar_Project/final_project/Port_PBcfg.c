/*
 * Port_PBcfg.c
 *
 *  Created on: Jul 31, 2024
 *      Author: youssef ghazali
 *      Description: Post Build Configuration Source file for TM4C123GH6PM Microcontroller - Port Driver
 */


#include "Port.h"


/*
 * SW Module Version is 1.0.0
 */
#define PORT_PBCFG_SW_MAJOR_VERSION             (1U)
#define PORT_PBCFG_SW_MINOR_VERSION             (0U)
#define PORT_PBCFG_SW_PATCH_VERSION             (0U)

/*
 * AUTOSAR Version is 4.0.2 compatible
 */
#define PORT_PBCFG_AR_RELEASE_MAJOR_VERSION     (4U)
#define PORT_PBCFG_AR_RELEASE_MINOR_VERSION     (0U)
#define PORT_PBCFG_AR_RELEASE_PATCH_VERSION     (3U)

/* Check compatibility of Port.h AUTOSAR version with
 * Port_PBcfg.c AUTOSAR version.
 */
#if ((PORT_PBCFG_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
        ||  (PORT_PBCFG_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
        ||  (PORT_PBCFG_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
#error "The AR version of Port.h does not match the expected version"
#endif

/* Check compatibility of Port.h SW version with
 * Port_PBcfg.c SW version.
 */
#if ((PORT_PBCFG_SW_MAJOR_VERSION != PORT_SW_MAJOR_VERSION)\
        ||  (PORT_PBCFG_SW_MINOR_VERSION != PORT_SW_MINOR_VERSION)\
        ||  (PORT_PBCFG_SW_PATCH_VERSION != PORT_SW_PATCH_VERSION))
#error "The AR version of Port.h does not match the expected version"
#endif
 // initial configuration of all the pins
const Port_ConfigType Port_Configuration = {
                                      PORT_A ,PIN0 ,PORTA_PIN0_INIT_MODE , PORT_PIN_IN, PULL_DOWN ,STD_LOW,
                                      PORT_A ,PIN1 ,PORTA_PIN1_INIT_MODE , PORT_PIN_IN, PULL_DOWN ,STD_LOW,
                                      PORT_A ,PIN2 ,PORTA_PIN2_INIT_MODE , PORT_PIN_IN, PULL_DOWN ,STD_LOW,
                                      PORT_A ,PIN3 ,PORTA_PIN3_INIT_MODE , PORT_PIN_IN, PULL_DOWN ,STD_LOW,
                                      PORT_A ,PIN4 ,PORTA_PIN4_INIT_MODE , PORT_PIN_IN, PULL_DOWN ,STD_LOW,
                                      PORT_A ,PIN5 ,PORTA_PIN5_INIT_MODE , PORT_PIN_IN, PULL_DOWN ,STD_LOW,
                                      PORT_A ,PIN6 ,PORTA_PIN6_INIT_MODE , PORT_PIN_IN, PULL_DOWN ,STD_LOW,
                                      PORT_A ,PIN7 ,PORTA_PIN7_INIT_MODE , PORT_PIN_IN, PULL_DOWN ,STD_LOW,
                                      PORT_B ,PIN0 ,PORTB_PIN0_INIT_MODE , PORT_PIN_IN, PULL_DOWN ,STD_LOW,
                                      PORT_B ,PIN1 ,PORTB_PIN1_INIT_MODE , PORT_PIN_IN, PULL_DOWN ,STD_LOW,
                                      PORT_B ,PIN2 ,PORTB_PIN2_INIT_MODE , PORT_PIN_IN, PULL_DOWN ,STD_LOW,
                                      PORT_B ,PIN3 ,PORTB_PIN3_INIT_MODE , PORT_PIN_IN, PULL_DOWN ,STD_LOW,
                                      PORT_B ,PIN4 ,PORTB_PIN4_INIT_MODE , PORT_PIN_IN, PULL_DOWN ,STD_LOW,
                                      PORT_B ,PIN5 ,PORTB_PIN5_INIT_MODE , PORT_PIN_IN, PULL_DOWN ,STD_LOW,
                                      PORT_B ,PIN6 ,PORTB_PIN6_INIT_MODE , PORT_PIN_IN, PULL_DOWN ,STD_LOW,
                                      PORT_B ,PIN7 ,PORTB_PIN7_INIT_MODE , PORT_PIN_IN, PULL_DOWN ,STD_LOW,
                                      PORT_C ,PIN0 ,PORTC_PIN0_INIT_MODE , PORT_PIN_IN, PULL_DOWN ,STD_LOW,
                                      PORT_C ,PIN1 ,PORTC_PIN1_INIT_MODE , PORT_PIN_IN, PULL_DOWN ,STD_LOW,
                                      PORT_C ,PIN2 ,PORTC_PIN2_INIT_MODE , PORT_PIN_IN, PULL_DOWN ,STD_LOW,
                                      PORT_C ,PIN3 ,PORTC_PIN3_INIT_MODE , PORT_PIN_IN, PULL_DOWN ,STD_LOW,
                                      PORT_C ,PIN4 ,PORTC_PIN4_INIT_MODE , PORT_PIN_IN, PULL_DOWN ,STD_LOW,
                                      PORT_C ,PIN5 ,PORTC_PIN5_INIT_MODE , PORT_PIN_IN, PULL_DOWN ,STD_LOW,
                                      PORT_C ,PIN6 ,PORTC_PIN6_INIT_MODE , PORT_PIN_IN, PULL_DOWN ,STD_LOW,
                                      PORT_C ,PIN7 ,PORTC_PIN7_INIT_MODE , PORT_PIN_IN, PULL_DOWN ,STD_LOW,
                                      PORT_D ,PIN0 ,PORTD_PIN0_INIT_MODE , PORT_PIN_IN, PULL_DOWN ,STD_LOW,
                                      PORT_D ,PIN1 ,PORTD_PIN1_INIT_MODE , PORT_PIN_IN, PULL_DOWN ,STD_LOW,
                                      PORT_D ,PIN2 ,PORTD_PIN2_INIT_MODE , PORT_PIN_IN, PULL_DOWN ,STD_LOW,
                                      PORT_D ,PIN3 ,PORTD_PIN3_INIT_MODE , PORT_PIN_IN, PULL_DOWN ,STD_LOW,
                                      PORT_D ,PIN4 ,PORTD_PIN4_INIT_MODE , PORT_PIN_IN, PULL_DOWN ,STD_LOW,
                                      PORT_D ,PIN5 ,PORTD_PIN5_INIT_MODE , PORT_PIN_IN, PULL_DOWN ,STD_LOW,
                                      PORT_D ,PIN6 ,PORTD_PIN6_INIT_MODE , PORT_PIN_IN, PULL_DOWN ,STD_LOW,
                                      PORT_D ,PIN7 ,PORTD_PIN7_INIT_MODE , PORT_PIN_IN, PULL_DOWN ,STD_LOW,
                                      PORT_E ,PIN0 ,PORTE_PIN0_INIT_MODE , PORT_PIN_IN, PULL_DOWN ,STD_LOW,
                                      PORT_E ,PIN1 ,PORTE_PIN1_INIT_MODE , PORT_PIN_IN, PULL_DOWN ,STD_LOW,
                                      PORT_E ,PIN2 ,PORTE_PIN2_INIT_MODE , PORT_PIN_IN, PULL_DOWN ,STD_LOW,
                                      PORT_E ,PIN3 ,PORTE_PIN3_INIT_MODE , PORT_PIN_IN, PULL_DOWN ,STD_LOW,
                                      PORT_E ,PIN4 ,PORTE_PIN4_INIT_MODE , PORT_PIN_IN, PULL_DOWN ,STD_LOW,
                                      PORT_E ,PIN5 ,PORTE_PIN5_INIT_MODE , PORT_PIN_IN, PULL_DOWN ,STD_LOW,
                                      PORT_E ,PIN0 ,PORTF_PIN0_INIT_MODE , PORT_PIN_IN, PULL_DOWN ,STD_LOW,
                                      PORT_F ,PIN1 ,PORTF_PIN1_INIT_MODE , PORT_PIN_OUT,PULL_DOWN ,STD_LOW,
                                      PORT_F ,PIN2 ,PORTF_PIN2_INIT_MODE , PORT_PIN_IN, PULL_DOWN ,STD_LOW,
                                      PORT_F ,PIN3 ,PORTF_PIN3_INIT_MODE , PORT_PIN_IN, PULL_DOWN ,STD_LOW,
                                      PORT_F ,PIN4 ,PORTF_PIN4_INIT_MODE , PORT_PIN_IN, PULL_DOWN ,STD_LOW,




};

