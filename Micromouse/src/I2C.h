#ifndef I2C_H
#define I2C_H

void InitI2C();

void StartI2C_Trans(unsigned char SLA); 

void StopI2C_Trans();

void Write(unsigned char data);

void Read_from(unsigned char SLA, unsigned char MEMADDRESS);

unsigned char Read_data();

void ReadAccelData(int16_t* x, int16_t* y, int16_t* z);

#endif