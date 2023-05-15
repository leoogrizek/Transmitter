/*
 * rf.cpp
 *
 * Created: 7. 05. 2023 16:07:11
 *  Author: Leo
 */
#include "rf.h"
#include "spi.h"
#include "uart.h"



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

void nrf24_send_cmd(uint8_t cmd) {
		PORTB &= ~(1 << CSN); // CSN low
		spi_transfer(cmd); // Send cmd
		PORTB |= (1 << CSN); // CSN high
}

void nrf24_set_channel(uint8_t channel) {
	nrf24_write_register(RF_CH, channel);
}

void nrf24_set_rx_address(uint8_t *address, uint8_t pipe, uint8_t len) {
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

void nrf24_set_tx_address(uint8_t *address, uint8_t len) {
	//sets TX address. Minimal needed for functioning is RX=TX on transmitter side equal to RX on receiver side.
	//for bidirectional communication both sides need to have matching RX and TX set.
	//address is an array of bytes containing the address
	//len is number of bytes in address, must match SETUP_AW
	
	
	nrf24_write_registers(TX_ADDR, address, len);
	nrf24_write_registers(RX_ADDR_P0, address, len); //RX_ADDR_P0 must be set to the sending address for auto ack to work
}

void nrf24_set_rx_tx_address(uint8_t *address, uint8_t pipe, uint8_t len) {
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
	
	// Set CSN as output
	DDRB |= (1 << CSN);

	// Set CE low
	PORTB &= ~(1 << CE);

	// Set CSN high
	PORTB |= (1 << CSN);
	
	_delay_ms(5); // Wait for power on reset
	
	nrf24_write_register(CONFIG, 0x00); // Set config - power down, no crc, reflect interrupts on irq pin (unused)
	nrf24_write_register(EN_AA, 0x00); // Disable auto acknowledgement
	nrf24_write_register(EN_RXADDR, 0x00); // Not enabling any data pipes right now
	nrf24_write_register(SETUP_AW, 0x03); // Set address width to 5 bytes
	nrf24_write_register(SETUP_RETR, 0x00); // Disable retransmission
	nrf24_write_register(RF_CH, 0x00); // Set channel to 0 for now
	nrf24_write_register(RF_SETUP, 0x0E); // Power = 0dB, data rate = 2Mbps
	
	// Set CE high
	PORTB |= (1 << CE);
		
	// Set CSN low
	PORTB &= ~(1 << CSN);
}

void nrf24_set_tx_mode(uint8_t *address, uint8_t channel) {
	
	PORTB &= ~(1 << CE); // Set CE low
	
	PORTB |= (1 << CSN); // Set CSN high
	
	nrf24_write_register(RF_CH, channel); // Select channel
	
	nrf24_write_registers(TX_ADDR, address, 5); // Set TX address
	
	// Power up the device
	uint8_t config = nrf24_read_register(CONFIG);
	config |= (1<<1);
	nrf24_write_register(CONFIG, config);
	
	_delay_ms(2); // Wait for the device to settle
	
	PORTB |= (1 << CE); // Set CE high
		
	
	PORTB &= ~(1 << CSN); // Set CSN low
}

void nrf24_set_rx_mode(uint8_t *address, uint8_t channel) {
	
	PORTB &= ~(1 << CE); // Set CE low
	
	PORTB |= (1 << CSN); // Set CSN high
	
	nrf24_write_register(RF_CH, channel); // Select channel
	
	// Enable data pipe 1
	uint8_t en_rxaddr = nrf24_read_register(EN_RXADDR);
	en_rxaddr |= 0x02;	
	nrf24_write_register(EN_RXADDR, en_rxaddr); 
	
	nrf24_write_registers(RX_ADDR_P1, address, 5); // Set RX address
	
	nrf24_write_register(RX_PW_P1, 32); //set payload size for pipe 1 to 32 bytes
	
	// Power up the device
	uint8_t config = nrf24_read_register(CONFIG);
	config |= (1<<0) | (1<<1);
	nrf24_write_register(CONFIG, config);
	
	_delay_ms(2); // Wait for the device to settle
	
	PORTB |= (1 << CE); // Set CE high	
	
	PORTB &= ~(1 << CSN); // Set CSN low
}

uint8_t nrf24_transmit(uint8_t *data, uint8_t len) {

	
	PORTB &= ~(1 << CSN); // Pull CSN low
	
	
	
	spi_transfer(W_TX_PAYLOAD); // Write payload command
	
	for (uint8_t i = 0; i < len; i++) { // Send the payload
		spi_transfer(data[i]);
	}
	
	PORTB |= (1 << CSN); // Pull CSN high
	

	
	// Pulse CE high to start transmission
	PORTB |= (1 << CE);
	_delay_us(20);
	PORTB &= ~(1 << CE);

	
	_delay_ms(1);
	
	uint8_t fifostatus = nrf24_read_register(FIFO_STATUS);
	uart_println("fifo");
	uart_print_binary(fifostatus);
	uart_newline();
	
	if ((fifostatus&(1<<4))&&(!(fifostatus&(1<<3)))) {
		nrf24_send_cmd(FLUSH_TX);
		uart_println("test");
		uart_newline();
		return 1;
	}
	return 0;


}

void nrf24_power_up_tx() {
	nrf24_write_register(CONFIG, nrf24_read_register(CONFIG) | 0x0A);
}

uint8_t nrf24_data_available(uint8_t pipe) {
	
	uint8_t status = nrf24_read_register(STATUS); // Get data received status
	
	if((status&(1<<6))&&(status&(pipe<<1))) { // Check if smth was received
		nrf24_write_register(STATUS, (1<<6));
		return 1;
	}
	return 0;
}

void nrf24_receive(uint8_t *buffer) {
	PORTB &= ~(1 << CSN); // Pull CSN low
		
	spi_transfer(W_TX_PAYLOAD); // Write payload command
		
	for (uint8_t i = 0; i < 32; i++) { // Send the payload
		buffer[i]=spi_transfer(0x00);
	}
		
	PORTB |= (1 << CSN); // Pull CSN high
		
	_delay_ms(1);
		
	nrf24_send_cmd(FLUSH_RX); // Flush rx fifo
}