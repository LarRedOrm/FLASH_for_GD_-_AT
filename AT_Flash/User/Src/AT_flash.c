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
  * **Manual**
  * � �������� ����������� ������ �� flash ��������� - ������� WriteHalfWord_to_flash,                           \n 
  * � ������ �� flash ������� �������� - WriteHalfWords_to_flash.                                                \n 
  * ������ ������ �������� � ������ ��������? �����:                                                             \n 
  *    AT32F413  - ����� ������ �� flash �� ����� (8 ���), �� ��������� (16 ���), � �� ������ ����� (32 ����).   \n
  *    GD32F103  - ����� ������ �� flash �� ��������� (16 ���), � �� ������ ����� (32 ����).                     \n 
  *    STM32F103 - ����� ������ �� flash ������ �� ��������� (16 ���).                                           \n 
  *    ������ ������ �� ��������� ������������ ����������� �����������������.                                    \n 
  *    ������������� ������� ������ ���� ���� ��� ���� �����������������, ������� ����� ������������,            \n 
  *    ��� � ���� �������� ������ ��� ������ ����� ������������� � ������ ������������ ������ �� 2 �����.        \n 
  *    ����� ������� ����� ���������� ��������������� �������������� �������� ��� ���� �����������������.        \n
  *
  * � ������, ���� ������������ �������������� �������� ���������� ��������� Flash ������������� �����������,    \n 
  * � ���������� ����� flash ��� ��������, ��������������, ���� ����������,                                      \n 
  * �������� ������������ ������ DEFAULT_FLASH_ADDRESS.                                                          \n 
  * ��� ���� ������ ����� ����������� �� ������ ��������� �� ���������.                                          \n 
  * � �������� �������� �� ��������� ��� AT32F413 ������ ����� ������ ��������� �������� flash.                  \n 
  * ������ �������� - 2 KB.
  *
  * ������ ����� �� ��������� ����� �������� Get_Default_Address.
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
