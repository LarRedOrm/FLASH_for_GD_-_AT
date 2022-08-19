/**
  ******************************************************************************
  * 
  * @file      FLASH_GD32F103R.c
  * 
  * @brief     ������� flash ��� GD32F103R.
  * 
  * @details   ������� ��� ������ � flash � ����������������� ��������� GD32F103R.
  *
  * @warning   **�����!**                                                                                  \n 
  * ��� ������ �������� ���������: 
  *                       + ���� gd32f10x_fmc.c - ���������� ������� ������ � flash �� GD;
  *                       + ���� gd32f10x_fmc.h - header for gd32f10x_fmc.c.
  *
  * **Manual** \n 
  * � �������� ����������� ��������� �������:
  * - Write_Config_to_flash (Config_struct* Config) - ������ �� FLASH ���������� ���������� ������ Config Page.        \n 
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
  * � ������������ GD32F103R ������ FLASH ����������� �� 16 KB (GD32F103R4T6) �� 3072 KB (GD32F103RKT6). \n 
  * ������ FLASH ���������� �������� ��������� (density):
  * - Medium-density devices are GD32F101xx and GD32F103xx microcontrollers which the flash memory density ranges from 16 to 128 Kbytes.
  * - High-density devices are GD32F101xx and GD32F103xx microcontrollers which the flash memory density ranges from 256 to 512 Kbytes.
  * - Extra-density devices are GD32F101xx and GD32F103xx microcontrollers which the flash memory density larger than 512 Kbytes.  \n 
  *
  * �������� density ���������� ����������� FLASH ������. \n \n 
  *
  * **����������� FLASH ������ ��� Medium-density** \n 
  * (GD32F10x_MD)
  *
  * | Block              | Name             | Address Range             | Size (bytes) |
  * | ------------------ | :--------------: | :-----------------------: | :----------: |
  * | Main Flash Block   | Page 0           | 0x0800 0000 - 0x0800 03FF | 1 KB         |
  * | ^                  | Page 1           | 0x0800 0400 - 0x0800 07FF | 1 KB         |
  * | ^                  | Page 2           | 0x0800 0800 - 0x0800 0BFF | 1 KB         |
  * | ^                  | ...              | ...                       | ...          |
  * | ^                  | Page 127         | 0x0801 FC00 - 0x0801 FFFF | 1 KB         |
  * | Information Block  | Boot loader area | 0x1FFF F000 - 0x1FFF F7FF | 2 KB         |
  * | Option bytes Block | Option bytes     | 0x1FFF F800 - 0x1FFF F80F | 16 B         |
  *  \n
  *
  * **����������� FLASH ������ ��� High-density � Extra-density** \n 
  * (GD32F10x_CL, GD32F10x_HD, GD32F10x_XD)
  *
  * | Block              | Density     | Name             | Address Range             | Size (bytes) |
  * | -------------------| :---------: | :--------------: | :-----------------------: | :----------: |
  * | Main Flash Block   | CL, HD, XD  | Page 0           | 0x0800 0000 - 0x0800 07FF | 2 KB         |
  * | ^                  | ^           | Page 1           | 0x0800 0800 - 0x0800 0FFF | 2 KB         |
  * | ^                  | ^           | Page 2           | 0x0800 1000 - 0x0800 17FF | 2 KB         |
  * | ^                  | ^           | ...              | ...                       | ...          |
  * | ^                  | ^           | Page 255         | 0x0807 F800 - 0x0807 FFFF | 2 KB         |
  * | ^                  | ^           | Page 256         | 0x0808 0000 - 0x0808 0FFF | 4 KB         |
  * | ^                  | ^           | Page 257         | 0x0808 1000 - 0x0808 1FFF | 4 KB         |
  * | ^                  | ^           | ...              | ...                       | ...          |
  * | ^                  | ^           | Page 895         | 0x082F F000 - 0x082F FFFF | 4 KB         |
  * | Information Block  | GD32F10x_HD | Boot loader area | 0x1FFF F000 - 0x1FFF F7FF | 2 KB         |
  * | ^                  | GD32F10x_XD | ^                | 0x1FFF E000 - 0x1FFF F7FF | 6 KB         |
  * | ^                  | GD32F10x_CL | ^                | 0x1FFF B000 - 0x1FFF F7FF | 18 KB        |
  * | Option bytes Block | CL, HD, XD  | Option bytes     | 0x1FFF F800 - 0x1FFF F80F | 16 B         |
  * \n 
  * ������ FLASH ������������ � ������������ � ������������� (���-�� �����-��) ������ ������ (����� ������ �����������). \n 
  * ��� �������� ���� ����� ������ ��������� �� ����� 128 Kbyte FLASH ������.                                            \n 
  * ���������������, ��� ������������ ���������� ����� GD32F103R c ������� FLASH ������ �� ����� 128 Kbyte.              \n 
  * � ���� �������� (����) ������������ ��� ������� ����������� FLASH ������ (� ������ ������������� ����� ������)       \n 
  * ��� ������������ � ���������� Medium Density � High/Extra Density.                                                   \n 
  * \n 
  * **����� ������ ��� ����������������� ����� GD32F103R Medium Density**
  * |   ����������    | ����� �������� | �������� �������          | ���������� �����  | ����� ������� |
  * | --------------- | :------------: | :-----------------------: | :---------------: | :-----------: |
  * | Bootloader      | Page 0         | 0x0800 0000 - 0x0800 03FF | 28 Kbyte (0x7000) | 28 �� 1 KB    |
  * | ^               | ...            | ...                       | ^                 | ^             |
  * | ^               | Page 27        | 0x0800 6BFF - 0x0800 6FFF | ^                 | ^             |
  * | Main Programm   | Page 28        | 0x0800 7000 - 0x0800 73FF | 48 Kbyte (0xC000) | 48 �� 1 KB    |
  * | ^               | ...            | ...                       | ^                 | ^             |
  * | ^               | Page 75        | 0x0801 2BFF - 0x0801 2FFF | ^                 | ^             |
  * | Download Buffer | Page 76        | 0x0801 3000 - 0x0800 33FF | 48 Kbyte (0xC000) | 48 �� 1 KB    |
  * | ^               | ...            | ...                       | ^                 | ^             |
  * | ^               | Page 123       | 0x0801 3400 - 0x0801 EFFF | ^                 | ^             |
  * | Config Page     | Page 124       | 0x0801 F000 - 0x0801 F3FF |  1 Kbyte (0x400)  | 2 �� 1 KB     |
  * | ^               | Page 125       | 0x0801 F400 - 0x0801 F7FF |  1 Kbyte (0x400)  | ^             |
  * | RO Constants    | Page 126       | 0x0801 F800 - 0x0801 FBFF |  1 Kbyte (0x400)  | 2 �� 1 KB     |
  * | ^               | Page 127       | 0x0801 FC00 - 0x0801 FFFF |  1 Kbyte (0x400)  | ^             |
  * \n 
  * **����� ������ ��� ����������������� ����� GD32F103R High/Extra Density**
  * |   ����������    | ����� �������� | �������� �������          | ���������� �����  | ����� ������� |
  * | --------------- | :------------: | :-----------------------: | :---------------: | :-----------: |
  * | Bootloader      | Page 0         | 0x0800 0000 - 0x0800 07FF | 28 Kbyte (0x7000) | 14 �� 2 KB    |
  * | ^               | ...            | ...                       | ^                 | ^             |
  * | ^               | Page 13        | 0x0800 6800 - 0x0800 6FFF | ^                 | ^             |
  * | Main Programm   | Page 14        | 0x0800 7000 - 0x0800 77FF | 48 Kbyte (0xC000) | 24 �� 2 KB    |
  * | ^               | ...            | ...                       | ^                 | ^             |
  * | ^               | Page 37        | 0x0801 2800 - 0x0801 2FFF | ^                 | ^             |
  * | Download Buffer | Page 38        | 0x0800 3000 - 0x0800 37FF | 48 Kbyte (0xC000) | 24 �� 2 KB    |
  * | ^               | ...            | ...                       | ^                 | ^             |
  * | ^               | Page 61        | 0x0801 E800 - 0x0801 EFFF | ^                 | ^             |
  * | Config Page     | Page 62        | 0x0801 F000 - 0x0801 F7FF | 2 Kbyte (0x800)   | 1 �� 2 KB     |
  * | RO Constants    | Page 63        | 0x0801 F800 - 0x0801 FFFF | 2 Kbyte (0x800)   | 1 �� 2 KB     |
  * | Unused_page_2KB | Page 64        | 0x0802 0000 - 0x0802 07FF | 384 Kbyte         | 192 �� 2 KB   |
  * | ^               | ...            | ...                       | ^                 | ^             |
  * | ^               | Page 255       | 0x0807 F800 - 0x0807 FFFF | ^                 | ^             |
  * | Unused_page_4KB | Page 256       | 0x0808 0000 - 0x0808 0FFF | 2556 Kbyte        | 639 �� 4 KB   |
  * | ^               | ...            | ...                       | ^                 | ^             |
  * | ^               | Page 895       | 0x082F F000 - 0x082F FFFF | ^                 | ^             |
  * 
  * Bootloader      - ������� ������������ ����������                                                  \n
  * Main Programm   - ������� ������������ �������� ���������                                          \n
  * Download Buffer - ������� ��� ���������� ����� �������� (������������ �����������)                 \n
  * Config Page     - ������� �������� ���������� ���������� ������                                    \n
  * RO Constants    - ������� �������� ������������ ���������� ������ (������������ ��� ������ ������) \n
  * \n 
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
  * | ��������                  |   �����    |   0x00   |   0x01   | 0x02 | 0x03 |
  * | ------------------------- | :--------: | :------: | :------: | :--: | :--: |
  * | ��� ������                | 0x0801F800 |  class   |   0xFF   | 0xFF | 0xFF |
  * | ���������� �������        | 0x0801F804 | hw_minor | hw_major | 0xFF | 0xFF |
  * | �������� ����� ������ lw  | 0x0801F808 |   0xFF   |   0xFF   | 0xFF | 0xFF |
  * | �������� ����� ������ hw  | 0x0801F80C |   0xFF   |   0xFF   | 0xFF | 0xFF |
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
#include "FLASH_GD32F103R.h"
//#include "gd32f10x_fmc.h"
//------------------------------------------------------------------------------//

//---Private macros-------------------------------------------------------------//
#define PAGE0_ADDR            0x08000000U                        /*!< ����� ������ ������ �������� FLASH.          */
#define END_ADDR_OF_LAST_PAGE (PAGE0_ADDR + FMC_SIZE * 1024 - 1) /*!< ����� ���������� ����� � ��������� ��������. */

//---������� ��������� FLASH ������ � Kbyte ��� ���������� ��������������� ��������---//
#define MEMSIZE_BOOTLOADER       28 /*!< ������ ��������� FLASH ������ � Kbyte.       */
#define MEMSIZE_MAIN_PROGRAM     48 /*!< ������ ��������� FLASH ������ � Kbyte.       */
#define MEMSIZE_DOWNLOAD_BUFFER  48 /*!< ������ ��������� FLASH ������ � Kbyte.       */
#define MEMSIZE_CONFIG_PAGE      2  /*!< ������ ��������� FLASH ������ � Kbyte.       */
#define MEMSIZE_RO_CONSTANS      2  /*!< ������ ��������� FLASH ������ � Kbyte.       */
//------------------------------------------------------------------------------------//

//---��������� ������ ��������������� �������� �� FLASH---//
#define ADDR_BOOTLOADER       PAGE0_ADDR                                              /*!< ��������� ����� ������ BootLoader.                   */
#define ADDR_MAIN_PROGRAM     (ADDR_BOOTLOADER      + MEMSIZE_BOOTLOADER      * 1024) /*!< 0x08007000U // ��������� ����� ������ MainProgram. �������� 1024 - ���������� ���� � ���������. */
#define ADDR_DOWNLOAD_BUFFER  (ADDR_MAIN_PROGRAM    + MEMSIZE_MAIN_PROGRAM    * 1024) /*!< 0x08013000U // ��������� ����� ������ DowloadBuffer. */
#define ADDR_CONFIG_PAGE      (ADDR_DOWNLOAD_BUFFER + MEMSIZE_DOWNLOAD_BUFFER * 1024) /*!< 0x0801F000U // ��������� ����� ������ ConfigPage.    */
#define ADDR_RO_CONSTANS      (ADDR_CONFIG_PAGE     + MEMSIZE_CONFIG_PAGE     * 1024) /*!< 0x0801F800U // ��������� ����� ������ RO_Constans.   */
//--------------------------------------------------------//

#define DEF_FLASH_ADDR (END_ADDR_OF_LAST_PAGE - PAGE_SIZE_2KB + 1) /*!< ����� ��� ������ �� ��������� - ����� ������ ��������� �������� flash (0x803F800). */

//#define NUM_OF_CONFIG_WORDS 9U /*!< ���������� ���������� ������ (� ���� 32-������ ����), ������� ����� ������������ � ������� Config Page. */
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
  * @param   Config - ��������� ���� Config_struct* �� ��������� � ������� Config.
  * @return  flash status.
  */
flash_status Write_Config_to_flash (Config_struct* Config)
{
fmc_state_enum state;

fmc_unlock(); // Unlock the main FMC operation.
state = fmc_page_erase(ADDR_CONFIG_PAGE);
if (state != FMC_READY)
  return FLASH_ERROR;
else
  {
  for (uint8_t i = 0; i < NUM_OF_CONFIG_WORDS; i++)
    {
    state = fmc_word_program(ADDR_CONFIG_PAGE + 4*i, *( ((uint32_t*)Config) + i )); // Program a word at the corresponding address.
    if (state != FMC_READY)
      {
      break;
      }
    }
  }
fmc_lock(); // Lock the main FMC operation.

if (state != FMC_READY)
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
  * @param   Address - ����� ��������� ������.
  * @param   Amount  - ���������� ������������ ����.
  * @param   Words   - ��������� ���� uint32_t* �� ������ � �������. ������ ������ ���� ��������� �� 4 �����.
  * @return  flash status.
  */
flash_status Write_Words_to_flash (uint32_t Address, uint32_t Amount, uint32_t *Words)
{
fmc_state_enum state;

if ( (Address < PAGE0_ADDR) || (Address > END_ADDR_OF_LAST_PAGE) )
  return FLASH_WROG_ADDRES;

fmc_unlock(); // Unlock the main FMC operation.
state = fmc_page_erase(Address);
if (state != FMC_READY)
  return FLASH_ERROR;
else
  {
  for (uint32_t i = 0; i < Amount; i++)
    {
    state = fmc_word_program(Address + 4*i, (uint32_t)*(Words + i)); // Program a halfword at the corresponding address.
    if (state != FMC_READY)
      {
      break;
      }
    }
  }
fmc_lock(); // Lock the main FMC operation.

if (state != FMC_READY)
  return FLASH_ERROR;
else
  return FLASH_OK;
}
//------------------------------------------------------------------------------//


/**
  * @brief   ������ ��������� Flash memory density.
  * @details ������ �������� ������� FLASH ������ (Flash memory density) ���������������� (STM, GD, AT).
  * @return  uint16_t - ������ FLASH ������ ������������� ���������������� (STM, GD, AT) � Kbyte.
  */
uint16_t Read_MCU_FMD (void)
{
return FMC_SIZE;
}
//------------------------------------------------------------------------------//


//***************************************END OF FILE**************************************//
