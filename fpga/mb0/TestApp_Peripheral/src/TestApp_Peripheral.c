/*
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A 
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR 
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION 
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE 
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO 
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO 
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE 
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY 
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 */

/*
 * Xilinx EDK 9.1.02 EDK_J_SP2.4
 *
 * This file is a sample test application
 *
 * This application is intended to test and/or illustrate some 
 * functionality of your system.  The contents of this file may
 * vary depending on the IP in your system and may use existing
 * IP driver functions.  These drivers will be generated in your
 * XPS project when you run the "Generate Libraries" menu item
 * in XPS.
 *
 * Your XPS project directory is at:
 *    /home/raph/designs/micro0/
 */


// Located in: microblaze_0/include/xparameters.h
#include "xparameters.h"

#include "stdio.h"

#include "uartlite_header.h"
#include "xbasic_types.h"
#include "xgpio.h"
#include "gpio_header.h"

//====================================================

int main (void) {
	int i=0,j;
	Xuint32 Data;
   volatile int Delay;
   Xuint32 LedBit;
   Xuint32 LedLoop;
   XStatus Status;
	XGpio GpioOutput;
	
	print("hello lpro v0.2--\r\n");
   Status = XGpio_Initialize(&GpioOutput, XPAR_LEDS_6BIT_DEVICE_ID);
   XGpio_SetDataDirection(&GpioOutput, 1, 0x0);
	while(1) { 
		i = (i + 1) % 8;
	   XGpio_DiscreteWrite(&GpioOutput, 1, 1 << i);
		for(j=0;j<100000000;j++) ;
		xil_printf("led = %d\n",i);
	   }

   return 0;
}

