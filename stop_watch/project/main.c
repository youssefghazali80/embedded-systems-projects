/*
 * main.c
 *
 *  Created on: Jan 29, 2024
 *      Author: youssef
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
#define F_CPU 1000000UL
//define the pins in proteus to make the code functionality clearer
#define SEC1 PA0
#define SEC2 PA1
#define MIN1 PA2
#define MIN2 PA3
#define HOUR1 PA4
#define HOUR2 PA5


// initializing global variables to store the time
unsigned char seconds = 0;
unsigned char minutes = 0;
unsigned char hours = 0;


void INT0_init(void){
	// initializing external interrupt one with falling edge
	DDRD &= ~(1<<PD2);   // make pin D2 input
	PORTD |= (1<<PD2);  // activate the internal pull up resistor
	GICR |= (1<<INT0);  // activate external interrupt 0;
	MCUCR |=(1<<ISC01); // the falling edge of pd2 generates the interrupt request

}
void TIMER1_init(void){
	TIMSK |=(1<<OCIE1A); // ACTIAVTE THE TIMER1 INTERRUPT
	/*
	 ACTIVATE THE COMPARE MODE AND PRESCALER 64
	 frequency = 1000000/64 =15625hz
	 so to count one second we need timer 1 to count till 15625
	 */
	TCCR1B |=(1<<WGM12)|(1<<CS10)|(1<<CS11);
	OCR1A = 15625;    // set the compare register to 15625

}
void INT1_init(void){
	DDRD &=~(1<<PD3);   //SET THE PIND3 AS INPUT PIN
	MCUCR |=(1<<ISC11)|(ISC10); // THE RISING EDGE OF PD3 GENERATE INTERRUPT REQUEST
	GICR  |=(1<<INT1);  //enable EXTERNAL INTERRUPT 1

}
void INT2_init(void){
	DDRB &=~(1<<PB2);   //SET PIN PBS AS INPUT PIN
	PORTB |=(1<<PB2);   // ACTIVAYE INTERNAL PULL UP RESISITOR
	MCUCSR &=~(1<<ISC2); // FALLING EGDE OF PIN PB2 GENERATE INTERRUPT REQUEST
	GICR |=(1<<INT2);
}




int main (void){
	SREG |=(1<<7);    // ACTIVATE GENERAL INTERRUPT FOR THE PROCESSOR
	DDRA |= 0x3F;     // SET THE FIRST 6 BITS IN PORTA AS OUTPUT PINS
	DDRC |= 0x0F;     // SET THE FIRST FOUR BITS IN PORTC AS OUTPUT PINS
	INT0_init(); // ACTIVATE EXTERNAL INTERRUPT 0
	INT1_init();  //ACTIVATE EXTERNAL INTERRUPT 1
	INT2_init(); //  ACTIVATE EXTERNAL INTERRUPT 2
	TIMER1_init(); //ACTIVATE TIMER TO COUNT EACH ONE SECOND AND UPDATE THE TIME VARIABLES





	while(1){
		/* looping on each seven segment and displaying
		 * digit corresponding to it
		 */
		PORTA =(1<<SEC1);
		PORTC=seconds%10;
		_delay_ms(3);
		PORTA = (1<<SEC2);
		PORTC = seconds/10;
		_delay_ms(3);
		PORTA = (1<<MIN1);
		PORTC=minutes%10;
		_delay_ms(3);
		PORTA = (1<<MIN2);
		PORTC=minutes/10;
		_delay_ms(3);
		PORTA = (1<<HOUR1);
		PORTC=hours%10;
		_delay_ms(3);
		PORTA = (1<<HOUR2);
		PORTC = hours/10;
		_delay_ms(3);

	}


}



ISR (INT0_vect){
	seconds =0;
	minutes =0;
	hours=0;
}
ISR (TIMER1_COMPA_vect){
	if (seconds==59){
		seconds=0;
		if (minutes==59){
			hours++;
			minutes=0;

		}
		else{
			minutes++;
		}
	}
	else{
		seconds++;

	}
}
ISR(INT1_vect){
	TCCR1B &=~(1<<CS10);
	TCCR1B &=~(1<<CS11);
	TCCR1B &=~(1<<CS12);
}
ISR(INT2_vect){
	TCCR1B |=(1<<CS10)|(1<<CS11);
}
