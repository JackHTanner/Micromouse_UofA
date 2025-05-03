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
// DESCRIPTION: This header file contains functions to interface
// with the distance sensor.

#pragma once

#include <Arduino.h>

enum class OPT3101Brightness : uint8_t {
  Low = 0,
  High = 1,
  Adaptive = 255
};

const uint8_t OPT3101ChannelAutoSwitch = 255;

class OPT3101
{
public:
  void setAddress(uint8_t address) { this->address = address; }
  uint8_t getAddress() { return address; }

  uint8_t getLastError() { return lastError; };

  void resetAndWait();
  void writeReg(uint8_t reg, uint32_t value);
  uint32_t readReg(uint8_t reg);

  void init();
  void configureDefault();
  void setChannel(uint8_t channel);
  void nextChannel();
  void setBrightness(OPT3101Brightness);
  void setMonoshotMode();
  void setContinuousMode();
  void setFrameTiming(uint16_t subFrameCount);
  void enableTimingGenerator();
  void disableTimingGenerator();
  void enableDataReadyOutput(uint8_t gpPin);
  void startSample();
  bool isSampleDone();
  void readOutputRegs();
  void sample();

  uint8_t channelUsed = 0;
  OPT3101Brightness brightnessUsed;
  uint16_t ambient;
  uint16_t temperature;
  int32_t i, q;
  uint16_t amplitude;
  int16_t phase;
  int16_t distanceMillimeters;

private:
  bool timingGeneratorEnabled = false;
  uint8_t lastError = 0;
  uint8_t address = 0x58;
  uint16_t startSampleTimeMs;
  uint16_t frameDelayTimeMs;
};
