/**
  ******************************************************************************
  * 
  * @file      AT_flash.c
  * 
  * @brief     ������� flash ��� AT32F413RCT7.
  * 
  * @details   ������� ��� ������ � flash � ����������������� ��������� AT32F413.
  *
  * @warning   **�����!**                                                                                        \n 
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
  * � ������������ AT32F403AR ������ FLASH ����� ����� ��������� ��������:
  * - 256 Kbyte  (AT32F403ARC),
  * - 512 Kbyte  (AT32F403ARE),
  * - 1024 Kbyte (AT32F403ARG). \n 
  *
  *
  * **����������� FLASH ������ ��� AT32F403ARG (1024 Kbyte)** \n 
  *
  * | Block              | Bank               | Name              | Address Range              | Size (bytes) |
  * | ------------------ |--------------------| :---------------: | :------------------------: | :----------: |
  * | Main Flash memory  | Bank 1 (512 Kbyte) | Page 0            | 0x0800 0000 - 0x0800 07FF  | 2 KB         |
  * | ^                  |                    | Page 1            | 0x0800 0800 � 0x0800 0FFF  | 2 KB         |
  * | ^                  | ^                  | Page 2            | 0x0800 1000 � 0x0800 17FF  | 2 KB         |
  * | ^                  | ^                  | ...               | ...                        | ...          |
  * | ^                  | ^                  | Page 255          | 0x0807 F800 � 0x0807 FFFF  | 2 KB         |
  * | ^                  | Bank 2 (512 Kbyte) | Page 256          | 0x0808 0000 � 0x0808 07FF  | 2 KB         |
  * | ^                  |                    | Page 257          | 0x0808 0800 � 0x0808 0FFF  | 2 KB         |
  * | ^                  | ^                  | Page 258          | 0x0808 1000 � 0x0808 17FF  | 2 KB         |
  * | ^                  | ^                  | ...               | ...                        | ...          |
  * | ^                  | ^                  | Page 511          | 0x080F F800 � 0x080F FFFF  | 2 KB         |
  * | External memory    | 16 MB              | Page 0            | 0x0840 0000 � 0x0840 0FFF  | 4 KB         |
  * | ^                  | ^                  | Page 1            | 0x0840 1000 � 0x0840 1FFF  | 4 KB         |
  * | ^                  | ^                  | Page 2            | 0x0840 2000 � 0x0840 2FFF  | 4 KB         |
  * | ^                  | ^                  | ...               | ...                        | ...          |
  * | ^                  | ^                  | Page 4095         | 0x093F F000 � 0x093F FFFF  | 4 KB         |
  * | Information Block  |                    | Boot loader       | 0x1FFF B000 � 0x1FFF EFFF  | 16 KB        |
  * | ^                  | ^                  | User option bytes | 0x1FFF F800 � 0x1FFF F82F  | 48 B         |
  *  \n 
  *
  * **����������� FLASH ������ ��� High-density � Extra-density** \n 
  * (GD32F10x_CL,_GD32F10x_HD,_GD32F10x_XD)
  *
  * | Block              | Density     | Name             | Address Range              | Size (bytes)      |
  * | -------------------| :---------: | :--------------: | :------------------------: | :---------------: |
  * | Main Flash Block   | CL, HD, XD  | Page 0           | 0x0800 0000 - 0x0800 07FF  | 2 KB              |
  * | ^                  | ^           | Page 1           | 0x0800 0800 - 0x0800 0FFF  | 2 KB              |
  * | ^                  | ^           | Page 2           | 0x0800 1000 - 0x0800 17FF  | 2 KB              |
  * | ^                  | ^           | ...              | ...                        | ...               |
  * | ^                  | ^           | Page 255         | 0x0807 F800 - 0x0807 FFFF  | 2 KB              |
  * | ^                  | ^           | Page 256         | 0x0808 0000 - 0x0808 0FFF  | 4 KB              |
  * | ^                  | ^           | Page 257         | 0x0808 1000 - 0x0808 1FFF  | 4 KB              |
  * | ^                  | ^           | ...              | ...                        | ...               |
  * | ^                  | ^           | Page 895         | 0x082F F000 - 0x082F FFFF  | 4 KB              |
  * | Information Block  | GD32F10x_HD | Boot loader area | 0x1FFF F000 - 0x1FFF F7FF  | 2 KB              |
  * | ^                  | GD32F10x_XD | ^                | 0x1FFF E000 - 0x1FFF F7FF  | 6 KB              |
  * | ^                  | GD32F10x_CL | ^                | 0x1FFF B000 - 0x1FFF F7FF  | 18 KB             |
  * | Option bytes Block | CL, HD, XD  | Option bytes     | 0x1FFF F800 - 0x1FFF F80F  | 16 B              |
  * \n 
  * ������ FLASH ������������ � ������������ � ������������� (���-�� �����-��) ������ ������ (����� ������ �����������). \n 
  * ��� �������� ���� ����� ������ ��������� �� ����� 128 Kbyte FLASH ������.                                            \n 
  * ���������������, ��� ������������ ���������� ����� GD32F103R c ������� FLASH ������ �� ����� 128 Kbyte.              \n 
  * � ���� �������� (����) ������������ ��� ������� ����������� FLASH ������ (� ������ ������������� ����� ������)       \n 
  * ��� ������������ � ���������� Medium Density � High/Extra Density.                                                   \n 
  * \n 
  * **����� ������ ��� ����������������� ����� GD32F103R Medium Density**
  * ����������      | ����� �������� | �������� �������           | ���������� �����  | ����� �������
  * --------------- | -------------- | -------------------------- | ----------------- | -------------
  * Bootloader      | Page 0         | 0x0800 0000 - 0x0800 03FF  | 28 Kbyte (0x7000) | 28 �� 1 KB
  * ^               | ...            | ...                        | ^                 | ^
  * ^               | Page 27        | 0x0800 6BFF - 0x0800 6FFF  | ^                 | ^
  * Main Programm   | Page 28        | 0x0800 7000 - 0x0800 73FF  | 48 Kbyte (0xC000) | 48 �� 1 KB
  * ^               | ...            | ...                        | ^                 | ^
  * ^               | Page 75        | 0x0801 2BFF - 0x0801 2FFF  | ^                 | ^
  * Download Buffer | Page 76        | 0x0801 3000 - 0x0800 33FF  | 48 Kbyte (0xC000) | 48 �� 1 KB
  * ^               | ...            | ...                        | ^                 | ^
  * ^               | Page 123       | 0x0801 3400 - 0x0801 EFFF  | ^                 | ^
  * Config Page     | Page 124       | 0x0801 F000 - 0x0801 F3FF  |  1 Kbyte (0x400)  | 2 �� 1 KB
  * ^               | Page 125       | 0x0801 F400 - 0x0801 F7FF  |  1 Kbyte (0x400)  | ^
  * RO Constants    | Page 126       | 0x0801 F800 - 0x0801 FBFF  |  1 Kbyte (0x400)  | 2 �� 1 KB
  * ^               | Page 127       | 0x0801 FC00 - 0x0801 FFFF  |  1 Kbyte (0x400)  | ^
  * \n 
  * **����� ������ ��� ����������������� ����� GD32F103R High/Extra Density**
  * ����������      | ����� �������� | �������� �������           | ���������� �����  | ����� �������
  * --------------- | -------------- | -------------------------- | ----------------- | -------------
  * Bootloader      | Page 0         | 0x0800 0000 - 0x0800 07FF  | 28 Kbyte (0x7000) | 14 �� 2 KB
  * ^               | ...            | ...                        | ^                 | ^
  * ^               | Page 13        | 0x0800 6800 - 0x0800 6FFF  | ^                 | ^
  * Main Programm   | Page 14        | 0x0800 7000 - 0x0800 77FF  | 48 Kbyte (0xC000) | 24 �� 2 KB
  * ^               | ...            | ...                        | ^                 | ^
  * ^               | Page 37        | 0x0801 2800 - 0x0801 2FFF  | ^                 | ^
  * Download Buffer | Page 38        | 0x0800 3000 - 0x0800 37FF  | 48 Kbyte (0xC000) | 24 �� 2 KB
  * ^               | ...            | ...                        | ^                 | ^
  * ^               | Page 61        | 0x0801 E800 - 0x0801 EFFF  | ^                 | ^
  * Config Page     | Page 62        | 0x0801 F000 - 0x0801 F7FF  | 2 Kbyte (0x800)   | 1 �� 2 KB
  * RO Constants    | Page 63        | 0x0801 F800 - 0x0801 FFFF  | 2 Kbyte (0x800)   | 1 �� 2 KB
  * Unused_page_2KB | Page 64        | 0x0802 0000 - 0x0802 07FF  | 384 Kbyte         | 192 �� 2 KB
  * ^               | ...            | ...                        | ^                 | ^
  * ^               | Page 255       | 0x0807 F800 - 0x0807 FFFF  | ^                 | ^
  * Unused_page_4KB | Page 256       | 0x0808 0000 - 0x0808 0FFF  | 2556 Kbyte        | 639 �� 4 KB
  * ^               | ...            | ...                        | ^                 | ^
  * ^               | Page 895       | 0x082F F000 - 0x082F FFFF  | ^                 | ^
  * 
  * Bootloader      - ������� ������������ ����������                                                  \n
  * Main Programm   - ������� ������������ �������� ���������                                          \n
  * Download Buffer - ������� ��� ���������� ����� �������� (������������ �����������)                 \n
  * Config Page     - ������� �������� ���������� ���������� ������                                    \n
  * RO Constants    - ������� �������� ������������ ���������� ������ (������������ ��� ������ ������) \n
  * \n 
  *
  * **����� ������ Config Page**
  * ��������                        | �����      | 0x00 | 0x01 | 0x02 | 0x03
  * ------------------------------- | ---------- | ---- | ---- | ---- | ---- 
  * ����� ������                    | 0x0801F000 | addr | 0xFF | 0xFF | 0xFF
  * ������ ����������               | 0x0801F004 | minor| major| 0xFF | 0xFF
  * ������ ���������                | 0x0801F008 | minor| major| 0xFF | 0xFF
  * ���� ������� �������            | 0x0801F00C | flag | 0xFF | 0xFF | 0xFF
  * �� ������������ � ������ Modbus | 0x0801F010 | 0xFF | 0xFF | 0xFF | 0xFF
  * ��������� Modbus ����� 0        | 0x0801F014 | baud | par  | stop | 0xFF
  * ��������� Modbus ����� 1        | 0x0801F018 | baud | par  | stop | 0xFF
  * ��������� Modbus ����� 2        | 0x0801F01C | baud | par  | stop | 0xFF
  * ��������� Modbus ����� 3        | 0x0801F020 | baud | par  | stop | 0xFF
  * �������� Config Page �������� ��������� ������, ���������� � ���� 32-������ ����.
  * \n \n 
  *
  * **����� ������ RO Constants**
  * ��������             | �����      | 0x00  | 0x01  | 0x02 | 0x03
  * -------------------- | ---------- | ----- | ----- | ---- | ---- 
  * ��� ������           | 0x0801F800 | class | 0xFF  | 0xFF | 0xFF
  * ���������� �������   | 0x0801F804 | minor | major | 0xFF | 0xFF
  * �������� ����� hw    | 0x0801F808 | 0xFF  | 0xFF  | 0xFF | 0xFF
  * �������� ����� lw    | 0x0801F810 | 0xFF  | 0xFF  | 0xFF | 0xFF
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
#define AT32F413RCT7_HD

#define F_SIZE     (*(uint32_t*)0x1FFFF7E0) // Flash size, in terms of KByte.

#ifdef AT32F413RCT7_HD
#define PAGE0_ADDR             0x08000000                                     // ����� ������ ������ ��������.
#define NUMBER_OF_PAGES        (F_SIZE / 2)                                   // ���������� �������.
#define PAGE_SIZE              0x0800                                         // ������ �������� � ������.
#define END_ADDR_OF_LAST_PAGE  (PAGE0_ADDR + PAGE_SIZE * NUMBER_OF_PAGES - 1) // 0x803FFFF // ����� ���������� ����� � ��������� ��������.
#define DEF_FLASH_ADDR         (END_ADDR_OF_LAST_PAGE - PAGE_SIZE + 1)        // 0x803F800 // ����� ��� ������ �� ��������� - ����� ������ ��������� �������� flash.
#endif

#define WRONG_ADDRESS  0xEE
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
uint32_t fmc_page_address_calc (uint32_t Address);
//------------------------------------------------------------------------------//

//---Exported functions---------------------------------------------------------//
/**
  * @brief  ������ ��������� (16 ���) �� flash.
  * @param  Address  - ����� ��������� ������. ����� ������ ������ ���� �������� ������������ ������������ ������ �������� �� 2 �����.
  * @return HalfWord - ���������.
  */
flash_status_enum WriteHalfWord_to_flash (uint32_t Address, uint16_t HalfWord)
{
flash_status_type state;
//uint32_t          page_address;

if (Address == DEFAULT_FLASH_ADDRESS)
  Address = DEF_FLASH_ADDR;

if ( (Address < PAGE0_ADDR) || (Address > END_ADDR_OF_LAST_PAGE) )
  return FLASH_WROG_ADDRES;
/*
page_address = fmc_page_address_calc(Address); // ������ ���������� ������ ��������.
if (page_address == WRONG_ADDRESS)
  return FLASH_WROG_ADDRES;
*/

flash_unlock();                                // Unlock the main FMC operation.
state = flash_sector_erase(Address);
//state = flash_sector_erase(page_address);      // Erase page.
if (state != FLASH_OPERATE_DONE)
  return FLASH_ERROR;
else
  state = flash_halfword_program(Address, HalfWord); // Program a halfword at the corresponding address.
flash_lock();                                        // Lock the main FMC operation.

if (state != FLASH_OPERATE_DONE)
  return FLASH_ERROR;
else
  return FLASH_OK;
}
//------------------------------------------------------------------------------//


/**
  * @brief  ������ ������� �������� �� flash.
  * @param  Address   - ����� ��������� ������. ����� ������ ������ ���� �������� ������������ ������������ ������ �������� �� 2 �����.
  * @return HalfWords - ��������� ���� uint16_t* �� ������ � �������.
  */
flash_status_enum WriteHalfWords_to_flash (uint32_t Address, uint32_t Amount, uint16_t *HalfWords)
{
flash_status_type state;
//uint32_t       page_address;
  
if (Address == DEFAULT_FLASH_ADDRESS)
  Address = DEF_FLASH_ADDR;

if ( (Address < PAGE0_ADDR) || (Address > END_ADDR_OF_LAST_PAGE) )
  return FLASH_WROG_ADDRES;
/*
page_address = fmc_page_address_calc(Address); // ������ ���������� ������ ��������.
if (page_address == WRONG_ADDRESS)
  return FLASH_WROG_ADDRES;
*/

flash_unlock();                                 // Unlock the main FMC operation.
state = flash_sector_erase(Address);
//state = flash_sector_erase(page_address);       // Erase page.
if (state != FLASH_OPERATE_DONE)
  return FLASH_ERROR;
else
  {
  for (uint32_t i = 0; i < Amount; i++)
    {
    state = flash_halfword_program(Address + 2*i, (uint32_t)*(HalfWords + i)); // Program a halfword at the corresponding address.
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
  * @brief  ������� �������� ������ ��� ������ �� flash �� ���������.
  * @param  None
  * @return uint32_t - ����� ��� ������ �� flash �� ���������.
  */
uint32_t Get_Default_Address (void)
{
return DEF_FLASH_ADDR;
}
//------------------------------------------------------------------------------//


/**
  * @brief  ������ ����� �� flash.
  * @param  Address - ����� ��������� ������. ����� ������ ������ ���� �������� ������������ ������������ ������ �������� �� 4 �����.
  * @return Word    - �����.
  */
/*
flash_status_type WriteWord_to_flash (uint32_t Address, uint32_t Word)
{
flash_status_type state;
//uint32_t          page_address;

#ifdef DEFAULT_FLASH_ADDRESS
  Address = DEF_FLASH_ADDR;
#endif

if ( (Address < PAGE0_ADDR) || (Address > END_ADDR_OF_LAST_PAGE) )
  return FLASH_WROG_ADDRES;

// page_address = fmc_page_address_calc(Address); // ������ ���������� ������ ��������.
// if (page_address == WRONG_ADDRESS)
//   return FLASH_WROG_ADDRES;

flash_unlock();                                // Unlock the main FMC operation.
state = flash_sector_erase(Address);
//state = flash_sector_erase(page_address);       // Erase page.
if (state != FLASH_OPERATE_DONE)
  return state;
else
  state = flash_word_program(Address, Word); // Program a word at the corresponding address.
flash_lock();                                // Lock the main FMC operation.
return state;
}
//------------------------------------------------------------------------------//
*/

/**
  * @brief  ������ ������� ���� �� flash.
  * @param  Address - ����� ��������� ������. ����� ������ ������ ���� �������� ������������ ������������ ������ �������� �� 4 �����.
  * @return Word    - ��������� ���� uint32_t* �� ������ � �������.
  */
/*
flash_status_type WriteWords_to_flash (uint32_t  Address, uint32_t Amount, uint32_t *Words)
{
flash_status_type state;
//uint32_t       page_address;

#ifdef DEFAULT_FLASH_ADDRESS
  Address = DEF_FLASH_ADDR;
#endif

if ( (Address < PAGE0_ADDR) || (Address > END_ADDR_OF_LAST_PAGE) )
  return FLASH_WROG_ADDRES;

// page_address = fmc_page_address_calc(Address); // ������ ���������� ������ ��������.
// if (page_address == WRONG_ADDRESS)
//   return FLASH_WROG_ADDRES;

flash_unlock();                                 // Unlock the main FMC operation.
state = flash_sector_erase(Address);
//state = flash_sector_erase(page_address);       // Erase page.
if (state != FLASH_OPERATE_DONE)
  return state;
else
  {
  for (uint32_t i = 0; i < Amount; i++)
    {
    state = flash_word_program(Address + 4*i, (uint32_t)*(Words + i)); // Program a word at the corresponding address.
    if (state != FLASH_OPERATE_DONE)
      {
      break;
      }
    }
  }
flash_lock(); // Lock the main FMC operation.
return state;
}
//------------------------------------------------------------------------------//
*/
//------------------------------------------------------------------------------//


//---Private functions----------------------------------------------------------//
/**
  * @brief   ������ ���������� ������ ��������.
  * @details ������� ������� ���������� ������ �������� � ����������� �� ���������� ������ ������.
  * @param   Address  - ����� ��������� ������.
  * @return  uint32_t - ��������� ����� ��������.
  */
uint32_t fmc_page_address_calc (uint32_t Address)
{
uint32_t temp_address = PAGE0_ADDR;
if ( (Address < PAGE0_ADDR) || (Address > END_ADDR_OF_LAST_PAGE) )
  return WRONG_ADDRESS;
else
  {
  while (Address > (temp_address + PAGE_SIZE - 1) )
    {
    temp_address = temp_address + PAGE_SIZE;
    }
  return temp_address;
  }
}
//------------------------------------------------------------------------------//


//***************************************END OF FILE**************************************//
