/*---------------------------------------------------------------
Copyright (c) 1994  GreenSpring Computers

$Header: /gs/ip/stepper/RCS/_stepper.h,v 1.2 1995/11/18 03:02:24 roy Rel $

FILE:			stepper.h
DESCRIPTION:
	Register maps and descriptions for IP-Stepper board
	VME address maps.

REVISIONS: (most recent first)
yr/mo/day	who	description
---------	---	-----------
94/08/16	roy	new
-----------------------------------------------------------------*/
#ifndef __IP_STEPPER_H__
#define __IP_STEPPER_H__

#include "ptypes.h"
//#include "qpmotion.h"
//#include "qp_event.h"
#include "pcl240mk.h"
#include "ls7166.h"


/*==============================================================*/
/*			IP-Stepper Control/Status Registers					*/
/*==============================================================*/


#define ESTP_IMSK	0x08	/* E-Stop interrupt mask		*/
#define ESTP_ICLR	0x10	/* Bit to clear E-stop pending			*/
#define ESTP_IPENDING	0x10	/* Status bit: E-Stop pending active	*/

#define CT_IMSK		0x02	/* Quadrature counter interrupt mask	*/
#define CT_ICLR		0x04	/* Clear counter interrupt pending		*/
#define CT_IPENDING	0x04	/* Status bit: counter interrupt pending*/

#define MT_IMSK		0x01	/* Motor controller interrupt mask	*/

typedef struct
{
  UINT8		stepperctrl_space0;
  UINT8		InterruptReg;
  UINT8		stepperctrl_space2[3];
  UINT8		Polarity;
  UINT8		stepperctrl_space4[3];
  UINT8		Source;

} IP_STEPPER_CTRL;


/*==========================================================*/
/*			IP-Stepper I/O Map Structures					*/
/*==========================================================*/

/*
NAME
	IP_STEPPER - 	register map for the IP-Stepper board

DESCRIPTION
	Register map to all the components on the IP-Stepper board.
	VME addressing.
*/

typedef struct
{
  PCL240MK			Motor1;
  PCL240MK			Motor2;
  LS7166			Encoder1;
  UINT8				ipstep_sp0[4];
  LS7166			Encoder2;
  UINT8				ipstep_sp1[4];
  IP_STEPPER_CTRL		AxisCtrl1;
  UINT8				ipstep_sp2[2];
  IP_STEPPER_CTRL		AxisCtrl2;
  UINT8				ipstep_sp3[3];
  UINT8				SyncCtrl;
  UINT8				ipstep_sp4[3];
  UINT8				intVector;

} IP_STEPPER;

#if 0
/*==============================================================*/
/*				IP-Stepper Status Codes							*/
/*==============================================================*/

typedef enum
{
STEPPER_NO_COORD,	/* Stepper relative motion
					   coordinates are not set	*/
STEPPER_OK_COORD	/* Stepper relative motion
					   coordinates are OK		*/
} IP_STEPPER_COORD_STATUS;


typedef enum
{
  STEPPER_NO_TRAJ,
  STEPPER_OK_TRAJ

} IP_STEPPER_TRAJ_STATUS;

/*==========================================================*/
/*		IP-Stepper Object Description and Defaults			*/
/*==========================================================*/

/*
** Number of axes per IP-Stepper
*/
#define STEPPER_NUM_AXES	2

/*
NAME
	IP_STEPPER_AXIS - stepper axis description structure.

DESCRIPTION
	Pointers to the relevant register sets for one stepper axis
*/

typedef struct
{
  INT						AxisID;
  IP_STEPPER_COORD_STATUS	CoordStatus;
  IP_STEPPER_TRAJ_STATUS	TrajStatus;


  PCL240MK			*pMotor;
  LS7166			*pEncoder;
  IP_STEPPER_CTRL	*pAxisCtrl;
  IP_STEPPER		*pIPStep;
	
  REAL		StepPerCount;		/* Stepper count per encoder count	*/
  INT		Acceleration;		/* Stepper counts/sec/sec			*/
  INT		CruiseVelocity;		/* Stepper counts/sec				*/
  INT32		MoveDistance;		/* Stepper counts					*/
  INT32		WayPoint;			/* Encoder counts					*/
	
  INT32		IntrMask;			/* Interrupt enable mask			*/

} IP_STEPPER_AXIS;



/*==============================================================*/
/*			IP-Stepper Motion Default Values					*/
/*==============================================================*/

/* Stepper count per encoder count calibration factor	*/

#define ST_AXIS_1_SCALE	1		/* IP-Stepper axis 1	*/
#define ST_AXIS_2_SCALE	1		/* IP-Stepper axis 2	*/

/* Default velocity profile parameters					*/

#define ACCELERATION	400		/* Acceleration, stepper counts/sec/sec	*/
#define CRUISE_VELOCITY	400		/* Cruise velocity, stepper counts/sec	*/


/*==============================================================*/
/*				IP-Stepper Interrupt Data						*/
/*==============================================================*/

#define STEPPER_IRQ_VECTOR		0x80
#define STEPPER_ISR_PRIORITY	1
#define STEPPER_IRQ_PER_AXIS	3	/* Number of events per axis
									   for an IP-Stepper	*/

/*
** Array index to the AXIS_EVENT structure for the given
** interrupt events in the pAxis->pEvent[] array.
*/
#define STEP_IRQ_ESTOP		0
#define STEP_IRQ_WAYPOINT	1
#define STEP_IRQ_ON_TARGET	2

#endif
#endif
/*------------------ end of stepper.h -----------------*/
