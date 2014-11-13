/*---------------------------------------------------------------
Copyright (c) 1994, 1995  GreenSpring Computers

$Header: /gs/device/ls7166/RCS/_ls7166.h,v 1.4 1995/11/22 02:00:20 roy Rel $

FILE:			ls7166.h
DESCRIPTION:	Data structure descriptions for the LS7166 chip
				for VME address map.

REVISIONS: (most recent first)
yr/mo/day	who		description
---------	---		-----------
94/08/16	roy		new
-----------------------------------------------------------------*/

#ifndef __LS7166_H__
#define __LS7166_H__

/*======================================================================*/
/*				LS7166 Quadrature Counter Chip							*/
/*======================================================================*/

/*
** Master Control Register
*/

#define MASTER_CTL_REG	0x00	/* Write to Master Control register				*/
#define MASTER_RESET	0x20	/* Master reset									*/
#define COMP_MATCH_RESET 0x10	/* Reset comparator match flip-flop				*/
#define LOAD_PRESET	0x08	/* Load Preset Register value into counter		*/
#define COUNTER_RESET	0x04	/* Reset counter								*/
#define LATCH_COUNT	0x02	/* Latch the counter value into Output Latch	*/
#define RESET_PR_OL_PTR	0x01	/* Reset Preset / Output Latch address pointer	*/


/*
** Input Control Register
*/

#define INPUT_CTL_REG	0x40	/* Write to Input Control register	*/
#define ENABLE_AB	0x08	/* Enable inputs A/B				*/
#define ARM_POS_LATCH	0x20	/* Arm pin 3 to latch the counter	*/
#define ARM_POS_CLEAR	0x00	/* Arm pin 4 to clear the counter	*/


/*
** Output Control Register
*/

#define OUTPUT_CTL_REG	0x80	/* Write to Output Control register	*/
#define BINARY_COUNT	0x00	/* Binary count mode	0x01 BCD	*/
#define WRAP_COUNT	0x00	/* Wrap around count mode		*/
#define COMP_MATCH	0x30	/* Enable comparator match		*/

/*
** Quadrature Register
*/

#define QUAD_REG	0xc0	/* Write to Quadrature register		*/
#define QUAD_X4		0x03	/* Decode all 4 edges				*/

/*
NAME
	LS7166 - register map for the LS7166 quadrature counter chip
	
DESCRIPTION

	+++ NOTE: THIS STRUCTURE IS HIGHLY SENSITIVE TO COMPILER IMPLEMENTATION +++
	+++		  SOME OPTIMIZING COMPILERS WILL WORD OR LONG WORD ALIGN DATA   +++
	+++		  BE SURE TO USE THE RIGHT #PRAGMA STATEMENTS AROUND THIS STRUCT ++
	+++		  TO FORCE THE CORRECT OFFSETS.									+++
	
*/

typedef struct
{
  UINT8		ls7166_space0;
  UINT8		data;					/* LS7166 data register		*/
  UINT8		ls7166_space2;
  UINT8		control;				/* LS7166 control register	*/

} LS7166;

#endif

/*------------------------ end of ls7166.h ---------------------------------*/
