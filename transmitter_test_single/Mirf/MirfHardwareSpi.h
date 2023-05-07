#include <avr/io.h>
#include "MirfSpiDriver.h"

class MirfHardwareSpiDriver : public MirfSpiDriver {
public:
    void begin() {
        // Set MOSI, SCK, and SS as output, MISO as input
        DDRB |= (1 << DDB3) | (1 << DDB5) | (1 << DDB2);
        DDRB &= ~(1 << DDB4);

        // Enable SPI, set as master, and set the clock rate to fosc/16
        SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
    }

    uint8_t transfer(uint8_t data) {
        // Load data into the SPI data register
        SPDR = data;

        // Wait for the transmission to complete
        while (!(SPSR & (1 << SPIF)));

        // Return the received data
        return SPDR;
    }
};

extern MirfHardwareSpiDriver MirfHardwareSpi;
