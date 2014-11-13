/******************************************************************************
*******************************************************************************
**                                                                           **
**                                                                           **
**                          @@@@@@@@@@@@@@@@@@@@@@@                          **
**                          @                     @                          **
**                          @   T 8 1 6 _ E X A   @                          **
**                          @                     @                          **
**                          @@@@@@@@@@@@@@@@@@@@@@@                          **
**                                                                           **
**                                                                           **
**    Project          TIP816 - Device Driver                                **
**                                                                           **
**    File             t816_exa.c                                            **
**                                                                           **
**    Function         OS-9 example application program                      **
**                                                                           **
**    Owner            Tews Datentechnik GmbH                                **
**                     Am Bahnhof 7                                          **
**                     D-25469 Halstenbek                                    **
**                     Germany                                               **
**                                                                           **
**                     Tel (04810) 42637                                     **
**                                                                           **
**                                                                           **
**                     Copyright (c) 1996                                    **
**                     Tews Datentechnik GmbH                                **
**                                                                           **
**                                                                           **
**    Author           Torsten Klegin   05/30/96                             **
**                                                                           **
**                                                                           **
**                                                                           **
*******************************************************************************
******************************************************************************/

#include <stdio.h>
#include "tip816.h"

typedef	unsigned long	ulong;
typedef	unsigned int	uint;

#define TRUE                    1
#define FALSE                   0
#define MAX_TIP816_DEVICES      4

struct init_args
{
	ulong	io_addr;
	ulong	mem_addr;
	ulong	vector;
	ulong	level;
	ulong	datarate;
};

char* devname[]	   = {
			"/t816A",
			"/t816B",
			"/t816C",
			"/t816D",
		     };

ulong DeviceFound[] = { FALSE,
			FALSE,
			FALSE,
			FALSE,
		     };

unsigned short TIMING_CONST[] = { T816_1_6MBIT, T816_1MBIT, T816_500KBIT,
				  T816_250KBIT, T816_125KBIT, T816_100KBIT,
				  T816_50KBIT, T816_20KBIT, T816_10KBIT,
				  T816_5KBIT };
 

unsigned long	can_fd[MAX_TIP816_DEVICES];



void main()
{
  ulong			DevNum;
  char                  select[100];
  int			i, j, selnum, status;
  uint			result;
  ACCEPT_MASKS    	AcceptMasks;
  ulong			value;
  DC_ARGS	        dc;
  MSGDEF		MsgDef;
  T816_STATUS		mess_stat;
  t816_IO_BUFFER	rw;


  printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
  printf(" ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  printf(" ++  Example application program for VxWorks target systems  ++\n");
  printf(" ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n");

  printf(" Searching for TIP816 Devices ...\n");

  for (i = 0; i < MAX_TIP816_DEVICES; i++) 
  {

	result = t816_open(	devname[i],
				&can_fd[i]);

	if (result == E_NOERR) 
	{
		DeviceFound[i] = TRUE;
		printf(" -- TIP816 Device [%d] found --", i);
		printf(" initialization successful completed\n");
	}
	else
	{
		printf(" -- TIP816 Device [%d] not found\n", i);
	}
  }


  do 
  {
	do 
	{
		printf("\n Please select function\n\n");
		printf("        0  --  Define Acceptance Filter Masks\n");
		printf("        1  --  Get Acceptance Filter Masks\n");
		printf("        2  --  Define Bit Timing\n");
		printf("        3  --  Define a Message Object\n");
		printf("        4  --  Update a Message Object\n");
		printf("        5  --  Cancel a Message Object\n");
		printf("        6  --  Get Status of a Message Object\n");
		printf("        7  --  Get CAN Controller Status Register\n");
		printf("        8  --  Flush message FIFO\n");
		printf("        9  --  Enter Bus On mode\n");
		printf("       10  --  Send a CAN Message\n");
		printf("       11  --  Receive a CAN Message\n");
		printf("       12  --  Test TIP816-Modul\n");
		printf("       13  --  quit example application\n\n");
		printf(" select ");
		gets(select);
		selnum = atoi(select);

	} while (selnum > 13);

	if (selnum < 12) 
	{
		printf(" Device Number [%d] : ", DevNum);
		gets(select);
		DevNum = strlen(select) ? atoi(select) : DevNum;
		if (DevNum >= MAX_TIP816_DEVICES ||
				   !DeviceFound[DevNum]) 
		{
			printf("\n ##  Not a valid Device  ##\n\n");
			continue;
		}
	}


	printf("\n\n");

	switch(selnum) 
	{

	  case 0:    /* Define Acceptance Filter */

	  	do 
	  	{
			printf(" Global Standard Mask (Hex)  : ");
			gets(select);
			sscanf(select, "%x", &result);  
	  	} while (result > 0xffff);

	  	AcceptMasks.GlobalMaskStandard = result;


	  	printf(" Global Extended Mask (Hex)  : ");
	  	gets(select);
	  	sscanf(select, "%x", &result);        

	  	AcceptMasks.GlobalMaskExtended = result;


	  	printf(" Local Message 15 Mask (Hex) : ");
	  	gets(select);
	  	sscanf(select, "%x", &result);        

	  	AcceptMasks.Message15Mask = result;


	  	result = t816_cntrl(	can_fd[DevNum],
					SS_SETFILTER,
					(uint)&AcceptMasks);

	  	if (result == E_NOERR) 
		{
		    printf("\n\n -- Setting Acceptance Filter successful\n");
	  	}
	  	else
		{
		    printf("\n\n -- Setting Acceptance Filter failed (error = %xh)\n", result);
	  	}

	  	break;



	  case 1:    /* Get Acceptance Filter Masks*/

	  	result = t816_cntrl(	can_fd[DevNum],
					SS_GETFILTER,
					(uint)&AcceptMasks);

	  	if (result == E_NOERR) 
		{
		  printf("\n\n -- Getting Acceptance Filter successful\n\n");
		  printf(" Global Standard Mask (Hex)  : %08X\n",
			AcceptMasks.GlobalMaskStandard);
		  printf(" Global Extended Mask (Hex)  : %08X\n",
			AcceptMasks.GlobalMaskExtended);
		  printf(" Local Message 15 Mask (Hex) : %08X\n",
			AcceptMasks.Message15Mask);
	  	}
	  	else
		{
			printf("\n\n -- Setting Acceptance Filter failed (error = %xh)\n", result);
	  	}

	  	break;




	  case 2:    /* Define Bit Timing */

		do 
		{
			printf("\n\n Please select Bit Timing\n\n");
			printf("        0  --     1.6 Mbps\n");
			printf("        1  --     1.0 Mbps\n");
			printf("        2  --     500 Kbps\n");
			printf("        3  --     250 Kbps\n");
			printf("        4  --     125 Kbps\n");
			printf("        5  --     100 Kbps\n");
			printf("        6  --      50 Kbps\n");
			printf("        7  --      20 Kbps\n");
			printf("        8  --      10 Kbps\n");
			printf("        9  --       5 Kbps\n");
			printf("       10  --         other\n");
			printf(" select ");
			gets(select);
			selnum = atoi(select);

		} while (selnum > 10);

	  	if (selnum == 10) 
		{

			do 
			{
				printf(" Bit Timing Register 0 (Hex)  : ");
				gets(select);
				sscanf(select, "%x", &result);  
			} while (result > 0xff);

			value = result << 8;;

			do 
			{
				printf(" Bit Timing Register 1 (Hex)  : ");
				gets(select);
				sscanf(select, "%x", &result);  
			} while (result > 0xff);

			value |= (result & 0xff);

	  	}
	  	else 
		{
			value = TIMING_CONST[selnum];
	  	}

		dc.flags = 0;
		dc.arg   = (unsigned long)value;

	  	result = t816_cntrl(	can_fd[DevNum],
					SS_BITTIMING,
					&dc);

		if (result == E_NOERR) {
			printf("\n\n -- Setting Bit Timing successful\n");
	  	}
	  	else 
		{
			printf("\n\n -- Setting Bit Timing failed (error = %xh)\n", result);
	  	}

		break;


	  case 3:    /* Define a Message Object */

		dc.flags = 0;
		dc.arg   = (unsigned long)&MsgDef;


		printf(" Message Object Number (1..13, 15)  : ");
		gets(select);
		sscanf(select, "%d", &result);        

	  	MsgDef.MsgNum = result;


	  	printf(" Extended Identifier (Y/N)          : ");
	  	gets(select);
	  	MsgDef.extended = (toupper(select[0]) == 'Y') ? TRUE : FALSE;


	  	printf(" Message Identifier (decimal)       : ");
	  	gets(select);
	  	sscanf(select, "%d", &result);        

	  	MsgDef.identifier = result;


	  	do 
		{
			printf("\n Please select kind of message object\n\n");
			printf("        0  --  Receive Message Object\n");
			printf("        1  --  Receive Message Object with Remote Frame Request\n");
			printf("        2  --  Transmit Message Object (start transmission)\n");
			printf("        3  --  Transmit Message Object (don't start transmission) \n");
			printf(" select ");
			gets(select);
			selnum = atoi(select);
	 	} while (selnum > 3);

		switch (selnum)
		{
		  case 0: 
			dc.flags = T816_RECEIVE;
		    	break;
	    	  case 1: 
			dc.flags = T816_RECEIVE | T816_REMOTE;
		    	break;
	    	  case 2:
			dc.flags = T816_TRANSMIT;
		    	break;
	    	  case 3: 
			dc.flags = T816_TRANSMIT | T816_REMOTE;
		    	break;
	  	}


		if (dc.flags & T816_TRANSMIT) 
		{
			printf(" Message Text (max. 8 Bytes)        : ");
			gets(select);
			MsgDef.length = strlen(select);
			if (MsgDef.length > 8) MsgDef.length = 8;
			for (i=0; i<MsgDef.length; i++) 
				MsgDef.data[i] = select[i];
	  	}

	  	result = t816_cntrl(	can_fd[DevNum],
					SS_DEFINE_MSG,
					&dc);

	  	if (result == E_NOERR) 
		{
			printf("\n\n -- Message Object defined\n");
	  	}
	  	else 
		{
			printf("\n\n -- Defining Message Object failed (error = %xh)\n", result);
	  	}

	  	break;



	  case 4:    /* Update a Message Object */
		dc.flags = 0;
		dc.arg   = (unsigned long)&MsgDef;

	  	printf(" Message Object Number (1..13, 15)  : ");
	  	gets(select);
	  	sscanf(select, "%d", &result);        

	  	MsgDef.MsgNum = result;


	  	printf(" Transmit Message Object (Y/N)  [N] : ");
	  	gets(select);
	  	if (toupper(select[0]) == 'Y')
		{
			printf(" Message Text (max. 8 Bytes)        : ");
			gets(select);
			MsgDef.length = strlen(select);
			if (MsgDef.length > 8)
				MsgDef.length = 8;
			for (i=0; i<MsgDef.length; i++)
				MsgDef.data[i] = select[i];

			printf(" Start Transmission (Y/N)       [N] : ");
			gets(select);
			if (toupper(select[0]) != 'Y')
			{
				dc.flags |= T816_REMOTE;
				printf("\n Then wait for a remote frame ...\n\n");
			}
	  	}

	  	result = t816_cntrl(	can_fd[DevNum],
					SS_UPDATE_MSG,
					&dc);

	  	if (result == E_NOERR) {
			printf("\n\n -- Update Message Object successful\n");
	  	}
	  	else 
		{
			printf("\n\n -- Update Message Object failed (error = %xh)\n", result);
	  	}
	  	break;


	  case 5:    /* Cancel a Message Object */

	  	printf(" Message Object Number (0, 1..15)  : ");
	  	gets(select);
	  	sscanf(select, "%d", &result);        

	  	result = t816_cntrl(	can_fd[DevNum],
					SS_CANCEL_MSG,
					result);

	  	if (result == E_NOERR) 
	  	{
			printf("\n\n -- Cancel Message Object successful\n");
	  	}
	  	else 
		{
			printf("\n\n -- Cancel Message Object failed (error = %xh)\n", result);
	  	}
	  	break;


	  case 6:    /* Get Status of a Message Object */

	  	printf(" Message Object Number (0, 1..15)  : ");
	  	gets(select);
	  	sscanf(select, "%d", &mess_stat.message_sel);        

	  	result = t816_cntrl(	can_fd[DevNum],
					SS_STATUS,
					&mess_stat);

	  	printf("\n Message Object Status = %xh\n", mess_stat.status);

	  	break;


	  case 7:    /* Get contents of CAN controller Status Register */

	  	result = t816_cntrl(	can_fd[DevNum],
					SS_CAN_STATUS,
					&status);

	  	printf("\n CAN Controller Status = %02Xh\n", status);

	  	break;



	  case 8:
	  	result = t816_cntrl(	can_fd[DevNum],
					SS_FLUSH,
					0);

	  	if (result == E_NOERR) 
		{
			printf("\n -- Flush successful completed\n");
	  	}
	  	else 
		{
			printf("\n\n -- Flushing FIFO failed (error = %xh)\n", result);
	  	}
	  	break;

	  case 9:

	  	result = t816_cntrl(	can_fd[DevNum],
					SS_BUSON,
					0);

	  	if (result == E_NOERR) 
		{
			printf("\n --> Bus On successful completed\n");
	  	}
	  	else
	 	{
			printf("\n\n -- Bus On failed (error = %xh)\n", result);
	  	}
	  	break;

	  case 10:    /* send a message */

		rw.flags = 0;
		rw.timeout = 1500;

	  	printf(" Extended Identifier (Y/N)          : ");
	  	gets(select);
	  	rw.extended = (toupper(select[0]) == 'Y') ? TRUE : FALSE;


	  	printf(" Message Identifier (decimal)       : ");
	  	gets(select);
	  	sscanf(select, "%d", &result);        

	  	rw.identifier = result;

	  	printf(" Message Text (max. 8 Bytes)        : ");
	  	gets(select);
	  	rw.length = strlen(select);
	  	if (rw.length > 8)
			rw.length = 8;
	  	for (i=0; i<rw.length; i++) 
			rw.data[i] = select[i];
		  
		result = t816_write(can_fd[DevNum], &rw);
	  	if (result == E_NOERR) 
		{
			printf("\n -- Message successful transmitted\n");
	  	}
	  	else 
		{
			printf("\n -- Write failed (error = %xh)\n", result);
	  	}

	  	break;

	  case 11:    /* receive a message */

	  	rw.flags = 0;
		rw.length = 0;
	  	rw.timeout = 3000;
		for(i = 0; i < 8; i++)
		{
			rw.data[i] = 0;
		}

		result = t816_read(can_fd[DevNum], &rw);

	  	if (result == E_NOERR || 
		    result == E_OVERRUN)
		{
			printf("\n -- Message with Identifier %d received (status = %xh)\n", rw.identifier, result);
			printf("    Message length = %d Byte\n", rw.length);
			printf("    Message text   = ");
			for(i = 0; i < rw.length; i++)
			{
				printf("%c", rw.data[i]);
			}
			printf("\n\n");
	  	}
	  	else 
		{
			printf("\n -- Read failed (error = %xh)\n", result);
	  	}
	  	break;

	  case 12:    /* Test TIP816 Modul */
	  	for (i=0; i<1; i++) 
		{
			/*
		 	 * Reset all message objects
		 	 */
			for (j=1; j<=15; j++) 
			{
				if (j == 14) continue;

	  			result = t816_cntrl(	can_fd[i],
							SS_CANCEL_MSG,
							j);


				if (result != E_NOERR) 
				{
			    		printf(" Unexpected Error (error = %xh)\n",result);
			    		break;
				}
			}       


			/*
			 * mask all identifier bits as relevant
			 */
			AcceptMasks.GlobalMaskStandard = 0xffff;
			AcceptMasks.GlobalMaskExtended = 0xffffffff;
			AcceptMasks.Message15Mask      = 0xffffffff;

	  		result = t816_cntrl(	can_fd[i],
						SS_SETFILTER,
						&AcceptMasks);

			if (result != E_NOERR) 
			{
				printf(" Unexpected Error (error = %xh)\n",result);
				break;
			}


			/*
			 * set bit timing to 1 MByte/s
			 */
	  		result = t816_cntrl(	can_fd[i],
						SS_BITTIMING,
						T816_1MBIT);

			if (result != E_NOERR) 
			{
				printf(" Unexpected Error <FIO_BITTIMING> (error = %xh)\n",result);
				break;
			}

	  		result = t816_cntrl(	can_fd[i],
						SS_BUSON,
						0);

			if (result != E_NOERR) 
			{
				printf(" Unexpected Error <FIO_BUSON> (error = %xh)\n",result);
				break;
			}

	  		result = t816_cntrl(	can_fd[i],
						SS_FLUSH,
						0);

			if (result != E_NOERR)
			{
				printf(" Unexpected Error <FIO_FLUSH> (error = %xh)\n",result);
				break;
			}
	  	}
	  	if (result != E_NOERR) break;

	  	if (result == E_NOERR) 
		{
			printf("\n\n Test successful !\n");
	  	}
	  	else 
		{
			printf("\n\n Test failed ! \n");
	  	}

	  	break;

	  default:
	  	printf("\n\n Not implemented ! \n\n");
	  	break;

	}


  } while (selnum != 13);


  printf(" Exit example application ...\n\n\n");
}  

