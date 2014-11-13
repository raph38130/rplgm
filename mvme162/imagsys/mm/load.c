/*
 * chargement par tftp (primitive 16xBug)
 *	- alloue de la mémoire
 *	- maintient une correspondance nom --- adresse
 */

#include "net.h"
#include "types.h"
#include "load.h"
#include "mem.h"
#include "register.h"
#include <stddef.h>
int PROGID;

void init_load()
{
int i;
PROGID=0;
for(i=0;i<NBPROGS;i++) {
   PROGS[i].name[0]=0;
   PROGS[i].adr=NULL;
   }
}
	
char * getadr(char*nom)
{
int i=0;

do {
   if (strcmp(nom,PROGS[i].name)==0) return PROGS[i].adr;
   i++;
   }
while(i<NBPROGS);
return NULL;
}

//renvoie ... en cas d'erreur
int sys_load(char*file)
{
netrw_blk_16xbug b;
int s;

cli();
b.ctrl_lun=b.dev_lun=0;
b.max_length=0;	//tout le fichier
b.addr=(int) PageAlloc(DRAM);
b.offset=0;
strcpy(b.filename,file);

//passer en mode transparant
asm("move.l #0x0000c040,%d0
     movec.l %d0,%dtt0");
if ((s=netrd_16xbug(&b))!=0) print("erreur lors de load");
//fin de transparant
asm("move.l #0x00000000,%d0
     movec.l %d0,%dtt0");
     
strncmp(PROGS[PROGID].name,file,namelength);
PROGS[PROGID].adr = (char*) b.addr;
PROGID++;
if (!s) sys_create(b.addr,0,10,file,0,0);

return s;
}
