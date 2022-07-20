/**
  ******************************************************************************
  * 
  * @file      GD_flash.c
  * 
  * @brief     ������� flash ��� GD32F103R.
  * 
  * @details   ������� ��� ������ � flash � ����������������� ��������� GD32F103R.
  *
  * @warning   **�����!**                                                                                        \n 
  * ��� ������ �������� ���������: 
  *                       + ���� gd32f10x_fmc.c - ���������� ������� ������ � flash �� GD;
  *                       + ���� gd32f10x_fmc.h - header for gd32f10x_fmc.c.
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
  * � �������� �������� �� ��������� ��� GD32F103R ������ ����� ������ ��������� �������� flash.                 \n 
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
#include "GD_flash.h"
//------------------------------------------------------------------------------//

//---Private macros-------------------------------------------------------------//
#ifdef GD32F10X_HD
#define GD32F103VCT6_HD
//#define
//#define
#endif

#ifdef GD32F103VCT6_HD
#define PAGE0_ADDR             0x08000000                                     // ����� ������ ������ ��������.
#define NUMBER_OF_PAGES        (FMC_SIZE / 2)                                 // ���������� �������.
#define PAGE_SIZE              0x0800                                         // ������ �������� � ������.
#define END_ADDR_OF_LAST_PAGE  (PAGE0_ADDR + PAGE_SIZE * NUMBER_OF_PAGES - 1) // 0x803FFFF // ����� ���������� ����� � ��������� ��������.
#define DEF_FLASH_ADDR         (END_ADDR_OF_LAST_PAGE - PAGE_SIZE + 1 )       // 0x803F800 // ����� ��� ������ �� ��������� - ����� ������ ��������� �������� flash.
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
flash_status_enum WriteHalfWord_to_flash (uint32_t Address, uint32_t Amount, uint16_t HalfWord)
{
fmc_state_enum state;
uint32_t       page_address;

if (Address == DEFAULT_FLASH_ADDRESS)
  Address = DEF_FLASH_ADDR;

page_address = fmc_page_address_calc(Address); // ������ ���������� ������ ��������.
if (page_address == WRONG_ADDRESS)
  return FLASH_WROG_ADDRES;

fmc_unlock();                                 // Unlock the main FMC operation.
state = fmc_page_erase(page_address);         // Erase page.
if (state != FMC_READY)
  return FLASH_ERROR;
else
  state = fmc_halfword_program(Address, HalfWord); // Program a halfword at the corresponding address.
fmc_lock();                                        // Lock the main FMC operation.

if (state != FMC_READY)
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
fmc_state_enum state;
uint32_t       page_address;
  
if (Address == DEFAULT_FLASH_ADDRESS)
  Address = DEF_FLASH_ADDR;

page_address = fmc_page_address_calc(Address); // ������ ���������� ������ ��������.
if (page_address == WRONG_ADDRESS)
  return FLASH_WROG_ADDRES;

fmc_unlock();                                 // Unlock the main FMC operation.
state = fmc_page_erase(page_address);         // Erase page.
if (state != FMC_READY)
  return FLASH_ERROR;
else
  {
  for (uint32_t i = 0; i < Amount; i++)
    {
    state = fmc_halfword_program(Address + 2*i, (uint32_t)*(HalfWords + i)); // Program a halfword at the corresponding address.
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
flash_status_enum WriteWord_to_flash (uint32_t Address, uint32_t Word)
{
fmc_state_enum state;
uint32_t       page_address;

if (Address == DEFAULT_FLASH_ADDRESS)
  Address = DEF_FLASH_ADDR;
  
page_address = fmc_page_address_calc(Address); // ������ ���������� ������ ��������.
if (page_address == WRONG_ADDRESS)
  return FLASH_WROG_ADDRES;

fmc_unlock();                                  // Unlock the main FMC operation.
state = fmc_page_erase(page_address);          // Erase page.
if (state != FMC_READY)
  return FLASH_ERROR;
else
  state = fmc_word_program(Address, Word); // Program a word at the corresponding address.
fmc_lock();                                // Lock the main FMC operation.

if (state != FMC_READY)
  return FLASH_ERROR;
else
  return FLASH_OK;
}
//------------------------------------------------------------------------------//
*/

/**
  * @brief  ������ ������� ���� �� flash.
  * @param  Address - ����� ��������� ������. ����� ������ ������ ���� �������� ������������ ������������ ������ �������� �� 4 �����.
  * @return Word    - ��������� ���� uint32_t* �� ������ � �������.
  */
/*
flash_status_enum WriteWords_to_flash (uint32_t  Address, uint32_t Amount, uint32_t *Words)
{
fmc_state_enum state;
uint32_t       page_address;
  
if (Address == DEFAULT_FLASH_ADDRESS)
  Address = DEF_FLASH_ADDR;

page_address = fmc_page_address_calc(Address); // ������ ���������� ������ ��������.
if (page_address == WRONG_ADDRESS)
  return FLASH_WROG_ADDRES;

fmc_unlock();                                   // Unlock the main FMC operation.
state = fmc_page_erase(page_address);           // Erase page.
if (state != FMC_READY)
  return FLASH_ERROR;
else
  {
  for (uint32_t i = 0; i < Amount; i++)
    {
    state = fmc_word_program(Address + 4*i, (uint32_t)*(Words + i)); // Program a word at the corresponding address.
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
  while (Address >= (temp_address + PAGE_SIZE - 1) )
    {
    temp_address = temp_address + PAGE_SIZE;
    }
  return temp_address;
  }
}
//------------------------------------------------------------------------------//


//***************************************END OF FILE**************************************//
