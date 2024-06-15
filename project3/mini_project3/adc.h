/*
 * adc.h
 *
 *  Created on: Feb 20, 2024
 *      Author: youssef
 */

#ifndef ADC_H_
#define ADC_H_
#include "std_types.h"
/********************************   DEFINITIONS   ****************************************/

#define ADC_MAXIMUM_VALUE    1023
#define ADC_REF_VOLT_VALUE   2.56



/*********************************** TYPES DECLARATIONS *********************************/

//  Enum to describe the options of v_reference in the ADC module

typedef enum {
	AREF,AVCC=0x40,INTERNAL=0xC0

}ADC_ReferenceVolatge;

//      enum to describe the prescaler the values describe the value of (ADPS0,ADPS1,ADPS2)
typedef enum {
	TWO,FOUR=2,EIGHT,SIXTEEN,THIRTY_TWO,SIXTY_FOUR,HUNDRED_TWENTY_EIGHT
} ADC_Prescaler;




typedef struct{
 ADC_ReferenceVolatge ref_volt;
 ADC_Prescaler prescaler;
}ADC_ConfigType;



 /********************************** FUNCTIONS DECLARATIONS *************************************/




void ADC_init(const ADC_ConfigType * Config_Ptr);




uint16 ADC_readChannel(uint8 channel_num);


#endif /* ADC_H_ */
