# DS1307 Real-Time Clock (RTC) Driver

This repository contains a driver for interfacing with the DS1307 Real-Time Clock (RTC) using I2C communication. The driver supports both binary and Binary-Coded Decimal (BCD) formats for reading and writing time and date information.

## Features

- Initialize the DS1307 RTC with configurable square wave output.
- Read and write time and date in both binary and BCD formats.
- Handle basic I2C communication with error checking.

## Files

- `ds1307.h`: Header file with function declarations and type definitions.
- `ds1307.c`: Implementation file with function definitions.

## Functions

### Initialization

- `DS1307_Status_t DS1307_Init(I2C_HandleTypeDef *handler, DS1307_SQWO_t sqwOut)`

### Read Operations

- `DS1307_Status_t DS1307_ReadReg(uint8_t regAdd, uint8_t *dataRead, uint8_t readLen)`
- `DS1307_Status_t DS1307_ReadTime_Bin(DS1307_Time_t* dataRead)`
- `DS1307_Status_t DS1307_ReadTime_BCD(DS1307_Time_t* dataRead)`
- `DS1307_Status_t DS1307_ReadDate_Bin(DS1307_Date_t* dataRead)`
- `DS1307_Status_t DS1307_ReadDate_BCD(DS1307_Date_t *dataRead)`
- `DS1307_Status_t DS1307_ReadDateTime_Bin(DS1307_DateTime_t *dataRead)`
- `DS1307_Status_t DS1307_ReadDateTime_BCD(DS1307_DateTime_t *dataRead)`

### Write Operations

- `DS1307_Status_t DS1307_WriteReg(uint8_t regAdd, uint8_t *dataWrite, uint8_t writeLen)`

## Dependencies

- STM32 HAL Library for I2C communication.
- Standard C library.

## Compilation

To compile the driver, include `ds1307.c` and `ds1307.h` in your project. Make sure the STM32 HAL library is properly set up in your build environment.

## Example Usage

```c
#include "ds1307.h"

// Example initialization
I2C_HandleTypeDef hi2c1;  // Assume this is initialized elsewhere
DS1307_Init(&hi2c1, _1Hz);

// Example reading time
DS1307_Time_t time;
DS1307_ReadTime_Bin(&time);
printf("Current Time: %02d:%02d:%02d\n", time.Hour, time.Min, time.Sec);