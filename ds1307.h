/**
 *
 *
 *
 **/

#ifndef _INC_DS1307_H_
#define _INC_DS1307_H_

#include <main.h>
#include <stdio.h>
#include <stdint.h>

#define DS1307_Debug /* Uncomment this line to get printf debugging statements */
#define DS1307_TIMEOUT 10
#define DS1307_MAX_BUFF_SIZE 64
/* DS1307 IMPORTANT CONFIGURATIONS AND DEFINATIONS*/
#define D_DS1307_ADDR 0x68 /* 1101000 Slave Address is 7 bits */
#define D_DS1307_READ 1
#define D_DS1307_WRITE 0

/* DS1307 TIMEKEEPER REGISTERS */
#define D_DS1307_REG_SEC 0x00
#define D_DS1307_REG_MIN 0x01
#define D_DS1307_REG_HRS 0x02
#define D_DS1307_REG_DAY 0x03
#define D_DS1307_REG_DATE 0x04
#define D_DS1307_REG_MONTH 0x05
#define D_DS1307_REG_YEAR 0x06
#define D_DS1307_REG_CTRL 0x07

/* DS1307 RAM REGISTERS */
#define D_DS1307_REG_RAM01 0x08
#define D_DS1307_REG_RAM02 0x09
#define D_DS1307_REG_RAM03 0x0A
#define D_DS1307_REG_RAM04 0x0B
#define D_DS1307_REG_RAM05 0x0C
#define D_DS1307_REG_RAM06 0x0D
#define D_DS1307_REG_RAM07 0x0E
#define D_DS1307_REG_RAM08 0x0F
#define D_DS1307_REG_RAM09 0x10
#define D_DS1307_REG_RAM10 0x11
#define D_DS1307_REG_RAM11 0x12
#define D_DS1307_REG_RAM12 0x13
#define D_DS1307_REG_RAM13 0x14
#define D_DS1307_REG_RAM14 0x15
#define D_DS1307_REG_RAM15 0x16
#define D_DS1307_REG_RAM16 0x17
#define D_DS1307_REG_RAM17 0x18
#define D_DS1307_REG_RAM18 0x19
#define D_DS1307_REG_RAM19 0x1A
#define D_DS1307_REG_RAM20 0x1B
#define D_DS1307_REG_RAM21 0x1C
#define D_DS1307_REG_RAM22 0x1D
#define D_DS1307_REG_RAM23 0x1E
#define D_DS1307_REG_RAM24 0x1F
#define D_DS1307_REG_RAM25 0x20
#define D_DS1307_REG_RAM26 0x21
#define D_DS1307_REG_RAM27 0x22
#define D_DS1307_REG_RAM28 0x23
#define D_DS1307_REG_RAM29 0x24
#define D_DS1307_REG_RAM30 0x25
#define D_DS1307_REG_RAM31 0x26
#define D_DS1307_REG_RAM32 0x27
#define D_DS1307_REG_RAM33 0x28
#define D_DS1307_REG_RAM34 0x29
#define D_DS1307_REG_RAM35 0x2A
#define D_DS1307_REG_RAM36 0x2B
#define D_DS1307_REG_RAM37 0x2C
#define D_DS1307_REG_RAM38 0x2D
#define D_DS1307_REG_RAM39 0x2E
#define D_DS1307_REG_RAM40 0x2F
#define D_DS1307_REG_RAM41 0x30
#define D_DS1307_REG_RAM42 0x31
#define D_DS1307_REG_RAM43 0x32
#define D_DS1307_REG_RAM44 0x33
#define D_DS1307_REG_RAM45 0x34
#define D_DS1307_REG_RAM46 0x35
#define D_DS1307_REG_RAM47 0x36
#define D_DS1307_REG_RAM48 0x37
#define D_DS1307_REG_RAM49 0x38
#define D_DS1307_REG_RAM50 0x39
#define D_DS1307_REG_RAM51 0x3A
#define D_DS1307_REG_RAM52 0x3B
#define D_DS1307_REG_RAM53 0x3C
#define D_DS1307_REG_RAM54 0x3D
#define D_DS1307_REG_RAM55 0x3E
#define D_DS1307_REG_RAM56 0x3F

/* DS1307 HOURS FORMAT */
#define D_DS1307_TF_12HR 0x01
#define D_DS1307_TF_24HR 0x02

/* DS1307 TIME FORMAT */
#define D_DS1307_IND_AM 0x01
#define D_DS1307_IND_PM 0x02

/* DS1307 DAYS FORMAT */
#define D_DS1307_SUNDAY 0x01
#define D_DS1307_MONDAY 0x02
#define D_DS1307_TUESDAY 0x03
#define D_DS1307_WEDNESDAY 0x04
#define D_DS1307_THURSDAY 0x05
#define D_DS1307_FRIDAY 0x06
#define D_DS1307_SATURDAY 0x07

/* DS1307 BIT DEFINATIONS */
#define D_DS1307_SET 1
#define D_DS1307_RESET 0
#define D_DS1307_ENABLE D_DS1307_SET
#define D_DS1307_DISABLE D_DS1307_RESET
#define D_DS1307_BIT_CH 7
#define D_DS1307_BIT_HRS 6
#define D_DS1307_BIT_AMPM 5
#define D_DS1307_BIT_OUT 7
#define D_DS1307_BIT_SQWE 4
#define D_DS1307_BIT_RS1 1
#define D_DS1307_BIT_RS0 0

/* DS1307 SQUARE WAVE OUTPUT DEFINATIONS */
typedef enum
{
    _1Hz = 0x10,
    _4096Hz = 0x11,
    _8192Hz = 0x12,
    _32768Hz = 0x13,
    _No_Output_1 = 0x80,
    _No_Output_0 = 0x00,
} DS1307_SQWO_t;

typedef enum
{
    DS1307_OK = 0,
    DS1307_ERROR = 1,
    DS1307_BUSY = 2,
    DS1307_TIMEOUT_ERR = 3,
    DS1307_NOT_FOUND = 4,
    DS1307_DATA_SIZE_ERROR = 5,
}
DS1307_Status_t;

typedef struct
{
    uint8_t Hour;
    uint8_t Min;
    uint8_t Sec;
} DS1307_Time_t;

typedef struct
{
    uint8_t Day;
    uint8_t Date;
    uint8_t Month;
    uint8_t Year;
} DS1307_Date_t;

typedef struct
{
    DS1307_Date_t date;
    DS1307_Time_t time;
} DS1307_DateTime_t;

DS1307_Status_t DS1307_Init(I2C_HandleTypeDef *handler, DS1307_SQWO_t sqwOut);
DS1307_Status_t DS1307_ReadReg(uint8_t regAdd, uint8_t *dataRead, uint8_t readLen);
DS1307_Status_t DS1307_WriteReg(uint8_t regAdd, uint8_t *dataWrite, uint8_t writeLen);
DS1307_Status_t DS1307_ReadTime_Bin(DS1307_Time_t* dataRead);
DS1307_Status_t DS1307_ReadTime_BCD(DS1307_Time_t* dataRead);
DS1307_Status_t DS1307_ReadDate_Bin(DS1307_Date_t* dataRead);
DS1307_Status_t DS1307_ReadDate_BCD(DS1307_Date_t* dataRead);
DS1307_Status_t DS1307_ReadDateTime_Bin(DS1307_DateTime_t* dataRead);
DS1307_Status_t DS1307_ReadDateTime_BCD(DS1307_DateTime_t* dataRead);

/* */
/*
rise time is 300ns - 1000nS
bus capacitance 10pF - 400pF
 */
#endif /* _INC_DS1307_H_ */