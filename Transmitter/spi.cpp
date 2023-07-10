/*
 * spi.cpp
 *
 * Created: 9. 05. 2023 18:23:22
 *  Author: Leo
 */ 

#include <avr/io.h>
#include "spi.h"

void spi_init() {
	// Set MOSI, SCK, and CSN as output
	DDRB |= (1 << MOSI) | (1 << SCK) | (1 << CSN);

	// Set MISO as input
	DDRB &= ~(1 << MISO);

	// Enable SPI, set as Master, with SCK = CLK/16
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

uint8_t spi_transfer(uint8_t data) {
	// Load data into the SPI data register
	SPDR = data;

	// Wait for transmission to complete
	while (!(SPSR & (1 << SPIF)));

	// Return the received data
	return SPDR;
}