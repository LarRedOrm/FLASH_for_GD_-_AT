#include "GD_32103C-EVAL.h"
#include "FLASH.h"

#define ARRAY_SIZE   10
#define CHECK_NUM    2

uint32_t temp       = 0;
uint16_t temparray [10];
uint32_t TestAddress = DEFAULT_FLASH_ADDRESS;
uint32_t ReadAddress = 0;

uint32_t Config [9];

Config_struct Cfg_struct, Cfg_struct_rd;
Config_struct* Cfg    = &Cfg_struct;
Config_struct* Cfg_rd = &Cfg_struct_rd;


RO_Constants_struct RO_Constants, RO_Constants_rd;
RO_Constants_struct* RO_Const    = &RO_Constants;
RO_Constants_struct* RO_Const_rd = &RO_Constants_rd;

int main (void)
{
Init_MCU();
  
  
Config [0] = 0x11223344;
Config [1] = 0x55667788;
Config [2] = 0x99AABBCC;
Config [3] = 0xDDEEFF00;
Config [4] = 0x11223344;
Config [5] = 0x55667788;
Config [6] = 0x99AABBCC;
Config [7] = 0xDDEEFF00;
Config [8] = 0xEE1234AA;

Write_Config_to_flash(Config);

Cfg->AddrModule = 1;
Cfg->BootloaderVersion.minor = 2;
Cfg->BootloaderVersion.major = 3;
Cfg->ProgramVersion.minor    = 4;
Cfg->ProgramVersion.major    = 5;
Cfg->FirstRunFlag            = 0xAF;
Cfg->ModbusPort0Param.baud   = 6;
Cfg->ModbusPort0Param.par    = 7;
Cfg->ModbusPort0Param.stop   = 8;
Cfg->ModbusPort0Param.reserved_param = 0xEE;

  
/*
  * **Карта памяти Config Page**
  * Параметр                        | Адрес      | 0x00 | 0x01 | 0x02 | 0x03
  * ------------------------------- | ---------- | ---- | ---- | ---- | ---- 
  * Адрес модуля                    | 0x0801F000 | addr | 0xFF | 0xFF | 0xFF
  * Версия загрузчика               | 0x0801F004 | minor| major| 0xFF | 0xFF
  * Версия программы                | 0x0801F008 | minor| major| 0xFF | 0xFF
  * Флаг первого запуска            | 0x0801F00C | flag | 0xFF | 0xFF | 0xFF
  * Не используется в модуле Modbus | 0x0801F010 | 0xFF | 0xFF | 0xFF | 0xFF
  * Параметры Modbus порта 0        | 0x0801F014 | baud | par  | stop | 0xFF
  * Параметры Modbus порта 1        | 0x0801F018 | baud | par  | stop | 0xFF
  * Параметры Modbus порта 2        | 0x0801F01C | baud | par  | stop | 0xFF
  * Параметры Modbus порта 3        | 0x0801F020 | baud | par  | stop | 0xFF
*/

Write_Config_to_flash((uint32_t*)Cfg);
Read_Config_from_flash (Cfg_rd);

Write_Words_to_flash (0x0801F800U, 9, Config);

RO_Const->ModulType              = 0x12;
RO_Const->HardwareRevision.minor = 0x34;
RO_Const->HardwareRevision.major = 0x56;
RO_Const->reserved_hrev          = 0xEEEE;
RO_Const->SerialNumberHW         = 0x789ABCDE;
RO_Const->SerialNumberLW         = 0xF0F1F2F3;

Write_Words_to_flash (0x0801F800U, 4, (uint32_t*)RO_Const);

Read_RO_Constants_from_flash(RO_Const_rd);
/*
  * **Карта памяти RO Constants**
  * Параметр             | Адрес      | 0x00  | 0x01  | 0x02 | 0x03
  * -------------------- | ---------- | ----- | ----- | ---- | ---- 
  * Тип модуля           | 0x0801F800 | class | 0xFF  | 0xFF | 0xFF
  * Аппаратная ревизия   | 0x0801F804 | minor | major | 0xFF | 0xFF
  * Серийный номер hw    | 0x0801F808 | 0xFF  | 0xFF  | 0xFF | 0xFF
  * Серийный номер lw    | 0x0801F810 | 0xFF  | 0xFF  | 0xFF | 0xFF
*/

while(1)
  {

  Blink();
  }
}
  

/*

for (uint8_t i = 0; i < ARRAY_SIZE; i++)
  {
  temparray [i] = 0xA001 + i;
  }

WriteHalfWords_to_flash(TestAddress, ARRAY_SIZE, temparray);
ReadAddress = Get_Default_Address();

temp = *((uint16_t*)(ReadAddress + 2*CHECK_NUM));

while(1)
  {
  if (temp == temparray[CHECK_NUM])
    Blink();
  }
}
*/
