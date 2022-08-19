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
  * Config Page  - область хранения изменяемых параметров модуля.                                                             \n
  * RO Constants - область хранения неизменяемых параметров модуля (записывается при сборке модуля).                          \n
  *                                                                                                                           \n 
  *
  * **Карта памяти Config Page**
  * | Параметр                        |   Адрес    |   0x00   |   0x01   | 0x02 | 0x03 |
  * | ------------------------------- | :--------: | :------: | :------: | :--: | :--: |
  * | Адрес модуля                    | 0x0801F000 |   addr   |   0xFF   | 0xFF | 0xFF |
  * | Скорость CAN                    | 0x0801F004 |          |          |      |      |
  * | Версия загрузчика               | 0x0801F008 | bl_minor | bl_major | 0xFF | 0xFF |
  * | Версия программы                | 0x0801F00C | sw_minor | sw_major | 0xFF | 0xFF |
  * | Флаг первого запуска            | 0x0801F010 |   flag   |   0xFF   | 0xFF | 0xFF |
  * | Не используется в модуле Modbus | 0x0801F014 |   0xFF   |   0xFF   | 0xFF | 0xFF |
  * | Параметры Modbus порта 0        | 0x0801F018 |   baud   |    par   | stop | 0xFF |
  * | Параметры Modbus порта 1        | 0x0801F01C |   baud   |    par   | stop | 0xFF |
  * | Параметры Modbus порта 2        | 0x0801F020 |   baud   |    par   | stop | 0xFF |
  * | Параметры Modbus порта 3        | 0x0801F024 |   baud   |    par   | stop | 0xFF |
  *
  * Страница Config Page содержит параметры модуля, записанные в виде 32-битных слов.
  * \n \n 
  *
  * **Карта памяти RO Constants**
  * | Параметр           |   Адрес    |   0x00   |   0x01   | 0x02 | 0x03 |
  * | ------------------ | :--------: | :------: | :------: | :--: | :--: |
  * | Тип модуля         | 0x0801F800 |  class   |   0xFF   | 0xFF | 0xFF |
  * | Аппаратная ревизия | 0x0801F804 | hw_minor | hw_major | 0xFF | 0xFF |
  * | Серийный номер lw  | 0x0801F808 |   0xFF   |   0xFF   | 0xFF | 0xFF |
  * | Серийный номер hw  | 0x0801F80C |   0xFF   |   0xFF   | 0xFF | 0xFF |
  *
  * Страница RO Constants содержит параметры модуля, записанные в виде 32-битных слов.
  * \n \n 
  ******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FLASH_H
#define __FLASH_H

//---Includes-------------------------------------------------------------------//
#include <stdint.h>
//------------------------------------------------------------------------------//

//---Defines--------------------------------------------------------------------//
#define NUM_OF_CONFIG_WORDS 10U /*!< Количество параметров модуля (в виде 32-битных слов), которые будут записываться в область Config Page. */

#define CAN_SPEED_ADDR_IN_FLASH         0x0801F004    /*!< Адрес FLASH памяти (в области Config Page), где хранится значение скорости CAN.                                       */
#define MODULE_ADDR_IN_FLASH            0x0801F000    /*!< Адрес FLASH памяти (в области Config Page), где хранится cобственный адрес модуля (микроконтроллера STM, GD, AT ...). */
#define HARDWARE_REVISION_ADDR_IN_FLASH 0x0801F804    /*!< Адрес FLASH памяти (в области RO Constants), где хранится значение аппаратной ревизии.                                */
#define SERIAL_NUMBER_ADDR_IN_FLASH     0x0801F808    /*!< Адрес FLASH памяти (в области RO Constants), где хранится значение серийного номера lw.                               */
#define RO_CONST_START_ADDR_IN_FLASH    0x0801F800    /*!< Начальный адрес во FLASH памяти (в области RO Constants), где хранится карта памяти RO Constants.                     */

#define MODULE_ADDR_MASK                (uint8_t)0x1F /*!< Маска для cобственного адреса модуля (микроконтроллера STM, GD, AT ...).                                              */

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
uint32_t         AddrModule;        /*!< Адрес модуля.                  */
uint32_t         CanSpeed;          /*!< Скорость работы CAN (битрейт). */
Version          BootloaderVersion; /*!< Версия загрузчика.             */
uint16_t         reserved_bver;     /*!< Зарезервировано.               */
Version          ProgramVersion;    /*!< Версия программы.              */
uint16_t         reserved_pver;     /*!< Зарезервировано.               */
uint32_t         FirstRunFlag;      /*!< Флаг первого запуска.          */
uint32_t         reserved_unused;   /*!< Зарезервировано.               */
ModbusPortParam  ModbusPort0Param;  /*!< Параметры Modbus порта 0.      */
ModbusPortParam  ModbusPort1Param;  /*!< Параметры Modbus порта 1.      */
ModbusPortParam  ModbusPort2Param;  /*!< Параметры Modbus порта 2.      */
ModbusPortParam  ModbusPort3Param;  /*!< Параметры Modbus порта 3.      */
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
flash_status  Write_Config_to_flash        (Config_struct* Config);
void          Read_Config_from_flash       (Config_struct* Config);
void          Read_RO_Constants_from_flash (RO_Constants_struct* RO_Constants);
flash_status  Write_Words_to_flash         (uint32_t Address, uint32_t Amount, uint32_t *Words);
uint16_t      Read_MCU_FMD                 (void);
//------------------------------------------------------------------------------//

  
#endif /* __FLASH_H */

//***********************************END OF FILE***********************************
