/**
  ******************************************************************************
  *
  * @file      FLASH.h
  *
  * @brief     Header for FLASH.c file.
  *
  * @copyright Copyright (C) 2022 Awada Systems. Все права защищены.
  *
  * @author    Larionov A.S. (larion.alex@mail.ru)
  *
  ******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FLASH_H
#define __FLASH_H

//---Includes-------------------------------------------------------------------//
#include <stdint.h>
//------------------------------------------------------------------------------//

//---Defines--------------------------------------------------------------------//
#define DEFAULT_FLASH_ADDRESS (uint32_t)0xEE
//------------------------------------------------------------------------------//

//---Private types--------------------------------------------------------------//
//------------------------------------------------------------------------------//

//---Exported types-------------------------------------------------------------//
/**
  * @brief  flash status enum
  */
typedef enum
{
FLASH_WROG_ADDRES = 0, /*!< flash status is wrong address   */
FLASH_OK             , /*!< flash status is operate done    */
FLASH_ERROR            /*!< flash status is operate busy    \n 
                            flash status is program error   \n 
                            flash status is epp error       \n 
                            flash status is operate done    \n 
                            flash status is operate timeout */
} flash_status_enum;
//------------------------------------------------------------------------------//

//---Function prototypes--------------------------------------------------------//
flash_status_enum WriteHalfWords_to_flash (uint32_t Address, uint32_t Amount, uint16_t *HalfWords);
uint32_t          Get_Default_Address     (void);
//------------------------------------------------------------------------------//

  
#endif /* __FLASH_H */

//***********************************END OF FILE***********************************
