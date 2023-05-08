/*
 * rf.h
 *
 * Created: 7. 05. 2023 16:06:12
 *  Author: Leo
 */ 

#include <avr/io.h>

#define F_CPU 16000000UL
#include <util/delay.h>

#ifndef RF_H_
#define RF_H_

// NRF24L01+ pins
#define CE PORTB1
#define CSN PORTB2
#define MOSI PORTB3
#define MISO PORTB4
#define SCK PORTB5


//NRF24L01+ bit mnemonics

#define CONFIG      0x00	// Configuration Register
#define EN_AA       0x01	// Enable 'Auto Acknowledgment' Function
#define EN_RXADDR   0x02	// Enable RX Addresses
#define SETUP_AW    0x03	// Setup of Address Widths
#define SETUP_RETR  0x04	// Setup of Automatic Retransmission
#define RF_CH       0x05	// RF Channel
#define RF_SETUP    0x06	// RF Setup Register
#define STATUS      0x07	// Status Register
#define TX_ADDR     0x10	// Transmit Address
#define RX_PW_P0    0x11	// Number of bytes in TX payload
#define RX_ADDR_P0  0x0A	// Receive Address Data Pipe 0
#define RX_ADDR_P1  0x0B	// Receive Address Data Pipe 1
#define RX_ADDR_P2  0x0C	// Receive Address Data Pipe 2
#define RX_ADDR_P3  0x0D	// Receive Address Data Pipe 3
#define RX_ADDR_P4  0x0E	// Receive Address Data Pipe 4
#define RX_ADDR_P5  0x0F	// Receive Address Data Pipe 5
#define RX_PW_P1    0x12	// Number of bytes in RX payload in data pipe 1
#define RX_PW_P2    0x13	// Number of bytes in RX payload in data pipe 2
#define RX_PW_P3    0x14	// Number of bytes in RX payload in data pipe 3
#define RX_PW_P4    0x15	// Number of bytes in RX payload in data pipe 4
#define RX_PW_P5    0x16	// Number of bytes in RX payload in data pipe 5
#define FIFO_STATUS 0x17	// FIFO Status Register



//functions

void spi_init();
uint8_t spi_transfer(uint8_t data);
void nrf24_write_register(uint8_t reg, uint8_t value);
void nrf24_write_registers(uint8_t reg, uint8_t *values, uint8_t len);
void nrf24_init();
void nrf24_transmit_byte(uint8_t data);

#endif /* RF_H_ */