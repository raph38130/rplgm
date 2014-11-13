/*---------------------------------------------------------------------------
 
 * This file contains function definitions for I2C access functions.
 * This file is intended for use with the Keil MicroVision (uVision) C compiler.
 *
 * Wed Feb  4 13:09:42 CET 2004 RP
 *	- hack by RP/LGM for sdcc
 *
 * ---------------------------------------------------------------------------
 */
#ifndef __tini_i2c_
#define __tini_i2c_

/** \file tini_i2c.h
 *  \brief I2C function library
 *
 *  This library contains functions for communicating to I2C devices
 *  via user specified port pins.
 *
 *  For detailed information on the DS80C400 please see the
 *  <a href="http://pdfserv.maxim-ic.com/arpdf/Design/DS80C400UG.pdf">
 *  High-Speed Microcontroller User's Guide: DS80C400 Supplement</a>.
 *
 *  \warning  The functions in this library are <b>NOT</b> multi-process 
 *            safe--that is, if you call the same method from two different 
 *            processes at the same time, the parameters to the function 
 *            may be destroyed, yielding unpredictable results.  However, I2C
 *            pins are a system resource and should not be shared among 
 *            different processes.
 */

/** Version number associated with this header file.  Should be the same as
 * the version number returned by the <i>#i2c_version</i> function.
 * \sa #i2c_version */
#define TINI_I2C_VERSION         1


/**
 * \brief         Return the library version.
 */
//---------------------------------------------------------------------------
int i2c_version();

/**
 * \brief         Delay function.
 */
//---------------------------------------------------------------------------

void i2c_delay(void);
/**
 * \brief         Performs an I2C start condition
 */
//---------------------------------------------------------------------------

void i2c_start(void);
/**
 * \brief         Performs an I2C bit write
 *
 * \param singlebit  Bit to write on I2C bus
 */
//---------------------------------------------------------------------------

void i2c_bit(unsigned char singlebit);
/**
 * \brief         Performs an I2C bit read
 *
 * \return        Value of SDA line during read timeslot
 */
//---------------------------------------------------------------------------
unsigned char i2c_readbit(void);

/**
 * \brief         Performs an I2C stop condition
 */
//---------------------------------------------------------------------------
void i2c_stop(void);

/**
 * \brief         Performs an I2C byte read
 *
 * \param doACK   Set to 1 to assert acknowledge after reading 8 bits,
 *                  or 0 to not assert ACK.
 *
 * \return        Value of SDA line during read timeslot
 */
//---------------------------------------------------------------------------
unsigned char i2c_readbyte(unsigned char doACK);

/**
 * \brief         Performs an I2C byte write
 *
 * \param singlebyte Value to write to bus.
 *
 * \return        0 if byte was acknowledged
 */
//---------------------------------------------------------------------------
unsigned char i2c_writebyte(unsigned char singlebyte);

/**
 * \brief         Perform I2C start, address selection.
 *
 * \param address Address of device to select.  Upper 7 bits
 *                are address, LSbit denotes read if 1 and write if 0.
 *
 * \return        0 if device acknowledged address selection
 */
//---------------------------------------------------------------------------
unsigned char i2c_select(unsigned char address);

/**
 * \brief         Perform I2C start, address selection, write specified
 *                bytes and I2C stop.
 *
 * \param address Address of device to select.  Upper 7 bits
 *                are address, LSbit automatically set to 0 by
 *                function.
 * \param barr    Array of bytes to write
 * \param length  Number of bytes to write
 *
 * \return        0 if device acknowledged address selection and data transfer
 */
//---------------------------------------------------------------------------
unsigned char i2c_writeblock(unsigned char address,unsigned char *barr,int length);

/**
 * \brief         Perform I2C start, address selection, read specified
 *                number of bytes and I2C stop.
 *
 * \param address Address of device to select.  Upper 7 bits
 *                are address, LSbit automatically set to 1 by
 *                function.
 * \param barr    Array destination for read bytes
 * \param length  Number of bytes to read
 *
 * \return        0 if device acknowledged address selection and data transfer
 */
//---------------------------------------------------------------------------
unsigned char i2c_readblock(unsigned char address,unsigned char *barr,int length);

/**
 * \brief         Perform I2C start, address selection, write specified
 *                bytes, I2C start, address slection, read bytes and I2C stop.
 *
 * \param address Address of device to select.  Upper 7 bits
 *                are address, LSbit automatically set to 0 by
 *                function.
 * \param barr1   Array of bytes to write
 * \param length1 Number of bytes to write
 * \param barr2   Array destination for read bytes
 * \param length2 Number of bytes to read
 *
 * \return        0 if device acknowledged address selection and data transfer
 */
//---------------------------------------------------------------------------
unsigned char i2c_writereadblock(unsigned char address,unsigned char *barr1,int length1,
                                                       unsigned char *barr2,int length2);

/* Define SCL and SDA to talk on the designated TINIs400 pins */
//#define I2C_SDA  P1_0
//#define I2C_SCL  P1_1

/** Define SDA (data) line to talk to the DS1672 on the TINIm400
 */
//#define I2C_SDA  P3_4

/** Define SCL (clock) line to talk to the DS1672 on the TINIm400
 */
//#define I2C_SCL  P3_5

/** Enable communication with slow slave devices.
 *  Value of 1 enables SCL waiting/flow control.
 */
#define I2C_ENABLE_SCL_WAIT_FOR_SLOW_SLAVES 0

/** Number of loops to wait for SCL to return high if SCL flow
 * control is used.
 */
#define I2C_MAXIMUM_SCL_WAITCOUNT 10000
//#define I2C_MAXIMUM_SCL_WAITCOUNT 500000

/** Number of loops to wait between any host SCL and SDA transitions
 */
#define I2C_DELAY_LOOP_COUNT 0

#endif


