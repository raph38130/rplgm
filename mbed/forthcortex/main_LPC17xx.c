/**************************************************************************//**
 * @file     main.c
 * @brief    CMSIS Cortex-M3 Blinky example
 *           Blink a LED using CM3 SysTick
 * @version  V1.03
 * @date     24. September 2009
 *
 * @note
 * Copyright (C) 2009 ARM Limited. All rights reserved.
 *
 * @par
 * ARM Limited (ARM) is supplying this software for use with Cortex-M 
 * processor based microcontrollers.  This file can be freely distributed 
 * within development tools that are supporting such ARM based processors. 
 *
 * @par
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *


arm-none-eabi-gcc main_LPC17xx.c uart.c startup_LPC17xx.s system_LPC17xx.c ../../../CoreSupport/core_cm3.c -T LPC17xx.ld -I ../../../CoreSupport/ -I ../../../DeviceSupport/NXP/LPC17xx/ -o main.elf -mcpu=cortex-m3 -mthumb


arm-none-eabi-objcopy -O binary -j .data -j .text main.elf main.bin



 ******************************************************************************/
 


#include "LPC17xx.h"


volatile uint32_t msTicks;                            /* counts 1ms timeTicks */
/*----------------------------------------------------------------------------
  SysTick_Handler
 *----------------------------------------------------------------------------*/
void SysTick_Handler(void) {
  msTicks++;                        /* increment counter necessary in Delay() */
}

/*------------------------------------------------------------------------------
  delays number of tick Systicks (happens every 1 ms)
 *------------------------------------------------------------------------------*/
__INLINE static void Delay (uint32_t dlyTicks) {
  uint32_t curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks);
}

/*------------------------------------------------------------------------------
  configer LED pins
 *------------------------------------------------------------------------------*/
__INLINE static void LED_Config(void) {

  LPC_GPIO1->FIODIR = 0xffffffff;               /* LEDs PORT1 are Output */
}

/*------------------------------------------------------------------------------
  Switch on LEDs
 *------------------------------------------------------------------------------*/
__INLINE static void LED_On (uint32_t led) {

  LPC_GPIO1->FIOPIN |=  (led);                  /* Turn On  LED */
}


/*------------------------------------------------------------------------------
  Switch off LEDs
 *------------------------------------------------------------------------------*/
__INLINE static void LED_Off (uint32_t led) {

  LPC_GPIO1->FIOPIN &= ~(led);                  /* Turn Off LED */
}

/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void) {
int i;


  SystemCoreClockUpdate();
  if (SysTick_Config(SystemCoreClock / 1000)) { /* Setup SysTick Timer for 1 msec interrupts  */
    while (1);                                  /* Capture error */
  }
  
  LED_Config(); 
  SerialInit(9600);                            
 

uart_puts("hello mbed/lpro");
  while(1) {
    LED_On ((1<<23));                           /* Turn on the LED. */
    Delay (10);                                /* delay  100 Msec */
//uart_putchar('A'+ i++ % 26);
uart_puts("UART Communication is ok");
    LED_Off ((1<<23));                          /* Turn off the LED. */
    Delay (10);                                /* delay  100 Msec */
uart_puts(" hello to lgm/rp");
  }
  
}

