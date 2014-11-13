/*---------------------------------------------------------------
Copyright (c) 1994,1995  GreenSpring Computers

$Header: /gs/util/std_incl/RCS/_ptypes.h,v 1.5 1995/11/06 21:47:26 steve Rel $

FILE:			ptypes.h
DESCRIPTION:	Basic type definitions for portability.

	Numerical Types:
	----------------
	Certain types, integers in particular, are sensitive
	to the target CPU and compiler.  Please review these
	types and redefine them if necessary to ensure the
	desired integer word lengths are obtained.
	
	QuickPacks use some floating point arithmetic.  The default
	precision used is declared by the typedef REAL.  Please
	modify this type definition to suit the precision required by
	your application.
	

	Function Return Status:
	-----------------------
	Function return status conventions are generally programming
	environment dependent, i.e. some programmers write their functions
	to return 1 for success while others will return 0.  QuickPacks
	use the definitions SUCCESS, FAILURE and NOT_AVAILABLE as function
	return status values.  Please modify these function return
	status definitions to suit the convention of your programming
	environment. For future expansion, allocate codes for your own
	programs starting at USER_RETURN_STATUS or higher.


	Word/Long Integer Byte Ordering
	-------------------------------
	Many peripheral chips have byte wide registers and require
	word or long integer values to be loaded one byte at a time.
	Two unions are declared in this file to facilitate byte-wise
	loading of integer values.  These unions are CPU sensitive!
	The order of the byte declarations in the union must be
	modified to match the integer byte ordering of the target CPU.


REVISIONS: (most recent first)
yr/mo/day	who		description
---------	---		-----------
95/11/06    SAFW    Added conditionals for smoothing
95/10/26	roy		function return status type RETURN_STATUS
95/02/21	roy		added byte ordering unions
94/07/14	roy		new
-----------------------------------------------------------------*/

#ifndef __PTYPES_H__
#define __PTYPES_H__


typedef char	CHAR;
typedef char *	STRING;

/*------------------------------------------*/
/*		Integer type definitions			*/
/*		------------------------			*/
/* Integer types are the most sensitive to	*/
/* target CPU and compiler implementation	*/
/* The C language does not enforce integer	*/
/* word length for int, short or long.		*/
/* It is generally assumed that a char is	*/
/* 8 bits, a short is 16 bits and a long	*/
/* is 32 bits.  Please review your compiler	*/
/* manual and modify the type definitions	*/
/* below as necessary.						*/
/*------------------------------------------*/

typedef unsigned char	UINT8;		/* 8 bit unsigned integer	*/
typedef unsigned short	UINT16;		/* 16 bit unsigned integer	*/
typedef unsigned long	UINT32;		/* 32 bit unsigned integer	*/
typedef unsigned int	UINT;		/* unsigned integer native to this CPU	*/

typedef char			INT8;		/*  8 bit signed integer	*/
typedef short			INT16;		/* 16 bit signed integer	*/
typedef long			INT32;		/* 32 bit signed integer	*/
typedef int				INT;		/* integer length native to this CPU	*/


/*------------------------------------------*/
/*		Word/Long Byte Ordering				*/
/*		-----------------------				*/
/* The following unions are used to			*/
/* disassemble words and long words into	*/
/* their byte components.  This is used to	*/
/* feed peripheral chips that have byte		*/
/* wide registers and require loading word	*/
/* or long integer values one byte at a 	*/
/* time. 									*/
/*											*/
/* NOTE: THE ORDER OF THE BYTES IN THE		*/
/*		 UNION IS DEPENDENT ON THE CPU		*/
/*		 USED.  Motorola CPU's place the	*/
/*		 high byte first, while Intel CPU's	*/
/*		 place the low byte first.  Please	*/
/*		 modify the byte ordering in these	*/
/*		 unions to match the byte ordering	*/
/*		 of your target CPU.				*/
/*------------------------------------------*/

typedef union
{
  UINT16	val;
  struct
	{
#ifdef LITTLE_ENDIAN
	  UINT8 lo;
	  UINT8 hi;
#else			/* BIG_ENDIAN */
	  UINT8	hi;
	  UINT8	lo;
#endif
	} byte;

} WORD_BYTE_ORDER;


typedef union
{
  UINT32	val;

  struct
	{
#ifdef LITTLE_ENDIAN
	  UINT16 lo;
	  UINT16 hi;
#else			/* BIG_ENDIAN */
	  UINT16 hi;
	  UINT16 lo;
#endif
	} word;

  struct
	{
#ifdef LITTLE_ENDIAN
      UINT8 byte0;
      UINT8 byte1;
      UINT8 byte2;
      UINT8 byte3;
#else			/* BIG_ENDIAN */
      UINT8 byte3;
      UINT8 byte2;
      UINT8 byte1;
      UINT8 byte0;
#endif
	} byteval;

} LONG_BYTE_ORDER;


/*------------------------------------------*/
/*		Floating Point definition			*/
/*		-------------------------			*/
/* This floating point definition is		*/
/* the is the default floating point		*/
/* precision used by QuickPack software.	*/
/* All floating point variables and			*/
/* operations are declared REAL.  When a	*/
/* particular precision is needed, e.g.		*/
/* double, it must be explicitly declared.	*/
/*------------------------------------------*/

typedef double	REAL;		/* Default floating point precision	*/


/*------------------------------------------*/
/*		NULL pointer definitions			*/
/*------------------------------------------*/

typedef void	VOID;
/* #define NULL (void *)0  -- typically defined in standard C headers */


/*----------------------------------------------*/
/*			Function return status				*/
/*			----------------------				*/
/* Please modify the return status to follow	*/
/* the convention of your programming			*/
/* environment.									*/
/*----------------------------------------------*/

typedef enum
{
	SUCCESS			=	0,
	FAILURE			=	1,
	NOT_AVAILABLE	=	2,	/* This return status is unique to QuickPacks.
							   It indicates that the module for the given
							   IndustryPack is not installed, or a particular
							   function is not available in the given IP.
							   Please ensure that this status value does
							   not conflict with the status or error return
							   values in your programming environment.
							*/
	USER_RETURN_STATUS = 1000  /* User's should create status numbers from
								  here upward only */
} RETURN_STATUS;


/*----------------------------------------------*/
/*	Function or Variable Scope Definitions		*/
/*	--------------------------------------------*/
/* These scope definitions are QuickPack		*/
/* programming conventions only.  QuickPacks	*/
/* are divided into modules, where each IP is	*/
/* one module.  Functions or variables that		*/
/* should be known only within one module		*/
/* are marked by the declaration LOCAL.  Other 	*/
/* modules should respect this privacy and not	*/
/* call LOCAL functions or variables.  The C	*/
/* compiler or linker does not enforce these	*/
/* bounds.  Functions or variables labeled		*/
/* GLOBAL are intended for use by anyone and	*/
/* may be used as such.							*/
/*----------------------------------------------*/

#define HIDDEN static		/* for use within the file only		*/
#define LOCAL				/* for use within this module only	*/
#define GLOBAL				/* globally available to anyone		*/


/*--------------------------*/
/*	Logic definitions		*/
/*--------------------------*/

#if !defined(TRUE)
#define TRUE	1
#endif

#if !defined(FALSE)
#define FALSE	0
#endif

#define YES		1
#define NO		0

#endif

/*------------------ end of ptypes.h -----------------*/
