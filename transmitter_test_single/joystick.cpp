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
	
	// Reference voltage: AVCC, right adjust the ADC value (10 bit precision)
	ADMUX = (1<<REFS0);  // AVCC with external capacitor at AREF pin

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

	return (ADC>>2);  // Shift the 10-bit ADC value to get an 8-bit result
}

void read_joystick_values(uint8_t* joystick_values)
{
	//255-value is due to physical orientation of joysticks and depends on hardware implementation
	
	joystick_values[0] = 255-ADC_read(0);  // Read from channel 0 (pin A0)
	joystick_values[1] = 255-ADC_read(1);  // Read from channel 1 (pin A1)
	joystick_values[2] = 255-ADC_read(2);  // Read from channel 2 (pin A2)
	joystick_values[3] = ADC_read(3);  // Read from channel 3 (pin A3)
}
