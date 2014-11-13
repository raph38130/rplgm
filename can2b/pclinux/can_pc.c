/* RP 11 / 11 / 98
 *
 * programmation de la carte can SECCOM
 *
 * attention : 
 *	1) exécuter en suid root (voir iopl())
 *		chown root.root can_pc
 *		chmod a+s can_pc
 *	2) carte en 0x440
 *	3) compiler avec O2 : gcc -O2 can_pc.c -ocan_pc
 *
 * Exemple : 	ac 	00
 *		am 	FF
 *		btr0 	47
 *		btr1 	2F
 *		oc	FA
 *		pour liaison à 50 kbs (utilisable par 82C150)
 */
 
#include <stdio.h>
#include <unistd.h>
#include <asm/io.h>

//adresse de base carte Seccom sur PC
#define BASE	0x440
//noms des registres
char *Names[]={"cr","mcr","sr","int","ac","am","bt0","bt1","oc","tst",
		"id", "dlc","b1","b2","b3","b4","b5","b6","b7","b8",};
main()
{
int i,n;

iopl(3);
while(1)
{
for (i=0;i<10;i++) {
	outb(i,BASE);
	printf("%d %s\t: %x\n",i,Names[i],inb(BASE+1));
   }
printf("\n");
for (i=0;i<10;i++) {
	outb(10+i,BASE);
	printf("%d %s\t: %x",10+i,Names[10+i],inb(BASE+1));
	outb(20+i,BASE);
	printf("\t%d %s\t: %x\n",20+i,Names[10+i],inb(BASE+1));
   }
  
printf("registre : ");
scanf("%d",&n);
outb(n,BASE);
printf("data : ");
scanf("%x",&n);
outb(n,BASE+1);
}
}
