/*******************************************************************************
*******************************************************************************
**                                                                           **
**                                                                           **
**                          @@@@@@@@@@@@@@@@@@@@@@@                          **
**                          @                     @                          **
**                          @     t i p 8 1 6     @                          **
**                          @                     @                          **
**                          @@@@@@@@@@@@@@@@@@@@@@@                          **
**                                                                           **
**                                                                           **
**    Project          TIP816 - Device Driver                                **
**                                                                           **
**    File             tip816.h                                              **
**                                                                           **
**    Function         TIP816 Programming Model Definitions                  **
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
**    Author           Torsten Klegin   05/28/96                             **
**                                                                           **
**                                                                           **
*******************************************************************************
*******************************************************************************/

#ifndef __INCtip816h
#define __INCtip816h


/*----------------------------------------------------------------------
  additional cntrl function codes supported by this driver
  ----------------------------------------------------------------------*/

#define	SS_READ		200
#define	SS_WRITE	201
#define SS_FLUSH	206
#define	SS_BUSON	210
#define	SS_SETFILTER	211
#define	SS_BITTIMING	212
#define	SS_CANCEL	213
#define	SS_DEFINE_MSG	220
#define	SS_UPDATE_MSG	221
#define	SS_CANCEL_MSG	222
#define	SS_STATUS	230
#define	SS_CAN_STATUS	231
#define	SS_GETFILTER	232


/*----------------------------------------------------------------------
  io_flags
  ----------------------------------------------------------------------*/

#define	T816_RECEIVE		(1<<0)
#define	T816_TRANSMIT		(1<<1)
#define	T816_REMOTE		(1<<2)
#define T816_FLUSH		(1<<3)
#define	T816_NOWAIT		(1<<4)
#define T816_THREE_SAMPLES	(1<<5)


/*----------------------------------------------------------------------
  Bit Rates
  ----------------------------------------------------------------------*/

#define	T816_1_6MBIT	0x0011	/* 1.6 Mbit/s   max. distance = 20 m 	*/
#define	T816_1MBIT	0x0014	/*   1 Mbit/s   max. distance = 40 m 	*/
#define	T816_500KBIT	0x001c	/* 500 Kbit/s	max. distance = 130 m	*/
#define	T816_250KBIT	0x011c	/* 250 Kbit/s	max. distance = 270 m	*/
#define	T816_125KBIT	0x031c	/* 125 Kbit/s	max. distance = 530 m	*/
#define	T816_100KBIT	0x432f	/* 100 Kbit/s	max. distance = 620 m	*/
#define	T816_50KBIT	0x472f	/*  50 Kbit/s	max. distance = 1.3 km	*/
#define	T816_20KBIT	0x532f	/*  20 Kbit/s	max. distance = 3.3 km  */
#define	T816_10KBIT	0x672f	/*  10 Kbit/s	max. distance = 6.7 km	*/
#define	T816_5KBIT	0x7f7f	/*   5 Kbit/s	max. distance = 10 km	*/
/*			    xx................. Bit Timing 1		*/
/*			  xx................... Bit Timing 0		*/


/*----------------------------------------------------------------------
  Status & Error codes
  ----------------------------------------------------------------------*/

#define E_NOERR		0	/* No error */
#define E_NXIO		0x100	/* no tip816 device */
#define E_ICMD		0x102	/* illegal cntrl command */
#define E_TIMEOUT	0x105	/* timeout */
#define E_IO		0x10b
#define E_BUSY		0x10d	/* the device is busy */
#define E_INIT		0x10e	/* error during initialize */
#define E_ABORT		0x10f	/* request was aborted */
#define E_NOTINIT	0x110	/* device is not initialized */
#define E_IDEVICE	0x140	/* invalid device */
#define E_OVERRUN	0x141	/* data overrun */
#define E_BUSOFF	0x142	/* controller is in Bus Off state */
#define E_NODATA	0x143	/* no message received */
#define E_IPARAM	0x144	/* illegal initialization param. */
#define E_PARAM_MISMATCH 0x145	/* common parameter mismatch */
#define E_IMSGNUM	0x146	/* illegal message number */
#define E_MSGBUSY	0x147	/* message already allocated */
#define E_MSGNOTDEF	0x148	/* message not defined */


/****************************************************************************
 * I/O parameter block for read() and write() requests
 ****************************************************************************/

typedef struct
{
  unsigned long flags;          /* I/O Flags */
  unsigned long	status;		/* message status */
  unsigned long timeout;	/* timeout in ticks */
  unsigned long	identifier;	/* standard or extended identifier */
  unsigned char extended;	/* TRUE if extended 29-bit identifier */
  unsigned char	length;		/* 0..8 Byte data */
  unsigned char	data[8];
} t816_IO_BUFFER;



/****************************************************************************
 * I/O parameter block for cntrl() request
 ****************************************************************************/

typedef struct
{
  unsigned long  cmd;    /* Function code for special I/O operation */
  unsigned long  flags;  /* I/O Flags */
  unsigned long	 arg;	 /* optional argument */
} DC_ARGS;



/****************************************************************************
 * Parameter block for message status request
 ****************************************************************************/

typedef struct
{
  unsigned long	message_sel;
  unsigned long	status;
} T816_STATUS;

/****************************************************************************
 * Masks for acceptance filtering
 ****************************************************************************/

typedef struct 
{
  unsigned short GlobalMaskStandard;
  unsigned long  GlobalMaskExtended;
  unsigned long  Message15Mask;
} ACCEPT_MASKS;


/****************************************************************************
 * Message Object Definitions
 ****************************************************************************/

typedef struct
{
  unsigned char MsgNum;		/* Number of message object to define */
  unsigned long	identifier;	/* standard or extended identifier */
  unsigned char extended;	/* TRUE if extended 29-bit identifier */
  unsigned char	length;		/* 0..8 Byte data */
  unsigned char	data[8];
}  MSGDEF;


#endif /* __INCtip816h */
