/**
  ******************************************************************************
  *
  * @file      FLASH_GD32F103R.h
  *
  * @brief     Header for FLASH_GD32F103R.c file.
  *
  * @copyright Copyright (C) 2022 Awada Systems. Все права защищены.
  *
  * @author    Larionov A.S. (larion.alex@mail.ru)
  *
  ******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FLASH_GD32F103R_H
#define __FLASH_GD32F103R_H

//---Includes-------------------------------------------------------------------//
#include <stdint.h>
#include "gd32f10x.h"
#include "FLASH.h"
//------------------------------------------------------------------------------//

//---Defines--------------------------------------------------------------------//
//------------------------------------------------------------------------------//

//---Private types--------------------------------------------------------------//
//------------------------------------------------------------------------------//

//---Exported types-------------------------------------------------------------//
//------------------------------------------------------------------------------//

//---Function prototypes--------------------------------------------------------//
flash_status  Write_Config_to_flash        (Config_struct* Config);
void          Read_Config_from_flash       (Config_struct* Config);
void          Read_RO_Constants_from_flash (RO_Constants_struct* RO_Constants);
flash_status  Write_Words_to_flash         (uint32_t Address, uint32_t Amount, uint32_t *Words);
uint16_t      Read_MCU_FMD                 (void);
//------------------------------------------------------------------------------//


#endif /* __FLASH_GD32F103R_H */


//***********************************END OF FILE***********************************
