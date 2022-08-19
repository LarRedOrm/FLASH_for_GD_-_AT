/**
  ******************************************************************************
  *
  * @file      FLASH.h
  *
  * @brief     Header for FLASH.c file.
  *
  * @copyright Copyright (C) 2022 Awada Systems. ��� ����� ��������.
  *
  * @author    Larionov A.S. (larion.alex@mail.ru)
  *
  * Config Page  - ������� �������� ���������� ���������� ������.                                                             \n
  * RO Constants - ������� �������� ������������ ���������� ������ (������������ ��� ������ ������).                          \n
  *                                                                                                                           \n 
  *
  * **����� ������ Config Page**
  * | ��������                        |   �����    |   0x00   |   0x01   | 0x02 | 0x03 |
  * | ------------------------------- | :--------: | :------: | :------: | :--: | :--: |
  * | ����� ������                    | 0x0801F000 |   addr   |   0xFF   | 0xFF | 0xFF |
  * | �������� CAN                    | 0x0801F004 |          |          |      |      |
  * | ������ ����������               | 0x0801F008 | bl_minor | bl_major | 0xFF | 0xFF |
  * | ������ ���������                | 0x0801F00C | sw_minor | sw_major | 0xFF | 0xFF |
  * | ���� ������� �������            | 0x0801F010 |   flag   |   0xFF   | 0xFF | 0xFF |
  * | �� ������������ � ������ Modbus | 0x0801F014 |   0xFF   |   0xFF   | 0xFF | 0xFF |
  * | ��������� Modbus ����� 0        | 0x0801F018 |   baud   |    par   | stop | 0xFF |
  * | ��������� Modbus ����� 1        | 0x0801F01C |   baud   |    par   | stop | 0xFF |
  * | ��������� Modbus ����� 2        | 0x0801F020 |   baud   |    par   | stop | 0xFF |
  * | ��������� Modbus ����� 3        | 0x0801F024 |   baud   |    par   | stop | 0xFF |
  *
  * �������� Config Page �������� ��������� ������, ���������� � ���� 32-������ ����.
  * \n \n 
  *
  * **����� ������ RO Constants**
  * | ��������           |   �����    |   0x00   |   0x01   | 0x02 | 0x03 |
  * | ------------------ | :--------: | :------: | :------: | :--: | :--: |
  * | ��� ������         | 0x0801F800 |  class   |   0xFF   | 0xFF | 0xFF |
  * | ���������� ������� | 0x0801F804 | hw_minor | hw_major | 0xFF | 0xFF |
  * | �������� ����� lw  | 0x0801F808 |   0xFF   |   0xFF   | 0xFF | 0xFF |
  * | �������� ����� hw  | 0x0801F80C |   0xFF   |   0xFF   | 0xFF | 0xFF |
  *
  * �������� RO Constants �������� ��������� ������, ���������� � ���� 32-������ ����.
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
#define NUM_OF_CONFIG_WORDS 10U /*!< ���������� ���������� ������ (� ���� 32-������ ����), ������� ����� ������������ � ������� Config Page. */

#define CAN_SPEED_ADDR_IN_FLASH         0x0801F004    /*!< ����� FLASH ������ (� ������� Config Page), ��� �������� �������� �������� CAN.                                       */
#define MODULE_ADDR_IN_FLASH            0x0801F000    /*!< ����� FLASH ������ (� ������� Config Page), ��� �������� c���������� ����� ������ (���������������� STM, GD, AT ...). */
#define HARDWARE_REVISION_ADDR_IN_FLASH 0x0801F804    /*!< ����� FLASH ������ (� ������� RO Constants), ��� �������� �������� ���������� �������.                                */
#define SERIAL_NUMBER_ADDR_IN_FLASH     0x0801F808    /*!< ����� FLASH ������ (� ������� RO Constants), ��� �������� �������� ��������� ������ lw.                               */
#define RO_CONST_START_ADDR_IN_FLASH    0x0801F800    /*!< ��������� ����� �� FLASH ������ (� ������� RO Constants), ��� �������� ����� ������ RO Constants.                     */

#define MODULE_ADDR_MASK                (uint8_t)0x1F /*!< ����� ��� c����������� ������ ������ (���������������� STM, GD, AT ...).                                              */

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
  * @brief ��������� ��� �������� �������� ������.
  */
#pragma pack(push, 2)
typedef struct{
uint8_t minor; /*!< �����. */
uint8_t major; /*!< �����. */
} Version;


/**
  * @brief ��������� ��� �������� ���������� Modbus.
  */
typedef struct{
uint8_t baud;           /*!< Boudrate.            */
uint8_t par;            /*!< Parity.              */
uint8_t stop;           /*!< Number of stop bits. */
uint8_t reserved_param; /*!< ���������������.     */
} ModbusPortParam;
#pragma pack(pop)


/**
  * @brief ��������� ��� �������� ������ Config Page.
  */
typedef struct{
uint32_t         AddrModule;        /*!< ����� ������.                  */
uint32_t         CanSpeed;          /*!< �������� ������ CAN (�������). */
Version          BootloaderVersion; /*!< ������ ����������.             */
uint16_t         reserved_bver;     /*!< ���������������.               */
Version          ProgramVersion;    /*!< ������ ���������.              */
uint16_t         reserved_pver;     /*!< ���������������.               */
uint32_t         FirstRunFlag;      /*!< ���� ������� �������.          */
uint32_t         reserved_unused;   /*!< ���������������.               */
ModbusPortParam  ModbusPort0Param;  /*!< ��������� Modbus ����� 0.      */
ModbusPortParam  ModbusPort1Param;  /*!< ��������� Modbus ����� 1.      */
ModbusPortParam  ModbusPort2Param;  /*!< ��������� Modbus ����� 2.      */
ModbusPortParam  ModbusPort3Param;  /*!< ��������� Modbus ����� 3.      */
} Config_struct;


/**
  * @brief ��������� ��� �������� ������ RO Constants.
  */
typedef struct{
uint32_t ModulType;        /*!< ��� ������.                   */
Version  HardwareRevision; /*!< ���������� �������.           */
uint16_t reserved_hrev;    /*!< ���������������.              */
uint32_t SerialNumberLW;   /*!< �������� ����� ������� �����. */
uint32_t SerialNumberHW;   /*!< �������� ����� ������� �����. */
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
