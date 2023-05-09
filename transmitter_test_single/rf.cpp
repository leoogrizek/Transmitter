/*
 * rf.cpp
 *
 * Created: 7. 05. 2023 16:07:11
 *  Author: Leo
 */
#include "rf.h"
#include "spi.h"



void nrf24_write_register(uint8_t reg, uint8_t value) {
	PORTB &= ~(1 << CSN); // CSN low
	spi_transfer(W_REGISTER | (0x1F & reg)); // Send write command
	spi_transfer(value); // Transfer register value
	PORTB |= (1 << CSN); // CSN high
}

void nrf24_write_registers(uint8_t reg, uint8_t *values, uint8_t len) {
	PORTB &= ~(1 << CSN); // CSN low
	spi_transfer(W_REGISTER | (0x1F & reg)); // Send write command

	for (uint8_t i = 0; i < len; i++) { // Transfer register values
		spi_transfer(values[i]);
	}

	PORTB |= (1 << CSN); // CSN high
}

uint8_t nrf24_read_register(uint8_t reg) {
	PORTB &= ~(1 << CSN); // CSN low
	spi_transfer(R_REGISTER | (0x1F & reg)); // Send read command
	uint8_t reg_value = spi_transfer(0x00); // Read register value
	PORTB |= (1 << CSN); // CSN high
	return reg_value;
}

void nrf24_read_registers(uint8_t reg, uint8_t *values, uint8_t len) {
	PORTB &= ~(1 << CSN); // CSN low
	spi_transfer(R_REGISTER | (0x1F & reg)); // Send read command

	for (uint8_t i = 0; i < len; i++) { // Read register values
		values[i]=spi_transfer(0x00);
	}

	PORTB |= (1 << CSN); // CSN high
}

void nrf24_set_channel(uint8_t channel) {
	nrf24_write_register(RF_CH, channel);
}

void nrf24_set_rx_address(uint8_t* address, uint8_t pipe, uint8_t len) {
	//sets RX address. Minimal needed for functioning is RX=TX on transmitter side equal to RX on receiver side.
	//for bidirectional communication both sides need to have matching RX and TX set.
	//address is an array of bytes containing the address
	//pipe is data pipe for which this is set
	//len is number of bytes in address, must match SETUP_AW
	
	uint8_t reg;
	switch (pipe) {
		case 0:
			reg=RX_ADDR_P0;
			break;
		case 1:
			reg=RX_ADDR_P1;
			break;
		case 2:
			reg=RX_ADDR_P2;
			break;
		case 3:
			reg=RX_ADDR_P3;
			break;
		case 4:
			reg=RX_ADDR_P4;
			break;
		case 5:
			reg=RX_ADDR_P5;
			break;
		default:
			return;					
	}
	
	nrf24_write_registers(reg, address, len);
}

void nrf24_set_tx_address(uint8_t* address, uint8_t len) {
	//sets TX address. Minimal needed for functioning is RX=TX on transmitter side equal to RX on receiver side.
	//for bidirectional communication both sides need to have matching RX and TX set.
	//address is an array of bytes containing the address
	//len is number of bytes in address, must match SETUP_AW
	
	nrf24_write_registers(TX_ADDR, address, len);
}

void nrf24_set_rx_tx_address(uint8_t* address, uint8_t pipe, uint8_t len) {
	//sets both RX and TX address. Minimal needed for functioning is RX=TX on transmitter side equal to RX on receiver side.
	//for bidirectional communication both sides need to have matching RX and TX set.
	//address is an array of bytes containing the address
	//len is number of bytes in address, must match SETUP_AW	
	
	nrf24_set_rx_address(address, pipe, len);
	nrf24_set_tx_address(address, len);
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
	_delay_us(15);
	PORTB &= ~(1 << CE);
}