/**
  ******************************************************************************
  *
  * @file      GD_flash.h
  *
  * @brief     Header for GD_flash.c file.
  *
  * @copyright Copyright (C) 2022 Awada Systems. Все права защищены.
  *
  * @author    Larionov A.S. (larion.alex@mail.ru)
  *
  ******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GD_FLASH_H
#define __GD_FLASH_H

//---Includes-------------------------------------------------------------------//
#include <stdint.h>
#include "gd32f10x_fmc.h"
#include "FLASH.h"
//------------------------------------------------------------------------------//

//---Defines--------------------------------------------------------------------//
//------------------------------------------------------------------------------//

//---Private types--------------------------------------------------------------//
//------------------------------------------------------------------------------//

//---Exported types-------------------------------------------------------------//
//------------------------------------------------------------------------------//

//---Function prototypes--------------------------------------------------------//
flash_status_enum WriteHalfWord_to_flash  (uint32_t  Address, uint32_t Amount, uint16_t HalfWord);
flash_status_enum WriteHalfWords_to_flash (uint32_t  Address, uint32_t Amount, uint16_t *HalfWords);

//fmc_state_enum WriteWord_to_flash    (uint32_t Address, uint32_t Word);
//fmc_state_enum WriteWords_to_flash   (uint32_t  Address, uint32_t Amount, uint32_t *Words);

//------------------------------------------------------------------------------//

  
  
#endif /* __GD_FLASH_H */

//***********************************END OF FILE***********************************
