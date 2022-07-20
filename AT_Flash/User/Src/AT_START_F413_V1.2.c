/**
  ******************************************************************************
  *
  * @file      AT_START_F413_V1.2.c
  *
  * @brief     Аппаратный драйвер для работы с платой AT-START-F413 V1.2.
  * 
  * @copyright Copyright (C) 2022 Awada Systems. Все права защищены.
  *
  * @author    Larionov A.S. (larion.alex@mail.ru)
  *
  ******************************************************************************
**/

//---Includes-------------------------------------------------------------------//
#include "AT_START_F413_V1.2.h"
#include "at32f413_conf.h"
//------------------------------------------------------------------------------//

//---Private macros-------------------------------------------------------------//
#define GPIO_MODE_OUTPUT_LARGE_STRENGTH 1 // Output mode, large sourcing/sinking strength.
#define GPIO_FUNC_OUTPUT_PUSH_PULL      0 // General-purpose push-pull output.
#define FLOATING_INPUT                  1 // Floating input (reset state).
#define ALT_FUNC_PUSH_PULL              2 // Alternate function push-pull output.
#define PULLUP_PULLDOWN_INPUT           2 // Pull-up/pull-down input.

#define LED_ON  0
#define LED_OFF 1

#define MS_TICK (system_core_clock / 1000U)

#define MAX_DELAY 0xFFFFFFFFU // Max delay can be used in mDelay.
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
  * @details Настройки тактирования:                                                 \n 
  *          After a system reset, the HICK oscillator is selected as system clock.  \n 
  *          The internal frequency of the HICK clock is 48 MHz.                     \n 
  *          Вкл. питание -> internal HICK (48MHz) -> external HEXT (8MHz) -> PLL -> \n 
  *          -> 192MHz -> SCLK 192MHz -> AHB 192MHz -> /2 -> APB2 96 MHz             \n 
  *                                                |                                 \n 
  *                                                 -> /4 -> APB1 48 MHz
  * @param  None
  * @return None
  */
void Init_MCU (void)
{
crm_reset();                                          // Reset the crm register.
crm_clock_source_enable(CRM_CLOCK_SOURCE_HEXT, TRUE); // Enable or disable the crm clock source.

while(crm_hext_stable_wait() == ERROR) // Wait till hext is ready.
  {;}

crm_pll_config(CRM_PLL_SOURCE_HEXT, CRM_PLL_MULT_24, CRM_PLL_OUTPUT_RANGE_GT72MHZ); // Config pll clock resource.
crm_clock_source_enable(CRM_CLOCK_SOURCE_PLL, TRUE);                                // Еnable pll.

while(crm_flag_get(CRM_PLL_STABLE_FLAG) != SET) // Wait till pll is ready.
  {;}

crm_ahb_div_set(CRM_AHB_DIV_1);   // Config ahbclk.
crm_apb2_div_set(CRM_APB2_DIV_2); // Config apb2clk.
crm_apb1_div_set(CRM_APB1_DIV_4); // Config apb1clk.
crm_auto_step_mode_enable(TRUE);  // Enable auto step mode.
crm_sysclk_switch(CRM_SCLK_PLL);  // Select pll as system clock source.
while(crm_sysclk_switch_status_get() != CRM_SCLK_PLL) // Wait till pll is used as system clock source.
  {;}
crm_auto_step_mode_enable(FALSE); // Disable auto step mode.
system_core_clock_update();       // Update system_core_clock global variable.
//--------------------------------------------------------------------------//

NVIC_SetPriorityGrouping(NVIC_PRIORITY_GROUP_4); // Set the prigroup[10:8] bits according to nvic_prioritygroup value.

CRM->apb2en_bit.gpiocen = SET;                             // GPIOC clock enable.
//---Настройка LED2 Red, PC2---//
GPIOC->cfglr_bit.iomc2 &= 0;                               // Clear mode configuration.
GPIOC->cfglr_bit.iofc2 &= 0;                               // Clear function configuration.
GPIOC->cfglr_bit.iomc2 |= GPIO_MODE_OUTPUT_LARGE_STRENGTH; // Set mode configuration    : Output mode, large sourcing/sinking strength.
GPIOC->cfglr_bit.iofc2 |= GPIO_FUNC_OUTPUT_PUSH_PULL;      // Set function configuration: General-purpose push-pull output.
GPIOC->odt_bit.odt2     = LED_OFF;
//-----------------------------//
//---Настройка LED3 White, PC3---//
GPIOC->cfglr_bit.iomc3 &= 0;                               // Clear mode configuration.
GPIOC->cfglr_bit.iofc3 &= 0;                               // Clear function configuration.
GPIOC->cfglr_bit.iomc3 |= GPIO_MODE_OUTPUT_LARGE_STRENGTH; // Set mode configuration    : Output mode, large sourcing/sinking strength.
GPIOC->cfglr_bit.iofc3 |= GPIO_FUNC_OUTPUT_PUSH_PULL;      // Set function configuration: General-purpose push-pull output.
GPIOC->odt_bit.odt3     = LED_OFF;
//-------------------------------//
//---Настройка LED3 Green, PC5---//
GPIOC->cfglr_bit.iomc5 &= 0;                               // Clear mode configuration.
GPIOC->cfglr_bit.iofc5 &= 0;                               // Clear function configuration.
GPIOC->cfglr_bit.iomc5 |= GPIO_MODE_OUTPUT_LARGE_STRENGTH; // Set mode configuration    : Output mode, large sourcing/sinking strength.
GPIOC->cfglr_bit.iofc5 |= GPIO_FUNC_OUTPUT_PUSH_PULL;      // Set function configuration: General-purpose push-pull output.
GPIOC->odt_bit.odt5     = LED_OFF;
//-------------------------------//

systick_clock_source_config(SYSTICK_CLOCK_SOURCE_AHBCLK_NODIV); // Config systick clock source.
SysTick_Config(MS_TICK);                                        // Config systick reload value and enable interrupt.
}
//------------------------------------------------------------------------------//


//---Функция ожидания на SysTick----------------------------------------------------------//
void mDelay (uint32_t Delay)
{
__IO uint32_t  tmp = SysTick->CTRL; // Clear the COUNTFLAG first.
uint32_t tmpDelay = Delay;

((void)tmp); // Add this code to indicate that local variable is not used.

if(tmpDelay < MAX_DELAY) // Add a period to guaranty minimum wait.
  tmpDelay++;

while (tmpDelay != 0U)
  {
  if((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) != 0U)
    {
    tmpDelay--;
    }
  }
}
//----------------------------------------------------------------------------------------//


void Blink (void)
{
GPIOC->odt_bit.odt5 = LED_ON;
mDelay(100);
GPIOC->odt_bit.odt5 = LED_OFF;
mDelay(100);
}








//------------------------------------------------------------------------------//



//***************************************END OF FILE**************************************//
