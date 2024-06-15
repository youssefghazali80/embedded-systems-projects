/*
 * main.c
 *
 *  Created on: Feb 21, 2024
 *      Author: youssef
 */
#include    "adc.h"
#include    "lm35_sensor.h"
#include    "pwm.h"
#include    "motor.h"
#include    "lcd.h"
#include    "gpio.h"
#include    "std_types.h"



int main (void){

	/*
	 * initialize ADC peripheral with prescaler = 8 and Vref =2.56 internal
	 */
	ADC_ConfigType configuration ;
	configuration.prescaler=EIGHT;
	configuration.ref_volt=INTERNAL;
	ADC_init(&configuration);


	// initialize the LCD
	LCD_init();


	//initialize the motor
	DcMotor_init();

    uint8 temp;
	// initialize the sensor
	while(1){
		temp = LM35_getTemperature();

		if (temp < 30){
			// stop the motor and display the status of the fan

			DcMotor_Rotate(STOP,0);
			LCD_displayStringRowColumn(0,2,"FAN IS OFF");



		}else if (temp>=30 && temp<60){

			DcMotor_Rotate(CW,25);
			LCD_displayStringRowColumn(0,2,"FAN IS ON ");



		}else if (temp>=60 && temp<90){

			DcMotor_Rotate(CW,50);
			LCD_displayStringRowColumn(0,2,"FAN IS ON ");
		}
		else if (temp>=90 &&  temp<120){

			DcMotor_Rotate(CW,75);
			LCD_displayStringRowColumn(0,2,"FAN IS ON ");
			}
		else  if (temp>=120){

			DcMotor_Rotate(CW,100);
			LCD_displayStringRowColumn(0,2,"FAN IS ON ");

		}
		    if (temp<100&&temp>10){
			LCD_displayStringRowColumn(1,2,"Temp = ");
		    }else if(temp<10){
		    LCD_displayStringRowColumn(1,2,"Temp =  ");
		    }
		    else{
		    	LCD_displayStringRowColumn(1,2,"Temp =");
		    }
			LCD_intgerToString(temp);
			LCD_displayCharacter('C');

	}

}



