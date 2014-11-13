/*
 * RP 10 / 11 / 98
 *
 * mise en oeuvre ip can tip816/10
 *
 */


#include "../../../OS9/i82527.h"
#include "can.h"

char *pb;
short int *pw;
int *plw;

//descripteur du controleur can
i82527_REGS *can;

void init_IPIC()
{
//init ipic pour tip816
//les autres valeurs ont été initialisées par ENV de 162Bug
pb = (char*)IPPIC+IPAGCR;
*pb = 9;	//IPMEN IPWIDTH_16
pb = (char*)IPPIC+IPAICR0;
*pb = 0;//IEN level ... 
pb = (char*)IPPIC+IPAICR1;
*pb = 0;//IEN level ... 
//can->reg.VectorReg = 0x60;
}

void init_tip816()
{
//init pour i82527
can = (i82527_REGS *) IPABase;
can->reg.ControlReg = icCCE | icInit;	
can->reg.StatusReg = 0x07;
can->reg.CPUInterfaceReg = 0;
can->reg.GlobalMaskStandard = 0xFFFF;
can->reg.GlobalMaskExtended = 0xFFFFFFFF;
can->reg.Message15Mask = 0xFFFFFFFF;
can->reg.BusConfigReg = icDcT1;
can->reg.BitTimingReg0 =  0x47;
can->reg.BitTimingReg1 = 0x2F;
}

/* ------------------------------ */
/* initialize all message objects */
/* ------------------------------ */
void init_MsgObjects()
{
int i,j;
  
for (i=0; i<MAX_MSGOBJS; i++) {
	can->msg.object[i].Control0 =	RESET_MsgVal &
				RESET_TXIE   &
				RESET_RXIE   &
				RESET_IntPnd;

	can->msg.object[i].Control1 = RESET_RmtPnd &
				RESET_TxRqst &
				RESET_MsgLst &
				RESET_NewDat;

	can->msg.object[i].Arbitration   = 0;
	can->msg.object[i].MessageConfig = 0;
	for (j = 0; j < 8; j++)
		can->msg.object[i].data[j] = 0;
  	}
}

/*
 * disable write access to configuration register and disable software
 * initialization.
 */
void end_init() 
{
can->reg.ControlReg &= (~icCCE & ~icInit);//icEIE | icIE | 
}
