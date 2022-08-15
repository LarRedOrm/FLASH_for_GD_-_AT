/**
  ******************************************************************************
  *
  * @file      FLASH.h
  *
  * @brief     Header for FLASH.c file.
  *
  * @copyright Copyright (C) 2022 Awada Systems. Все права защищены.
  *
  * @author    Larionov A.S. (larion.alex@mail.ru)
  *
  ******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FLASH_H
#define __FLASH_H

//---Includes-------------------------------------------------------------------//
#include <stdint.h>
//------------------------------------------------------------------------------//

//---Defines--------------------------------------------------------------------//
#define NUM_OF_CONFIG_WORDS 9U /*!< Количество параметров модуля (в виде 32-битных слов), которые будут записываться в область Config Page. */
//------------------------------------------------------------------------------//

//---Private types--------------------------------------------------------------//
//------------------------------------------------------------------------------//

//---Exported types-------------------------------------------------------------//
/**
  * @brief  flash status enum
  */
typedef enum
{
FLASH_WROG_ADDRES = 0, /*!< flash status is wrong address   */
FLASH_OK             , /*!< flash status is operate done    */
FLASH_ERROR            /*!< flash status is operate busy    \n 
                            flash status is program error   \n 
                            flash status is epp error       \n 
                            flash status is operate done    \n 
                            flash status is operate timeout */
} flash_status;

/**
  * @brief Структура для хранения значения версии.
  */
#pragma pack(push, 2)
typedef struct{
uint8_t minor; /*!< Минор. */
uint8_t major; /*!< Мажор. */
} Version;


/**
  * @brief Структура для хранения параметров Modbus.
  */
typedef struct{
uint8_t baud;           /*!< Boudrate.            */
uint8_t par;            /*!< Parity.              */
uint8_t stop;           /*!< Number of stop bits. */
uint8_t reserved_param; /*!< Зарезервировано.     */
} ModbusPortParam;
#pragma pack(pop)


/**
  * @brief Структура для хранения данных Config Page.
  */
typedef struct{
uint32_t         AddrModule;        /*!< Адрес модуля.             */
Version          BootloaderVersion; /*!< Версия загрузчика.        */
uint16_t         reserved_bver;     /*!< Зарезервировано.          */
Version          ProgramVersion;    /*!< Версия программы.         */
uint16_t         reserved_pver;     /*!< Зарезервировано.          */
uint32_t         FirstRunFlag;      /*!< Флаг первого запуска.     */
uint32_t         reserved_unused;   /*!< Зарезервировано.          */
ModbusPortParam  ModbusPort0Param;  /*!< Параметры Modbus порта 0. */
ModbusPortParam  ModbusPort1Param;  /*!< Параметры Modbus порта 1. */
ModbusPortParam  ModbusPort2Param;  /*!< Параметры Modbus порта 2. */
ModbusPortParam  ModbusPort3Param;  /*!< Параметры Modbus порта 3. */
} Config_struct;


/**
  * @brief Структура для хранения данных RO Constants.
  */
typedef struct{
uint32_t ModulType;        /*!< Тип модуля.                   */
Version  HardwareRevision; /*!< Аппаратная ревизия.           */
uint16_t reserved_hrev;    /*!< Зарезервировано.              */
uint32_t SerialNumberLW;   /*!< Серийный номер младшее слово. */
uint32_t SerialNumberHW;   /*!< Серийный номер старшее слово. */
} RO_Constants_struct;
//------------------------------------------------------------------------------//


//---Function prototypes--------------------------------------------------------//
flash_status  Write_Config_to_flash        (uint32_t *Data);
void          Read_Config_from_flash       (Config_struct* Config);
void          Read_RO_Constants_from_flash (RO_Constants_struct* RO_Constants);
flash_status  Write_Words_to_flash         (uint32_t Address, uint32_t Amount, uint32_t *Words);
//------------------------------------------------------------------------------//

  
#endif /* __FLASH_H */

//***********************************END OF FILE***********************************
