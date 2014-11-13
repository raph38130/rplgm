/*---------------------------------------------------------------------------
 *  Copyright (C) 2003 Dallas Semiconductor Corporation, All Rights Reserved.
 * 
 *  
 * This file contains function definitions for I2C access functions.
 * This file is intended for use with the Keil MicroVision (uVision) C compiler.
 *
 * Wed Feb  4 13:09:42 CET 2004 RP
 *	- hack by RP for sdcc
 *
rm *asm *rel *lst *sym *lib	
../../tools/sdcc/bin/sdcc -mds400 --model-flat24 --stack-10bit tini_i2c.c -c
../../tools/sdcc/bin/sdcclib -a i2clib.lib tini_i2c.rel	  
 
 * ---------------------------------------------------------------------------
 */

/** \file tini_i2c.c
 *  \brief I2C function library
 *
 *  This library contains functions for communicating to I2C devices
 *  via user specified port pins.
 */
#include <stdio.h>
#include <tinibios.h>
//#include <reg400.h>
#include "tini_i2c.h"
#define SCL_HIGH (P1 |= 2)
#define SCL_LOW (P1 &= ~2)
#define SDA_HIGH (P1 |= 1)
#define SDA_LOW (P1 &= ~1)
#define SDA_OUT(b) (b ? SDA_HIGH : SDA_LOW)
#define SDA_IN (P1&1)
#define SCL_IN ((P1>>1)&1)

//RP error in original file
//#define SDA_IN ((P1>>1)&1)
//#define SCL_IN (P1&1)
/**
 * \brief         Return the library version.
 */
//---------------------------------------------------------------------------
int i2c_version(void)
{
  return 1;
}

/**
 * \brief         Delay function.
 */
//---------------------------------------------------------------------------
/*void i2c_delay(void)
{
  int loop;
  int value = 0;

  for (loop = 0;loop < I2C_DELAY_LOOP_COUNT;loop++)
    ;
}*/
//8 nop ok avec PCF
#define i2c_delay() \
	_asm \
	nop \
	nop \
	nop \
	nop \
	nop \
	nop \
	nop \
	_endasm


#ifdef I2C_ENABLE_SCL_WAIT_FOR_SLOW_SLAVES
/**
 * \brief         Waits for the SCL line to return high.
 *
 * Some I2C slave devices will delay the SCL rising edge for flow control
 * reasons.  The <i>I2C_MAXIMUM_SCL_WAITCOUNT</i> parameter should be tuned for 
 * the intended system setup.
 */
//---------------------------------------------------------------------------
void i2c_waitforscl()
{
  int waitcount;
  
//printf("                        wait");
  // Wait for SCL to come high
  for (waitcount = 0;waitcount < I2C_MAXIMUM_SCL_WAITCOUNT;waitcount++)
  {
    if (SCL_IN) return;
  }
}
#endif

/**
 * \brief         Performs an I2C start condition
 */
//---------------------------------------------------------------------------
void i2c_start(void)
{
  SDA_LOW;
  i2c_delay();
  SCL_LOW;
  i2c_delay();
}

/**
 * \brief         Performs an I2C bit write
 *
 * \param singlebit  Bit to write on I2C bus
 */
//---------------------------------------------------------------------------
void i2c_bit(unsigned char singlebit)
{
  SDA_OUT(singlebit);
  i2c_delay();
  SCL_HIGH;
#ifdef I2C_ENABLE_SCL_WAIT_FOR_SLOW_SLAVES
  i2c_waitforscl();
#endif
  i2c_delay();
  SCL_LOW;
  i2c_delay();
}

/**
 * \brief         Performs an I2C bit read
 *
 * \return        Value of SDA line during read timeslot
 */
//---------------------------------------------------------------------------
unsigned char i2c_readbit(void)
{
  unsigned char retVal;

  SDA_HIGH;
  i2c_delay();
  SCL_HIGH;
#ifdef I2C_ENABLE_SCL_WAIT_FOR_SLOW_SLAVES
  i2c_waitforscl();
#endif
  i2c_delay();
  retVal = SDA_IN;
  i2c_delay();
  SCL_LOW;
  i2c_delay();

  return retVal;
}

/**
 * \brief         Performs an I2C stop condition
 */
//---------------------------------------------------------------------------
void i2c_stop(void)
{
  SDA_LOW;
  i2c_delay();
  SCL_HIGH;
  i2c_delay();
  SDA_HIGH;
  i2c_delay();
}

/**
 * \brief         Performs an I2C byte read
 *
 * \param doACK   Set to 1 to assert acknowledge after reading 8 bits,
 *                  or 0 to not assert ACK.
 *
 * \return        Value of SDA line during read timeslot
 */
//---------------------------------------------------------------------------
unsigned char i2c_readbyte(unsigned char doACK)
{
  unsigned char i;
  unsigned char result = 0;

  for (i = 0;i < 8;i++)
  {
    result <<= 1;
    result |= (i2c_readbit() & 0x01);
  }

  if (doACK == 0)
    i2c_bit(1);  // No ACK
  else
    i2c_bit(0);  // Do the ACK

  return result;
}

/**
 * \brief         Performs an I2C byte write
 *
 * \param singlebyte Value to write to bus.
 *
 * \return        0 if byte was acknowledged
 */
//---------------------------------------------------------------------------
unsigned char i2c_writebyte(unsigned char singlebyte)
{
  unsigned char i;

  for (i = 0;i < 8;i++)
  {
    if (singlebyte & 0x80)
      i2c_bit(1);
	else
	  i2c_bit(0);
    singlebyte <<= 1;
  }

  return i2c_readbit();
}

/**
 * \brief         Perform I2C start, address selection.
 *
 * \param address Address of device to select.  Upper 7 bits
 *                are address, LSbit denotes read if 1 and write if 0.
 *
 * \return        0 if device acknowledged address selection
 */
//---------------------------------------------------------------------------
unsigned char i2c_select(unsigned char address)
{
  i2c_start();
  return i2c_writebyte(address);
}

/**
 * \brief         Perform I2C start, address selection, write specified
 *                bytes and I2C stop.
 *
 * \param address Address of device to select.  Upper 7 bits
 *                are address, LSbit automatically set to 0 by
 *                function.
 * \param barr    Array of bytes to write
 * \param offset  Offset to first byte to write
 * \param length  Number of bytes to write
 *
 * \return        0 if device acknowledged address selection and data transfer
 */
//---------------------------------------------------------------------------
unsigned char i2c_writeblock(unsigned char address,unsigned char *barr,int length)
{
  int i;

  if (i2c_select(address & 0xFE) != 0)
    return 1;

  for (i = 0;i < length;i++)
    if (i2c_writebyte(barr[i]) != 0) return 1;

  i2c_stop();

  return 0;
}

/**
 * \brief         Perform I2C start, address selection, read specified
 *                number of bytes and I2C stop.
 *
 * \param address Address of device to select.  Upper 7 bits
 *                are address, LSbit automatically set to 1 by
 *                function.
 * \param barr    Array destination for read bytes
 * \param offset  Offset location for first read
 * \param length  Number of bytes to read
 *
 * \return        0 if device acknowledged address selection and data transfer
 */
//---------------------------------------------------------------------------
unsigned char i2c_readblock(unsigned char address,unsigned char *barr,int length)
{
  int i;

  if (i2c_select(address | 0x01) != 0)
    return 1;

  // Reduce the length by one.  The last byte must not be ACKed.
  length--;

  // Read every byte but the last
  for (i = 0;i < length;i++)
    barr[i] = i2c_readbyte(1);

  // Don't ack the last byte
  barr[i] = i2c_readbyte(0);

  i2c_stop();

  return 0;
}

/**
 * \brief         Perform I2C start, address selection, write specified
 *                bytes, I2C start, address slection, read bytes and I2C stop.
 *
 * \param address Address of device to select.  Upper 7 bits
 *                are address, LSbit automatically set to 0 by
 *                function.
 * \param barr1   Array of bytes to write
 * \param offset1 Offset to first byte to write
 * \param length1 Number of bytes to write
 * \param barr2   Array destination for read bytes
 * \param offset2 Offset location for first read
 * \param length2 Number of bytes to read
 *
 * \return        0 if device acknowledged address selection and data transfer
 */
//---------------------------------------------------------------------------
unsigned char i2c_writereadblock(unsigned char address,unsigned char *barr1,int length1,unsigned char *barr2,int length2)
{
  int i;

  if (i2c_select(address & 0xFE) != 0)
    return 1;

  for (i = 0;i < length1;i++)
    i2c_writebyte(barr1[i]);

  SDA_HIGH;
  i2c_delay();
  SCL_HIGH;
  i2c_delay();

  if (i2c_select(address | 0x01) != 0)
    return 1;

  // Reduce the length by one.  The last byte must not be ACKed.
  length2--;

  // Read every byte but the last
  for (i = 0;i < length2;i++)
    barr2[i] = i2c_readbyte(1);

  // Don't ack the last byte
  barr2[i] = i2c_readbyte(0);

  i2c_stop();

  return 0;
}


