/**
  ******************************************************************************
  * 
  * @file      AT_flash.c
  * 
  * @brief     ������� flash ��� AT32F413RCT7.
  * 
  * @details   ������� ��� ������ � flash � ����������������� ��������� AT32F413.
  *
  * @warning   **�����!**                                                                                              \n 
  * ��� ������ �������� ���������: 
  *                       + ���� at32f413_flash.c - ���������� ������� ������ � flash �� Artery;
  *                       + ���� at32f413_flash.h - header for at32f413_flash.c.
  *
  * **Manual** \n 
  * � �������� ����������� ��������� �������:
  * - Write_Config_to_flash (uint32_t *Data) - ������ �� FLASH ���������� ���������� ������ Config Page.               \n 
  *   ���������� ��������� ������ ����� ��������� � ��������� ���� uint32_t ��� ���� Config_struct.                    \n 
  *   ���������� ����� ��� ������/������ ���������� ���������� ������ ��/�� FLASH �������� �������� ADDR_CONFIG_PAGE.
  *
  * - Read_Config_from_flash (Config_struct* Config) - ������ ���������� ���������� ������ �� FLASH                    \n 
  *   � ��������� ���� Config_struct.
  *   
  * - Read_RO_Constants_from_flash (RO_Constants_struct* RO_Constants) - ������ ������������ ���������� ������         \n 
  *   � ��������� ���� RO_Constants_struct.
  *
  * - Write_Words_to_flash (uint32_t Address, uint32_t Amount, uint32_t *Words) - ������ �� FLASH ������������� ������� ����.
  *
  * **����������� ����������� FLASH ������ � �������** \n 
  * � ������������ ����� AT32F403AR ������ FLASH ����� ����� ��������� ��������:
  * - 256 Kbyte  (AT32F403ARC),
  * - 512 Kbyte  (AT32F403ARE),
  * - 1024 Kbyte (AT32F403ARG).
  * \n \n 
  *
  * **����������� FLASH ������ ��� AT32F403ARG (1024 Kbyte)** \n 
  *
  * | Block             | Bank               | Name              | Address Range             | Size (bytes) |
  * | ----------------- | :----------------: | :---------------: | :-----------------------: | :----------: |
  * | Main Flash memory | Bank 1 (512 Kbyte) | Page 0            | 0x0800 0000 � 0x0800 07FF | 2 KB         |
  * | ^                 | ^                  | Page 1            | 0x0800 0800 � 0x0800 0FFF | 2 KB         |
  * | ^                 | ^                  | Page 2            | 0x0800 1000 � 0x0800 17FF | 2 KB         |
  * | ^                 | ^                  | ...               | ...                       | ...          |
  * | ^                 | ^                  | Page 255          | 0x0807 F800 � 0x0807 FFFF | 2 KB         |
  * | ^                 | Bank 2 (512 Kbyte) | Page 256          | 0x0808 0000 � 0x0808 07FF | 2 KB         |
  * | ^                 | ^                  | Page 257          | 0x0808 0800 � 0x0808 0FFF | 2 KB         |
  * | ^                 | ^                  | Page 258          | 0x0808 1000 � 0x0808 17FF | 2 KB         |
  * | ^                 | ^                  | ...               | ...                       | ...          |
  * | ^                 | ^                  | Page 511          | 0x080F F800 � 0x080F FFFF | 2 KB         |
  * | External memory   | 16 MB              | Page 0            | 0x0840 0000 � 0x0840 0FFF | 4 KB         |
  * | ^                 | ^                  | Page 1            | 0x0840 1000 � 0x0840 1FFF | 4 KB         |
  * | ^                 | ^                  | Page 2            | 0x0840 2000 � 0x0840 2FFF | 4 KB         |
  * | ^                 | ^                  | ...               | ...                       | ...          |
  * | ^                 | ^                  | Page 4095         | 0x093F F000 � 0x093F FFFF | 4 KB         |
  * | Information Block |                    | Boot loader       | 0x1FFF B000 � 0x1FFF EFFF | 16 KB        |
  * | ^                 | ^                  | User option bytes | 0x1FFF F800 � 0x1FFF F82F | 48 B         |
  *  \n 
  *
  * **����������� FLASH ������ ��� AT32F403ARE (512 Kbyte)** \n 
  *
  * | Block             | Bank               | Name              | Address Range             | Size (bytes) |
  * | ------------------| :----------------: | :---------------: | :-----------------------: | :----------: |
  * | Main Flash Block  | Bank 1 (512 Kbyte) | Page 0            | 0x0800 0000 � 0x0800 07FF | 2 KB         |
  * | ^                 | ^                  | Page 1            | 0x0800 0800 � 0x0800 0FFF | 2 KB         |
  * | ^                 | ^                  | Page 2            | 0x0800 1000 - 0x0800 17FF | 2 KB         |
  * | ^                 | ^                  | ...               | ...                       | ...          |
  * | ^                 | ^                  | Page 255          | 0x0807 F800 � 0x0807 FFFF | 2 KB         |
  * | External memory   | 16 MB              | Page 0            | 0x0840 0000 � 0x0840 0FFF | 4 KB         |
  * | ^                 | ^                  | Page 1            | 0x0840 1000 � 0x0840 1FFF | 4 KB         |
  * | ^                 | ^                  | Page 2            | 0x0840 2000 � 0x0840 2FFF | 4 KB         |
  * | ^                 | ^                  | ...               | ...                       | ...          |
  * | ^                 | ^                  | Page 4095         | 0x093F F000 � 0x093F FFFF | 4 KB         |
  * | Information Block |                    | Boot loader       | 0x1FFF B000 � 0x1FFF EFFF | 16 KB        |
  * | ^                 | ^                  | User option bytes | 0x1FFF F800 � 0x1FFF F82F | 48 B         |
  * \n 
  *
  * **����������� FLASH ������ ��� AT32F403ARC (256 Kbyte)** \n 
  *
  * | Block             | Bank               | Name              | Address Range             | Size (bytes) |
  * | ------------------| :----------------: | :---------------: | :-----------------------: | :----------: |
  * | Main Flash Block  | Bank 1 (256 Kbyte) | Page 0            | 0x0800 0000 � 0x0800 07FF | 2 KB         |
  * | ^                 | ^                  | Page 1            | 0x0800 0800 � 0x0800 0FFF | 2 KB         |
  * | ^                 | ^                  | Page 2            | 0x0800 1000 � 0x0800 17FF | 2 KB         |
  * | ^                 | ^                  | ...               | ...                       | ...          |
  * | ^                 | ^                  | Page 127          | 0x0803 F800 � 0x0803 FFFF | 2 KB         |
  * | External memory   | 16 MB              | Page 0            | 0x0840 0000 � 0x0840 0FFF | 4 KB         |
  * | ^                 | ^                  | Page 1            | 0x0840 1000 � 0x0840 1FFF | 4 KB         |
  * | ^                 | ^                  | Page 2            | 0x0840 2000 � 0x0840 2FFF | 4 KB         |
  * | ^                 | ^                  | ...               | ...                       | ...          |
  * | ^                 | ^                  | Page 4095         | 0x093F F000 � 0x093F FFFF | 4 KB         |
  * | Information Block |                    | Boot loader       | 0x1FFF B000 � 0x1FFF EFFF | 16 KB        |
  * | ^                 | ^                  | User option bytes | 0x1FFF F800 � 0x1FFF F82F | 48 B         |
  * \n 
  *
  * ������ FLASH ������������ � ������������ � ������������� (���-�� �����-��) ������ ������ (����� ������ �����������). \n 
  * ��� �������� ���� ����� ������ ��������� �� ����� 128 Kbyte FLASH ������.                                            \n 
  * ���������������, ��� ������������ ���������� ����� AT32F403AR c ������� FLASH ������ 256 Kbyte ��� �����.            \n 
  * � ������� (����) ����������� ������ ����������� FLASH ������ (� ������ ������������� ����� ������)                   \n 
  * ��� ���� ������������ ����� AT32F403AR.                                                                              \n 
  * ��� ����� ������ �� ���� ������������ ����� ����������� � Bank 1 Page0...Page63.                                     \n 
  * \n 
  * 
  * **����� ������ ��� ����������������� ����� AT32F403AR**
  * |    ����������     |        Bank        |  ����� ��������   |     �������� �������      | ���������� �����  | ����� ������� |
  * | ----------------- | :----------------: | :---------------: | :-----------------------: | :---------------: | :-----------: |
  * | Bootloader        | Bank 1 (256 Kbyte) | Page 0            | 0x0800 0000 � 0x0800 07FF | 28 Kbyte (0x7000) | 14 �� 2 KB    |
  * | ^                 | ^                  | ...               | ...                       | ^                 | ^             |
  * | ^                 | ^                  | Page 13           | 0x0800 6800 � 0x0800 6FFF | ^                 | ^             |
  * | Main Programm     | ^                  | Page 14           | 0x0800 7000 � 0x0800 77FF | 48 Kbyte (0xC000) | 24 �� 2 KB    |
  * | ^                 | ^                  | ...               | ...                       | ^                 | ^             |
  * | ^                 | ^                  | Page 37           | 0x0801 2800 � 0x0801 2FFF | ^                 | ^             |
  * | Download Buffer   | ^                  | Page 38           | 0x0800 3000 � 0x0800 37FF | 48 Kbyte (0xC000) | 24 �� 2 KB    |
  * | ^                 | ^                  | ...               | ...                       | ^                 | ^             |
  * | ^                 | ^                  | Page 61           | 0x0801 E800 � 0x0801 EFFF | ^                 | ^             |
  * | Config Page       | ^                  | Page 62           | 0x0801 F000 � 0x0801 F7FF | 2 Kbyte (0x800)   | 1 �� 2 KB     |
  * | RO Constants      | ^                  | Page 63           | 0x0801 F800 � 0x0801 FFFF | 2 Kbyte (0x800)   | 1 �� 2 KB     |
  * | Unused            | ^                  | Page 64           | 0x0802 0000 � 0x0802 07FF | 2 Kbyte (0x800)   | 1 �� 2 KB     |
  * | ^                 | ^                  | ...               | ...                       | ...               | ...           |
  * | ^                 | ^                  | Page 127          | 0x0807 F800 � 0x0807 FFFF | 2 Kbyte (0x800)   | 1 �� 2 KB     |
  * | ^                 | ^                  | ...               | ...                       | ...               | ...           |
  * | ^                 | ...                | ...               | ...                       | ...               | ...           |
  * | Information Block ||                     Boot loader       | 0x1FFF B000 � 0x1FFF EFFF | 16 KB             |               |
  * | ^                 ||                     User option bytes | 0x1FFF F800 � 0x1FFF F82F | 48 B              |               |
  * 
  * Bootloader      - ������� ������������ ����������                                                  \n
  * Main Programm   - ������� ������������ �������� ���������                                          \n
  * Download Buffer - ������� ��� ���������� ����� �������� (������������ �����������)                 \n
  * Config Page     - ������� �������� ���������� ���������� ������                                    \n
  * RO Constants    - ������� �������� ������������ ���������� ������ (������������ ��� ������ ������) \n
  * \n 
  *
  * **����� ������ Config Page**
  * | ��������                        |   �����    | 0x00  | 0x01  | 0x02 | 0x03 |
  * | ------------------------------- | :--------: | :---: | :---: | :--: | :--: |
  * | ����� ������                    | 0x0801F000 | addr  | 0xFF  | 0xFF | 0xFF |
  * | ������ ����������               | 0x0801F004 | minor | major | 0xFF | 0xFF |
  * | ������ ���������                | 0x0801F008 | minor | major | 0xFF | 0xFF |
  * | ���� ������� �������            | 0x0801F00C | flag  | 0xFF  | 0xFF | 0xFF |
  * | �� ������������ � ������ Modbus | 0x0801F010 | 0xFF  | 0xFF  | 0xFF | 0xFF |
  * | ��������� Modbus ����� 0        | 0x0801F014 | baud  |  par  | stop | 0xFF |
  * | ��������� Modbus ����� 1        | 0x0801F018 | baud  |  par  | stop | 0xFF |
  * | ��������� Modbus ����� 2        | 0x0801F01C | baud  |  par  | stop | 0xFF |
  * | ��������� Modbus ����� 3        | 0x0801F020 | baud  |  par  | stop | 0xFF |
  * �������� Config Page �������� ��������� ������, ���������� � ���� 32-������ ����.
  * \n \n 
  *
  * **����� ������ RO Constants**
  * | ��������           |   �����    | 0x00  | 0x01  | 0x02 | 0x03 |
  * | ------------------ | :--------: | :---: | :---: | :--: | :--: |
  * | ��� ������         | 0x0801F800 | class | 0xFF  | 0xFF | 0xFF |
  * | ���������� ������� | 0x0801F804 | minor | major | 0xFF | 0xFF |
  * | �������� ����� lw  | 0x0801F808 | 0xFF  | 0xFF  | 0xFF | 0xFF |
  * | �������� ����� hw  | 0x0801F80C | 0xFF  | 0xFF  | 0xFF | 0xFF |
  *
  * �������� RO Constants �������� ��������� ������, ���������� � ���� 32-������ ����.
  * \n \n 
  *
  * 
  * @copyright Copyright (C) 2022 Awada Systems. ��� ����� ��������.
  *
  * @author    Larionov A.S. (larion.alex@mail.ru)
  * 
  ******************************************************************************
**/

//---Includes-------------------------------------------------------------------//
#include "AT_flash.h"
#include "at32f413_flash.h"
//------------------------------------------------------------------------------//

//---Private macros-------------------------------------------------------------//
#define FLASH_SIZE            (*(uint32_t*)0x1FFFF7E0)             /*!< Flash size, in terms of KByte.               */
#define PAGE0_ADDR            0x08000000U                          /*!< ����� ������ ������ �������� FLASH.          */
#define END_ADDR_OF_LAST_PAGE (PAGE0_ADDR + FLASH_SIZE * 1024 - 1) /*!< ����� ���������� ����� � ��������� ��������. */

//---������� ��������� FLASH ������ � Kbyte ��� ���������� ��������������� ��������---//
#define MEMSIZE_BOOTLOADER       28 /*!< ������ ��������� FLASH ������ � Kbyte.       */
#define MEMSIZE_MAIN_PROGRAM     48 /*!< ������ ��������� FLASH ������ � Kbyte.       */
#define MEMSIZE_DOWNLOAD_BUFFER  48 /*!< ������ ��������� FLASH ������ � Kbyte.       */
#define MEMSIZE_CONFIG_PAGE      2  /*!< ������ ��������� FLASH ������ � Kbyte.       */
#define MEMSIZE_RO_CONSTANS      2  /*!< ������ ��������� FLASH ������ � Kbyte.       */
//------------------------------------------------------------------------------------//

//---��������� ������ ��������������� �������� �� FLASH---//
#define ADDR_BOOTLOADER       PAGE0_ADDR                                              /*!< ��������� ����� ������ BootLoader.                   */
#define ADDR_MAIN_PROGRAM     (ADDR_BOOTLOADER      + MEMSIZE_BOOTLOADER      * 1024) /*!< 0x08007000U // ��������� ����� ������ MainProgram.   */
#define ADDR_DOWNLOAD_BUFFER  (ADDR_MAIN_PROGRAM    + MEMSIZE_MAIN_PROGRAM    * 1024) /*!< 0x08013000U // ��������� ����� ������ DowloadBuffer. */
#define ADDR_CONFIG_PAGE      (ADDR_DOWNLOAD_BUFFER + MEMSIZE_DOWNLOAD_BUFFER * 1024) /*!< 0x0801F000U // ��������� ����� ������ ConfigPage.    */
#define ADDR_RO_CONSTANS      (ADDR_CONFIG_PAGE     + MEMSIZE_CONFIG_PAGE     * 1024) /*!< 0x0801F800U // ��������� ����� ������ RO_Constans.   */
//--------------------------------------------------------//

#define DEF_FLASH_ADDR (END_ADDR_OF_LAST_PAGE - PAGE_SIZE_2KB + 1 ) /*!< ����� ��� ������ �� ��������� - ����� ������ ��������� �������� flash (0x803F800). */

//------------------------------------------------------------------------------//

//---Exported variables---------------------------------------------------------//
//------------------------------------------------------------------------------//

//---Private types--------------------------------------------------------------//
//------------------------------------------------------------------------------//

//---Private variables----------------------------------------------------------//
//------------------------------------------------------------------------------//

//---Private constants----------------------------------------------------------//
//------------------------------------------------------------------------------//

//---Function prototypes--------------------------------------------------------//
//------------------------------------------------------------------------------//

//---Exported functions---------------------------------------------------------//
/**
  * @brief   ������ Config �� FLASH.
  * @param   Data - ��������� ���� uint32_t* �� ��������� � ������� Config.
  * @return  flash status.
  */
flash_status Write_Config_to_flash (uint32_t* Data)
{
flash_status_type state;

flash_unlock(); // Unlock the main FMC operation.
state = flash_sector_erase(ADDR_CONFIG_PAGE);
if (state != FLASH_OPERATE_DONE)
  return FLASH_ERROR;
else
  {
  for (uint8_t i = 0; i < NUM_OF_CONFIG_WORDS; i++)
    {
    state = flash_word_program(ADDR_CONFIG_PAGE + 4*i, *(Data+i)); // Program a word at the corresponding address.
    if (state != FLASH_OPERATE_DONE)
      {
      break;
      }
    }
  }
flash_lock(); // Lock the main FMC operation.

if (state != FLASH_OPERATE_DONE)
  return FLASH_ERROR;
else
  return FLASH_OK;
}
//------------------------------------------------------------------------------//


/**
  * @brief   ������ Config �� FLASH.
  * @param   Config - ��������� ���� Config_struct* �� ��������� � ������� Config.
  * @return  None.
  */
void Read_Config_from_flash (Config_struct* Config)
{
*Config = *((Config_struct*)ADDR_CONFIG_PAGE);
}
//------------------------------------------------------------------------------//


/**
  * @brief   ������ RO_Constants �� FLASH.
  * @param   RO_Constants - ��������� ���� RO_Constants_struct* �� ��������� � ������� RO_Constants.
  * @return  None.
  */
void Read_RO_Constants_from_flash (RO_Constants_struct* RO_Constants)
{
*RO_Constants = *((RO_Constants_struct*)ADDR_RO_CONSTANS);
}
//------------------------------------------------------------------------------//


/**
  * @brief   ������ ������� ������ �� FLASH.
  * @details ������� ������ ������� ���� �� FLASH.
  * @param   Address - ����� ��������� ������ ������.
  * @param   Amount  - ���������� ������������ ����.
  * @param   Words   - ��������� ���� uint32_t* �� ������ � �������. ������ ������ ���� ��������� �� 4 �����.
  * @return  flash status.
  */
flash_status Write_Words_to_flash (uint32_t Address, uint32_t Amount, uint32_t *Words)
{
flash_status_type state;

if ( (Address < PAGE0_ADDR) || (Address > END_ADDR_OF_LAST_PAGE) )
  return FLASH_WROG_ADDRES;

flash_unlock(); // Unlock the main FMC operation.
state = flash_sector_erase(Address);
if (state != FLASH_OPERATE_DONE)
  return FLASH_ERROR;
else
  {
  for (uint32_t i = 0; i < Amount; i++)
    {
    state = flash_word_program(Address + 4*i, (uint32_t)*(Words + i)); // Program a halfword at the corresponding address.
    if (state != FLASH_OPERATE_DONE)
      {
      break;
      }
    }
  }
flash_lock(); // Lock the main FMC operation.

if (state != FLASH_OPERATE_DONE)
  return FLASH_ERROR;
else
  return FLASH_OK;
}
//------------------------------------------------------------------------------//
//------------------------------------------------------------------------------//


//---Private functions----------------------------------------------------------//
//------------------------------------------------------------------------------//


//***************************************END OF FILE**************************************//
