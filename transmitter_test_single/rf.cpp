/*
 * rf.cpp
 *
 * Created: 7. 05. 2023 16:07:11
 *  Author: Leo
 */
#include "rf.h"

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

void nrf24_write_register(uint8_t reg, uint8_t value) {
	PORTB &= ~(1 << CSN); // Pull CSN low
	spi_transfer(reg);
	spi_transfer(value);
	PORTB |= (1 << CSN); // Pull CSN high
}

void nrf24_write_registers(uint8_t reg, uint8_t *values, uint8_t len) {
	PORTB &= ~(1 << CSN); // CSN low
	spi_transfer(0x20 | (0x1F & reg)); // Write command

	for (uint8_t i = 0; i < len; i++) {
		spi_transfer(values[i]);
	}

	PORTB |= (1 << CSN); // CSN high
}


void nrf24_init() {
	//setup SPI
	spi_init();
	
	// Set CE as output
	DDRB |= (1 << CE);

	// Set CE low
	PORTB &= ~(1 << CE);

	// Set CSN as output
	DDRB |= (1 << CSN);

	// Set CSN high
	PORTB &= ~(1 << CSN);
	
	_delay_ms(5); //wait for power on reset
	
	
}


void nrf24_transmit_byte(uint8_t data) {
	PORTB &= ~(1 << CSN); // Pull CSN low
	spi_transfer(0xA0); // Write payload command
	spi_transfer(data);
	PORTB |= (1 << CSN); // Pull CSN high

	// Pulse CE high to start transmission
	PORTB |= (1 << CE);
	_delay_us(10);
	PORTB &= ~(1 << CE);
}