#include "lcd.h"
#include "ultrasonic.h"
#include <avr/io.h>
#define F_CPU 8000000UL

int main (void){
	SREG|=(1<<7);
	LCD_init();
	Ultrasonic_init();
	LCD_displayString("Distance = ");
	uint16 distance;
	while (1){
		distance =Ultrasonic_readDistance();
		LCD_intgerToString(distance);
		if(distance < 100){
		LCD_displayCharacter(' ');
		}
		LCD_moveCursor(0,11);


	}
}
