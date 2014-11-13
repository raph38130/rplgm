/*
 * RP 10 / 11 / 98
 *
 * mise en oeuvre ip can tip816/10 / avec 82C150
 *
 * version "scrutation"
 *
 *	82C150 : can id 01110000100
 */

#include "../../../OS9/i82527.h"
#include "/disk/VME162/IMAGSYS/ARCH/68040/register.h"
#include "/disk/VME162/IMAGSYS/TARGET/MVME162/vme162.h"
#include "can.h" 
#define print(s) outstr(s,s+strlen(s))
#define TICK 50
#define IVBR	80

//trame de synchro du 82C150
//il faut envoyer 2 trames tous les 8192 bits
//DLC,A0,A1,B1,B2,B3,...
#define ID	0x70
char sy[]={2,0x15,0x40,0xAA,0x04};
//output enable
char oe[] ={3,ID,0x80,0x4,0xFF,0xFF};
//valeur des ports
char io0[]={3,ID,0x80,0x3,0x00,0x00};	//tout à 0
char io1[]={3,ID,0x80,0x3,0x20,0x00};	//port13 à 1

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
msg->Control1 = SET_TxRqst & RESET_CPUUpd & RESET_NewDat;
}

int SendOK(int N)
{
MSGOBJ *msg;
msg = &(can->msg.object[N-1]);
return msg->Control1 & TEST_TxRqst;
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
//acquitement it
ack_tick();
}

void main() 
{
char buffer[80];

init_IPIC();
init_tip816();
init_MsgObjects();
end_init();

//programmer timer MCchip : une it toutes les TICK mili secondes
SetHandler(((IVBR+Timer1_IRQ)*4 + VBR), synchro );
StoreByte(MCchip_Base+Timer1_Control_R, 3);      	/*CEN COC*/
StoreLong(MCchip_Base+Timer1_Compare_R, TICK*1000);	
StoreByte(MCchip_Base+Timer1_Interrupt_R, 0x11);   	/*IEN  IL=1*/

delai();
//init 82C150
WriteMsg(1,sy);
SendMsg(1);
while(SendOK(1));
WriteMsg(1,sy);
SendMsg(1);
while(SendOK(1));
print("INIT 82C150 0K\n\r");

WriteMsg(2,oe);
SendMsg(2);
while(SendOK(2));

//démasquer les it
sti();
while(1) {
	//instr(80,buffer);
	delai();
   	//IO OFF
	print("    -\r");
	WriteMsg(1,io0);
	SendMsg(1);
	while(SendOK(1));
	//instr(80,buffer);
	delai();
	//IO ON
	print("    +\r");
	WriteMsg(1,io1);
	SendMsg(1);
	while(SendOK(1));		
	}

}
//fin
