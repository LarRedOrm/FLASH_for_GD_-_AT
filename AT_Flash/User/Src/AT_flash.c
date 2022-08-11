/**
  ******************************************************************************
  * 
  * @file      AT_flash.c
  * 
  * @brief     Драйвер flash для AT32F413RCT7.
  * 
  * @details   Функции для работы с flash в микроконтроллерах семейства AT32F413.
  *
  * @warning   **ВАЖНО!**                                                                                        \n 
  * Для работы драйвера требуется: 
  *                       + файл at32f413_flash.c - аппаратный драйвер работы с flash от Artery;
  *                       + файл at32f413_flash.h - header for at32f413_flash.c.
  *
  * **Manual**
  * В драйвере реализована запись во flash полуслова - функция WriteHalfWord_to_flash,                           \n 
  * и запись во flash массива полуслов - WriteHalfWords_to_flash.                                                \n 
  * Почему именно полуслов и только полуслов? Ответ:                                                             \n 
  *    AT32F413  - умеет писать во flash по байту (8 бит), по полуслову (16 бит), и по целому слову (32 бита).   \n
  *    GD32F103  - умеет писать во flash по полуслову (16 бит), и по целому слову (32 бита).                     \n 
  *    STM32F103 - умеет писать во flash только по полуслову (16 бит).                                           \n 
  *    Формат записи по полуслову поддерживает большинство микроконтроллеров.                                    \n 
  *    Надаппаратный драйвер должен быть один для всех микроконтроллеров, поэтому будем предполагать,            \n 
  *    что в этом драйвере данные для записи будут формироваться с учётом выравнивания только по 2 байта.        \n 
  *    Таким образом будет обеспечена универсальность надаппаратного драйвера для всех микроконтроллеров.        \n
  *
  * В случае, если пользователю надаппаратного драйвера неизвестна структура Flash используемого контроллера,    \n 
  * и допустимый адрес flash для записиси, соответственно, тоже неизвестен,                                      \n 
  * возможно использовать макрос DEFAULT_FLASH_ADDRESS.                                                          \n 
  * При этом запись будет проводиться по адресу заданному по умолчанию.                                          \n 
  * В качестве значения по умолчанию для AT32F413 выбран адрес начала последней страницы flash.                  \n 
  * Размер страницы - 2 KB.
  *
  * Узнать адрес по умолчанию можно функцией Get_Default_Address.
  *
  * @copyright Copyright (C) 2022 Awada Systems. Все права защищены.
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
#define PAGE0_ADDR             0x08000000                                     // Адрес начала первой страницы.
#define NUMBER_OF_PAGES        (F_SIZE / 2)                                   // Количество страниц.
#define PAGE_SIZE              0x0800                                         // Размер страницы в байтах.
#define END_ADDR_OF_LAST_PAGE  (PAGE0_ADDR + PAGE_SIZE * NUMBER_OF_PAGES - 1) // 0x803FFFF // Адрес последнего байта в последней странице.
#define DEF_FLASH_ADDR         (END_ADDR_OF_LAST_PAGE - PAGE_SIZE + 1)        // 0x803F800 // Адрес для записи по умолчанию - адрес начала последней страницы flash.
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
  * @brief  Запись полуслова (16 бит) во flash.
  * @param  Address  - адрес начальной ячейки. Адрес ячейки должен быть выровнен относительно началального адреса страницы по 2 байта.
  * @return HalfWord - полуслово.
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
page_address = fmc_page_address_calc(Address); // Расчёт начального адреса страницы.
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
  * @brief  Запись массива полуслов во flash.
  * @param  Address   - адрес начальной ячейки. Адрес ячейки должен быть выровнен относительно началального адреса страницы по 2 байта.
  * @return HalfWords - указатель типа uint16_t* на массив с данными.
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
page_address = fmc_page_address_calc(Address); // Расчёт начального адреса страницы.
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
  * @brief  Вернуть значение адреса для записи во flash по умолчанию.
  * @param  None
  * @return uint32_t - адрес для записи во flash по умолчанию.
  */
uint32_t Get_Default_Address (void)
{
return DEF_FLASH_ADDR;
}
//------------------------------------------------------------------------------//


/**
  * @brief  Запись слова во flash.
  * @param  Address - адрес выбранной ячейки. Адрес ячейки должен быть выровнен относительно началального адреса страницы по 4 байта.
  * @return Word    - слово.
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

// page_address = fmc_page_address_calc(Address); // Расчёт начального адреса страницы.
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
  * @brief  Запись массива слов во flash.
  * @param  Address - адрес начальной ячейки. Адрес ячейки должен быть выровнен относительно началального адреса страницы по 4 байта.
  * @return Word    - указатель типа uint32_t* на массив с данными.
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

// page_address = fmc_page_address_calc(Address); // Расчёт начального адреса страницы.
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
  * @brief   Расчёт начального адреса страницы.
  * @details Функция расчёта начального адреса страницы в зависимости от выбранного адреса ячейки.
  * @param   Address  - адрес выбранной ячейки.
  * @return  uint32_t - начальный адрес страницы.
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
