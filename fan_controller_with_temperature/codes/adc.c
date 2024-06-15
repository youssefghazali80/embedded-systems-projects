/*
 * adc.c
 *
 *  Created on: Feb 20, 2024
 *      Author: youssef
 */


#include "adc.h"
#include <avr/io.h>
#include "common_macros.h"



void ADC_init(const ADC_ConfigType * Config_Ptr){

	/*
	 * ADLAR =0     RIGHT ADJUSTED MODE
	 * MUX 4:0 =00000   CHOOSE CHANNEL 0 AS INITIALIZATION
	 * REFS1 AND REFS0 WILL BE SETTED BY THE REFERENCE VOLT CHOSSEN
	 */

	ADMUX |= (Config_Ptr->ref_volt);


	ADCSRA =(1<<ADEN)|(Config_Ptr->prescaler);

}




uint16 ADC_readChannel(uint8 channel_num){
	channel_num &=0x07;  // channel number should be between 0 and 7
    ADMUX &=0xE0;    //clear MUX 4:00
    ADMUX |= channel_num; // choose the channel according to the input to the user
    SET_BIT(ADCSRA,ADSC);  // set the start conversion bit
    while(BIT_IS_SET(ADCSRA,ADSC));
    SET_BIT(ADCSRA,ADIF); //clear the bit by writing it to 1
    return ADC;


}
