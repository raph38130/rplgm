/* essai.c
 * .
 *
 * Wed Feb  4 13:09:42 CET 2004 RP
 *	- initial revision
 *
rm *asm *ihx *hex *rel *lst *rst *lnk *sym *map *mem
../../sdcc/bin/sdcc -mds400 --model-flat24 --stack-10bit i2ctest.c -c
../../sdcc/bin/sdcc -mds400 --model-flat24 --stack-10bit \
-L. -li2clib \
-Wl-r --xram-loc 0x140000 --code-loc 0x490000 i2ctest.rel 
../../sdcc/bin/packihx i2ctest.ihx > i2ctest.hex


 *
 */
#include "stdio.h" 
#include "tinibios.h"
#include "tini_i2c.h"


extern char BitOutI2C(char bout);
extern char i2c_recv(char addr, char count);
extern char I2CSendStop(char addr, char count, char send_stop);
extern char I2CReset(void);
extern void I2CDumpError(char error);
//extern char i2cTransmitBuffer[I2C_BUFSIZE];     /* Global transfer buffers */
//extern char i2cReceiveBuffer[I2C_BUFSIZE];

void main()
{ 
int i=0;
char s=0,buffer[25];

printf("i2ctest LGM V0.0.7 !!!\r\n"); 
romInit('n',SPEED_2X);
printf("after init_rom!!!\r\n");
getchar();
printf("read i2c bus!!!\r\n");
buffer[0]=0x00;
buffer[1]=0x06;
buffer[2]=0x0F;
buffer[3]=0xF0;
buffer[4]=0xA0;
buffer[5]=0x0E;
while(1) {
	buffer[0]=0x33;
	//i2c_start();
	s=i2c_readblock(0x7E,buffer,1);
	//i2c_stop();
	printf("read %x",buffer[0]);
	//getchar();
	//buffer[0]=0x00;
	//i2c_start();
	//s=i2c_writeblock(0x39,buffer,6);
	//i2c_stop();
	}
}

 
