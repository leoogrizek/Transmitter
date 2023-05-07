#include <avr/io.h>

#define F_CPU 16000000UL
#include <util/delay.h>

#include "rf.h"


void nrf24_setup() {
	nrf24_init();
	
	nrf24_write_register(0x00, 0x0A); // Set to TX mode, 1-byte CRC enabled
	nrf24_write_register(0x01, 0x01); // Enable auto-acknowledgment for pipe 0
	nrf24_write_register(0x05, 0x01); // Set RF channel to 1
	nrf24_write_register(0x06, 0x06); // Set data rate to 1 Mbps and output power to -18 dBm
	nrf24_write_register(0x11, 0x01); // Set payload width for pipe 0 to 1 byte
	
	// Set TX address to 0xE8E8F0F0E1
	uint8_t tx_address[5] = {0xE8, 0xE8, 0xF0, 0xF0, 0xE1};
	nrf24_write_registers(0x10, tx_address, 5);
	
	// Set pipe 0 RX address to 0xE8E8F0F0E1
	uint8_t rx_address[5] = {0xE8, 0xE8, 0xF0, 0xF0, 0xE1};
	nrf24_write_registers(0x0A, rx_address, 5);
}

int main(void) {
	nrf24_setup();
	
	uint8_t data = 43; // Example data
	
	while (1) {
		nrf24_transmit_byte(data);
		_delay_ms(1000); // Send data every second
	}
	
	return 0;
}
