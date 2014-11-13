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
**    File             utillib.c					     **
**                                                                           **
**    Function         driver-independent functions			     **
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
**    Author           Rainer Harland   				     **
**		       Torsten Klegin	05/28/96                             **
**                                                                           **
**    System           OS-9/68K V2.4/V3.0                                    **
**                                                                           **
**                                                                           **
*******************************************************************************
*******************************************************************************/


#include "utillib.h"




/*==========================================================================

  Some function for working with double linked job queue's

  ==========================================================================*/


NODE *jobAttach(pNodeId, id)


NODE_ID	*pNodeId;
int	id;

{
  NODE	*NewJob, *LastJob;


  /* if no job is available return with nil */
  if (!pNodeId->ListFree) return NIL;


  NewJob = pNodeId->ListFree;
  pNodeId->ListFree = pNodeId->ListFree->succ;

  NewJob->id = id;


  if (pNodeId->ListTail == NIL) {

	/* the job queue is empty */

	pNodeId->ListHead = pNodeId->ListTail = NewJob;
	NewJob->succ = NewJob->pred = NIL;
	return NewJob;

  }
  else {

	/* append new job after the last job in the queue */

	LastJob = pNodeId->ListTail;
	NewJob->succ = NIL;
	NewJob->pred = LastJob;
	LastJob->succ = NewJob;
	pNodeId->ListTail = NewJob;
	
	return NewJob;

  }

}



NODE *jobGetFirst(pNodeId)


NODE_ID	*pNodeId;

{
  return (pNodeId->ListHead);
}



void jobRelease(pNodeId, pDelJob)

NODE_ID	*pNodeId;
NODE	*pDelJob;

{

  if (pDelJob->succ)
	/* the deleted job had a successor */
	pDelJob->succ->pred = pDelJob->pred;
  else
	/* the deleted job was the only one in the job queue */
	pNodeId->ListTail = pDelJob->pred;


  if (pDelJob->pred)
	/* the deleted job had a predecessor */
	pDelJob->pred->succ = pDelJob->succ;
  else
	/* the deleted job was the first job in the job queue */	
	pNodeId->ListHead = pDelJob->succ;


  /* insert the deleted job in the queue of free jobs */

  pDelJob->succ = pNodeId->ListFree;
  pNodeId->ListFree = pDelJob;

}


int jobIsEmpty(pNodeId)

NODE_ID	*pNodeId;

{

  return (pNodeId->ListHead ? FALSE : TRUE );

}



int jobCreateQueue(pNodeId, queue_mem, element_size, element_number)

NODE_ID	*pNodeId;
char	*queue_mem;		/* memory frame for queue elements */
int	element_size;		/* size of each element in bytes */
int	element_number;		/* initial number of free queue elements */

{
  int i;
  NODE	*next;


  pNodeId->ListHead = NIL;
  pNodeId->ListTail = NIL;
  pNodeId->ListFree = (NODE *)queue_mem;

  if (element_size < sizeof(NODE)) return FALSE;

  if (element_number <= 0) {
	pNodeId->ListFree = NIL;
	return TRUE;
  }

  next = pNodeId->ListFree;

  for (i=0; i<element_number; i++) {
	queue_mem += element_size;
	next->succ = (NODE *)(queue_mem);
	next = next->succ;
  }

  next->succ = NIL;

  return TRUE;

}



void jobKill(pNodeId, id)

NODE_ID	*pNodeId;
int	id;


{
  NODE	*node, *next;


  node = pNodeId->ListHead;

  while (node) {
	next = node->succ;
	if (node->id == id) jobRelease(pNodeId, node);
	node = next;
  }

}




/*==========================================================================

  ring buffer functions

  ==========================================================================*/


/* initialize a ring buffer data structure */

int rngCreate(ringId, pBuffer, size)

RING_ID *ringId;
char	*pBuffer;
int	size;


{

  if (size <= 0) return FALSE;

  ringId->pToBuf = ringId->pFromBuf = 0;
  ringId->bufSize = size;

  ringId->buf = pBuffer;

  return TRUE;
}



/* this function returns TRUE if the buffer is empty */

int rngIsEmpty(ringId)

RING_ID *ringId;


{
  return (ringId->pToBuf == ringId->pFromBuf ? TRUE : FALSE);
}




int rngBufGet(ringId, buffer, nBytes) 

RING_ID	*ringId;
char 	*buffer;
int	nBytes;

{
  register int i;

  for (i=0; i<nBytes; i++) {

	if (ringId->pFromBuf == ringId->pToBuf) return i;

	buffer[i] = ringId->buf[ringId->pFromBuf++];

	if (ringId->pFromBuf == ringId->bufSize) ringId->pFromBuf = 0;

  }

  return nBytes;

}




int rngBufPut(ringId, buffer, nBytes) 

RING_ID	*ringId;
char 	*buffer;
int	nBytes;

{
  register int i;

  for (i=0; i<nBytes; i++) {

	ringId->buf[ringId->pToBuf++] = buffer[i];

	if (ringId->pToBuf == ringId->bufSize) ringId->pToBuf = 0;

	if (ringId->pToBuf == ringId->pFromBuf) {
	  	ringId->pToBuf = (ringId->pToBuf) ? ringId->pToBuf - 1 : 
						    ringId->bufSize - 1;
		return i;
	}
  }

  return nBytes;

}



int rngFreeBytes(ringId)

RING_ID	*ringId;


{

  if (ringId->pToBuf == ringId->pFromBuf) return ringId->bufSize;

  if (ringId->pToBuf > ringId->pFromBuf) {

	return (ringId->bufSize - ringId->pToBuf + ringId->pFromBuf - 1);
  }
  else {
	return (ringId->pFromBuf - ringId->pToBuf - 1);
  }

}




void rngFlush(ringId)

RING_ID	*ringId;


{
  ringId->pToBuf = ringId->pFromBuf = 0;
}
