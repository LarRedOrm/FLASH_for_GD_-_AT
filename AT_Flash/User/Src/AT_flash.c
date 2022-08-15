/**
  ******************************************************************************
  * 
  * @file      AT_flash.c
  * 
  * @brief     Драйвер flash для AT32F413RCT7.
  * 
  * @details   Функции для работы с flash в микроконтроллерах семейства AT32F413.
  *
  * @warning   **ВАЖНО!**                                                                                              \n 
  * Для работы драйвера требуется: 
  *                       + файл at32f413_flash.c - аппаратный драйвер работы с flash от Artery;
  *                       + файл at32f413_flash.h - header for at32f413_flash.c.
  *
  * **Manual** \n 
  * В драйвере реализованы следующие функции:
  * - Write_Config_to_flash (uint32_t *Data) - запись во FLASH изменяемых параметров модуля Config Page.               \n 
  *   Изменяемые параметры модуля могут храниться в структуре типа uint32_t или типа Config_struct.                    \n 
  *   Начальныый адрес для записи/чтения изменяемых параметров модуля во/из FLASH определён макросом ADDR_CONFIG_PAGE.
  *
  * - Read_Config_from_flash (Config_struct* Config) - чтение изменяемых параметров модуля из FLASH                    \n 
  *   в структуру типа Config_struct.
  *   
  * - Read_RO_Constants_from_flash (RO_Constants_struct* RO_Constants) - чтение неизменяемых параметров модуля         \n 
  *   в структуру типа RO_Constants_struct.
  *
  * - Write_Words_to_flash (uint32_t Address, uint32_t Amount, uint32_t *Words) - запись во FLASH произвольного массива слов.
  *
  * **Особенности организации FLASH памяти в проекте** \n 
  * В контроллерах серии AT32F403AR размер FLASH может иметь следующие значения:
  * - 256 Kbyte  (AT32F403ARC),
  * - 512 Kbyte  (AT32F403ARE),
  * - 1024 Kbyte (AT32F403ARG).
  * \n \n 
  *
  * **Организация FLASH памяти для AT32F403ARG (1024 Kbyte)** \n 
  *
  * | Block             | Bank               | Name              | Address Range             | Size (bytes) |
  * | ----------------- | :----------------: | :---------------: | :-----------------------: | :----------: |
  * | Main Flash memory | Bank 1 (512 Kbyte) | Page 0            | 0x0800 0000 – 0x0800 07FF | 2 KB         |
  * | ^                 | ^                  | Page 1            | 0x0800 0800 – 0x0800 0FFF | 2 KB         |
  * | ^                 | ^                  | Page 2            | 0x0800 1000 – 0x0800 17FF | 2 KB         |
  * | ^                 | ^                  | ...               | ...                       | ...          |
  * | ^                 | ^                  | Page 255          | 0x0807 F800 – 0x0807 FFFF | 2 KB         |
  * | ^                 | Bank 2 (512 Kbyte) | Page 256          | 0x0808 0000 – 0x0808 07FF | 2 KB         |
  * | ^                 | ^                  | Page 257          | 0x0808 0800 – 0x0808 0FFF | 2 KB         |
  * | ^                 | ^                  | Page 258          | 0x0808 1000 – 0x0808 17FF | 2 KB         |
  * | ^                 | ^                  | ...               | ...                       | ...          |
  * | ^                 | ^                  | Page 511          | 0x080F F800 – 0x080F FFFF | 2 KB         |
  * | External memory   | 16 MB              | Page 0            | 0x0840 0000 – 0x0840 0FFF | 4 KB         |
  * | ^                 | ^                  | Page 1            | 0x0840 1000 – 0x0840 1FFF | 4 KB         |
  * | ^                 | ^                  | Page 2            | 0x0840 2000 – 0x0840 2FFF | 4 KB         |
  * | ^                 | ^                  | ...               | ...                       | ...          |
  * | ^                 | ^                  | Page 4095         | 0x093F F000 – 0x093F FFFF | 4 KB         |
  * | Information Block |                    | Boot loader       | 0x1FFF B000 – 0x1FFF EFFF | 16 KB        |
  * | ^                 | ^                  | User option bytes | 0x1FFF F800 – 0x1FFF F82F | 48 B         |
  *  \n 
  *
  * **Организация FLASH памяти для AT32F403ARE (512 Kbyte)** \n 
  *
  * | Block             | Bank               | Name              | Address Range             | Size (bytes) |
  * | ------------------| :----------------: | :---------------: | :-----------------------: | :----------: |
  * | Main Flash Block  | Bank 1 (512 Kbyte) | Page 0            | 0x0800 0000 – 0x0800 07FF | 2 KB         |
  * | ^                 | ^                  | Page 1            | 0x0800 0800 – 0x0800 0FFF | 2 KB         |
  * | ^                 | ^                  | Page 2            | 0x0800 1000 - 0x0800 17FF | 2 KB         |
  * | ^                 | ^                  | ...               | ...                       | ...          |
  * | ^                 | ^                  | Page 255          | 0x0807 F800 – 0x0807 FFFF | 2 KB         |
  * | External memory   | 16 MB              | Page 0            | 0x0840 0000 – 0x0840 0FFF | 4 KB         |
  * | ^                 | ^                  | Page 1            | 0x0840 1000 – 0x0840 1FFF | 4 KB         |
  * | ^                 | ^                  | Page 2            | 0x0840 2000 – 0x0840 2FFF | 4 KB         |
  * | ^                 | ^                  | ...               | ...                       | ...          |
  * | ^                 | ^                  | Page 4095         | 0x093F F000 – 0x093F FFFF | 4 KB         |
  * | Information Block |                    | Boot loader       | 0x1FFF B000 – 0x1FFF EFFF | 16 KB        |
  * | ^                 | ^                  | User option bytes | 0x1FFF F800 – 0x1FFF F82F | 48 B         |
  * \n 
  *
  * **Организация FLASH памяти для AT32F403ARC (256 Kbyte)** \n 
  *
  * | Block             | Bank               | Name              | Address Range             | Size (bytes) |
  * | ------------------| :----------------: | :---------------: | :-----------------------: | :----------: |
  * | Main Flash Block  | Bank 1 (256 Kbyte) | Page 0            | 0x0800 0000 – 0x0800 07FF | 2 KB         |
  * | ^                 | ^                  | Page 1            | 0x0800 0800 – 0x0800 0FFF | 2 KB         |
  * | ^                 | ^                  | Page 2            | 0x0800 1000 – 0x0800 17FF | 2 KB         |
  * | ^                 | ^                  | ...               | ...                       | ...          |
  * | ^                 | ^                  | Page 127          | 0x0803 F800 – 0x0803 FFFF | 2 KB         |
  * | External memory   | 16 MB              | Page 0            | 0x0840 0000 – 0x0840 0FFF | 4 KB         |
  * | ^                 | ^                  | Page 1            | 0x0840 1000 – 0x0840 1FFF | 4 KB         |
  * | ^                 | ^                  | Page 2            | 0x0840 2000 – 0x0840 2FFF | 4 KB         |
  * | ^                 | ^                  | ...               | ...                       | ...          |
  * | ^                 | ^                  | Page 4095         | 0x093F F000 – 0x093F FFFF | 4 KB         |
  * | Information Block |                    | Boot loader       | 0x1FFF B000 – 0x1FFF EFFF | 16 KB        |
  * | ^                 | ^                  | User option bytes | 0x1FFF F800 – 0x1FFF F82F | 48 B         |
  * \n 
  *
  * Память FLASH используется в соответствии с установленной (кем-то когда-то) картой памяти (такой способ организации). \n 
  * Для хранения этой карты памяти требуется не менее 128 Kbyte FLASH памяти.                                            \n 
  * Подразумевается, что используется контроллер серии AT32F403AR c объёмом FLASH памяти 256 Kbyte или более.            \n 
  * В таблице (ниже) представлен способ организации FLASH памяти (с учётом установленной карты памяти)                   \n 
  * для всех контроллеров серии AT32F403AR.                                                                              \n 
  * Вся карта памяти во всех контроллерах серии размещается в Bank 1 Page0...Page63.                                     \n 
  * \n 
  * 
  * **Карта памяти для микроконтроллеров серии AT32F403AR**
  * |    Назначение     |        Bank        |  Номер страницы   |     Диапазон адресов      | Занимаемый объем  | Число страниц |
  * | ----------------- | :----------------: | :---------------: | :-----------------------: | :---------------: | :-----------: |
  * | Bootloader        | Bank 1 (256 Kbyte) | Page 0            | 0x0800 0000 – 0x0800 07FF | 28 Kbyte (0x7000) | 14 по 2 KB    |
  * | ^                 | ^                  | ...               | ...                       | ^                 | ^             |
  * | ^                 | ^                  | Page 13           | 0x0800 6800 – 0x0800 6FFF | ^                 | ^             |
  * | Main Programm     | ^                  | Page 14           | 0x0800 7000 – 0x0800 77FF | 48 Kbyte (0xC000) | 24 по 2 KB    |
  * | ^                 | ^                  | ...               | ...                       | ^                 | ^             |
  * | ^                 | ^                  | Page 37           | 0x0801 2800 – 0x0801 2FFF | ^                 | ^             |
  * | Download Buffer   | ^                  | Page 38           | 0x0800 3000 – 0x0800 37FF | 48 Kbyte (0xC000) | 24 по 2 KB    |
  * | ^                 | ^                  | ...               | ...                       | ^                 | ^             |
  * | ^                 | ^                  | Page 61           | 0x0801 E800 – 0x0801 EFFF | ^                 | ^             |
  * | Config Page       | ^                  | Page 62           | 0x0801 F000 – 0x0801 F7FF | 2 Kbyte (0x800)   | 1 по 2 KB     |
  * | RO Constants      | ^                  | Page 63           | 0x0801 F800 – 0x0801 FFFF | 2 Kbyte (0x800)   | 1 по 2 KB     |
  * | Unused            | ^                  | Page 64           | 0x0802 0000 – 0x0802 07FF | 2 Kbyte (0x800)   | 1 по 2 KB     |
  * | ^                 | ^                  | ...               | ...                       | ...               | ...           |
  * | ^                 | ^                  | Page 127          | 0x0807 F800 – 0x0807 FFFF | 2 Kbyte (0x800)   | 1 по 2 KB     |
  * | ^                 | ^                  | ...               | ...                       | ...               | ...           |
  * | ^                 | ...                | ...               | ...                       | ...               | ...           |
  * | Information Block ||                     Boot loader       | 0x1FFF B000 – 0x1FFF EFFF | 16 KB             |               |
  * | ^                 ||                     User option bytes | 0x1FFF F800 – 0x1FFF F82F | 48 B              |               |
  * 
  * Bootloader      - область расположения загрузчика                                                  \n
  * Main Programm   - область расположения основной программы                                          \n
  * Download Buffer - область для сохранения новой прошивки (используется загрузчиком)                 \n
  * Config Page     - область хранения изменяемых параметров модуля                                    \n
  * RO Constants    - область хранения неизменяемых параметров модуля (записывается при сборке модуля) \n
  * \n 
  *
  * **Карта памяти Config Page**
  * | Параметр                        |   Адрес    | 0x00  | 0x01  | 0x02 | 0x03 |
  * | ------------------------------- | :--------: | :---: | :---: | :--: | :--: |
  * | Адрес модуля                    | 0x0801F000 | addr  | 0xFF  | 0xFF | 0xFF |
  * | Версия загрузчика               | 0x0801F004 | minor | major | 0xFF | 0xFF |
  * | Версия программы                | 0x0801F008 | minor | major | 0xFF | 0xFF |
  * | Флаг первого запуска            | 0x0801F00C | flag  | 0xFF  | 0xFF | 0xFF |
  * | Не используется в модуле Modbus | 0x0801F010 | 0xFF  | 0xFF  | 0xFF | 0xFF |
  * | Параметры Modbus порта 0        | 0x0801F014 | baud  |  par  | stop | 0xFF |
  * | Параметры Modbus порта 1        | 0x0801F018 | baud  |  par  | stop | 0xFF |
  * | Параметры Modbus порта 2        | 0x0801F01C | baud  |  par  | stop | 0xFF |
  * | Параметры Modbus порта 3        | 0x0801F020 | baud  |  par  | stop | 0xFF |
  * Страница Config Page содержит параметры модуля, записанные в виде 32-битных слов.
  * \n \n 
  *
  * **Карта памяти RO Constants**
  * | Параметр           |   Адрес    | 0x00  | 0x01  | 0x02 | 0x03 |
  * | ------------------ | :--------: | :---: | :---: | :--: | :--: |
  * | Тип модуля         | 0x0801F800 | class | 0xFF  | 0xFF | 0xFF |
  * | Аппаратная ревизия | 0x0801F804 | minor | major | 0xFF | 0xFF |
  * | Серийный номер lw  | 0x0801F808 | 0xFF  | 0xFF  | 0xFF | 0xFF |
  * | Серийный номер hw  | 0x0801F80C | 0xFF  | 0xFF  | 0xFF | 0xFF |
  *
  * Страница RO Constants содержит параметры модуля, записанные в виде 32-битных слов.
  * \n \n 
  *
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
#define FLASH_SIZE            (*(uint32_t*)0x1FFFF7E0)             /*!< Flash size, in terms of KByte.               */
#define PAGE0_ADDR            0x08000000U                          /*!< Адрес начала первой страницы FLASH.          */
#define END_ADDR_OF_LAST_PAGE (PAGE0_ADDR + FLASH_SIZE * 1024 - 1) /*!< Адрес последнего байта в последней странице. */

//---Размеры требуемой FLASH памяти в Kbyte для размещения соответствующих областей---//
#define MEMSIZE_BOOTLOADER       28 /*!< Размер требуемой FLASH памяти в Kbyte.       */
#define MEMSIZE_MAIN_PROGRAM     48 /*!< Размер требуемой FLASH памяти в Kbyte.       */
#define MEMSIZE_DOWNLOAD_BUFFER  48 /*!< Размер требуемой FLASH памяти в Kbyte.       */
#define MEMSIZE_CONFIG_PAGE      2  /*!< Размер требуемой FLASH памяти в Kbyte.       */
#define MEMSIZE_RO_CONSTANS      2  /*!< Размер требуемой FLASH памяти в Kbyte.       */
//------------------------------------------------------------------------------------//

//---Начальные адреса соответствующих областей во FLASH---//
#define ADDR_BOOTLOADER       PAGE0_ADDR                                              /*!< Начальный адрес секции BootLoader.                   */
#define ADDR_MAIN_PROGRAM     (ADDR_BOOTLOADER      + MEMSIZE_BOOTLOADER      * 1024) /*!< 0x08007000U // Начальный адрес секции MainProgram.   */
#define ADDR_DOWNLOAD_BUFFER  (ADDR_MAIN_PROGRAM    + MEMSIZE_MAIN_PROGRAM    * 1024) /*!< 0x08013000U // Начальный адрес секции DowloadBuffer. */
#define ADDR_CONFIG_PAGE      (ADDR_DOWNLOAD_BUFFER + MEMSIZE_DOWNLOAD_BUFFER * 1024) /*!< 0x0801F000U // Начальный адрес секции ConfigPage.    */
#define ADDR_RO_CONSTANS      (ADDR_CONFIG_PAGE     + MEMSIZE_CONFIG_PAGE     * 1024) /*!< 0x0801F800U // Начальный адрес секции RO_Constans.   */
//--------------------------------------------------------//

#define DEF_FLASH_ADDR (END_ADDR_OF_LAST_PAGE - PAGE_SIZE_2KB + 1 ) /*!< Адрес для записи по умолчанию - адрес начала последней страницы flash (0x803F800). */

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
  * @brief   Запись Config во FLASH.
  * @param   Data - указатель типа uint32_t* на структуру с данными Config.
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
  * @brief   Чтение Config из FLASH.
  * @param   Config - указатель типа Config_struct* на структуру с данными Config.
  * @return  None.
  */
void Read_Config_from_flash (Config_struct* Config)
{
*Config = *((Config_struct*)ADDR_CONFIG_PAGE);
}
//------------------------------------------------------------------------------//


/**
  * @brief   Чтение RO_Constants из FLASH.
  * @param   RO_Constants - указатель типа RO_Constants_struct* на структуру с данными RO_Constants.
  * @return  None.
  */
void Read_RO_Constants_from_flash (RO_Constants_struct* RO_Constants)
{
*RO_Constants = *((RO_Constants_struct*)ADDR_RO_CONSTANS);
}
//------------------------------------------------------------------------------//


/**
  * @brief   Запись массива данных во FLASH.
  * @details Функция записи массива слов во FLASH.
  * @param   Address - адрес начальной ячейки памяти.
  * @param   Amount  - количество записываемых слов.
  * @param   Words   - указатель типа uint32_t* на массив с данными. Данные должны быть выровнены по 4 байта.
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
