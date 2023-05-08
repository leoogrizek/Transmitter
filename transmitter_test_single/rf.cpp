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

void Nrf24l::configRegister(uint8_t reg, uint8_t value)
// Clocks only one byte into the given MiRF register
{
	csnLow();
	spi->transfer(W_REGISTER | (REGISTER_MASK & reg));
	spi->transfer(value);
	csnHi();
}

void nrf24_write_registers(uint8_t reg, uint8_t *values, uint8_t len) {
	PORTB &= ~(1 << CSN); // CSN low
	spi_transfer(0x20 | (0x1F & reg)); // Write command

	for (uint8_t i = 0; i < len; i++) {
		spi_transfer(values[i]);
	}

	PORTB |= (1 << CSN); // CSN high
}

nrf24_set_channel(uint8_t channel) {
	nrf24_write_register(RF_CH, channel);
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
	
	nrf24_set_channel(2);
	nrf24_write_register(RF_SETUP, 0x06); //1 Mbps data rate, 0dBm attenuation
	nrf24_write_register(EN_AA, 0x01); //Enable auto acknowledgement for data pipe 0
	
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