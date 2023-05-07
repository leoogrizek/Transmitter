#include "nrf24l01.h"
#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 16000000UL

#define NRF24_CE_DDR DDRB
#define NRF24_CE_PORT PORTB
#define NRF24_CE_PIN 1

#define NRF24_CSN_DDR DDRB
#define NRF24_CSN_PORT PORTB
#define NRF24_CSN_PIN 0

/*start of low level functions, specific to the mcu and compiler*/

/*delay in miliseconds*/
void delay_function(uint32_t duration_ms)
{
	_delay_ms(duration_ms);
}

/*contains all SPI configuations, such as pins and control registers*/
/*SPI control: master, interrupts disabled, clock polarity low when idle, clock phase falling edge, clock up tp 1 MHz*/
void SPI_Initializer()
{
	DDRB |= (1 << DDB3) | (1 << DDB5) | (1 << DDB2);
	DDRB &= ~(1 << DDB4);

	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

/*contains all CSN and CE pins gpio configurations, including setting them as gpio outputs and turning SPI off and CE '1'*/
void pinout_Initializer()
{
	// Set CE and CSN as output
	NRF24_CE_DDR |= (1 << NRF24_CE_PIN);
	NRF24_CSN_DDR |= (1 << NRF24_CSN_PIN);
	
	// Set CE to high
	NRF24_CE_PORT |= (1 << NRF24_CE_PIN);
	
	// Set CSN to high (SPI off)
	NRF24_CSN_PORT |= (1 << NRF24_CSN_PIN);
}

/*CSN pin manipulation to high or low (SPI on or off)*/
void nrf24_SPI(uint8_t input)
{
	if (input) {
		NRF24_CSN_PORT |= (1 << NRF24_CSN_PIN);
		} else {
		NRF24_CSN_PORT &= ~(1 << NRF24_CSN_PIN);
	}
}

/*1 byte SPI shift register send and receive routine*/
uint8_t SPI_send_command(uint8_t command)
{
	SPDR = command;
	
	while (!(SPSR & (1 << SPIF))) {
		// Wait for SPI transmission to complete
	}

	return SPDR;
}

/*CE pin maniplation to high or low*/
void nrf24_CE(uint8_t input)
{
	if (input) {
		NRF24_CE_PORT |= (1 << NRF24_CE_PIN);
		} else {
		NRF24_CE_PORT &= ~(1 << NRF24_CE_PIN);
	}
}
