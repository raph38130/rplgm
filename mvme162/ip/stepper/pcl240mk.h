/*---------------------------------------------------------------
Copyright (c) 1994, 1995  GreenSpring Computers

$Header: /gs/device/pcl240mk/RCS/_pcl240mk.h,v 1.5 1995/11/22 01:58:08 roy Rel $

FILE:			pcl240mk.h
DESCRIPTION:
	Register descriptions for PCL240MK stepper controller chip.
	VME address map.

REVISIONS: (most recent first)
yr/mo/day	who	description
---------	---	-----------
94/08/16	roy	new
-----------------------------------------------------------------*/

#ifndef __PCL240MK_H__
#define __PCL240MK_H__

/*==============================================================*/
/*			Stepper Motion Default Values						*/
/*==============================================================*/

#define ACCEL		380		/* Acceleration, steps/sec/sec		*/
#define CRUISE_VEL	400		/* Cruise velocity, steps/sec		*/

#define FCLK		4000000	/* 4 MHz timebase on IP-Stepper		*/

/*==============================================================*/
/*			PCL240MK Stepper Controller Chip 					*/
/*==============================================================*/

/*
** Status register (read csr)
*/

#define	IRQ_NOT_PEND	0x80	/* 1 = Interrupt NOT pending	*/
#define OP_IN_PROG		0x40	/* 1 = operation in progress,
								   0 = stop	*/
#define R0_IS_ZERO		0x08	/* Register R0 is zero			*/


/*
** Start-Stop command register
*/
#define START_STOP_CMD	0		/* Start-Stop command, bit D7-D6 = 0, p. 14*/
#define STOP_MOTION		0x08	/* immediate stop (reset)	*/
#define IRQ_AT_STOP		0x20	/* Interrupt at destination	*/
#define START_MOTION		0x10	/* Start motion		*/
#define FH1_SPEED		0x01	/* Cruise velocity at FH1 		*/
#define RAMP_UP_SPEED		0x04	/* Ramp up speed from FL to cruise velocity	*/
#define CLR_IRQ_AT_STOP	0		/* Clear interrupt pending bit	*/


/*
** Operation Mode command register
*/

#define OP_MODE_CMD		0x40	/* Operation Mode register		*/
#define EN_CURRENT_POS_CTR	0x20	/* Enable current position counter	*/
#define AUTO_RAMP_DOWN		0x10	/* Automatic ramp down			*/
#define STOP_ON_R0		0x04	/* Stop when R0 = 0				*/
#define CW_MOTION		0		/* Rotate clockwise				*/
#define CCW_MOTION		0x08	/* Rotate counterclockwise		*/

/*
** Output Mode command register
*/

#define OUTPUT_MODE_REG		0xc0	/* Output Mode register		*/
#define EXTENSION_MODE		0x08	/* Extension mode			*/

/*
** Register Select command register
*/

#define REG_SEL_CMD		0x80	/* Register Select register		*/
#define COLLECTIVE_IO		0x20	/* Collective input/output		*/
#define EXT_REG_SEL		0x10	/* Extension group register		*/


/*
NAME
	PCL240MK - register map for the PCL240MK stepper motor controller chip
	
DESCRIPTION

*/

typedef struct
{
  UINT8		pcl240mk_space0;
  UINT8		csr;
  UINT8		pcl240mk_space2;
  UINT8		data0;
  UINT8		pcl240mk_space4;
  UINT8		data1;
  UINT8		pcl240mk_space6;
  UINT8		data2;

} PCL240MK;

#endif

/*---------------------- end of pcl240mk.h --------------------*/
