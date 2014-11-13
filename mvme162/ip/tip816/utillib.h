/*******************************************************************************
*******************************************************************************
**                                                                           **
**                                                                           **
**                          @@@@@@@@@@@@@@@@@@@@@@@                          **
**                          @                     @                          **
**                          @   u t i l l i b     @                          **
**                          @                     @                          **
**                          @@@@@@@@@@@@@@@@@@@@@@@                          **
**                                                                           **
**                                                                           **
**    Project          Device Driver	                                     **
**                                                                           **
**    File             utillib.h					     **
**                                                                           **
**    Function         definitions for common used functions		     **
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
**    Author           Rainer Harland   05/28/96                             **
**                                                                           **
**    System           OS-9/68K V2.4/3.0                                     **
**                                                                           **
**                                                                           **
*******************************************************************************
*******************************************************************************/

#ifndef __INCutillibh
#define __INCutillibh


#ifndef TRUE
#define TRUE		1
#endif
#ifndef FALSE
#define FALSE		0
#endif
#ifndef NIL
#define NIL		0
#endif



typedef struct {        /* RING - ring buffer */

  int pToBuf;		/* offset from start of buffer where to write next */
  int pFromBuf;		/* offset from start of buffer where to read next */
  int bufSize;		/* size of ring in bytes */
  char *buf;		/* pointer to start of buffer */

} RING_ID;



typedef	struct node {	/* Node of a linked list */

  struct node	*pred;	/* Points to the previous node */
  struct node	*succ;	/* Points to the next node */
  int		id;	/* identification code for this node */

} NODE;
  


typedef struct {

  NODE	*ListHead;	/* Pionts to the head of a linked list */
  NODE	*ListTail;	/* Pionts to the tail of a linked list */
  NODE	*ListFree;	/* Pionts to the first free element */

} NODE_ID;




/* external declarations */

int 	rngCreate();
int	rngIsEmpty();
int	rngBufGet();
int	rngBufPut();
int	rngFreeBytes();
void	rngFlush();

int	jobCreate();
NODE	*jobAttach();
NODE	*jobGetFirst();
void	jobRelease();
int	jobIsEmpty();
void	jobKill();

int	intLock();
int	splhi();
void	intUnlock();
void	splx();

void	block();
void	wakup();

#endif /* __INCutillibh */

