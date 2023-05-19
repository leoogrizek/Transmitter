/*
 * joystick.cpp
 *
 * Created: 15. 05. 2023 23:24:38
 *  Author: Leo
 */ 

#include <avr/io.h>

void ADC_init()
{
	// Set pins A0-A3 as inputs
	DDRC &= ~((1<<DDC0)|(1<<DDC1)|(1<<DDC2)|(1<<DDC3));
	
	// Reference voltage: AREF, left adjust the ADC value (8 bit precision)
	ADMUX = (0<<REFS1)|(0<<REFS0)|(1<<ADLAR)|(0<<MUX3)|(0<<MUX2)|(0<<MUX1)|(0<<MUX0);

	// Enable ADC and set prescaler to 64x (16MHz / 64 = 250kHz ADC clock)
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1);
}

uint8_t ADC_read(uint8_t ch)
{
	// Select ADC channel
	ch &= 0b00000111;  // AND operation with 7
	ADMUX = (ADMUX & 0xF8)|ch;  // Clears the bottom 3 bits before ORing

	// Start single conversion
	ADCSRA |= (1<<ADSC);

	// Wait for conversion to complete
	while(!(ADCSRA & (1<<ADIF)));

	// Clear ADIF by writing one to it
	ADCSRA|=(1<<ADIF);

	return (ADCH);
}

void read_joystick_values(uint8_t* joystick_values)
{
	joystick_values[0] = ADC_read(0);  // Read from channel 0 (pin A0)
	joystick_values[1] = ADC_read(1);  // Read from channel 1 (pin A1)
	joystick_values[2] = ADC_read(2);  // Read from channel 2 (pin A2)
	joystick_values[3] = ADC_read(3);  // Read from channel 3 (pin A3)
}