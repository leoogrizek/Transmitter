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



//NRF24L01+ memory map

#define CONFIG        0x00	// Configuration Register
#define EN_AA         0x01	// Enable 'Auto Acknowledgment' Function
#define EN_RXADDR     0x02	// Enable RX Addresses
#define SETUP_AW      0x03	// Setup of Address Widths
#define SETUP_RETR    0x04	// Setup of Automatic Retransmission
#define RF_CH         0x05	// RF Channel
#define RF_SETUP      0x06	// RF Setup Register
#define STATUS        0x07	// Status Register
#define TX_ADDR       0x10	// Transmit Address
#define RX_PW_P0      0x11	// Number of bytes in TX payload
#define RX_ADDR_P0    0x0A	// Receive Address Data Pipe 0
#define RX_ADDR_P1    0x0B	// Receive Address Data Pipe 1
#define RX_ADDR_P2    0x0C	// Receive Address Data Pipe 2
#define RX_ADDR_P3    0x0D	// Receive Address Data Pipe 3
#define RX_ADDR_P4    0x0E	// Receive Address Data Pipe 4
#define RX_ADDR_P5    0x0F	// Receive Address Data Pipe 5
#define RX_PW_P1      0x12	// Number of bytes in RX payload in data pipe 1
#define RX_PW_P2      0x13	// Number of bytes in RX payload in data pipe 2
#define RX_PW_P3      0x14	// Number of bytes in RX payload in data pipe 3
#define RX_PW_P4      0x15	// Number of bytes in RX payload in data pipe 4
#define RX_PW_P5      0x16	// Number of bytes in RX payload in data pipe 5
#define FIFO_STATUS   0x17	// FIFO Status Register



//NRF24L01+ SPI commands

#define REGISTER_MASK 0x1F  // Mask for sending SPI commands to registers

#define R_REGISTER    0x00  // Read command and status registers, 0b000XXXXX last 5 bits=5 bit register map address
#define W_REGISTER    0x20  // Write command and status registers, 0b001XXXXX last 5 bits=5 bit register map address
#define R_RX_PAYLOAD  0x61  // Read RX payload for data pipe 0 to 5
#define W_TX_PAYLOAD  0xA0  // Write TX payload
#define FLUSH_TX      0xE1  // Flush TX FIFO
#define FLUSH_RX      0xE2  // Flush RX FIFO
#define REUSE_TX_PL   0xE3  // Reuse last transmitted payload



//functions

void nrf24_write_register(uint8_t reg, uint8_t value);
void nrf24_write_registers(uint8_t reg, uint8_t *values, uint8_t len);
void nrf24_init();
void nrf24_transmit_byte(uint8_t data);
void nrf24_set_rx_address(uint8_t *address, uint8_t pipe, uint8_t len);
void nrf24_set_tx_address(uint8_t *address, uint8_t len);
void nrf24_set_rx_tx_address(uint8_t *address, uint8_t pipe, uint8_t len);
uint8_t nrf24_read_register(uint8_t reg);
void nrf24_read_registers(uint8_t reg, uint8_t *values, uint8_t len);
void nrf24_power_up_tx();
void nrf24_send_cmd(uint8_t cmd);
uint8_t nrf24_transmit(uint8_t *data, uint8_t len);
void nrf24_set_tx_mode(uint8_t *address, uint8_t channel);
void nrf24_set_rx_mode(uint8_t *address, uint8_t channel);


#endif /* RF_H_ */