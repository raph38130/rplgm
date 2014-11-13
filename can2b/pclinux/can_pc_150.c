/* RP 11/ 11 / 98
 *
 * commande 82C150 par carte PC/can SECCOM
 *
 *		P13 (TOR) clignote
 *
 * attention : 
 *	1) IDcan 82C150 fixé à 01110000100 (switch carte : ON 234 OFF 1)
 *	2) exécuter en suid root (voir iopl())
 *		chown root.root can_pc_150
 *		chmod a+s can_pc_150
 *	3) carte en 0x440
 *	4) il faut paramétrer le 82C200 avant utilisation (50kbs)
 *	5) compiler avec O2 : gcc -O2
 */
 
#include <stdio.h>
#include <unistd.h>
#include <asm/io.h>

//adresse de la carte Seccom sur le PC
#define BASE	0x440
//id can du 82C150
#define ID	0x70
char *Names[]={"cr","mcr","sr","int","ac","am","bt0","bt1","oc","tst",
		"id", "dlc","b1","b2","b3","b4","b5","b6","b7","b8",};

//trame de synchro du 82C150
//il faut envoyer 2 trames tous les 9192 bits
char sy[]={0x15,0x42,0xAA,0x04};
//output enable
char oe[]={ID,0x83,0x4,0xFF,0xFF};
//valeur des ports
char io0[]={ID,0x83,0x3,0x00,0x00};	//tout à 0
char io1[]={ID,0x83,0x3,0x20,0x00};	//port13 à 1


void Write(char T[],int N)
{
int i;

for (i=0;i<N;i++) {
	outb(10+i,BASE);
	outb(T[i],BASE+1);
   }
//TxReq
outb(1,BASE);
outb(1,BASE+1);
}
void Reset()
{
outb(0,BASE);
outb(1,BASE+1);
outb(0,BASE);
outb(0,BASE+1);
}

void Print()
{
int i;
for (i=0;i<10;i++) {
	outb(20+i,BASE);
	printf("\t%d %s\t: %x\n",20+i,Names[10+i],inb(BASE+1));
   }
}

//renvoie VRAI s'il y a un message à lire
int MsgRecieved()
{
outb(2,BASE);
return (inb(BASE+1) & 0x1);
}

void Clear()
{
int i;
for (i=0;i<10;i++) {
	outb(20+i,BASE);
	outb(0,BASE+1);
   }

}

//renvoie un si le 82C150 acquitte correctement
int TxAck()
{
//on efface le buffer pour etre sur qu'on reçoit
Clear();
while(!MsgRecieved());
//release Rx Buffer
outb(1,BASE);
outb(4,BASE+1);
//a-t-on l'id du 82C150
outb(20,BASE);
return (inb(BASE+1)==ID);
}

void main()
{
int i=0;

//autoriser l'accès aux IO
//executer en suid root
iopl(3);

Reset();sleep(1);
Clear();
Write(sy,4);
Print();
Clear();
Write(sy,4);
if(TxAck()) printf("initialisation ok\n");
else exit(0);
Print();

//I/O operation
Write(oe,5);
if(TxAck()) printf("oe ok\n");
else exit(0);
while(1)
	{
	Write(sy,4);
	//en micro secondes
	usleep(100000);
	if((i % 10)==0){
		Write(io1,5);
		if(!TxAck()) exit(0);
		printf("         +\r");
		fflush(NULL);
		}
	if((i++ % 20)==0){
		Write(io0,5);
		if(!TxAck()) exit(0);
		printf("         -\r");
		fflush(NULL);
		}
	}
}
