#include <avr/io.h>
#include <util/delay.h>
#include "I2C.h"

#define wait_for_completion while (!(TWCR & (1 << TWINT)))

#define I2C_WRITE 0
#define I2C_READ 1
#define OPT3101_ADDR 0b1011111 // WHO_AM_I should return 0x68



 void InitI2C() {
    // Wake up the OPT3101 by writing 0x00 to PWR_MGMT_1 (0x6B)
    StartI2C_Trans(OPT3101_ADDR);    
    Write(0x00);  
    Write(0x01);  // Turning on the I2C continuous reading mode  
    Write(0x00); 
    Write(0x40);  // Turning on the Force enable slave.
    StopI2C_Trans();

    // I2C prescaler and bit rate
    TWSR &= ~((1 << TWPS1) | (1 << TWPS0)); // Prescaler = 1
    TWBR = 18;                              // ~100kHz for 16MHz system clock
    TWCR = (1 << TWEN);                     // Enable TWI

    _delay_ms(100);                         // Wait for OPT3101 to stabilize  !!!!!!!!!!!!!!        FIXME
}

void StartI2C_Trans(unsigned char SLA) {
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN); // Start condition
    wait_for_completion;

    TWDR = (SLA << 1) | I2C_WRITE;
    TWCR = (1 << TWINT) | (1 << TWEN);
    wait_for_completion;
}

void StopI2C_Trans() {
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO); // Stop condition
    while (TWCR & (1 << TWSTO)); // Wait for stop to complete
}

void Write(unsigned char data) {
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    wait_for_completion;
}

void ReadDistanceData(int16_t* Left, int16_t* Center, int16_t* Right) {
    uint8_t regAddr = 0x08; // Start at Phase out register

    // Write register address to start reading from
    StartI2C_Trans(OPT3101_ADDR);
    Write(regAddr);

    // Repeated start, switch to read mode
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    wait_for_completion;

    TWDR = (OPT3101_ADDR << 1) | I2C_READ;
    TWCR = (1 << TWINT) | (1 << TWEN);
    wait_for_completion;

    // Read 6 bytes (X, Y, Z high and low)
    uint8_t data[6];
    for (int i = 0; i < 5; i++) {
        TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA); // ACK
        wait_for_completion;
        data[i] = TWDR;
    }
    TWCR = (1 << TWINT) | (1 << TWEN); // NACK on last byte
    wait_for_completion;
    data[5] = TWDR;

    StopI2C_Trans();

    // Combine high and low bytes into signed 16-bit values
    *Left = (int16_t)((data[0] << 8) | data[1]);
    *Center = (int16_t)((data[2] << 8) | data[3]);
    *Right = (int16_t)((data[4] << 8) | data[5]);
}