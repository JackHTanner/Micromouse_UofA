
#include "TWI.h"
#include <stdint.h>


#define F_CPU         16000000UL
#define SCL_CLOCK     100000L


void TWI_Init() {
    TWSR = 0;
    TWBR = ((F_CPU / SCL_CLOCK) - 16) / 2;
    TWCR = (1<<TWEN);
  }
  
  void TWI_Start() {
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
    while (!(TWCR & (1<<TWINT)));
  }
  
  void TWI_Stop() {
    TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
    while (TWCR & (1<<TWSTO));
  }
  
  void TWI_Write(uint8_t b) {
    TWDR = b;
    TWCR = (1<<TWINT)|(1<<TWEN);
    while (!(TWCR & (1<<TWINT)));
  }
  
  uint8_t TWI_ReadACK() {
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
    while (!(TWCR & (1<<TWINT)));
    return TWDR;
  }
  
  uint8_t TWI_ReadNACK() {
    TWCR = (1<<TWINT)|(1<<TWEN);
    while (!(TWCR & (1<<TWINT)));
    return TWDR;
  }
  