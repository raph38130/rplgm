/* essai.c
 * .
 *
 * Wed Feb  4 13:09:42 CET 2004 RP
 *	- initial revision
 *
rm *asm *ihx *hex *rel *lst *rst *lnk *sym *map *mem
../tools/sdcc/bin/sdcc -mds400 --model-flat24 --stack-10bit essai.c -c
../tools/sdcc/bin/sdcc -mds400 --model-flat24 --stack-10bit \
-Wl-r --xram-loc 0x140000 --code-loc 0x490000 essai.rel 
../tools/sdcc/bin/packihx essai.ihx > essai.hex


 *
 */
#include "stdio.h" 
#include "tinibios.h"
void main()
{ 
int i=0;

printf("hello LGM V0.2.5!!!\r\n"); 
romInit('n',SPEED_2X);
printf("after init_rom!!!\r\n");
getchar();
printf("                    go!!!\r\n");
while(i<100) {
	printf("%02d ",i);
	if ((i % 10)==9) printf("\r\n");
	i++;
	} 
getchar();
printf("                    end\n\r");
getchar();
while(1);
}

 
