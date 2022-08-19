/**
  ******************************************************************************
  * 
  * @file      FLASH_GD32F103R.c
  * 
  * @brief     Драйвер flash для GD32F103R.
  * 
  * @details   Функции для работы с flash в микроконтроллерах семейства GD32F103R.
  *
  * @warning   **ВАЖНО!**                                                                                  \n 
  * Для работы драйвера требуется: 
  *                       + файл gd32f10x_fmc.c - аппаратный драйвер работы с flash от GD;
  *                       + файл gd32f10x_fmc.h - header for gd32f10x_fmc.c.
  *
  * **Manual** \n 
  * В драйвере реализованы следующие функции:
  * - Write_Config_to_flash (Config_struct* Config) - запись во FLASH изменяемых параметров модуля Config Page.        \n 
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
  * В контроллерах GD32F103R размер FLASH варьируется от 16 KB (GD32F103R4T6) до 3072 KB (GD32F103RKT6). \n 
  * Размер FLASH определяет параметр плотность (density):
  * - Medium-density devices are GD32F101xx and GD32F103xx microcontrollers which the flash memory density ranges from 16 to 128 Kbytes.
  * - High-density devices are GD32F101xx and GD32F103xx microcontrollers which the flash memory density ranges from 256 to 512 Kbytes.
  * - Extra-density devices are GD32F101xx and GD32F103xx microcontrollers which the flash memory density larger than 512 Kbytes.  \n 
  *
  * Параметр density опрелеляет организацию FLASH памяти. \n \n 
  *
  * **Организация FLASH памяти для Medium-density** \n 
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
  * **Организация FLASH памяти для High-density и Extra-density** \n 
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
  * Память FLASH используется в соответствии с установленной (кем-то когда-то) картой памяти (такой способ организации). \n 
  * Для хранения этой карты памяти требуется не менее 128 Kbyte FLASH памяти.                                            \n 
  * Подразумевается, что используется контроллер серии GD32F103R c объёмом FLASH памяти не менее 128 Kbyte.              \n 
  * В двух таблицах (ниже) представлены два способа организации FLASH памяти (с учётом установленной карты памяти)       \n 
  * для контроллеров с плотностью Medium Density и High/Extra Density.                                                   \n 
  * \n 
  * **Карта памяти для микроконтроллеров серии GD32F103R Medium Density**
  * |   Назначение    | Номер страницы | Диапазон адресов          | Занимаемый объем  | Число страниц |
  * | --------------- | :------------: | :-----------------------: | :---------------: | :-----------: |
  * | Bootloader      | Page 0         | 0x0800 0000 - 0x0800 03FF | 28 Kbyte (0x7000) | 28 по 1 KB    |
  * | ^               | ...            | ...                       | ^                 | ^             |
  * | ^               | Page 27        | 0x0800 6BFF - 0x0800 6FFF | ^                 | ^             |
  * | Main Programm   | Page 28        | 0x0800 7000 - 0x0800 73FF | 48 Kbyte (0xC000) | 48 по 1 KB    |
  * | ^               | ...            | ...                       | ^                 | ^             |
  * | ^               | Page 75        | 0x0801 2BFF - 0x0801 2FFF | ^                 | ^             |
  * | Download Buffer | Page 76        | 0x0801 3000 - 0x0800 33FF | 48 Kbyte (0xC000) | 48 по 1 KB    |
  * | ^               | ...            | ...                       | ^                 | ^             |
  * | ^               | Page 123       | 0x0801 3400 - 0x0801 EFFF | ^                 | ^             |
  * | Config Page     | Page 124       | 0x0801 F000 - 0x0801 F3FF |  1 Kbyte (0x400)  | 2 по 1 KB     |
  * | ^               | Page 125       | 0x0801 F400 - 0x0801 F7FF |  1 Kbyte (0x400)  | ^             |
  * | RO Constants    | Page 126       | 0x0801 F800 - 0x0801 FBFF |  1 Kbyte (0x400)  | 2 по 1 KB     |
  * | ^               | Page 127       | 0x0801 FC00 - 0x0801 FFFF |  1 Kbyte (0x400)  | ^             |
  * \n 
  * **Карта памяти для микроконтроллеров серии GD32F103R High/Extra Density**
  * |   Назначение    | Номер страницы | Диапазон адресов          | Занимаемый объем  | Число страниц |
  * | --------------- | :------------: | :-----------------------: | :---------------: | :-----------: |
  * | Bootloader      | Page 0         | 0x0800 0000 - 0x0800 07FF | 28 Kbyte (0x7000) | 14 по 2 KB    |
  * | ^               | ...            | ...                       | ^                 | ^             |
  * | ^               | Page 13        | 0x0800 6800 - 0x0800 6FFF | ^                 | ^             |
  * | Main Programm   | Page 14        | 0x0800 7000 - 0x0800 77FF | 48 Kbyte (0xC000) | 24 по 2 KB    |
  * | ^               | ...            | ...                       | ^                 | ^             |
  * | ^               | Page 37        | 0x0801 2800 - 0x0801 2FFF | ^                 | ^             |
  * | Download Buffer | Page 38        | 0x0800 3000 - 0x0800 37FF | 48 Kbyte (0xC000) | 24 по 2 KB    |
  * | ^               | ...            | ...                       | ^                 | ^             |
  * | ^               | Page 61        | 0x0801 E800 - 0x0801 EFFF | ^                 | ^             |
  * | Config Page     | Page 62        | 0x0801 F000 - 0x0801 F7FF | 2 Kbyte (0x800)   | 1 по 2 KB     |
  * | RO Constants    | Page 63        | 0x0801 F800 - 0x0801 FFFF | 2 Kbyte (0x800)   | 1 по 2 KB     |
  * | Unused_page_2KB | Page 64        | 0x0802 0000 - 0x0802 07FF | 384 Kbyte         | 192 по 2 KB   |
  * | ^               | ...            | ...                       | ^                 | ^             |
  * | ^               | Page 255       | 0x0807 F800 - 0x0807 FFFF | ^                 | ^             |
  * | Unused_page_4KB | Page 256       | 0x0808 0000 - 0x0808 0FFF | 2556 Kbyte        | 639 по 4 KB   |
  * | ^               | ...            | ...                       | ^                 | ^             |
  * | ^               | Page 895       | 0x082F F000 - 0x082F FFFF | ^                 | ^             |
  * 
  * Bootloader      - область расположения загрузчика                                                  \n
  * Main Programm   - область расположения основной программы                                          \n
  * Download Buffer - область для сохранения новой прошивки (используется загрузчиком)                 \n
  * Config Page     - область хранения изменяемых параметров модуля                                    \n
  * RO Constants    - область хранения неизменяемых параметров модуля (записывается при сборке модуля) \n
  * \n 
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
  * | Параметр                  |   Адрес    |   0x00   |   0x01   | 0x02 | 0x03 |
  * | ------------------------- | :--------: | :------: | :------: | :--: | :--: |
  * | Тип модуля                | 0x0801F800 |  class   |   0xFF   | 0xFF | 0xFF |
  * | Аппаратная ревизия        | 0x0801F804 | hw_minor | hw_major | 0xFF | 0xFF |
  * | Серийный номер модуля lw  | 0x0801F808 |   0xFF   |   0xFF   | 0xFF | 0xFF |
  * | Серийный номер модуля hw  | 0x0801F80C |   0xFF   |   0xFF   | 0xFF | 0xFF |
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
#include "FLASH_GD32F103R.h"
//#include "gd32f10x_fmc.h"
//------------------------------------------------------------------------------//

//---Private macros-------------------------------------------------------------//
#define PAGE0_ADDR            0x08000000U                        /*!< Адрес начала первой страницы FLASH.          */
#define END_ADDR_OF_LAST_PAGE (PAGE0_ADDR + FMC_SIZE * 1024 - 1) /*!< Адрес последнего байта в последней странице. */

//---Размеры требуемой FLASH памяти в Kbyte для размещения соответствующих областей---//
#define MEMSIZE_BOOTLOADER       28 /*!< Размер требуемой FLASH памяти в Kbyte.       */
#define MEMSIZE_MAIN_PROGRAM     48 /*!< Размер требуемой FLASH памяти в Kbyte.       */
#define MEMSIZE_DOWNLOAD_BUFFER  48 /*!< Размер требуемой FLASH памяти в Kbyte.       */
#define MEMSIZE_CONFIG_PAGE      2  /*!< Размер требуемой FLASH памяти в Kbyte.       */
#define MEMSIZE_RO_CONSTANS      2  /*!< Размер требуемой FLASH памяти в Kbyte.       */
//------------------------------------------------------------------------------------//

//---Начальные адреса соответствующих областей во FLASH---//
#define ADDR_BOOTLOADER       PAGE0_ADDR                                              /*!< Начальный адрес секции BootLoader.                   */
#define ADDR_MAIN_PROGRAM     (ADDR_BOOTLOADER      + MEMSIZE_BOOTLOADER      * 1024) /*!< 0x08007000U // Начальный адрес секции MainProgram. Значение 1024 - количество байт в килобайте. */
#define ADDR_DOWNLOAD_BUFFER  (ADDR_MAIN_PROGRAM    + MEMSIZE_MAIN_PROGRAM    * 1024) /*!< 0x08013000U // Начальный адрес секции DowloadBuffer. */
#define ADDR_CONFIG_PAGE      (ADDR_DOWNLOAD_BUFFER + MEMSIZE_DOWNLOAD_BUFFER * 1024) /*!< 0x0801F000U // Начальный адрес секции ConfigPage.    */
#define ADDR_RO_CONSTANS      (ADDR_CONFIG_PAGE     + MEMSIZE_CONFIG_PAGE     * 1024) /*!< 0x0801F800U // Начальный адрес секции RO_Constans.   */
//--------------------------------------------------------//

#define DEF_FLASH_ADDR (END_ADDR_OF_LAST_PAGE - PAGE_SIZE_2KB + 1) /*!< Адрес для записи по умолчанию - адрес начала последней страницы flash (0x803F800). */

//#define NUM_OF_CONFIG_WORDS 9U /*!< Количество параметров модуля (в виде 32-битных слов), которые будут записываться в область Config Page. */
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
  * @param   Config - указатель типа Config_struct* на структуру с данными Config.
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
  * @param   Address - адрес начальной ячейки.
  * @param   Amount  - количество записываемых слов.
  * @param   Words   - указатель типа uint32_t* на массив с данными. Данные должны быть выровнены по 4 байта.
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
  * @brief   Чтение параметра Flash memory density.
  * @details Чтение значения размера FLASH памяти (Flash memory density) микроконтроллера (STM, GD, AT).
  * @return  uint16_t - размер FLASH памяти используемого микроконтроллера (STM, GD, AT) в Kbyte.
  */
uint16_t Read_MCU_FMD (void)
{
return FMC_SIZE;
}
//------------------------------------------------------------------------------//


//***************************************END OF FILE**************************************//
