/**
 * @file ds1307.h
 * @brief Header file for the DS1307 RTC module.
 *
 * This header file contains definitions, function prototypes, and macros
 * for interfacing with the DS1307 real-time clock (RTC) module. The DS1307
 * is a low-power, I2C-bus real-time clock/calendar with a 56-byte battery-backed
 * SRAM. It keeps track of time and date and provides functions to read and write
 * time/date data.
 *
 * @details
 * To use this library:
 * 1. Generate the I2C handler using the STM32CubeMX tool under the Connectivity category.
 *    - Open STM32CubeMX, navigate to the I2C configuration, and set up the I2C peripheral.
 *    - Generate the initialization code for the project.
 *
 * 2. If needed, generate a UART handler for debug messages.
 *    - Configure UART under the Connectivity category in STM32CubeMX if debug UART is required.
 *    - Ensure the UART initialization code is generated and included in your project.
 *
 * 3. Include this header file in your main application source file:
 *    @code
 *    #include "ds1307.h"
 *    @endcode
 *
 * 4. Initialize the DS1307 RTC by calling the `DS1307_Init` function with the appropriate
 *    I2C handler and SQW output settings:
 *    @code
 *    DS1307_Status_t status = DS1307_Init(&hi2c1, DS1307_SQWO_DISABLE);
 *    @endcode
 *    where `hi2c1` is your I2C handler and `DS1307_SQWO_DISABLE` configures the Square Wave Output.
 *
 * 5. Use the provided functions to read date and time whenever needed:
 *    - Call functions like `DS1307_ReadDate()` or `DS1307_ReadTime()` as needed in your application.
 *
 * @note Ensure that the I2C handler is properly initialized in your main application
 *       before calling DS1307 functions.
 */

#ifndef _INC_DS1307_H_
#define _INC_DS1307_H_

/* Include Files */
#include <main.h>

#define DS1307_Debug /* Uncomment this line to get printf debugging statements */

#define DS1307_TIMEOUT                           10
#define DS1307_MAX_BUFF_SIZE                     64

/* DS1307 IMPORTANT CONFIGURATIONS AND DEFINATIONS*/
/**
 * @brief DS1307 Slave Address (7 bits).
 */
#define D_DS1307_ADDR                            0x68 /* 1101000 Slave Address is 7 bits */

/**
 * @brief Read operation for I2C.
 */
#define D_DS1307_READ                            1

/**
 * @brief Write operation for I2C.
 */
#define D_DS1307_WRITE                           0

/* DS1307 TIMEKEEPER REGISTERS */
/**
 * @brief DS1307 Seconds Register.
 */
#define D_DS1307_REG_SEC                         0x00

/**
 * @brief DS1307 Minutes Register.
 */
#define D_DS1307_REG_MIN                         0x01

/**
 * @brief DS1307 Hours Register.
 */
#define D_DS1307_REG_HRS                         0x02

/**
 * @brief DS1307 Day Register.
 */
#define D_DS1307_REG_DAY                         0x03

/**
 * @brief DS1307 Date Register.
 */
#define D_DS1307_REG_DATE                        0x04

/**
 * @brief DS1307 Month Register.
 */
#define D_DS1307_REG_MONTH                       0x05

/**
 * @brief DS1307 Year Register.
 */
#define D_DS1307_REG_YEAR                        0x06

/**
 * @brief DS1307 Control Register.
 */
#define D_DS1307_REG_CTRL                        0x07

/* DS1307 RAM REGISTERS */
/**
 * @brief DS1307 RAM Register 01.
 */
#define D_DS1307_REG_RAM01                       0x08

/**
 * @brief DS1307 RAM Register 02.
 */
#define D_DS1307_REG_RAM02                       0x09

/**
 * @brief DS1307 RAM Register 03.
 */
#define D_DS1307_REG_RAM03                       0x0A

/**
 * @brief DS1307 RAM Register 04.
 */
#define D_DS1307_REG_RAM04                       0x0B

/**
 * @brief DS1307 RAM Register 05.
 */
#define D_DS1307_REG_RAM05                       0x0C

/**
 * @brief DS1307 RAM Register 06.
 */
#define D_DS1307_REG_RAM06                       0x0D

/**
 * @brief DS1307 RAM Register 07.
 */
#define D_DS1307_REG_RAM07                       0x0E

/**
 * @brief DS1307 RAM Register 08.
 */
#define D_DS1307_REG_RAM08                       0x0F

/**
 * @brief DS1307 RAM Register 09.
 */
#define D_DS1307_REG_RAM09                       0x10

/**
 * @brief DS1307 RAM Register 10.
 */
#define D_DS1307_REG_RAM10                       0x11

/**
 * @brief DS1307 RAM Register 11.
 */
#define D_DS1307_REG_RAM11                       0x12

/**
 * @brief DS1307 RAM Register 12.
 */
#define D_DS1307_REG_RAM12                       0x13

/**
 * @brief DS1307 RAM Register 13.
 */
#define D_DS1307_REG_RAM13                       0x14

/**
 * @brief DS1307 RAM Register 14.
 */
#define D_DS1307_REG_RAM14                       0x15

/**
 * @brief DS1307 RAM Register 15.
 */
#define D_DS1307_REG_RAM15                       0x16

/**
 * @brief DS1307 RAM Register 16.
 */
#define D_DS1307_REG_RAM16                       0x17

/**
 * @brief DS1307 RAM Register 17.
 */
#define D_DS1307_REG_RAM17                       0x18

/**
 * @brief DS1307 RAM Register 18.
 */
#define D_DS1307_REG_RAM18                       0x19

/**
 * @brief DS1307 RAM Register 19.
 */
#define D_DS1307_REG_RAM19                       0x1A

/**
 * @brief DS1307 RAM Register 20.
 */
#define D_DS1307_REG_RAM20                       0x1B

/**
 * @brief DS1307 RAM Register 21.
 */
#define D_DS1307_REG_RAM21                       0x1C

/**
 * @brief DS1307 RAM Register 22.
 */
#define D_DS1307_REG_RAM22                       0x1D

/**
 * @brief DS1307 RAM Register 23.
 */
#define D_DS1307_REG_RAM23                       0x1E

/**
 * @brief DS1307 RAM Register 24.
 */
#define D_DS1307_REG_RAM24                       0x1F

/**
 * @brief DS1307 RAM Register 25.
 */
#define D_DS1307_REG_RAM25                       0x20

/**
 * @brief DS1307 RAM Register 26.
 */
#define D_DS1307_REG_RAM26                       0x21

/**
 * @brief DS1307 RAM Register 27.
 */
#define D_DS1307_REG_RAM27                       0x22

/**
 * @brief DS1307 RAM Register 28.
 */
#define D_DS1307_REG_RAM28                       0x23

/**
 * @brief DS1307 RAM Register 29.
 */
#define D_DS1307_REG_RAM29                       0x24

/**
 * @brief DS1307 RAM Register 30.
 */
#define D_DS1307_REG_RAM30                       0x25

/**
 * @brief DS1307 RAM Register 31.
 */
#define D_DS1307_REG_RAM31                       0x26

/**
 * @brief DS1307 RAM Register 32.
 */
#define D_DS1307_REG_RAM32                       0x27

/**
 * @brief DS1307 RAM Register 33.
 */
#define D_DS1307_REG_RAM33                       0x28

/**
 * @brief DS1307 RAM Register 34.
 */
#define D_DS1307_REG_RAM34                       0x29

/**
 * @brief DS1307 RAM Register 35.
 */
#define D_DS1307_REG_RAM35                       0x2A

/**
 * @brief DS1307 RAM Register 36.
 */
#define D_DS1307_REG_RAM36                       0x2B

/**
 * @brief DS1307 RAM Register 37.
 */
#define D_DS1307_REG_RAM37                       0x2C

/**
 * @brief DS1307 RAM Register 38.
 */
#define D_DS1307_REG_RAM38                       0x2D

/**
 * @brief DS1307 RAM Register 39.
 */
#define D_DS1307_REG_RAM39                       0x2E

/**
 * @brief DS1307 RAM Register 40.
 */
#define D_DS1307_REG_RAM40                       0x2F

/**
 * @brief DS1307 RAM Register 41.
 */
#define D_DS1307_REG_RAM41                       0x30

/**
 * @brief DS1307 RAM Register 42.
 */
#define D_DS1307_REG_RAM42                       0x31

/**
 * @brief DS1307 RAM Register 43.
 */
#define D_DS1307_REG_RAM43                       0x32

/**
 * @brief DS1307 RAM Register 44.
 */
#define D_DS1307_REG_RAM44                       0x33

/**
 * @brief DS1307 RAM Register 45.
 */
#define D_DS1307_REG_RAM45                       0x34

/**
 * @brief DS1307 RAM Register 46.
 */
#define D_DS1307_REG_RAM46                       0x35

/**
 * @brief DS1307 RAM Register 47.
 */
#define D_DS1307_REG_RAM47                       0x36

/**
 * @brief DS1307 RAM Register 48.
 */
#define D_DS1307_REG_RAM48                       0x37

/**
 * @brief DS1307 RAM Register 49.
 */
#define D_DS1307_REG_RAM49                       0x38

/**
 * @brief DS1307 RAM Register 50.
 */
#define D_DS1307_REG_RAM50                       0x39

/**
 * @brief DS1307 RAM Register 51.
 */
#define D_DS1307_REG_RAM51                       0x3A

/**
 * @brief DS1307 RAM Register 52.
 */
#define D_DS1307_REG_RAM52                       0x3B

/**
 * @brief DS1307 RAM Register 53.
 */
#define D_DS1307_REG_RAM53                       0x3C

/**
 * @brief DS1307 RAM Register 54.
 */
#define D_DS1307_REG_RAM54                       0x3D

/**
 * @brief DS1307 RAM Register 55.
 */
#define D_DS1307_REG_RAM55                       0x3E

/**
 * @brief DS1307 RAM Register 56.
 */
#define D_DS1307_REG_RAM56                       0x3F

/* DS1307 HOURS FORMAT */
/**
 * @brief 12-hour time format.
 */
#define D_DS1307_TF_12HR                         0x01

/**
 * @brief 24-hour time format.
 */
#define D_DS1307_TF_24HR                         0x02

/* DS1307 TIME FORMAT */
/**
 * @brief AM indicator.
 */
#define D_DS1307_IND_AM                          0x01

/**
 * @brief PM indicator.
 */
#define D_DS1307_IND_PM                           0x02

/* DS1307 DAYS FORMAT */
/**
 * @brief Sunday.
 */
#define D_DS1307_SUNDAY                          0x01

/**
 * @brief Monday.
 */
#define D_DS1307_MONDAY                          0x02

/**
 * @brief Tuesday.
 */
#define D_DS1307_TUESDAY                         0x03

/**
 * @brief Wednesday.
 */
#define D_DS1307_WEDNESDAY                       0x04

/**
 * @brief Thursday.
 */
#define D_DS1307_THURSDAY                        0x05

/**
 * @brief Friday.
 */
#define D_DS1307_FRIDAY                          0x06

/**
 * @brief Saturday.
 */
#define D_DS1307_SATURDAY                        0x07

/* DS1307 BIT DEFINATIONS */
/**
 * @brief Set bit.
 */
#define D_DS1307_SET                             1

/**
 * @brief Reset bit.
 */
#define D_DS1307_RESET                           0

/**
 * @brief Enable bit.
 */
#define D_DS1307_ENABLE                          D_DS1307_SET

/**
 * @brief Disable bit.
 */
#define D_DS1307_DISABLE                         D_DS1307_RESET

/**
 * @brief Clock Halt bit.
 */
#define D_DS1307_BIT_CH                          7

/**
 * @brief Hours bit.
 */
#define D_DS1307_BIT_HRS                         6

/**
 * @brief AM/PM bit.
 */
#define D_DS1307_BIT_AMPM                        5

/**
 * @brief Output Control bit.
 */
#define D_DS1307_BIT_OUT                         7

/**
 * @brief Square Wave Enable bit.
 */
#define D_DS1307_BIT_SQWE                        4

/**
 * @brief Rate Select bit 1.
 */
#define D_DS1307_BIT_RS1                         1

/**
 * @brief Rate Select bit 0.
 */
#define D_DS1307_BIT_RS0                         0

/**
 * @brief Enum for DS1307 square wave output configurations.
 * This enumeration defines the available options for configuring the square wave output 
 * on the DS1307 Real-Time Clock (RTC). The square wave output can be set to various frequencies
 * or disabled with different logic levels.
 */
typedef enum
{
    _1Hz = 0x10,      /**< 1 Hz square wave output. */
    _4096Hz = 0x11,   /**< 4.096 kHz square wave output. */
    _8192Hz = 0x12,   /**< 8.192 kHz square wave output. */
    _32768Hz = 0x13,  /**< 32.768 kHz square wave output. */
    _No_Output_1 = 0x80, /**< No square wave output and SQW/OUT pin is HIGH. */
    _No_Output_0 = 0x00, /**< No square wave output and SQW/OUT pin is LOW. */
} DS1307_SQWO_t;


/**
 * @brief Enum for DS1307 status codes.
 * This enumeration defines the various status codes that can be returned by DS1307 
 * related functions to indicate the outcome of operations.
 */
typedef enum
{
    DS1307_OK = 0,               /**< Operation completed successfully. */
    DS1307_ERROR = 1,            /**< Operation failed due to an error. */
    DS1307_BUSY = 2,             /**< The DS1307 is currently busy and cannot handle the request. */
    DS1307_TIMEOUT_ERR = 3,      /**< Operation timed out. */
    DS1307_NOT_FOUND = 4,        /**< DS1307 device not found on the I2C bus. */
    DS1307_DATA_SIZE_ERROR = 5,  /**< The size of the data to be written or read is incorrect. */
} DS1307_Status_t;

/**
 * @brief Structure for representing time in the DS1307 RTC.
 * This structure holds the time values including hours, minutes, and seconds.
 */
typedef struct
{
    uint8_t Hour; /**< Hours value (0-23 or 1-12 depending on 24-hour or 12-hour format). */
    uint8_t Min;  /**< Minutes value (0-59). */
    uint8_t Sec;  /**< Seconds value (0-59). */
} DS1307_Time_t;

/**
 * @brief Structure for representing date in the DS1307 RTC.
 * This structure holds the date values including day, date, month, and year.
 */
typedef struct
{
    uint8_t Day;   /**< Day of the week (1-7 where 1 is Sunday). */
    uint8_t Date;  /**< Date of the month (1-31). */
    uint8_t Month; /**< Month of the year (1-12). */
    uint8_t Year;  /**< Year (usually as a two-digit value representing the last two digits of the year). */
} DS1307_Date_t;

/**
 * @brief Structure for representing date and time combined in the DS1307 RTC.
 * This structure combines the date and time structures to provide a complete 
 * representation of both date and time.
 */
typedef struct
{
    DS1307_Date_t date; /**< Date information (Day, Date, Month, Year). */
    DS1307_Time_t time; /**< Time information (Hour, Min, Sec). */
} DS1307_DateTime_t;


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
DS1307_Status_t DS1307_Init(I2C_HandleTypeDef *handler, DS1307_SQWO_t sqwOut);

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
DS1307_Status_t DS1307_ReadReg(uint8_t regAdd, uint8_t *dataRead, uint8_t readLen);

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
DS1307_Status_t DS1307_WriteReg(uint8_t regAdd, uint8_t *dataWrite, uint8_t writeLen);

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
DS1307_Status_t DS1307_ReadTime_Bin(DS1307_Time_t* dataRead);

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
DS1307_Status_t DS1307_ReadTime_BCD(DS1307_Time_t* dataRead);

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
DS1307_Status_t DS1307_ReadDate_Bin(DS1307_Date_t* dataRead);

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
DS1307_Status_t DS1307_ReadDate_BCD(DS1307_Date_t* dataRead);

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
DS1307_Status_t DS1307_ReadDateTime_Bin(DS1307_DateTime_t* dataRead);

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
DS1307_Status_t DS1307_ReadDateTime_BCD(DS1307_DateTime_t* dataRead);

#endif /* _INC_DS1307_H_ */