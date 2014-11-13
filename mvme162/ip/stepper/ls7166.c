/*---------------------------------------------------------------
Copyright (c) 1994, 1995  GreenSpring Computers

$Header: /gs/device/ls7166/RCS/_ls7166.c,v 1.5 1995/11/22 02:00:18 roy Rel $

FILE:			ls7166.c
DESCRIPTION:

	Chip device driver - LS7166 Quadrature decoder

REVISIONS: (most recent first)
yr/mo/day	who	description
---------	---	-----------
94/08/16	roy	new
-----------------------------------------------------------------*/

// #include <stdio.h>

#include "ptypes.h"
#include "ls7166.h"

/* #define DEBUG */

/*-G---------------------------------------------------------------
NAME
	initLS7166 - initialize the LS7166 as a quadrature decoder

SYNOPSIS
	RETURN_STATUS initLS7166( LS7166 *pCtr )

DESCRIPTION
	initLS7166() initializes the LS7166 chip as a 4X quadrature 
	decoder.

RETURNS
	SUCCESS
*/

GLOBAL RETURN_STATUS initLS7166( LS7166 *pCtr )
{
  pCtr->control = MASTER_CTL_REG | MASTER_RESET;
  pCtr->control = INPUT_CTL_REG | ENABLE_AB;
  pCtr->control = OUTPUT_CTL_REG | BINARY_COUNT | WRAP_COUNT 
					| COMP_MATCH;
  pCtr->control = QUAD_REG | QUAD_X4;
  pCtr->control = MASTER_CTL_REG | COUNTER_RESET;
  return SUCCESS;
}

/*-G---------------------------------------------------------------
NAME
	readLS7166 - read the LS7166 quadrature counter

SYNOPSIS
	INT32 readLS7166( LS7166 *pCtr )

DESCRIPTION
	readLS7166() reads the 24 bit counter of the LS7166 and
	returns a 32 bit sign extended integer value.

RETURNS
	24 bit counter value sign extended to 32 bits.
*/

GLOBAL INT32 readLS7166( LS7166 *pCtr )
{
  UINT32 count = 0;
  UINT8 *p = (UINT8 *)&count;

  pCtr->control = MASTER_CTL_REG | RESET_PR_OL_PTR | LATCH_COUNT;

  p += 3;
  *p-- = pCtr->data;	/* Least significant byte	*/
  *p-- = pCtr->data;
  *p-- = pCtr->data;	/* Most significant byte	*/
  if( count & 0x00800000 )
    {
      /* 
      ** Test the sign bit of the 24 bit counter.
      ** If negative, extend the sign bit through the upper 8 bits.
      */

      count |= 0xff000000;
    }

  return (INT32)count;
}

/*-G-----------------------------------------------------------------------
NAME
	loadPrLS7166 - load the preset register of the LS7166

SYNOPSIS
	RETURN_STATUS loadPrLS7166( LS7166 *pCtr, INT32 value )

DESCRIPTION
	loadPrLS7166() loads the given value into the preset
	register of the LS7166.  The preset register is NOT loaded
	into the counter.

RETURNS
	SUCCESS
*/

GLOBAL RETURN_STATUS loadPrLS7166( LS7166 *pCtr, INT32 value )
{
  UINT8 *p = (UINT8 *)&value;

  pCtr->control = MASTER_CTL_REG | RESET_PR_OL_PTR;

  p += 3;
  pCtr->data = *p--;	/* Least significant byte	*/
  pCtr->data = *p--;
  pCtr->data = *p;	/* Most significant byte	*/

  return SUCCESS;
}

/*-G-----------------------------------------------------------------------
NAME
	loadCtrLS7166 - load the counter from the preset register of the LS7166

SYNOPSIS
	RETURN_STATUS loadCtrLS7166( LS7166 *pCtr )

DESCRIPTION
	loadCtrLS7166() loads the contents of the preset register into
	the counter register of the LS7166.

RETURNS
	SUCCESS
*/

GLOBAL RETURN_STATUS loadCtrLS7166( LS7166 *pCtr )
{
  pCtr->control = MASTER_CTL_REG | LOAD_PRESET;
  return SUCCESS;
}

/*-G-----------------------------------------------------------------------
NAME
	clearCtrLS7166 - clear the LS7166 counter

SYNOPSIS
	RETURN_STATUS clearCtrLS7166( LS7166 *pCtr )

DESCRIPTION
	clearCtrLS7166() loads the contents of the preset register into
	the counter register of the LS7166.

RETURNS
	SUCCESS
*/

GLOBAL RETURN_STATUS clearCtrLS7166( LS7166 *pCtr )
{
  pCtr->control = MASTER_CTL_REG | COUNTER_RESET;
  return SUCCESS;
}

/*-G-----------------------------------------------------------------------
NAME
	resetWayPtLS7166 - reset the way point IRQ bit

SYNOPSIS
	RETURN_STATUS resetWayPtLS7166( LS7166 *pCtr )

DESCRIPTION
	resetWayPtLS7166() clears the waypoint comparator match
	bit which causes an interrupt.

RETURNS
	SUCCESS
*/

GLOBAL RETURN_STATUS resetWayPtLS7166( LS7166 *pCtr )
{
  pCtr->control = MASTER_CTL_REG | COMP_MATCH_RESET;
  return SUCCESS;
}

/*-G-----------------------------------------------------------------------
NAME
	wayPtLS7166 - Set and arm a way point event at the given count

SYNOPSIS
	RETURN_STATUS wayPtLS7166( LS7166 *pCtr, INT32 wayPoint )

DESCRIPTION
	wayPtLS7166() sets and arms a way point event to occur when
	the counter of the LS7166 reaches the given count.

RETURNS
	SUCCESS
*/

GLOBAL RETURN_STATUS wayPtLS7166( LS7166 *pCtr, INT32 wayPoint )
{
  loadPrLS7166( pCtr, wayPoint );
  resetWayPtLS7166( pCtr );
  return SUCCESS;
}

/*-G-----------------------------------------------------------------
NAME
	armPosLatchLS7166 - Arm pin 3 of the LS7166 to latch the position

SYNOPSIS
	RETURN_STATUS armPosLatchLS7166( LS7166 *pCtr )

DESCRIPTION
	armPosLatchLS7166() arms the LS7166 to latch the position
	when pin 3 goes low.

RETURNS
	SUCCESS
*/

GLOBAL RETURN_STATUS armPosLatchLS7166( LS7166 *pCtr )
{
  pCtr->control = INPUT_CTL_REG | ENABLE_AB | ARM_POS_LATCH;
  return SUCCESS;
}

/*-G-----------------------------------------------------------------
NAME
	armPosClearLS7166 - Arm pin 4 of the LS7166 to clear the position

SYNOPSIS
	RETURN_STATUS armPosClearLS7166( LS7166 *pCtr )

DESCRIPTION
	armPosClearLS7166() arms the LS7166 to clear the position
	when pin 4 goes low.

RETURNS
	SUCCESS
*/

GLOBAL RETURN_STATUS armPosClearLS7166( LS7166 *pCtr )
{
  pCtr->control = INPUT_CTL_REG | ENABLE_AB | ARM_POS_CLEAR;
  return SUCCESS;
}

/*------------------------- end of ls7166.c -----------------------*/
