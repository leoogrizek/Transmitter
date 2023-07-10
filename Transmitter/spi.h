/*
 * spi.h
 *
 * Created: 9. 05. 2023 18:20:14
 *  Author: Leo
 */ 


#ifndef SPI_H_
#define SPI_H_

//SPI pins
#define CSN PORTB2
#define MOSI PORTB3
#define MISO PORTB4
#define SCK PORTB5

void spi_init(); //	Initializes SPI
uint8_t spi_transfer(uint8_t data);	// Sends one byte of data over SPI and returns the received byte

#endif /* SPI_H_ */