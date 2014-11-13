/*
 * $Id$
 */

#include "vme162.h"
#include "register.h"
#include "process.h"
#include "semaphore.h"
#include "mem.h"
#include "sys_call.h"
#include "drivers.h"

char I_BUFFER_0[Taille_Buffer];
char O_BUFFER_0[Taille_Buffer];
char I_BUFFER_1[Taille_Buffer];
char O_BUFFER_1[Taille_Buffer];
_tty tty[NTTY] = {
//port		idata/odata		iwrite/iread	full/empty echo/dispo
Z85230_0,	I_BUFFER_0,O_BUFFER_0,	0,0,		0,0,		1,1,
Z85230_1,	I_BUFFER_1,O_BUFFER_1,	0,0,		0,0,		1,1
                 };


int open_tty(int devnum, int mode)
{
_tty *t;
int port;
char s[30];
//tty_init(minor);
t = &(tty[devtab[devnum].minor]);
switch(mode) {
	case I_READ : 
		if (isbadsema(t->full))
			t->full = sys_screate(1, "full");
		t->iread = 0;		
		break;
	case I_WRITE :
		if (isbadsema(t->empty))
			t->empty = sys_screate(0, "empty");
		t->iwrite = 0;
		break;
	default : return -1;
	}
return 0;
}

int close_tty(int devnum)
{
_tty *t;

t = &(tty[devtab[devnum].minor]);
//A COMPLETER
sys_sdelete(t->empty);
sys_sdelete(t->full);
return 0;
}

//sort la chaine buf
int write_tty(int devnum, char *buf, int n)
{
_tty *t;

t = &(tty[devtab[devnum].minor]);
t->dispo = 0;		//plus "d'echo" possible sur ce tty
if (n == 0) n = strlen(buf)+1;
if (n >= Taille_Buffer) n = Taille_Buffer;
sys_wait(t->empty); 	//ici on attend la disponibilité du tty
   strncpy(t->odata, buf, n);
   StoreByte(t->port+2,t->odata[0]);
   sys_wait(t->empty); 	//ici on attend la fin de l'écriture par it 
sys_signal(t->empty);
t->dispo = 1; 
return 0;
}
//attend une entrée terminée par CR 
int read_tty(int devnum, char *buf, int n)
{
_tty *t;

t = &(tty[devtab[devnum].minor]);
sys_wait(t->full);
if (n == 0) n = strlen(t->idata)+1;
if (n >= Taille_Buffer) n = Taille_Buffer;
strncpy(buf, t->idata, n);
return 0;
}
int ioctl_tty(int devnum, int mode)
{
_tty *t;

t = &(tty[devtab[devnum].minor]);
switch(mode) {
   case ECHO : t->echo = 1; break;
   case RAW  : t->echo = 0; break;
   default : return -1;
   }
return 0;
}
/*
 * Sur reception de XOFF le driver doit cesser d'envoyer des caracteres
 * et reprendre sur reception de XON
 * A TERMINER
*/

#define ResetHighestIUS		0x38
#define ResetTxItPending	0x28
#define EnableItOnNextRx	0x20

#define XOFF 	0x13
#define XON	0x11

void lire(int port)
{
char c;
_tty *t;

t = &(tty[port]);
ReadByte(t->port+2, c);
StoreByte(t->port, ResetHighestIUS);
#define print(s) outstr(s,s+strlen(s))
if (c == XOFF) print("XOFF recu");
if (c == XON) print("XON recu");
//StoreByte(t->port+2,XOFF);
//StoreByte(t->port,ResetHighestIUS);
//attention : une sortie peut etre en cours
if (t->echo && t->dispo) {
   StoreByte(t->port+2, c);
   StoreByte(t->port,ResetTxItPending);
   }
if ((c == CR) || (c == LF)) {
    t->idata[t->iread] = 0;
    t->iread = 0;
    sys_signal(t->full);
    }
else {
    t->idata[t->iread] = c;
    t->iread = t->iread + 1;
    if (t->iread >= (Taille_Buffer-2)) {
	t->iread = Taille_Buffer-2;
	StoreByte(t->port+2, BEEP);
	}
    }
//StoreByte(t->port+2,XON);
//StoreByte(t->port,ResetHighestIUS);
}
void ecrire(int port)
{
_tty *t;

t = &(tty[port]);
StoreByte(t->port, ResetHighestIUS);
if ((t->odata[t->iwrite] == 0) || (t->iwrite >= Taille_Buffer)) {
   StoreByte(t->port, ResetTxItPending);
   t->iwrite = 0;
   sys_signal(t->empty);
   return ;
   }
t->iwrite = t->iwrite+1;
StoreByte(t->port+2,t->odata[t->iwrite]);
//LF -> LF + CR
//A TERMINER (si mode & CRMOD == 1)
if (t->odata[t->iwrite] == LF) StoreByte(t->port+2, CR) ;
}



/////////////////////////////
//version avec handler commun
/////////////////////////////
#define tbe_1	2
#define rbf_1	4
#define tbe_0	16
#define rbf_0	32
#define err_0	8
#define err_1	1

void handler_Z85230()
{
char c;

//déterminer la ou les causes par la lecture de RR3
l1 :
StoreByte(Z85230_0,3);
ReadByte(Z85230_0,c);
if (c & tbe_0) {ecrire(0); goto l1;}
if (c & rbf_0) {lire(0); goto l1;}
if (c & tbe_1) {ecrire(1); goto l1;}
if (c & rbf_1) {lire(1); goto l1;}
if ((c & err_0) || (c & err_1)) print("erreur reception");
}
