/*******************************************************************************
*******************************************************************************
**                                                                           **
**                                                                           **
**                          @@@@@@@@@@@@@@@@@@@@@@@                          **
**                          @                     @                          **
**                          @     i 8 2 5 2 7     @                          **
**                          @                     @                          **
**                          @@@@@@@@@@@@@@@@@@@@@@@                          **
**                                                                           **
**                                                                           **
**    Project          TIP816 - Device Driver                                **
**                                                                           **
**    File             i82527.h		                                     **
**                                                                           **
**    Function         Intel 82527 programming model 			     **
**                                                                           **
**                                                                           **
**    Owner            Tews Datentechnik GmbH                                **
**                     Am Bahnhof 7                                          **
**                     D-25469 Halstenbek                                    **
**                     Germany                                               **
**                                                                           **
**                     Tel (04101) 42637                                     **
**                                                                           **
**                                                                           **
**                     Copyright (c) 1996                                    **
**                     Tews Datentechnik GmbH                                **
**                                                                           **
**                                                                           **
**    Author           Torsten Klegin   05/29/96                             **
**                                                                           **
**                                                                           **
*******************************************************************************
*******************************************************************************/

#ifndef	__INCi82527h
#define __INCi82527h


#define MAX_MSGOBJS	15	/* number of message objects */


/*----------------------------------------------------------------------
  Control Register
  ----------------------------------------------------------------------*/
#define	icCCE	(1<<6)		/* Change Configuration Enable */
#define	icEIE	(1<<3)		/* Error Interrupt Enable */
#define	icSIE	(1<<2)		/* Status Change Interrupt Enable */
#define	icIE	(1<<1)		/* Interrupt Enable */
#define	icInit	(1<<0)		/* Initialization */


/*----------------------------------------------------------------------
  Status Register
  ----------------------------------------------------------------------*/
#define	icBOff	(1<<7)		/* Bus Off Status */
#define	icWarn	(1<<6)		/* Warning Status */
#define	icWake	(1<<5)		/* Wake up Status */
#define	icRXOK	(1<<4)		/* Receive Message Successfully */
#define	icTXOK	(1<<3)		/* Transmit Message Successfully */
#define	icLEC	7		/* Last Error Code Mask */


/*----------------------------------------------------------------------
  CPU Interface Register
  ----------------------------------------------------------------------*/
#define	icRstSt	(1<<7)		/* Hardware Reset Status */
#define	icDSC	(1<<6)		/* Devide System Clock */
#define	icDMC	(1<<5)		/* Device Memory Clock */
#define	icPwD	(1<<4)		/* Power Down Mode Enable */
#define	icSleep	(1<<3)		/* Enter Sleep Mode */
#define	icMUX	(1<<2)		/* Multiplex for ISO Low Speed Physical Layer */
#define	icCEn	(1<<0)		/* Clockout enable */


/*----------------------------------------------------------------------
  Bus Configuration Register
  ----------------------------------------------------------------------*/
#define	icCoBy	(1<<6)		/* Comparator Bypass */
#define	icPol	(1<<5)		/* Polarity */
#define	icDcT1	(1<<3)		/* Disconnect TX1 output */
#define	icDcR1	(1<<1)		/* Disconnect RX1 input */
#define	icDcR0	(1<<0)		/* Disconnect RX0 input */


/*----------------------------------------------------------------------
  Control 0/1 Register
  ----------------------------------------------------------------------*/
#define	RESET_BIT	2
#define	SET_BIT		1
#define TEST_BIT	2

#define	SET_MsgVal	(~(SET_BIT << 6))	/* Message Valid */
#define	RESET_MsgVal	(~(RESET_BIT << 6))
#define	TEST_MsgVal	(TEST_BIT<<6)

#define	SET_TXIE	(~(SET_BIT << 4))	/* Transmit Interrupt Enable */
#define	RESET_TXIE	(~(RESET_BIT << 4))
#define	TEST_TXIE	(TEST_BIT << 4)

#define	SET_RXIE	(~(SET_BIT << 2))	/* Receive Interrupt Enable */
#define	RESET_RXIE	(~(RESET_BIT << 2))
#define	TEST_RXIE	(TEST_BIT << 2)

#define	SET_IntPnd	(~(SET_BIT << 0))	/* Interrupt Pending */
#define	RESET_IntPnd	(~(RESET_BIT << 0))
#define	TEST_IntPnd	(TEST_BIT << 0)

#define	SET_RmtPnd	(~(SET_BIT << 6))	/* Remote Frame Pending */
#define	RESET_RmtPnd	(~(RESET_BIT << 6))
#define	TEST_RmtPnd	(TEST_BIT << 6)

#define	SET_TxRqst	(~(SET_BIT << 4))	/* Transmit Request */
#define	RESET_TxRqst	(~(RESET_BIT << 4))
#define	TEST_TxRqst	(TEST_BIT << 4)

#define	SET_MsgLst	(~(SET_BIT << 2))	/* Message Lost */
#define	RESET_MsgLst	(~(RESET_BIT << 2))
#define	TEST_MsgLst	(TEST_BIT << 2)

#define	SET_CPUUpd	(~(SET_BIT << 2))	/* CPU Updating */
#define	RESET_CPUUpd	(~(RESET_BIT << 2))
#define	TEST_CPUUpd	(TEST_BIT << 2)

#define	SET_NewDat	(~(SET_BIT << 0))	/* New Data */
#define	RESET_NewDat	(~(RESET_BIT << 0))
#define	TEST_NewDat	(TEST_BIT << 0)


/*----------------------------------------------------------------------
  Message Configuration Register
  ----------------------------------------------------------------------*/
#define	DLC_Shift	4		/* # shifts for Data Length Code */
#define	icDIRTX		(1<<3)		/* Direction = transmit */
#define	icXtd		(1<<2)		/* Extended Identifier */


/*----------------------------------------------------------------------
  i82527 Register Map ( see also i82527 hardware manual )
  ----------------------------------------------------------------------*/

typedef PACKED struct MessageObject 
{
  unsigned char	 Control0;
  unsigned char	 Control1;
  unsigned long  Arbitration;	/* Message Identifier */
  unsigned char  MessageConfig;	/* Message length, direction, extended ID*/
  unsigned char  data[8];	/* max. 8 byte message data */
  unsigned char  reserved;	/* don't write to this register */

} MSGOBJ;



typedef	PACKED union {

  PACKED struct {

  	unsigned char	 ControlReg;
  	unsigned char	 StatusReg;
  	unsigned char	 CPUInterfaceReg;
  	unsigned char	 Reserved1;
  	unsigned short	 HighSpeedRead;

  	unsigned short	 GlobalMaskStandard;
  	unsigned long 	 GlobalMaskExtended;
  	unsigned long 	 Message15Mask;

  	unsigned char 	 Message1[15];
	unsigned char	 CLKOUT_Reg;

  	unsigned char	 Message2[15];
  	unsigned char	 BusConfigReg;

  	unsigned char	 Message3[15];
  	unsigned char	 BitTimingReg0;

  	unsigned char	 Message4[15];
  	unsigned char	 BitTimingReg1;

  	unsigned char	 Message5[15];
  	unsigned char	 InterruptReg;

  	unsigned char	 Message6[15];
  	unsigned char	 Reserved2;

  	unsigned char	 Message7[15];
  	unsigned char	 Reserved3;

  	unsigned char	 Message8[15];
  	unsigned char	 Reserved4;

  	unsigned char	 Message9[15];
  	unsigned char	 P1CONF_Reg;

  	unsigned char	 Message10[15];
  	unsigned char	 P2CONF_Reg;

  	unsigned char	 Message11[15];
  	unsigned char	 P1IN_Reg;

  	unsigned char	 Message12[15];
  	unsigned char	 P2IN_Reg;

  	unsigned char	 Message13[15];
  	unsigned char	 P1OUT_Reg;

  	unsigned char	 Message14[15];
  	unsigned char	 P2OUT_Reg;

  	unsigned char	 Message15[15];
  	unsigned char	 SerialResetAddress;

  } reg;


  PACKED struct {

	unsigned char	 Reserved[16];

  	MSGOBJ 	 	 object[MAX_MSGOBJS];

  } msg;

} i82527_REGS;


#endif	/* __INCi82527h */
