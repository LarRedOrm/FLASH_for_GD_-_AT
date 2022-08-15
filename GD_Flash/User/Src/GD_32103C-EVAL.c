/**
  ******************************************************************************
  *
  * @file      CAN_GD32F103R.c
  *
  * @brief     Аппаратный драйвер для работы с платой GD_32103С-EVAL.
  * 
  * @copyright Copyright (C) 2022 Awada Systems. Все права защищены.
  *
  * @author    Larionov A.S. (larion.alex@mail.ru)
  *
  ******************************************************************************
**/

//---Includes-------------------------------------------------------------------//
#include "GD_32103C-EVAL.h"
#include "gd32f10x_gpio.h"
#include "systick.h"
//------------------------------------------------------------------------------//

//---Private macros ------------------------------------------------------------//
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
  * @brief   Инициализация микроконтроллера.
  * @details Настройки тактирования:
  *          Вкл. питание -> internal 8MRC (8MHz) -> external HXTAL (8MHz) -> PLL -> \n 
  *          -> 96MHz -> CK_SYS 96MHz -> AHB 96MHz -> /1 -> APB2 96 MHz              \n 
  *                                                |                                 \n 
  *                                                 -> /2 -> APB1 48 MHz
  * @param   None
  * @return  None
  */
void Init_MCU (void)
{
RCU_APB1EN |= RCU_APB1EN_PMUEN; // Enabled power management unit (PMU) clock.

//---Configure the system clock---
systick_config();

//---Настройка GPIO-----------------------------------------//
RCU_APB2EN |= RCU_APB2EN_PCEN; // IO port C clock enabled.
RCU_APB2EN |= RCU_APB2EN_PEEN; // IO port E clock enabled.

//---PC0, PC2 ---> LED2, LED3---//
GPIO_CTL0(GPIOC) &= (~( GPIO_CTL0_MD0 | GPIO_CTL0_CTL0 |
                        GPIO_CTL0_MD2 | GPIO_CTL0_CTL2 )); // Clear.
GPIO_CTL0(GPIOC) |= GPIO_MODE_SET(0, (GPIO_MODE_OUT_PP |
                                      GPIO_OSPEED_10MHZ)
                                      & (uint32_t)0x0FU); // GPIO output with push-pull.  
GPIO_CTL0(GPIOC) |= GPIO_MODE_SET(2, (GPIO_MODE_OUT_PP |
                                      GPIO_OSPEED_10MHZ)
                                      & (uint32_t)0x0FU); // GPIO output with push-pull.  
//------------------------------//
//---PE0, PE2 ---> LED4, LED5---//
GPIO_CTL0(GPIOE) &= (~( GPIO_CTL0_MD0 | GPIO_CTL0_CTL0 |
                        GPIO_CTL0_MD2 | GPIO_CTL0_CTL2 )); // Clear.
GPIO_CTL0(GPIOE) |= GPIO_MODE_SET(0, (GPIO_MODE_OUT_PP |
                                      GPIO_OSPEED_10MHZ)
                                      & (uint32_t)0x0FU); // GPIO output with push-pull.  
GPIO_CTL0(GPIOE) |= GPIO_MODE_SET(2, (GPIO_MODE_OUT_PP |
                                      GPIO_OSPEED_10MHZ)
                                      & (uint32_t)0x0FU); // GPIO output with push-pull.  
//------------------------------//
//----------------------------------------------------------//
}
//------------------------------------------------------------------------------//


void Blink (void)
{
GPIO_OCTL(GPIOC) |= GPIO_OCTL_OCTL0;
delay_1ms(100);
GPIO_OCTL(GPIOC) &= ~GPIO_OCTL_OCTL0;
delay_1ms(100);
}
//------------------------------------------------------------------------------//



//***************************************END OF FILE**************************************//
