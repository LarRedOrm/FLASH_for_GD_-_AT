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
  ******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FLASH_H
#define __FLASH_H

//---Includes-------------------------------------------------------------------//
#include <stdint.h>
//------------------------------------------------------------------------------//

//---Defines--------------------------------------------------------------------//
#define NUM_OF_CONFIG_WORDS 9U /*!< ���������� ���������� ������ (� ���� 32-������ ����), ������� ����� ������������ � ������� Config Page. */
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
uint32_t         AddrModule;        /*!< ����� ������.             */
Version          BootloaderVersion; /*!< ������ ����������.        */
uint16_t         reserved_bver;     /*!< ���������������.          */
Version          ProgramVersion;    /*!< ������ ���������.         */
uint16_t         reserved_pver;     /*!< ���������������.          */
uint32_t         FirstRunFlag;      /*!< ���� ������� �������.     */
uint32_t         reserved_unused;   /*!< ���������������.          */
ModbusPortParam  ModbusPort0Param;  /*!< ��������� Modbus ����� 0. */
ModbusPortParam  ModbusPort1Param;  /*!< ��������� Modbus ����� 1. */
ModbusPortParam  ModbusPort2Param;  /*!< ��������� Modbus ����� 2. */
ModbusPortParam  ModbusPort3Param;  /*!< ��������� Modbus ����� 3. */
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
flash_status  Write_Config_to_flash        (uint32_t *Data);
void          Read_Config_from_flash       (Config_struct* Config);
void          Read_RO_Constants_from_flash (RO_Constants_struct* RO_Constants);
flash_status  Write_Words_to_flash         (uint32_t Address, uint32_t Amount, uint32_t *Words);
//------------------------------------------------------------------------------//

  
#endif /* __FLASH_H */

//***********************************END OF FILE***********************************
