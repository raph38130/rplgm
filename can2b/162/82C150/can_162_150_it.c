/*
 * RP 10 / 11 / 98
 *
 * mise en oeuvre ip can tip816/10 avec 82C150
 *
 * version "interruption"
 *
 *	82C150 : can id 01110000100 (switch carte : ON 234 OFF 1)
 *
 *		port13	en sortie TOR clignotante : affichage + / -
 *		port12	en entrée TOR avec it     : affichage ON / OFF
 *
 *	Remarques :
 *		entrée haute impédance
 *		liaison 50kbs
 */

#include "i82527.h"
#include "register.h"
#include "vme162.h"
#include "can.h" 

#define print(s) outstr(s,s+strlen(s))

//une it horloge toutes les 50 ms pour la synchro du 82C150
#define TICK 	50	
#define IVBR	80

//format de la trame DLC,A0,A1,B1,B2,B3,...
#define ID1	0x70 //01110000100
#define ID2	0x80
//trame de synchro du 82C150
//il faut envoyer 2 trames tous les 8192 bits
char sy[]={2,0x15,0x40,0xAA,0x04};
//output enable
char oe[] ={3,ID1,ID2,0x4,0x20,0x00};
//interrupt on positive and negative edge
char pe[] ={3,ID1,ID2,0x1,0x10,0x00};
char ne[] ={3,ID1,ID2,0x2,0x10,0x00};
//valeur des ports
char io0[]={3,ID1,ID2,0x3,0x00,0x00};	//tout à 0
char io1[]={3,ID1,ID2,0x3,0x20,0x00};	//port13 à 1

void WriteMsg(int N,char T[])
{
int i;
MSGOBJ *msg;

msg = & (can->msg.object[N-1]);
msg->Control0 = SET_MsgVal & RESET_TXIE & RESET_RXIE & RESET_IntPnd;
msg->Control1 = RESET_RmtPnd & SET_CPUUpd & SET_NewDat;
msg->Arbitration = ((((T[1] & 0xFF) << 8) | (T[2]& 0xFF)) << 16);
msg->MessageConfig = ((T[0]& 0xFF) <<4)| icDIRTX;
for(i=0;i<T[0];i++) msg->data[i]=T[3+i];
}

void SendMsg(int N)
{
MSGOBJ *msg;

msg = &(can->msg.object[N-1]);
msg->Control1 = SET_TxRqst & RESET_CPUUpd;
}

int SendOK(int N)
{
MSGOBJ *msg;
msg = &(can->msg.object[N-1]);
return msg->Control1 & TEST_TxRqst;
}

void reception()
{
switch(can->reg.InterruptReg-3) {
	case 4 : //82C150
		 //on ne s'occupe que des messages concernant l'Input Register
		 if (can->msg.object[4].data[0]==0) {
		     if (can->msg.object[4].data[1] & 0x10)
		          print("                  ON \r");
		     else print("                  OFF\r");
		     }
		 break;
	case 5 : //seccom
		 print("message reçue de la carte PC CAN Seccom\n");
		 break;
	default: print("interruption imprévue\n");;
	}
//it ack
can->msg.object[can->reg.InterruptReg-3].Control0 = RESET_IntPnd;
}

void delai()
{
int i;
for(i=0;i<1000000;i++);
}

void synchro()
{
WriteMsg(1,sy);
SendMsg(1);
while(SendOK(1));
//acquitement it timer
ack_tick();
}

void main() 
{
char buffer[80];
int i,r,d;
init_IPIC();
init_tip816();
init_MsgObjects();
end_init();
delai();

//programmer timer MCchip : une it toutes les TICK mili secondes
StoreByte(MCchip_Base + General_Control_R, 0x2);	/* MIEN */
StoreByte(MCchip_Base+Timer1_Control_R, 3);      	/* CEN COC */
StoreLong(MCchip_Base+Timer1_Compare_R, TICK*1000);	
StoreByte(MCchip_Base+Timer1_Interrupt_R, 0x11);   	/* IEN IL=1 */

//handler RXIE et Timer1
SetHandler(IRQ_RX*4 + VBR, reception);
SetHandler(((IVBR+Timer1_IRQ)*4 + VBR), synchro );

/* initialisation de deux objets-messages :
 *	n°4 recoit les messages provenant du 82C150
 *	n°5 recoit les messages provenant du PC par la Seccom
 * Ces objets génèrent une it lors de chaque réception.
 */
//pour recevoir un message de la carte SECCOM
can->msg.object[5].Control0 = SET_MsgVal & SET_RXIE;
can->msg.object[5].Arbitration   = 0x80000000;
can->msg.object[5].MessageConfig &= ~icDIRTX | ~ icXtd;
can->msg.object[5].Control1 = RESET_CPUUpd & RESET_NewDat;

//pour recevoir le message de la 82C1250
can->msg.object[4].Control0 = SET_MsgVal & SET_RXIE;
can->msg.object[4].Arbitration   = 0x70a00000;
can->msg.object[4].MessageConfig &= ~icDIRTX | ~ icXtd;
can->msg.object[4].Control1 = RESET_CPUUpd & RESET_NewDat;

//initialisation 82C150 par l'envoi de deux trames de synchro
WriteMsg(1,sy);
SendMsg(1);
while(SendOK(1));
WriteMsg(1,sy);
SendMsg(1);
while(SendOK(1));
print("INIT 82C150 0K\n\r");
//configuration du port I/O TOR
WriteMsg(2,oe);
SendMsg(2);
while(SendOK(2));
WriteMsg(2,pe);
SendMsg(2);
while(SendOK(2));
WriteMsg(2,ne);
SendMsg(2);
while(SendOK(2));

//démasquer les it
sti();
while(1) {
	delai();
   	//IO OFF
	print("    -\r");
	WriteMsg(1,io0);
	SendMsg(1);
	while(SendOK(1));
	delai();
	//IO ON
	print("    +\r");
	WriteMsg(1,io1);
	SendMsg(1);
	while(SendOK(1));
	}

}
//fin
