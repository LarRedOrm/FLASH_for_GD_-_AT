#include "AT_START_F413_V1.2.h"
#include "FLASH.h"

#define ARRAY_SIZE   10
#define CHECK_NUM    0


uint32_t temp       = 0;
uint16_t temparray [10];
uint32_t TestAddress = 0x0803F700;//0x803F800 - 1;//DEFAULT_FLASH_ADDRESS;
uint32_t ReadAddress = 0;

int main (void)
{
Init_MCU();

for (uint8_t i = 0; i < ARRAY_SIZE; i++)
  {
  temparray [i] = 0xA001 + i;
  }

WriteHalfWords_to_flash(TestAddress, ARRAY_SIZE, temparray);
ReadAddress = TestAddress;//Get_Default_Address();

temp = *((uint16_t*)(ReadAddress + 2*CHECK_NUM));

while(1)
  {
  if (temp == temparray[CHECK_NUM])
    Blink();
  }
}


