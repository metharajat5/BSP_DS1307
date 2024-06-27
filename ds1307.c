/**
 *
 *
 *
 **/

#include "ds1307.h"
#include <stdio.h>
#include <string.h>

static void DS1307_Bin_to_BCD(uint8_t* data, uint8_t len);

static I2C_HandleTypeDef DS1307_I2C;
DS1307_Status_t DS1307_Init(I2C_HandleTypeDef *handler, DS1307_SQWO_t sqwOut)
{
    DS1307_Status_t status;
    uint8_t value = 0;

    memset(&DS1307_I2C, 0, sizeof(DS1307_I2C));
    memcpy(&DS1307_I2C, handler, sizeof(DS1307_I2C));

    /* Reset the CH Bit in REG0 */
    value &= ~(1 << D_DS1307_BIT_CH);
    status = HAL_I2C_Mem_Write(&DS1307_I2C, D_DS1307_ADDR, D_DS1307_REG_SEC, 1, &value, 1, DS1307_TIMEOUT);
    if (status == DS1307_ERROR){
#ifdef DS1307_Debug
    	printf("\nDS1307 with Slave Address %02X is Not Found", D_DS1307_ADDR);
#endif
        return DS1307_NOT_FOUND;
    }

    value = sqwOut;
    status = HAL_I2C_Mem_Write(&DS1307_I2C, D_DS1307_ADDR, D_DS1307_REG_CTRL, 1, &value, 1, DS1307_TIMEOUT);

    value = 0;
    status = HAL_I2C_Mem_Read(&DS1307_I2C, D_DS1307_ADDR, D_DS1307_REG_CTRL, 1, &value, 1, DS1307_TIMEOUT);

#ifdef DS1307_Debug
    switch (value)
    {
    case _1Hz:
        printf("\n1Hz Square Wave Output is Selected");
        break;
    case _4096Hz:
    	printf("\n4.096KHz Square Wave Output is Selected");
        break;
    case _8192Hz:
    	printf("\n8.192Hz Square Wave Output is Selected");
        break;
    case _32768Hz:
    	printf("\n32.768KHz Square Wave Output is Selected");
        break;
    case _No_Output_1:
    	printf("\nNo Square wave output and SQW/OUT Pin is HIGH");
        break;
    case _No_Output_0:
    	printf("\nNo Square wave output and SQW/OUT Pin is LOW");
        break;
    default:
    	printf("\nWrong Configuration Set");
        break;
    }
#endif
    return status;
}

DS1307_Status_t DS1307_ReadReg(uint8_t regAdd, uint8_t *dataRead, uint8_t readLen)
{
    DS1307_Status_t status;
    uint8_t value[DS1307_MAX_BUFF_SIZE] = {0}, dataLen = 0;
    ;

    status = HAL_I2C_Mem_Read(&DS1307_I2C, D_DS1307_ADDR, regAdd, 1, value, dataLen, DS1307_TIMEOUT);
    for (int i = 0; i < readLen; i++)
    {
        dataRead[i] = value[i];
    }

    return status;
}

DS1307_Status_t DS1307_WriteReg(uint8_t regAdd, uint8_t *dataWrite, uint8_t writeLen)
{
    DS1307_Status_t status;
    uint8_t value[DS1307_MAX_BUFF_SIZE] = {0}, dataLen = writeLen;

    if (dataLen > DS1307_MAX_BUFF_SIZE)
    {
#ifdef DS1307_Debug
        printf("\nDatasize Exceeded");
#endif
        return DS1307_DATA_SIZE_ERROR;
    }
    for (int i = 0; i < dataLen; i++)
    {
        value[i] = dataWrite[i];
    }
    status = HAL_I2C_Mem_Write(&DS1307_I2C, D_DS1307_ADDR, regAdd, 1, value, dataLen, DS1307_TIMEOUT);

    return status;
}

DS1307_Status_t DS1307_ReadTime_Bin(DS1307_Time_t* dataRead)
{
    DS1307_Status_t status;
    uint8_t value[3] = {0};

    status = DS1307_ReadReg(D_DS1307_REG_SEC, value, 3);
    dataRead->Sec = value[0];
    dataRead->Min = value[1];
    dataRead->Hour = value[2];
#ifdef DS1307_Debug
    printf("\nTime is %d:%d:%d", dataRead->Hour, dataRead->Min, dataRead->Sec);
#endif
    return status;
}

DS1307_Status_t DS1307_ReadTime_BCD(DS1307_Time_t* dataRead)
{
    DS1307_Status_t status;
    uint8_t value[3] = {0};

    status = DS1307_ReadReg(D_DS1307_REG_SEC, value, 3);
    DS1307_Bin_to_BCD(value, sizeof(value));
    dataRead->Sec = value[0];
    dataRead->Min = value[1];
    dataRead->Hour = value[2];
#ifdef DS1307_Debug
    printf("\nTime is %02X:%02X:%02X", dataRead->Hour, dataRead->Min, dataRead->Sec);
#endif
    return status;
}

DS1307_Status_t DS1307_ReadDate_Bin(DS1307_Date_t* dataRead)
{
    DS1307_Status_t status;
    uint8_t value[4] = {0};

    status = DS1307_ReadReg(D_DS1307_REG_DAY, value, 4);
    dataRead->Day = value[0];
    dataRead->Date = value[1];
    dataRead->Month = value[2];
    dataRead->Year = value[3];
#ifdef DS1307_Debug
    printf("\nDay: %d Date: %d-%d-%d", dataRead->Day, dataRead->Date, dataRead->Month, dataRead->Year);
#endif
    return status;
}

DS1307_Status_t DS1307_ReadDate_BCD(DS1307_Date_t *dataRead)
{
    DS1307_Status_t status;
    uint8_t value[4] = {0};

    status = DS1307_ReadReg(D_DS1307_REG_DAY, value, 4);
    DS1307_Bin_to_BCD(value, sizeof(value));
    dataRead->Day = value[0];
    dataRead->Date = value[1];
    dataRead->Month = value[2];
    dataRead->Year = value[3];
#ifdef DS1307_Debug
    printf("\nDay: %02X Date: %02X-%02X-%02X", dataRead->Day, dataRead->Date, dataRead->Month, dataRead->Year);
#endif
    return status;
}

DS1307_Status_t DS1307_ReadDateTime_Bin(DS1307_DateTime_t *dataRead)
{
    DS1307_Status_t status;

    status = DS1307_ReadDate_Bin(&dataRead->date);
    status = DS1307_ReadTime_Bin(&dataRead->time);
    return status;
}

DS1307_Status_t DS1307_ReadDateTime_BCD(DS1307_DateTime_t *dataRead)
{
    DS1307_Status_t status;

    status = DS1307_ReadDate_BCD(&dataRead->date);
    status = DS1307_ReadTime_BCD(&dataRead->time);
    return status;
}

void DS1307_Bin_to_BCD(uint8_t* data, uint8_t len)
{
    for (int i = 0; i < len; i++)
    {
        data[i] = ((data[i] / 10) << 4) | (data[i] % 10);
    }
}