/**
 * @file ds1307.c
 * @brief DS1307 Real-Time Clock (RTC) Driver Implementation.
 * This file provides functions to interface with the DS1307 RTC via I2C, including
 * initialization, reading and writing of date and time in both binary and BCD formats.
 * @note This file requires the inclusion of "ds1307.h" and appropriate I2C HAL drivers.
 */

/* Include Files */
#include "ds1307.h"
#include <stdio.h>
#include <string.h>

/**
 * @brief Converts binary values in an array to binary-coded decimal (BCD) format.
 * This function converts each element of the provided data array from binary format
 * to binary-coded decimal (BCD) format. The conversion is performed by calculating
 * the tens and units place of each value and combining them into a BCD representation.
 * @param[in,out] data Pointer to the array of binary values to be converted to BCD.
 * @param[in] len Length of the data array.
 * @note The function assumes that the input data array contains values in binary format.
 */
static void DS1307_Bin_to_BCD(uint8_t* data, uint8_t len);

/**
 * @brief I2C handle for DS1307 operations.
 * 
 * This static variable holds the I2C handle structure used for communicating
 * with the DS1307 real-time clock (RTC) device.
 */
static I2C_HandleTypeDef DS1307_I2C;

/**
 * @brief Initializes the DS1307 RTC with the specified I2C handler and square wave output setting.
 * This function initializes the DS1307 real-time clock (RTC) by configuring its I2C 
 * handler and setting the square wave output (SQW/OUT) according to the provided 
 * configuration. The initialization process includes resetting the CH bit to enable 
 * the oscillator and configuring the square wave output frequency.
 * @param[in] handler Pointer to an I2C_HandleTypeDef structure that contains the configuration 
 *                    information for the I2C peripheral to be used for communication with 
 *                    the DS1307 RTC.
 * @param[in] sqwOut Square wave output configuration. This parameter sets the frequency of 
 *                    the square wave output or disables it. The configuration should be 
 *                    specified using the DS1307_SQWO_t enumeration.
 * @return DS1307_Status_t Status of the initialization operation. Returns DS1307_OK on 
 *         success, or an error code if the initialization fails. Specifically, it returns 
 *         DS1307_NOT_FOUND if the DS1307 RTC is not detected.
 */
DS1307_Status_t DS1307_Init(I2C_HandleTypeDef *handler, DS1307_SQWO_t sqwOut)
{
    DS1307_Status_t status; /**< Status of the initialization operation. */
    uint8_t value = 0;      /**< Temporary variable for I2C operations. */

    /* Initialize the I2C handler for DS1307 communication */
    memset(&DS1307_I2C, 0, sizeof(DS1307_I2C));   /**< Clear the DS1307_I2C structure. */
    memcpy(&DS1307_I2C, handler, sizeof(DS1307_I2C)); /**< Copy the user-provided I2C handler to DS1307_I2C. */

    /* Reset the CH bit in the seconds register (REG0) to enable the oscillator */
    value &= ~(1 << D_DS1307_BIT_CH);  /**< Clear the CH bit (bit 7) to enable the oscillator. */
    status = HAL_I2C_Mem_Write(&DS1307_I2C, D_DS1307_ADDR, D_DS1307_REG_SEC, 1, &value, 1, DS1307_TIMEOUT); /**< Write to the seconds register. */
    
    if (status == DS1307_ERROR) {
#ifdef DS1307_Debug
        printf("\nDS1307 with Slave Address %02X is Not Found", D_DS1307_ADDR); /**< Print error message if DS1307 is not found. */
#endif
        return DS1307_NOT_FOUND; /**< Return error code if DS1307 is not found. */
    }

    /* Set the square wave output frequency */
    value = sqwOut;  /**< Set the square wave output configuration. */
    status = HAL_I2C_Mem_Write(&DS1307_I2C, D_DS1307_ADDR, D_DS1307_REG_CTRL, 1, &value, 1, DS1307_TIMEOUT); /**< Write to the control register. */

    /* Verify the square wave output setting */
    value = 0; /**< Clear the value variable. */
    status = HAL_I2C_Mem_Read(&DS1307_I2C, D_DS1307_ADDR, D_DS1307_REG_CTRL, 1, &value, 1, DS1307_TIMEOUT); /**< Read back the control register. */

#ifdef DS1307_Debug
    /* Print the current square wave output setting */
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

    return status; /**< Return the status of the initialization operation. */
}

/**
 * @brief Reads data from a specified register of the DS1307 RTC.
 * 
 * This function reads data from a specific register of the DS1307 real-time clock (RTC)
 * and stores it in the provided buffer. It uses I2C communication to access the register
 * and retrieves the requested number of bytes.
 * @param[in] regAdd The address of the register to read from.
 * @param[out] dataRead Pointer to the buffer where the read data will be stored.
 * @param[in] readLen The number of bytes to read from the register.
 * @return DS1307_Status_t Status of the read operation. Returns DS1307_OK on success,
 *         or an error code if the operation fails.
 */
DS1307_Status_t DS1307_ReadReg(uint8_t regAdd, uint8_t *dataRead, uint8_t readLen)
{
    DS1307_Status_t status; /**< Status of the I2C read operation. */
    uint8_t value[DS1307_MAX_BUFF_SIZE] = {0}, /**< Buffer to hold the read data. */
            dataLen = 0; /**< Length of data to read. Initialized to 0. */

    /* Perform I2C read operation to read data from the specified register */
    status = HAL_I2C_Mem_Read(&DS1307_I2C, D_DS1307_ADDR, regAdd, 1, value, dataLen, DS1307_TIMEOUT);

    /* Copy the read data from the buffer to the output buffer */
    for (int i = 0; i < readLen; i++)
    {
        dataRead[i] = value[i];
    }

    return status; /**< Return the status of the read operation. */
}

/**
 * @brief Writes data to a specified register of the DS1307 RTC.
 * This function writes a specified number of bytes to a register in the DS1307 real-time 
 * clock (RTC) using I2C communication. It first checks if the data size exceeds the maximum 
 * buffer size and then performs the write operation if valid.
 * @param[in] regAdd The address of the register to write to.
 * @param[in] dataWrite Pointer to the buffer containing the data to be written.
 * @param[in] writeLen The number of bytes to write to the register.
 * @return DS1307_Status_t Status of the write operation. Returns DS1307_OK on success, 
 *         or an error code if the operation fails. Specifically, returns DS1307_DATA_SIZE_ERROR 
 *         if the data length exceeds the maximum buffer size.
 */
DS1307_Status_t DS1307_WriteReg(uint8_t regAdd, uint8_t *dataWrite, uint8_t writeLen)
{
    DS1307_Status_t status; /**< Status of the I2C write operation. */
    uint8_t value[DS1307_MAX_BUFF_SIZE] = {0}, /**< Buffer to hold the data to be written. */
            dataLen = writeLen; /**< Length of data to write, set to the input writeLen. */

    /* Check if the data length exceeds the maximum buffer size */
    if (dataLen > DS1307_MAX_BUFF_SIZE)
    {
#ifdef DS1307_Debug
        /* Print an error message if data size exceeds the limit */
        printf("\nDatasize Exceeded");
#endif
        return DS1307_DATA_SIZE_ERROR; /**< Return error status for data size exceeding buffer. */
    }

    /* Copy data from the input buffer to the internal buffer */
    for (int i = 0; i < dataLen; i++)
    {
        value[i] = dataWrite[i];
    }

    /* Perform I2C write operation to the specified register */
    status = HAL_I2C_Mem_Write(&DS1307_I2C, D_DS1307_ADDR, regAdd, 1, value, dataLen, DS1307_TIMEOUT);

    return status; /**< Return the status of the write operation. */
}

/**
 * @brief Reads the current time from the DS1307 RTC in binary format.
 * 
 * This function reads the seconds, minutes, and hours from the DS1307 real-time 
 * clock (RTC) in binary format and stores the values in the provided DS1307_Time_t 
 * structure.
 * @param[out] dataRead Pointer to a DS1307_Time_t structure where the read time values 
 *                      will be stored. The structure's fields are updated with the 
 *                      current time read from the RTC.
 * @return DS1307_Status_t Status of the read operation. Returns DS1307_OK on success, 
 *         or an error code if the operation fails. 
 */
DS1307_Status_t DS1307_ReadTime_Bin(DS1307_Time_t* dataRead)
{
    DS1307_Status_t status; /**< Status of the read operation. */
    uint8_t value[3] = {0}; /**< Buffer to hold the raw time data read from the RTC. */

    /* Read the seconds, minutes, and hours from the DS1307 registers */
    status = DS1307_ReadReg(D_DS1307_REG_SEC, value, 3);

    /* Store the read values into the DS1307_Time_t structure */
    dataRead->Sec = value[0];  /**< Assign the seconds value to the structure. */
    dataRead->Min = value[1];  /**< Assign the minutes value to the structure. */
    dataRead->Hour = value[2]; /**< Assign the hours value to the structure. */

#ifdef DS1307_Debug
    /* Print the current time in HH:MM:SS format if debugging is enabled */
    printf("\nTime is %d:%d:%d", dataRead->Hour, dataRead->Min, dataRead->Sec);
#endif

    return status; /**< Return the status of the read operation. */
}


/**
 * @brief Reads the current time from the DS1307 RTC in BCD format.
 * This function reads the seconds, minutes, and hours from the DS1307 real-time 
 * clock (RTC) in Binary-Coded Decimal (BCD) format and stores the values in the 
 * provided DS1307_Time_t structure.
 * @param[out] dataRead Pointer to a DS1307_Time_t structure where the read time values 
 *                      will be stored. The structure's fields are updated with the 
 *                      current time read from the RTC in BCD format.
 * @return DS1307_Status_t Status of the read operation. Returns DS1307_OK on success, 
 *         or an error code if the operation fails.
 */
DS1307_Status_t DS1307_ReadTime_BCD(DS1307_Time_t* dataRead)
{
    DS1307_Status_t status; /**< Status of the read operation. */
    uint8_t value[3] = {0}; /**< Buffer to hold the raw time data read from the RTC in BCD format. */

    /* Read the seconds, minutes, and hours from the DS1307 registers in BCD format */
    status = DS1307_ReadReg(D_DS1307_REG_SEC, value, 3);

    /* Convert the BCD values to binary format */
    DS1307_Bin_to_BCD(value, sizeof(value));

    /* Store the converted values into the DS1307_Time_t structure */
    dataRead->Sec = value[0];  /**< Assign the seconds value to the structure. */
    dataRead->Min = value[1];  /**< Assign the minutes value to the structure. */
    dataRead->Hour = value[2]; /**< Assign the hours value to the structure. */

#ifdef DS1307_Debug
    /* Print the current time in HH:MM:SS format in BCD if debugging is enabled */
    printf("\nTime is %02X:%02X:%02X", dataRead->Hour, dataRead->Min, dataRead->Sec);
#endif

    return status; /**< Return the status of the read operation. */
}

/**
 * @brief Reads the current date from the DS1307 RTC in binary format.
 * This function reads the day, date, month, and year from the DS1307 real-time 
 * clock (RTC) in binary format and stores the values in the provided DS1307_Date_t 
 * structure.
 * @param[out] dataRead Pointer to a DS1307_Date_t structure where the read date values 
 *                      will be stored. The structure's fields are updated with the 
 *                      current date read from the RTC in binary format.
 * @return DS1307_Status_t Status of the read operation. Returns DS1307_OK on success, 
 *         or an error code if the operation fails.
 */
DS1307_Status_t DS1307_ReadDate_Bin(DS1307_Date_t* dataRead)
{
    DS1307_Status_t status; /**< Status of the read operation. */
    uint8_t value[4] = {0}; /**< Buffer to hold the raw date data read from the RTC in binary format. */

    /* Read the day, date, month, and year from the DS1307 registers in binary format */
    status = DS1307_ReadReg(D_DS1307_REG_DAY, value, 4);

    /* Store the read values into the DS1307_Date_t structure */
    dataRead->Day = value[0];   /**< Assign the day value to the structure. */
    dataRead->Date = value[1];  /**< Assign the date value to the structure. */
    dataRead->Month = value[2]; /**< Assign the month value to the structure. */
    dataRead->Year = value[3];  /**< Assign the year value to the structure. */

#ifdef DS1307_Debug
    /* Print the current date in Day: Date-Month-Year format if debugging is enabled */
    printf("\nDay: %d Date: %d-%d-%d", dataRead->Day, dataRead->Date, dataRead->Month, dataRead->Year);
#endif

    return status; /**< Return the status of the read operation. */
}

/**
 * @brief Reads the current date from the DS1307 RTC in BCD format.
 * This function reads the day, date, month, and year from the DS1307 real-time 
 * clock (RTC) in Binary-Coded Decimal (BCD) format and stores the values in the 
 * provided DS1307_Date_t structure.
 * @param[out] dataRead Pointer to a DS1307_Date_t structure where the read date values 
 *                      will be stored. The structure's fields are updated with the 
 *                      current date read from the RTC in BCD format.
 * @return DS1307_Status_t Status of the read operation. Returns DS1307_OK on success, 
 *         or an error code if the operation fails.
 */
DS1307_Status_t DS1307_ReadDate_BCD(DS1307_Date_t *dataRead)
{
    DS1307_Status_t status; /**< Status of the read operation. */
    uint8_t value[4] = {0}; /**< Buffer to hold the raw date data read from the RTC in BCD format. */

    /* Read the day, date, month, and year from the DS1307 registers in binary format */
    status = DS1307_ReadReg(D_DS1307_REG_DAY, value, 4);

    /* Convert the binary data to BCD format */
    DS1307_Bin_to_BCD(value, sizeof(value));

    /* Store the converted BCD values into the DS1307_Date_t structure */
    dataRead->Day = value[0];   /**< Assign the BCD day value to the structure. */
    dataRead->Date = value[1];  /**< Assign the BCD date value to the structure. */
    dataRead->Month = value[2]; /**< Assign the BCD month value to the structure. */
    dataRead->Year = value[3];  /**< Assign the BCD year value to the structure. */

#ifdef DS1307_Debug
    /* Print the current date in BCD format if debugging is enabled */
    printf("\nDay: %02X Date: %02X-%02X-%02X", dataRead->Day, dataRead->Date, dataRead->Month, dataRead->Year);
#endif

    return status; /**< Return the status of the read operation. */
}

/**
 * @brief Reads the current date and time from the DS1307 RTC in binary format.
 * This function reads the date and time from the DS1307 real-time clock (RTC) in 
 * binary format and stores the values in the provided DS1307_DateTime_t structure. 
 * The function first reads the date and then reads the time.
 * @param[out] dataRead Pointer to a DS1307_DateTime_t structure where the read date and 
 *                      time values will be stored. The structure's fields are updated 
 *                      with the current date and time read from the RTC in binary format.
 * @return DS1307_Status_t Status of the read operation. Returns DS1307_OK on success, 
 *         or an error code if the operation fails. If an error occurs during either 
 *         date or time read, the function returns the status of the last operation.
 */
DS1307_Status_t DS1307_ReadDateTime_Bin(DS1307_DateTime_t *dataRead)
{
    DS1307_Status_t status; /**< Status of the read operation. */

    /* Read the current date in binary format and store it in the date field of the structure */
    status = DS1307_ReadDate_Bin(&dataRead->date);

    /* Read the current time in binary format and store it in the time field of the structure */
    status = DS1307_ReadTime_Bin(&dataRead->time);

    return status; /**< Return the status of the last read operation (time read). */
}

/**
 * @brief Reads the current date and time from the DS1307 RTC in BCD format.
 * 
 * This function reads the date and time from the DS1307 real-time clock (RTC) in 
 * Binary-Coded Decimal (BCD) format and stores the values in the provided 
 * DS1307_DateTime_t structure. The function first reads the date and then reads the time.
 * @param[out] dataRead Pointer to a DS1307_DateTime_t structure where the read date and 
 *                      time values will be stored. The structure's fields are updated 
 *                      with the current date and time read from the RTC in BCD format.
 * @return DS1307_Status_t Status of the read operation. Returns DS1307_OK on success, 
 *         or an error code if the operation fails. If an error occurs during either 
 *         date or time read, the function returns the status of the last operation.
 */
DS1307_Status_t DS1307_ReadDateTime_BCD(DS1307_DateTime_t *dataRead)
{
    DS1307_Status_t status; /**< Status of the read operation. */

    /* Read the current date in BCD format and store it in the date field of the structure */
    status = DS1307_ReadDate_BCD(&dataRead->date);

    /* Read the current time in BCD format and store it in the time field of the structure */
    status = DS1307_ReadTime_BCD(&dataRead->time);

    return status; /**< Return the status of the last read operation (time read). */
}

/**
 * @brief Converts binary values in an array to binary-coded decimal (BCD) format.
 * This function converts each element of the provided data array from binary format
 * to binary-coded decimal (BCD) format. The conversion is performed by calculating
 * the tens and units place of each value and combining them into a BCD representation.
 * @param[in,out] data Pointer to the array of binary values to be converted to BCD.
 * @param[in] len Length of the data array.
 * @note The function assumes that the input data array contains values in binary format.
 */
void DS1307_Bin_to_BCD(uint8_t* data, uint8_t len)
{
    for (int i = 0; i < len; i++)
    {
        data[i] = ((data[i] / 10) << 4) | (data[i] % 10);
    }
}
