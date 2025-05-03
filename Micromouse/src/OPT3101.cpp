/*
Copyright (c) 2020 Pololu Corporation (www.pololu.com)

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/

/*
***********************************************************
By the terms of the LICENSE, this software is used lawfully 
in order to interface with the Pololu 3-Channel Wide FOV 
Time-of-Flight Distance Sensor Using OPT3101.
***********************************************************
*/

// ECE 372A: Group 3 Section C 
// DESCRIPTION: This is a library that helps interface with the
// distance sensor.

#include "OPT3101.h"
#include <Wire.h>

static const uint32_t reg80Default = 0x4e1e;

// Resets the OPT3101 and then waits for the INIT_LOAD_DONE bit to be 1.
void OPT3101::resetAndWait()
{
  timingGeneratorEnabled = false;

  // Set SOFTWARE_RESET to 1, but don't use writeReg, because the OPT3101 will
  // stop acknowledging after it receives the first register value byte.
  Wire.beginTransmission(address);
  Wire.write(0);
  Wire.write(1);
  lastError = Wire.endTransmission();
  if (lastError) { return; }

  // Give the sensor some time to get ready.
  delay(5);

  // Wait for INIT_LOAD_DONE to be set, indicating that the OPT3101 is done
  // loading settings from its EEPROM.
  while (!(readReg(3) & (1 << 8)))
  {
    if (getLastError()) { return; }
    delay(1);
  }
}

// Writes the given value to the an OPT3101 register. 
// The OPT3101 registers are 24-bit, so the upper 8 bits of value are ignored.
void OPT3101::writeReg(uint8_t reg, uint32_t value)
{
  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.write((uint8_t)(value >> 0));
  Wire.write((uint8_t)(value >> 8));
  Wire.write((uint8_t)(value >> 16));
  lastError = Wire.endTransmission();
}

// Reads an OPT3101 register and returns the value read.
uint32_t OPT3101::readReg(uint8_t reg)
{
  Wire.beginTransmission(address);
  Wire.write(reg);
  lastError = Wire.endTransmission(false); // no stop (repeated start)
  if (lastError)
  {
    return 0;
  }

// Reads and returns the byte count at a specific address.
uint8_t byteCount = Wire.requestFrom(address, (uint8_t)3);
  if (byteCount != 3)
  {
    lastError = 50;
    return 0;
  }

  lastError = 0;
  uint32_t value = Wire.read();
  value |= (uint16_t)Wire.read() << 8;
  value |= (uint32_t)Wire.read() << 16;
  return value;
}

// Initializes and configures the sensor. 
// This includes calling resetAndWait() and configureDefault().
void OPT3101::init()
{
  resetAndWait();
  if (getLastError()) { return; }
  configureDefault();
}

// Configures the sensor with some default settings. 
// This includes calling setMonoshotMode() and setFrameTiming(512).
void OPT3101::configureDefault()
{
  writeReg(0x89, 7000);      // TG_OVL_WINDOW_START = 7000
  if (getLastError()) { return; }
  writeReg(0x6e, 0x0a0000);  // EN_TEMP_CONV = 1
  if (getLastError()) { return; }
  writeReg(0x50, 0x200101);  // CLIP_MODE_FC = 1
                             // CLIP_MODE_TEMP = 0
                             // CLIP_MODE_OFFSET = 0
  if (getLastError()) { return; }

  // IQ_READ_DATA_SEL = 2: This lets us read "raw" IQ values later.
  uint32_t reg2e = readReg(0x2e);
  if (getLastError()) { return; }
  reg2e = (reg2e & ~((uint32_t)7 << 9)) | (2 << 9);
  writeReg(0x2e, reg2e);
  if (getLastError()) { return; }

  setMonoshotMode();
  if (getLastError()) { return; }
  setFrameTiming(512);
}

/* Configures the sensor to use the specified channel. 
This determines which set of infrared LEDs will be turned on while taking a reading. 
Valid arguments are:

 - 0 for TX0
 - 1 for TX1
 - 2 for TX2
 - OPT3101ChannelAutoSwitch to automatically cycle through all channels
 */
void OPT3101::setChannel(uint8_t channel)
{
  if (channel > 2) { channel = OPT3101ChannelAutoSwitch; }

  uint32_t reg2a = readReg(0x2a);
  if (getLastError()) { return; }

  if (channel == OPT3101ChannelAutoSwitch)
  {
    reg2a |= (1 << 0);   // EN_TX_SWITCH = 1
  }
  else
  {
    reg2a &= ~((uint32_t)1 << 0);  // EN_TX_SWITCH = 0
    reg2a = reg2a & ~((uint32_t)3 << 1) | (channel & 3) << 1;
  }

  writeReg(0x2a, reg2a);
}

// This convenience method configures the sensor to use the channel that 
// comes numerically after the last channel that was sampled 
// (according to the channelUsed member).
void OPT3101::nextChannel()
{
  uint8_t nextChannel = channelUsed + 1;
  if (nextChannel > 2) { nextChannel = 0; }
  setChannel(nextChannel);
}

/*
Sets the OPT3101 to use the specified LED brightness. Valid arguments are:

- OPT3101Brightness::Low (0)
- OPT3101Brightness::High (1)
- OPT3101Brightness::Adaptive (255) Low brightness mode only works well for nearby objects.
  High brightness mode works for a wide range of distances, but objects that are too reflective 
  or too close can cause the sensor to saturate, meaning that it fails to measure a distance. 
  Adaptive mode automatically uses low or high brightness depending on how much reflected light 
  the sensor is seeing.
*/
void OPT3101::setBrightness(OPT3101Brightness br)
{
  uint32_t reg2a = readReg(0x2a);
  if (getLastError()) { return; }

  if (br == OPT3101Brightness::Adaptive)
  {
    reg2a |= (uint16_t)1 << 15;  // EN_ADAPTIVE_HDR = 1
  }
  else
  {
    // EN_ADAPTIVE_HDR = 0
    // SEL_HDR_MODE = hdr
    reg2a = reg2a & ~(uint32_t)0x18000 | ((uint32_t)br & 1) << 16;
  }

  writeReg(0x2a, reg2a);
}

// Configures the sensor to use monoshot mode, which means it only turns on the LEDs and
// takes measurements when we request it to. This function is called by configureDefault() 
// which is called by init() so most applications will not need to use it directly.
void OPT3101::setMonoshotMode()
{
  // MONOSHOT_FZ_CLKCNT = default
  // MONOSHOT_NUMFRAME = 1
  // MONOSHOT_MODE = 3
  writeReg(0x27, 0x26ac07);
  if (getLastError()) { return; }

  // DIS_GLB_PD_OSC = 1
  // DIS_GLB_PD_AMB_DAC = 1
  // DIS_GLB_PD_REFSYS = 1
  // (other fields default)
  writeReg(0x76, 0x000121);
  if (getLastError()) { return; }

  // POWERUP_DELAY = 95
  writeReg(0x26, (uint32_t)95 << 10 | 0xF);
}

// Configures the sensor to use continuous mode, where it automatically takes readings as 
// long as the timing generator is enabled.
void OPT3101::setContinuousMode()
{
  // MONOSHOT_FZ_CLKCNT = default
  // MONOSHOT_NUMFRAME = 1
  // MONOSHOT_MODE = 0
  writeReg(0x27, 0x26ac04);
}

/*
Configures the OPT3101 to average the specified number of sub-frames. Each frame of the OPT3101 
(i.e. sample) consists of a certain number of sub-frames which take 0.25 ms each. The OPT3101 
averages the results from these subframes together. Choosing a high number here makes the samples 
take longer, but could give increased accuracy. subFrameCount can be any power of 2 between 1 and 4096, 
but it should be at least 2 if you are using adaptive brightness.
*/
void OPT3101::setFrameTiming(uint16_t subFrameCount)
{
  // Make sure subFrameCount is a power of 2 between 1 and 4096.
  if (subFrameCount < 1 || subFrameCount > 4096 ||
    subFrameCount & (subFrameCount - 1))
  {
    subFrameCount = 4096;
  }

  // Implement equation 6 from sbau310.pdf to calculate
  // XTALK_FILT_TIME CONST, but without floating-point operations.
  uint8_t timeConst = 0;
  while ((subFrameCount << timeConst) < 1024) { timeConst++; }

  uint32_t reg2e = readReg(0x2e);
  if (getLastError()) { return; }
  reg2e = reg2e & ~(uint32_t)0xF00000 | (uint32_t)timeConst << 20;
  writeReg(0x2e, reg2e);
  if (getLastError()) { return; }

  // Set NUM_SUB_FRAMES and NUM_AVG_SUB_FRAMES.
  writeReg(0x9f, (subFrameCount - 1) | (uint32_t)(subFrameCount - 1) << 12);
  if (getLastError()) { return; }

  // Set TG_SEQ_INT_MASK_START and TG_SEQ_INT_MASK_END according to what
  // the OPT3101 datasheet says, but it's probably not needed.
  writeReg(0x97, (subFrameCount - 1) | (uint32_t)(subFrameCount - 1) << 12);

  // Assuming that SUB_VD_CLK_CNT has not been changed, each sub-frame is
  // 0.25 ms.  The +3 is to make sure we round up.
  uint16_t frameTimeMs = (subFrameCount + 3) / 4;

  // Add a ~6% margin in case the OPT3101 clock is running faster.
  frameDelayTimeMs = frameTimeMs + (frameTimeMs + 15) / 16;
}

// Enables the timing generator by setting the TG_EN bit to 1. This is automatically
// called by startSample() so most applications will not need to use it directly.
void OPT3101::enableTimingGenerator()
{
  writeReg(0x80, reg80Default | 1);  // TG_EN = 1
  timingGeneratorEnabled = true;
}

// Disables the timing generator by setting the TG_EN bit to 0.
void OPT3101::disableTimingGenerator()
{
  writeReg(0x80, reg80Default);  // TG_EN = 0
  timingGeneratorEnabled = false;
}

// Configures one of the GPIO pins of the OPT3101 to be used as a data ready output which 
// drives high when a new reading is available. Valid arguments are 1 for GP1 and 2 for GP2.
void OPT3101::enableDataReadyOutput(uint8_t gpPin)
{
  if (gpPin < 1) { gpPin = 1; }
  if (gpPin > 2) { gpPin = 2; }

  // DIG_GPO_SEL0 = 9 (DATA_RDY)
  uint32_t reg0b = readReg(0x0b);
  reg0b = (reg0b & ~(uint32_t)0xF) | 9;
  writeReg(0x0b, reg0b);
  if (getLastError()) { return; }

  uint32_t reg78 = readReg(0x78);
  switch (gpPin)
  {
  case 1:
    // GPO1_MUX_SEL = 2 (DIG_GPO_0)
    // GPIO1_OBUF_EN = 1
    reg78 = (reg78 & ~((uint32_t)7 << 6)) | (2 << 6) | (1 << 12);
  case 2:
    // GPO2_MUX_SEL = 2 (DIG_GPO_0)
    // GPIO2_OBUF_EN = 1
    reg78 = (reg78 & ~((uint32_t)7 << 9)) | (2 << 9) | ((uint16_t)1 << 15);
  }
  writeReg(0x78, reg78);
  if (getLastError()) { return; }
}

// Tells the sensor to start taking a sample. The sensor will start emitting light from its LEDs 
// according to the previosuly-specified settings. This calls enableTimingGenerator() if that has 
// not been done already.
void OPT3101::startSample()
{
  if (!timingGeneratorEnabled) { enableTimingGenerator(); }
  // Set MONOSHOT_BIT to 0 before setting it to 1, as recommended here:
  // https://e2e.ti.com/support/sensors/f/1023/p/756598/2825649#2825649
  writeReg(0x00, 0x000000);
  if (getLastError()) { return; }
  writeReg(0x00, 0x800000);
  startSampleTimeMs = millis();
}

// Returns true if enough time has elapsed so that the sample started by startSample() is expected 
// to be complete. This function only uses Arduino timing functions; it does not communicate with the 
// OPT3101, and does not report any errors using getLastError().
bool OPT3101::isSampleDone()
{
  return (uint16_t)(millis() - startSampleTimeMs) > frameDelayTimeMs;
}

// Reads the sensor's output registers and updates the data in the following member variables: channelUsed, 
// brightnessUsed, i, q, amplitude, phase, distanceMillimeters, ambient, temperature.
void OPT3101::readOutputRegs()
{
  uint32_t reg08 = readReg(0x08);
  if (getLastError()) { return; }
  uint32_t reg09 = readReg(0x09);
  if (getLastError()) { return; }
  uint32_t reg0a = readReg(0x0a);
  if (getLastError()) { return; }

  channelUsed = reg08 >> 18 & 3;
  if (channelUsed > 2) { channelUsed = 2; }
  brightnessUsed = (OPT3101Brightness)(reg08 >> 17 & 1);

  i = readReg(0x3b);
  if (getLastError()) { return; }
  if (i > 0x7fffff) { i -= 0x1000000; }
  q = readReg(0x3c);
  if (getLastError()) { return; }
  if (q > 0x7fffff) { q -= 0x1000000; }

  amplitude = reg09 & 0xFFFF;  // AMP_OUT
  phase = reg08 & 0xFFFF;  // PHASE_OUT

  // c / (2 * 10 MHz * 0x10000) = 0.22872349395 mm ~= 14990/0x10000
  distanceMillimeters = (int32_t)phase * 14990 >> 16;

  ambient = reg0a >> 2 & 0x3FF;  // AMB_DATA

  temperature = reg0a >> 12 & 0xFFF;  // TMAIN
}

// This calls startSample(), then delays until the sample is complete, then calls readOutputRegs() 
// to read the results from the sample.
void OPT3101::sample()
{
  startSample();
  if (getLastError()) { return; }
  delay(frameDelayTimeMs);
  readOutputRegs();
}
