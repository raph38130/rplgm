/*******************************************************************************
*******************************************************************************
**                                                                           **
**                                                                           **
**                          @@@@@@@@@@@@@@@@@@@@@@@                          **
**                          @                     @                          **
**                          @   T 8 1 6 D E F S   @                          **
**                          @                     @                          **
**                          @@@@@@@@@@@@@@@@@@@@@@@                          **
**                                                                           **
**                                                                           **
**    Project          TIP816 - Device Driver                                **
**                                                                           **
**    File             t816defs.h	                                     **
**                                                                           **
**    Function         local symbol definitions (not used for applications)  **
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
**    System           OS-9/68K V2.4/V3.0                                    **
**                                                                           **
**                                                                           **
*******************************************************************************
*******************************************************************************/

#ifndef __INCt816defsh
#define __INCt816defsh

#include	<scf.h>
#include	<reg.h>
#include	<procid.h>
#include	<sg_codes.h>
#include	<path.h>

#include	"i82527.h"
#include	"utillib.h"
#include	"tip816.h"

/****************************************************************************
 * T816_UNITS defines the maximum number of minor devices supported by
 * this driver (Default = 4; 1 minor devices per slot)
 ****************************************************************************/

#define T812_UNITS      	4
#define MAX_CAN_CONTROLLER	1	/* Number of CAN-Controllers per */
					/* TIP816 modul                  */

#define	MAX_FIFO_MSG		200	/* length of received message FIFO */
#define MAX_READ_JOB		20
#define MAX_WRITE_JOB		20

#define	DONE_ERR		-1
#define	DONE			0
#define READY			1
#define BUSY			2
/*
#define TRUE			1
#define FALSE			0
#define NIL			0
*/

#define BLOCK_EVENT	0x400000	/* Block Event for synchron read */
					/* commands                      */

/*----------------------------------------------------------------------
  IPAC-Modul definitions
  ----------------------------------------------------------------------*/

#define READ_ACCESS     	-1
#define ID_LENGTH       	32
#define ID_OFFSET       	0x80

#define POS_MANUFACTOR  	4
#define POS_MODEL       	5
#define POS_REVISION    	6
#define POS_NO_BYTES    	10
#define POS_CRC         	11
#define	POS_TYPE		12
#define MANUFACTOR_TEWS 	0xb3    /* TEWS Datentechnik */
#define TIP816       		0x1B 	/* Model Number */ 


/*----------------------------------------------------------------------
  simple type definitions
  ----------------------------------------------------------------------*/

typedef unsigned int 	uint; 
typedef unsigned int 	ulong; 
/*
typedef unsigned short 	ushort; 
*/
typedef unsigned char 	uchar;
typedef uchar 		BYTE;
typedef	ushort 		WORD;



/*----------------------------------------------------------------------
  Job Entry for receive and transmit job's
  ----------------------------------------------------------------------*/
typedef struct
{
  NODE		node;
  ushort	io_tid;		/* task id of the calling task		*/
  int		*io_state;	/* state for write job's (READ/BUSY)	*/
  t816_IO_BUFFER *io_bufp;	/* physical address of data buffer	*/
} JOB;

/*----------------------------------------------------------------------
  Message FIFO
  ----------------------------------------------------------------------*/

typedef struct 
{
  unsigned long	identifier;	/* standard or extended identifier */
  unsigned char	length;		/* 0..8 Byte data */
  unsigned char extended;	/* TRUE if extended 29-bit identifier */
  unsigned long	data[8];
} CANMSG;


typedef struct 
{
  unsigned long	put;		/* write next element */
  unsigned long	get;		/* read next element */
  CANMSG	msg[MAX_FIFO_MSG];
} FIFO;


/*----------------------------------------------------------------------
  TIP816 Register Map ( see also 82527 Address Map )
  ----------------------------------------------------------------------*/

typedef PACKED struct 
{
  i82527_REGS	can;
  uchar		Reserved;
  uchar		VectorReg;
} t816_REGISTERS;




/*----------------------------------------------------------------------
  Device Control Block  (DCB)
  ----------------------------------------------------------------------*/

typedef struct 
{
  unsigned int	     EyeCatcher;	/* "!DCB" 			*/
  unsigned int	     open;		/* number of opened pathes	*/
  
  NODE_ID	     rdJobQ;		/* read job queue header	*/
  NODE_ID	     wrtJobQ;		/* write job queue header	*/

  unsigned char      *io_space;		/* Address of TIP816 I/O-space	*/
  unsigned char      *id_space;		/* Address of TIP816 ID-space	*/
  t816_REGISTERS      *mem;		/* TIP816 Device Register	*/
  i82527_REGS	     *can;		/* CAN-Controller Register	*/
  unsigned short     ch;		/* CAN-Controller Channel Index	*/
  unsigned long	     LastRxdErr;	/* status of last receive operation  */
  unsigned long	     LastTxdErr;	/* status of last transmit operation */
  unsigned long	     BusOff;		/* TRUE if controller in Bus Off     */

  JOB		     readJobs[MAX_READ_JOB];	/* read job pool	*/
  JOB		     writeJobs[MAX_WRITE_JOB];	/* write job pool	*/
  FIFO		     fifo;		/* read message buffer		*/
} t816_DCB;


/*---------------------------------------------------------------------
  Device descriptor block
  ---------------------------------------------------------------------*/
typedef struct
{
  struct scfstatic	v_scf;		/* Static storage for the SCF 	*/
  t816_DCB		dcb;		/* Device Control Block		*/
} dev_stat;

typedef struct
{
  mod_dev		dd;		/* Device descriptor (OS-9)	*/
  ulong			memBaseAddr;	/* Memory Base Adddress	(mapped)*/
  ushort		BusTiming;	/* initial value for bus timing	*/
} dev_desc;

#endif
