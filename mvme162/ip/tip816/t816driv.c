/******************************************************************************
*******************************************************************************
**                                                                           **
**                                                                           **
**                          @@@@@@@@@@@@@@@@@@@@@@@                          **
**                          @                     @                          **
**                          @   t 8 1 6 d r i v   @                          **
**                          @                     @                          **
**                          @@@@@@@@@@@@@@@@@@@@@@@                          **
**                                                                           **
**                                                                           **
**    Project          TIP816 - Device Driver                                **
**                                                                           **
**    File             t816driv.c                                            **
**                                                                           **
**    Function         TIP816 device driver                                  **
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
**    System           OS-9 V2.4/V3.0                                        **
**                                                                           **
**                                                                           **
*******************************************************************************
*******************************************************************************/

#define	PACKED

#include "ipchip.h"
#include "i82527.h"
#include "tip816.h"
#include "t816defs.h"
#include "utillib.h"

/****************************************************************************
 * Prototypes
 ****************************************************************************/

#ifdef VXWORKS
void	t816ResourceFree(t816_DCB	*pDCB);
#endif


/****************************************************************************
 *
 * t816Init() 	- Initialize the TIP816 driver
 *
 * RETURNS:	- OK, or ERROR if the driver cannot be installed
 *
 ****************************************************************************/
uint t816init (DevDesc,	DevStatS, ProcDesc)
	dev_desc	*DevDesc;
	dev_stat	*DevStatS;
	procid		*ProcDesc;
{
  int		CheckIPAC();

  IPIC		*ipchip = (IPIC*)0xfffbc000;
  t816_DCB	*pDCB;
  int		i,j;

  /* ========================================================== */
  /*			  Check the ID-Prom			*/
  /* ========================================================== */
  if(CheckIPAC(DevDesc->dd._mport) != E_NOERR)
  {
	return	E_NXIO;
  }

  /* ========================================================== */
  /*		   Initialize Device Control Block		*/
  /* ========================================================== */
  pDCB = &DevStatS->dcb;

  pDCB->EyeCatcher	= *(uint*)("!DCB");
  pDCB->open		= 0;

  pDCB->io_space      	= (unsigned char*)DevDesc->dd._mport;
  pDCB->id_space      	= (unsigned char*)DevDesc->dd._mport + ID_OFFSET;
  pDCB->mem           	= (t816_REGISTERS*)DevDesc->memBaseAddr;
  pDCB->can		= &pDCB->mem->can;

  pDCB->LastRxdErr    	= E_NOERR;
  pDCB->LastTxdErr    	= E_NOERR;
  pDCB->BusOff        	= FALSE;
  pDCB->fifo.get	= 0;
  pDCB->fifo.put	= 0;

  /*-------------------*/
  /* Create job queues */
  /*-------------------*/
  if (!jobCreateQueue(	&pDCB->rdJobQ, 
			pDCB->readJobs, 
			sizeof(JOB), 
			MAX_READ_JOB))
  {
	return E_INIT;
  }

  if (!jobCreateQueue(	&pDCB->wrtJobQ, 
			pDCB->writeJobs, 
			sizeof(JOB), 
			MAX_WRITE_JOB))
  {
	return E_INIT;
  }

  /* ------------- */
  /* Setup IP-chip */
  /* ------------- */
  switch ((uint)pDCB->io_space) 
  {
	case 0xfff58000: 
			 ipchip->ipambar = ((uint)pDCB->mem) >> 16;
			 ipchip->ipamsr  = 0;           /* 64KB */
			 ipchip->ipagcr  = IPWIDTH_16 | IPMEN; 
			 ipchip->ipaicr0 = DevDesc->dd._mirqlvl | IP_IEN;
			 ipchip->ipaicr1 = DevDesc->dd._mirqlvl | IP_IEN;
			 break;

	case 0xfff58100:
			 ipchip->ipbmbar = ((uint)pDCB->mem) >> 16;
			 ipchip->ipbmsr  = 0;           /* 64KB */

			 if (!(ipchip->ipagcr & (IPWIDTH_8 | IPWIDTH_16))) {
				ipchip->ipagcr  |= IPWIDTH_16;
			 }

			 ipchip->ipbgcr  = IPWIDTH_16 | IPMEN; 
			 ipchip->ipbicr0 = DevDesc->dd._mirqlvl | IP_IEN;
			 ipchip->ipbicr1 = DevDesc->dd._mirqlvl | IP_IEN;
			 break;

	case 0xfff58200:
			 ipchip->ipcmbar = ((uint)pDCB->mem) >> 16;
			 ipchip->ipcmsr  = 0;           /* 64KB */
			 ipchip->ipcgcr  = IPWIDTH_16 | IPMEN; 
			 ipchip->ipcicr0 = DevDesc->dd._mirqlvl | IP_IEN;
			 ipchip->ipcicr1 = DevDesc->dd._mirqlvl | IP_IEN;
			 break;

	case 0xfff58300:
			 ipchip->ipdmbar = ((uint)pDCB->mem) >> 16;
			 ipchip->ipdmsr  = 0;           /* 64KB */

			 if (!(ipchip->ipcgcr & (IPWIDTH_8 | IPWIDTH_16))) {
				ipchip->ipcgcr  |= IPWIDTH_16;
			 }

			 ipchip->ipdgcr  = IPWIDTH_16 | IPMEN; 
			 ipchip->ipdicr0 = DevDesc->dd._mirqlvl | IP_IEN;
			 ipchip->ipdicr1 = DevDesc->dd._mirqlvl | IP_IEN;
			 break;
  }


  /* ------------------- */
  /* Initialize Hardware */
  /* ------------------- */

  pDCB->mem->VectorReg = DevDesc->dd._mvector;

  pDCB->can->reg.ControlReg           = icCCE | icEIE | icIE | icInit;
  pDCB->can->reg.StatusReg            = 0x07;
  pDCB->can->reg.CPUInterfaceReg      = 0;

  pDCB->can->reg.GlobalMaskStandard   = 0xFFFF;
  pDCB->can->reg.GlobalMaskExtended   = 0xFFFFFFFF;
  pDCB->can->reg.Message15Mask        = 0xFFFFFFFF;

  pDCB->can->reg.BusConfigReg         = icDcT1;
  pDCB->can->reg.BitTimingReg0        = (DevDesc->BusTiming >> 8) & 0xff;
  pDCB->can->reg.BitTimingReg1        = DevDesc->BusTiming & 0xff;

  /* ------------------------------ */
  /* initialize all message objects */
  /* ------------------------------ */
  
  for (i=0; i<MAX_MSGOBJS; i++)
  {
	pDCB->can->msg.object[i].Control0 =	RESET_MsgVal &
						RESET_TXIE   &
						RESET_RXIE   &
						RESET_IntPnd;

	pDCB->can->msg.object[i].Control1 =   	RESET_RmtPnd &
					      	RESET_TxRqst &
					      	RESET_MsgLst &
					      	RESET_NewDat;

	pDCB->can->msg.object[i].Arbitration   = 0;
	pDCB->can->msg.object[i].MessageConfig = 0;
	for (j = 0; j < 8; j++)
		pDCB->can->msg.object[i].data[j] = 0;
  }

  /*
   * disable write access to configuration register and disable software
   * initialization.
   */

  pDCB->can->reg.ControlReg &= (~icCCE & ~icInit);


  return E_NOERR;
}


/****************************************************************************
 *
 * t816term() 	- Terminate the TIP816 driver
 *
 * RETURNS:	- always OK
 *
 ****************************************************************************/
uint t816term (DevDesc,	DevStatS, ProcDesc)
	dev_desc	*DevDesc;
	dev_stat	*DevStatS;
	procid		*ProcDesc;
{
  t816_DCB	*pDCB = &DevStatS->dcb;

  /* Disable Controller, enter reset state */
  pDCB->can->reg.ControlReg           = icCCE | icEIE | icIE | icInit;

  return E_NOERR;  
}





/****************************************************************************
 *
 * t816Read() - Read a message from a TIP816 device
 *
 ****************************************************************************/
uint t816Read (pDCB, tid, pMSG, ProcDesc)
	t816_DCB	*pDCB;
	uint		tid;
	t816_IO_BUFFER	*pMSG;
	procid		*ProcDesc;
{
  register CANMSG	*fifo_msg;
  uint			result;
  int			ilvl, i, state;
  JOB			*job;
  uint			rem_time;

  /***********************************************************************
   * Controller in Bus Off State ?
   ***********************************************************************/

  if (pDCB->BusOff)
  {
	return E_BUSOFF;
  }

  ilvl = intLock();	/* block interrupts */


  /*---------------------------------------------------------------------
    Flush FIFO before reading ?
    ---------------------------------------------------------------------*/
  if (pMSG->flags & T816_FLUSH)
  {
	pDCB->fifo.get = pDCB->fifo.put = 0;
  }


  /*---------------------------------------------------------------------
    check if message available
    ---------------------------------------------------------------------*/

  if (pDCB->fifo.get == pDCB->fifo.put)
  {
	if (pMSG->flags & T816_NOWAIT)
	{
		/*
		 * Don't wait for a message
		 */
		intUnlock (ilvl);
		return E_NODATA;
	}

	/*
	 * There is no message in the FIFO and so we have to wait until
	 * we received one.
	 */

	pDCB->LastRxdErr = E_NOERR;

	if (job = (JOB*)jobAttach(&pDCB->rdJobQ, tid))
	{
		job->io_tid	= tid;
		job->io_state	= &state;
		job->io_bufp	= pMSG;

		state = READY;
		rem_time = pMSG->timeout;

		intUnlock(ilvl);      /* unblock interrupts */

		do
		{
			block(&rem_time);
		} while ((rem_time != 0) &&
			 (state == READY) &&
			 !(ProcDesc->_state & (PS_CONDEMN << 8)));

		/*-----------------------------------------------------------
	  	  get message from the interrupt function
	  	  -----------------------------------------------------------*/
		ilvl = intLock();	/* block interrupts */

		switch (state)
		{
		  case DONE:
			result = E_NOERR;
			break;

		  case DONE_ERR:
			result = pMSG->status;
			break;

		  default:
			jobRelease(&pDCB->rdJobQ, job);
			result = E_TIMEOUT;
		}
	}
	else
	{
		result = E_BUSY;
	}

	intUnlock(ilvl);      /* unblock interrupts */

  }
  else 
  {
	/*-------------------------------------------------------------------
	  get message out of the message FIFO
	  -------------------------------------------------------------------*/

	fifo_msg = &pDCB->fifo.msg[pDCB->fifo.get];

	pMSG->identifier = fifo_msg->identifier;
	pMSG->extended   = fifo_msg->extended;
	pMSG->length     = fifo_msg->length;

	for (i = 0; i < fifo_msg->length; i++)
	{
		pMSG->data[i] = fifo_msg->data[i];
	}

	if (++pDCB->fifo.get == MAX_FIFO_MSG)
	{
		pDCB->fifo.get = 0;
	}

	pMSG->status = pDCB->LastRxdErr;

	pDCB->LastRxdErr = E_NOERR;

	intUnlock(ilvl);      /* unblock interrupts */

	result = E_NOERR;
  } 

  return result;

}


/****************************************************************************
 *
 * T816Write() - Write a message to a TIP816 device
 *
 ****************************************************************************/

int t816Write(	pDCB, tid, pMSG, ProcDesc)
	t816_DCB	*pDCB;
	uint		tid;
	t816_IO_BUFFER	*pMSG;
	procid		*ProcDesc;
{
  int			ilvl, i, free, state;
  register MSGOBJ	*msg;
  uint			result;
  JOB			*job;
  uint			rem_time;

  /***********************************************************************
   * Controller in Bus Off State ?
   ***********************************************************************/
  if (pDCB->BusOff)
  {
	return E_BUSOFF;
  }

  /***********************************************************************
   * Check arguments
   ***********************************************************************/
  if (pMSG->length > 8)
  {
	return E_IPARAM;
  }

  ilvl = intLock();      /* block interrupts */

  free = jobIsEmpty(&pDCB->wrtJobQ);

  if (job = (JOB*)jobAttach(&pDCB->wrtJobQ, tid))
  {
	job->io_tid	= tid;
	job->io_state	= &state;
	job->io_bufp	= pMSG;

  	/*------------------------------------------------------------------
  	  Is message object 14 (index=13!) available for transmission ?
  	  ------------------------------------------------------------------*/
	msg = &pDCB->can->msg.object[13];

	if (free)
	{
  		msg->Control0 = SET_MsgVal & SET_TXIE 
			      & RESET_RXIE & RESET_IntPnd;

  		msg->Control1 = RESET_RmtPnd & SET_CPUUpd & SET_NewDat;

  		if (pMSG->extended) 
	  	{
			msg->MessageConfig = (pMSG->length << DLC_Shift) 
					   | icDIRTX | icXtd;
			msg->Arbitration   = pMSG->identifier << 3;
  		}
  		else 
  		{
			msg->MessageConfig = (pMSG->length << DLC_Shift) 
					   | icDIRTX;
			msg->Arbitration   = pMSG->identifier << 21;
  		}

  		for (i = 0; i < pMSG->length; i ++)
  		{
			msg->data[i] = pMSG->data[i];
  		}

  		/*
  		 *  start transmission of this message
   		 */
		msg->Control1 = SET_TxRqst & RESET_CPUUpd;

		state = BUSY;
	}
	else
	{
		state = READY;
	}

	intUnlock(ilvl);

	if (!(pMSG->flags & T816_NOWAIT))
	{
		/* -------------------------------------------------------- */
		/* wait until data was transmitted (when NOWAIT iosn't set) */
		/* -------------------------------------------------------- */
		rem_time = pMSG->timeout;

		do
		{
			block(&rem_time);
		} while ((rem_time != 0) &&
			 (state == READY) &&
			 !(ProcDesc->_state & (PS_CONDEMN << 8)));

		ilvl = intLock();

		switch (state)
		{
		  case READY:
			/* ------------------------ */
			/* no data were transmitted */
			/* ------------------------ */
			msg->Control0 = RESET_MsgVal & RESET_TXIE
				      & RESET_RXIE & RESET_IntPnd;
			msg->Control1 = RESET_TxRqst & RESET_CPUUpd
				      & RESET_RmtPnd & RESET_NewDat;
			jobRelease(&pDCB->wrtJobQ, job);
			result = E_TIMEOUT;
			break;

		  case BUSY:
			/* ------------------------------ */
			/* transmit is in progress, so we */
			/* have to abort the transmission */
			/* ------------------------------ */
			msg->Control0 = RESET_MsgVal & RESET_TXIE
				      & RESET_RXIE & RESET_IntPnd;
			msg->Control1 = RESET_TxRqst & RESET_CPUUpd
				      & RESET_RmtPnd & RESET_NewDat;
			jobRelease(&pDCB->wrtJobQ, job);
			result = E_TIMEOUT;
			break;

		  case DONE:
			/* --------------------------------- */
			/* transmission completed successful */
			/* --------------------------------- */
			result = E_NOERR;
			break;

		  case DONE_ERR:
			/* --------------------------------- */
			/* transmission completed with error */
			/* --------------------------------- */
			result = pMSG->status;
			break;

		  default:
			result = E_IO;
		}
	}
	else
	{
		result = E_NOERR;
	}
  }
  else
  {
	result = E_BUSY;
  }

  intUnlock(ilvl);

  return result;
}


/****************************************************************************
 *
 * t816Ioctl() - perform special I/O control function
 *
 ****************************************************************************/
uint t816cntrl (request, DevStatS, Path, ProcDesc, UserArg)
	uint		request;
	dev_stat	*DevStatS;
	union pathdesc	*Path;
	procid		*ProcDesc;
	REGISTERS	*UserArg;
{
  t816_DCB		*pDCB = &DevStatS->dcb;
  uchar                 MsgNum;
  register MSGOBJ 	*msg;
  MSGDEF		*MsgDef;
  ACCEPT_MASKS		*AcceptMasks;
  DC_ARGS		*dc_arg;
  T816_STATUS		*stat_msg;
  int			ilvl, i;
  JOB			*job;
  ushort		tid;
  uint			result;

  switch (request) 
  {

    /*-----------------------------------------------------------------
      SS_READ -- read request
      -----------------------------------------------------------------*/
    case SS_READ:
	result = t816Read(pDCB, ProcDesc->_id, (t816_IO_BUFFER*)UserArg->d[2]);
	break;

    /*-----------------------------------------------------------------
      SS_WRITE -- write request
      -----------------------------------------------------------------*/
    case SS_WRITE:
	result = t816Write(pDCB, ProcDesc->_id, (t816_IO_BUFFER*)UserArg->d[2]);
	break;

    /*-----------------------------------------------------------------
      SS_Open -- open request
      -----------------------------------------------------------------*/
    case SS_Open:
	pDCB->open ++;
	result = E_NOERR;
	break;

    /*-----------------------------------------------------------------
      SS_Close -- close request
      -----------------------------------------------------------------*/
    case SS_Close:
	tid = Path->path.pd_cpr;

	/* Kill the outstanding jobs */
	jobKill(&pDCB->rdJobQ, tid);
	jobKill(&pDCB->wrtJobQ, tid);

	pDCB->open --;
	result = E_NOERR;
	break;

    /*-------------------------------------------------------------------
      SS_CANCEL -- Cancel any pending read or write request of the device
      -------------------------------------------------------------------*/
    case SS_CANCEL:
	ilvl = intLock();
	while (job = (JOB*)jobGetFirst(&pDCB->rdJobQ))
	{
		*job->io_state = DONE_ERR;
		job->io_bufp->status = E_ABORT;
		wakeup(job->io_tid);
		jobRelease(&pDCB->rdJobQ, job);
	}

	while (job = (JOB*)jobGetFirst(&pDCB->wrtJobQ))
	{
		*job->io_state = DONE_ERR;
		job->io_bufp->status = E_ABORT;
		wakeup(job->io_tid);
		jobRelease(&pDCB->wrtJobQ, job);
	}

	result = E_NOERR;
	intUnlock(ilvl);
	break;

    /*-----------------------------------------------------------------
      SS_SETFILTER -- setup acceptance filter masks
      -----------------------------------------------------------------*/
    case SS_SETFILTER:

	AcceptMasks = (ACCEPT_MASKS *)UserArg->d[2];

	pDCB->can->reg.GlobalMaskStandard = AcceptMasks->GlobalMaskStandard;
	pDCB->can->reg.GlobalMaskExtended = AcceptMasks->GlobalMaskExtended;
	pDCB->can->reg.Message15Mask      = AcceptMasks->Message15Mask;

	result = E_NOERR;
	break;


    /*-----------------------------------------------------------------
      SS_GETFILTER -- get contents of acceptance filter masks
      -----------------------------------------------------------------*/
    case SS_GETFILTER:

	AcceptMasks = (ACCEPT_MASKS *)UserArg->d[2];

	AcceptMasks->GlobalMaskStandard = pDCB->can->reg.GlobalMaskStandard;
	AcceptMasks->GlobalMaskExtended = pDCB->can->reg.GlobalMaskExtended;
	AcceptMasks->Message15Mask      = pDCB->can->reg.Message15Mask;

	result = E_NOERR;
	break;


    /*-----------------------------------------------------------------
      SS_BITTIMING -- setup bit timing register
      -----------------------------------------------------------------*/
    case SS_BITTIMING:

	dc_arg = (DC_ARGS*)UserArg->d[2];

	ilvl = intLock();

	/*
	 *  Set software initialization and allow write access to 
	 *  configuration parameter
	 */
	pDCB->can->reg.ControlReg |= (icCCE | icInit);

	pDCB->can->reg.BitTimingReg0 = dc_arg->arg >> 8;
	pDCB->can->reg.BitTimingReg1 = dc_arg->arg;

	if (dc_arg->flags & T816_THREE_SAMPLES)
	{
		pDCB->can->reg.BitTimingReg1 |= (1<<7);
	}

	pDCB->can->reg.ControlReg &= (~icCCE & ~icInit);

	intUnlock(ilvl);

	result = E_NOERR;
	break;


    /*-----------------------------------------------------------------
      SS_DEFINE_MSG -- Define Message Object
      -----------------------------------------------------------------*/
    case SS_DEFINE_MSG:

	dc_arg = (DC_ARGS*)UserArg->d[2];
	MsgDef = (MSGDEF *)dc_arg->arg;

	if ( MsgDef->MsgNum < 1  || 
	     MsgDef->MsgNum > 15 || 
	     MsgDef->MsgNum == 14) 
	{
		return E_IMSGNUM;
		break;
	}

	msg = &pDCB->can->msg.object[MsgDef->MsgNum - 1];

	/*
	 * check if message object already allocated
	 */
	if (msg->Control0 & TEST_MsgVal)
	{
		return E_MSGBUSY;
		break;
	}


	if (dc_arg->flags & T816_RECEIVE)
	{
		msg->Control0 = RESET_TXIE & SET_RXIE & RESET_IntPnd;
		msg->Control1 = RESET_RmtPnd & RESET_MsgLst & RESET_NewDat;

		if (MsgDef->extended) 
		{
			msg->MessageConfig = icXtd;
			msg->Arbitration   = MsgDef->identifier << 3;
		}
		else 
		{
			msg->MessageConfig = 0;
			msg->Arbitration   = MsgDef->identifier << 21;
		}

		msg->Control0 = SET_MsgVal;

		if (dc_arg->flags & T816_REMOTE)
		{
			msg->Control1 = SET_TxRqst;
		}
 
	}
	else if (dc_arg->flags & T816_TRANSMIT) 
	{

		if (MsgDef->MsgNum == 15) 
		{
			return E_IMSGNUM;
			break;
		}

		msg->Control0 = SET_MsgVal & RESET_TXIE & 
				RESET_RXIE & RESET_IntPnd;
		msg->Control1 = RESET_RmtPnd & SET_CPUUpd & SET_NewDat;

		if (MsgDef->extended)
		{
			msg->MessageConfig = (MsgDef->length << DLC_Shift) | 
					     icDIRTX | icXtd;
			msg->Arbitration   = MsgDef->identifier << 3;
		}
		else 
		{
			msg->MessageConfig = (MsgDef->length<<DLC_Shift) | 
					     icDIRTX;
			msg->Arbitration   = MsgDef->identifier << 21;
		}

		for (i = 0; i < MsgDef->length; i++)
		{
			msg->data[i] = MsgDef->data[i];
		}

		if (dc_arg->flags & T816_REMOTE) 
		{
			/*
			 *  this message is transmitted after receiving of
			 *  a remote frame
			 */
			msg->Control1 = RESET_CPUUpd;
		}
		else 
		{
			/* 
			 *  start transmission
			 */
			msg->Control1 = SET_TxRqst & RESET_CPUUpd;
		}
	}
	else 
	{
		return E_IPARAM;
		break;
	}

	result = E_NOERR;
	break;


    /*-----------------------------------------------------------------
      SS_UPDATE_MSG -- Update Message Object
      -----------------------------------------------------------------*/
    case SS_UPDATE_MSG:

	dc_arg = (DC_ARGS*)UserArg->d[2];
	MsgDef = (MSGDEF *)dc_arg->arg;

	if (MsgDef->MsgNum < 1  || 
	    MsgDef->MsgNum > 15 || 
	    MsgDef->MsgNum == 14)
	{
		return E_IMSGNUM;
		break;
	}

	msg = &pDCB->can->msg.object[MsgDef->MsgNum - 1];

	/*
	 * check if message object already allocated
	 */
	if (!(msg->Control0 & TEST_MsgVal))
	{
		return E_MSGNOTDEF;
		break;
	}


	if (msg->MessageConfig & icDIRTX)
	{
		/*
		 * check if transmission is pending
		 */
		if (msg->Control1 & TEST_TxRqst)
		{
			return E_MSGBUSY;
			break;
		}

		/*
		 *  update data in the specified message object and start
		 *  transmission if this is not a answer for a remote frame
		 */
		msg->Control1 = SET_CPUUpd;

		for (i = 0; i < MsgDef->length; i++)
		{
			msg->data[i] = MsgDef->data[i];
		}

		msg->MessageConfig = msg->MessageConfig & 0x0f |
				     (MsgDef->length << DLC_Shift);

		if (dc_arg->flags & T816_REMOTE)
		{
			/*
			 *  this message is transmitted after receiving of
			 *  a remote frame
			 */
			msg->Control1 = RESET_CPUUpd;
		}
		else 
		{
			/* 
			 *  start transmission
			 */
			msg->Control1 = SET_TxRqst & RESET_CPUUpd;
		}
	}
	else 
	{
		/*
		 *  start send of a remote frame
		 */
		msg->Control1 = SET_TxRqst;
	}

	result = E_NOERR;
	break;



    /*-----------------------------------------------------------------
      SS_CANCEL_MSG -- Cancel Message Object
      -----------------------------------------------------------------*/
    case SS_CANCEL_MSG:

	MsgNum = UserArg->d[2];

	if (MsgNum == 0) MsgNum = 14;

	if (MsgNum < 1 || 
	    MsgNum > 15) 
	{
		return E_IMSGNUM;
		break;
	}

	msg = &pDCB->can->msg.object[MsgNum - 1];

	/*
	 *  mark message object invalid
	 */
	msg->Control0 = RESET_MsgVal & RESET_TXIE &
			RESET_RXIE & RESET_IntPnd;
	msg->Control1 = RESET_TxRqst & RESET_CPUUpd & 
			RESET_RmtPnd & RESET_NewDat;

	result = E_NOERR;
	break;



    /*-----------------------------------------------------------------
      SS_BUSON - Enter Bus On state
      -----------------------------------------------------------------*/
    case SS_BUSON:


	pDCB->BusOff = FALSE;
	pDCB->can->reg.ControlReg &= (~icCCE & ~icInit);

	/* 
	 *  wait minimum of 128 packets of 11 consecutive recessive bits
	 */

	tm_wkafter((500) / 4 | (1 << 31));
		
	if(pDCB->BusOff)
	{
		return E_BUSOFF;
	}

	result = E_NOERR;
	break;



    /*-----------------------------------------------------------------
      SS_FLUSH - flush FIFO
      -----------------------------------------------------------------*/
    case SS_FLUSH:

	ilvl = intLock();

	pDCB->LastRxdErr = 0;
	pDCB->fifo.get = pDCB->fifo.put = 0; 

	intUnlock(ilvl);
		
	result = E_NOERR;
	break;



    /*-----------------------------------------------------------------
      SS_STATUS - get status information of a message object
      -----------------------------------------------------------------*/
    case SS_STATUS:

	stat_msg = (T816_STATUS*)UserArg->d[2];

	MsgNum = stat_msg->message_sel;
	stat_msg->status = 0;

	if (MsgNum == 0)
	{
		MsgNum = 14;
	}

	if (MsgNum < 1 || 
	    MsgNum > 15)
	{
		return E_IMSGNUM;
		break;
	}

	msg = &pDCB->can->msg.object[MsgNum - 1];

	/*
	 * check if message defined
	 */
	if (!(msg->Control0 & TEST_MsgVal))
	{
		stat_msg->status = E_MSGNOTDEF;
		return E_NOERR;
	}


	if (msg->MessageConfig & icDIRTX)
	{
		if(msg->Control1 & TEST_TxRqst)
		{
			stat_msg->status = E_BUSY;
			return E_NOERR;
		}
	}
	else
	{
		stat_msg->status = E_IMSGNUM;
		return E_NOERR;
	}

	result = E_NOERR;
	break;



    /*-----------------------------------------------------------------
      SS_CAN_STATUS - get contents of CAN controller Status Register
      -----------------------------------------------------------------*/
    case SS_CAN_STATUS:

	*((ulong*)UserArg->d[2]) = pDCB->can->reg.StatusReg;

	result = E_NOERR;
	break;

    /*-----------------------------------------------------------------
      all other command codes are illegal
      -----------------------------------------------------------------*/

    default :
	return E_ICMD;
	break;
		
  }

  return result;

}


/******************************************************************************/
/*                                                                            */
/*    t816intr - Common Interrupt Service Routine                             */
/*                                                                            */
/******************************************************************************/

void t816intr ( DevStatS )
dev_stat	*DevStatS;
{
  t816_DCB	   *pDCB = &DevStatS->dcb;
  register MSGOBJ  *msg;
  register t816_IO_BUFFER	*io_msg;
  register CANMSG  *fifo_msg;
  uchar            InterruptReg = pDCB->can->reg.InterruptReg;
  int		   i;
  JOB		   *job;

  do 
  {

    if (InterruptReg == 1) 
    {
	/*****************************/
	/* Status Register Interrupt */
	/*****************************/
	if (pDCB->can->reg.StatusReg & icBOff) 
  	{
		pDCB->LastTxdErr = E_BUSOFF;
		pDCB->LastRxdErr = E_BUSOFF;
		pDCB->BusOff     = TRUE;

		/*
		 *  wake up all waiting tasks
		 */
		while (job = (JOB*)jobGetFirst(&pDCB->rdJobQ))
		{
			*job->io_state = DONE_ERR;
			job->io_bufp->status = E_BUSOFF;

			wakeup(job->io_tid);

			jobRelease(&pDCB->rdJobQ, job);
		}

		while (job = (JOB*)jobGetFirst(&pDCB->wrtJobQ))
		{
			*job->io_state = DONE_ERR;
			job->io_bufp->status = E_BUSOFF;

			wakeup(job->io_tid);

			jobRelease(&pDCB->wrtJobQ, job);
		}
	}
  }
  else if (InterruptReg == 2) 
  {
	/*******************************/
	/* Message Object 15 Interrupt */
	/*******************************/
	/*
 	 *  get the received message
	 *  (object 15 can only receive messages !)
	 */

	msg      = &pDCB->can->msg.object[14];

	job = (JOB*)jobGetFirst(&pDCB->rdJobQ);

	if (!job)
	{	/* no job is waiting => message --> FIFO */
		fifo_msg = &pDCB->fifo.msg[pDCB->fifo.put];

		fifo_msg->extended   = (msg->MessageConfig & icXtd) != 0;
		fifo_msg->length     = msg->MessageConfig >> DLC_Shift;
		fifo_msg->identifier = 
			fifo_msg->extended ? 
				msg->Arbitration >> 3 :
				msg->Arbitration >> 21;

		for (i = 0; i < fifo_msg->length; i++)
		{
			fifo_msg->data[i]= msg->data[i];
		}

		if (++pDCB->fifo.put == MAX_FIFO_MSG) pDCB->fifo.put = 0;


		/*
		 *  check for data overrun condition
		 */
		if (pDCB->fifo.put == pDCB->fifo.get) 
		{
			if (++pDCB->fifo.get == MAX_FIFO_MSG) 
			{
				pDCB->fifo.get = 0;
			}
			pDCB->LastRxdErr = E_OVERRUN;
		}

		if (msg->Control1 & TEST_MsgLst) 
		{
			pDCB->LastRxdErr = E_OVERRUN;
		}
	}
	else	/* There is a job waiting, write direct */
	{
		io_msg = job->io_bufp;

		io_msg->extended   = (msg->MessageConfig & icXtd) != 0;
		io_msg->length     = msg->MessageConfig >> DLC_Shift;
		io_msg->identifier = 
			io_msg->extended ? 
				msg->Arbitration >> 3 :
				msg->Arbitration >> 21;

		for (i = 0; i < io_msg->length; i++)
		{
			io_msg->data[i]= msg->data[i];
		}

		if (msg->Control1 & TEST_MsgLst) 
		{
			pDCB->LastRxdErr = E_OVERRUN;
		}

		*job->io_state = DONE;
		job->io_bufp->status = pDCB->LastRxdErr;
		pDCB->LastRxdErr = E_NOERR;

		wakeup(job->io_tid);

		jobRelease(&pDCB->rdJobQ, job);
	}
	/*
	 *  release message object and clear interrupt pending
	 */
	msg->Control0 = RESET_IntPnd;
	msg->Control1 = RESET_RmtPnd & RESET_MsgLst & RESET_NewDat;
  }
  else if (InterruptReg >= 3 && InterruptReg <= 16) 
  {

	/**********************************/
	/* Message Object 1..14 Interrupt */
	/**********************************/

	msg = &pDCB->can->msg.object[InterruptReg - 3];


	if (msg->MessageConfig & icDIRTX) 
	{

		/*
		 * Direction = TRANSMIT
		 */

		msg->Control0 = RESET_IntPnd;
		

		/*
		 *  Only message object 14 (InterruptReg = 16) is reserved
		 *  for driver write requests
		 */
		if (InterruptReg == 16) 
		{

			job = (JOB*)jobGetFirst(&pDCB->wrtJobQ);
			while (job)
			{
				if(*job->io_state == BUSY)
				{
					*job->io_state = DONE;
					job->io_bufp->status = E_NOERR;

					wakeup(job->io_tid);

					jobRelease(&pDCB->wrtJobQ, job);

					job = (JOB*)jobGetFirst(&pDCB->wrtJobQ);
				}
				else if(*job->io_state == READY)
				{	/* Start a ready job */
					*job->io_state = BUSY;

					/* Start transmission */
					io_msg = job->io_bufp;

  					msg->Control0 = SET_MsgVal 
						      & SET_TXIE 
			      			      & RESET_RXIE 
						      & RESET_IntPnd;

  					msg->Control1 = RESET_RmtPnd 
						      & SET_CPUUpd 
						      & SET_NewDat;

  					if (io_msg->extended) 
	  				{
						msg->MessageConfig = 
							(io_msg->length 
							  << DLC_Shift) 
					   		| icDIRTX 
							| icXtd;
						msg->Arbitration = 
							io_msg->identifier 
							<< 3;
  					}
			  		else 
  					{
						msg->MessageConfig = 
							(io_msg->length 
							  << DLC_Shift) 
							| icDIRTX;
						msg->Arbitration = 
							io_msg->identifier
							<< 21;
  					}

			  		for (i = 0; i < io_msg->length; i ++)
  					{
						msg->data[i] =
							io_msg->data[i];
			  		}
					
					msg->Control1 	= SET_TxRqst 
							& RESET_CPUUpd;
				}
				else
				{	/* compltition of NOWAIT or ABORT */
					/* request			  */
					jobRelease(&pDCB->wrtJobQ, job);
					job = (JOB*)jobGetFirst(&pDCB->wrtJobQ);
				}
			}
		}
	}
	else 
	{
		job = (JOB*)jobGetFirst(&pDCB->rdJobQ);

		if (!job)
		{	/* no job is waiting => message --> FIFO */
			fifo_msg = &pDCB->fifo.msg[pDCB->fifo.put];

			fifo_msg->extended   =
				(msg->MessageConfig & icXtd) != 0;
			fifo_msg->length     = 
				msg->MessageConfig >> DLC_Shift;
			fifo_msg->identifier = 
				fifo_msg->extended ? 
					msg->Arbitration >> 3 :
					msg->Arbitration >> 21;

			for (i = 0; i < fifo_msg->length; i++)
			{
				fifo_msg->data[i]= msg->data[i];
			}

			if (++pDCB->fifo.put == MAX_FIFO_MSG) 
			{
				pDCB->fifo.put = 0;
			}

			/*
			 *  check for data overrun condition
			 */
			if (pDCB->fifo.put == pDCB->fifo.get) 
			{
				if (++pDCB->fifo.get == MAX_FIFO_MSG) 
				{
					pDCB->fifo.get = 0;
				}
				pDCB->LastRxdErr = E_OVERRUN;
			}

			if (msg->Control1 & TEST_MsgLst) 
			{
				pDCB->LastRxdErr = E_OVERRUN;
			}
		}
		else	/* There is a job waiting, write direct */
		{
			io_msg = job->io_bufp;

			io_msg->extended   = (msg->MessageConfig & icXtd) != 0;
			io_msg->length     = msg->MessageConfig >> DLC_Shift;
			io_msg->identifier = 
				io_msg->extended ? 
					msg->Arbitration >> 3 :
					msg->Arbitration >> 21;

			for (i = 0; i < io_msg->length; i++)
			{
				io_msg->data[i]= msg->data[i];
			}

			if (msg->Control1 & TEST_MsgLst) 
			{
				pDCB->LastRxdErr = E_OVERRUN;
			}

			*job->io_state = DONE;
			job->io_bufp->status = pDCB->LastRxdErr;
			pDCB->LastRxdErr = E_NOERR;

			wakeup(job->io_tid);

			jobRelease(&pDCB->rdJobQ, job);
		}

		/*
		 *  release message object and clear interrupt pending
		 */
		msg->Control0 = RESET_IntPnd;
		msg->Control1 = RESET_RmtPnd & RESET_MsgLst & RESET_NewDat;
	}
    }


    /********************************************************************
     look for other pending interrupts
     ********************************************************************/

    InterruptReg = pDCB->can->reg.InterruptReg;


  } while (InterruptReg != 0);


}


/******************************************************************************/
/*                                                                            */
/*    CalcCRC - calculate check sum of data buffer                            */
/*                                                                            */
/*    Entry:                                                                  *
/*           data    = data buffer                                            */
/*           len     = size of data buffer                                    */
/*                                                                            */
/*    Exits to: return to caller with calculated check sum                    */
/*                                                                            */
/******************************************************************************/

static uchar CalcCRC(data, len)

uchar data[];
int len;
{
  uint i, crc = 0xffff;
  uchar mask;

  for (i=0; i<len; i++) 
  {
        mask = 0x80;

        while (mask) 
	{
                if (data[i] & mask) crc ^= 0x8000;

                crc += crc;

                if (crc > 0xffff) crc = crc & 0xffff ^ 0x1021;

                mask = mask >> 1;
        }
  }

  return ~crc & 0xff;
}



/******************************************************************************/
/*                                                                            */
/*    CheckCRC - compares check sum of ID-PROM with calculated check sum      */
/*                                                                            */
/*    Entry:                                                                  */
/*           ID      = contents of ID-PROM                                    */
/*                                                                            */
/*    Exits to: return to caller with return code                             */
/*              E_NOERR = check sum of the ID-PROM is ok                     */
/*              E_NXIO  = check sum is not ok                                */
/*                                                                            */
/******************************************************************************/

static int CheckCRC(ID)

uchar ID[];
{
  uchar CalcCRC(), save_CRC;

  save_CRC = ID[POS_CRC];

  ID[POS_CRC] = 0;

  if (CalcCRC(ID, ID[POS_NO_BYTES]) == save_CRC)
  {
        ID[POS_CRC] = save_CRC;
        return E_NOERR;

  }
  else 
  {
        ID[POS_CRC] = save_CRC;

        return E_NXIO;
  }

  return E_NOERR;
}


/******************************************************************************/
/*                                                                            */
/*    ReadIPAC_ID - read contents of the ID-PROM                              */
/*                                                                            */
/*    Entry:                                                                  */
/*              data_ptr  = address of ID_PROM                                */
/*                                                                            */
/*    Exits to: return to caller                                              */
/*              E_NOERR  = TIP-Modul is ok                                    */
/*              E_NXIO   = check failed                                       */
/*                                                                            */
/******************************************************************************/

static int ReadIPAC_ID(data_ptr, data)

uchar *data_ptr;
uchar data[];
{
  int		dummy;

  register	int i;

  data_ptr++;

  for (i = 0; i < ID_LENGTH; i++)
  {
        data[i] = *data_ptr;

        /*--------------------------------------------
          only odd addresses 1, 3, 5, 7, ...
          --------------------------------------------*/

        data_ptr += 2;
  }

  return E_NOERR;
}


/******************************************************************************/
/*                                                                            */
/*    CheckIPAC -  perform versatile checks for the TIP-Modul                 */
/*                                                                            */
/*    Entry:                                                                  */
/*              Address  = address of the TIP-Modul                           */
/*                                                                            */
/*    Exits to: return to caller with return code                             */
/*              E_NOERR  = TIP-Modul is ok                                    */
/*              E_NXIO   = check failed                                       */
/*                                                                            */
/******************************************************************************/

static int CheckIPAC(Address)

char    *Address;
{
  int   i;
  int   CheckCRC();
  int	ReadIPAC_ID();
  BYTE  ID_Data[ID_LENGTH];

  /*----------------------------------------------------------------------
    check CRC of ID-PROM
    ----------------------------------------------------------------------*/

  if (ReadIPAC_ID(Address + ID_OFFSET, ID_Data)) return E_NXIO;

  if (CheckCRC(ID_Data) != E_NOERR) return E_NXIO;

  if (ID_Data[0] != 'I' || ID_Data[1] != 'P' || ID_Data[2] != 'A' ||
      ID_Data[3] != 'C')
  {
        return E_NXIO;
  }

  if (ID_Data[POS_MANUFACTOR] != MANUFACTOR_TEWS)
  {
        return E_NXIO;
  }

  if (ID_Data[POS_MODEL] != TIP816)
  {
        return E_NXIO;
  }

  return E_NOERR;
}

