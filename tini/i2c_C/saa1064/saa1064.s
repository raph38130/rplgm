
;****************************************************************************

;                     Software Implemented I2C Drivers

; These routines allow an 80C51 based microcontroller to drive the I2C bus 
; as a single master. The main program at the end demonstrates writing and 
; reading several types of devices: 

;    PCF8570 256 byte static RAM.
;    PCF8574 8-bit I/O expander.
;    SAA1064 4 digit LED display driver.


;             Written by G.Goodhue, Philips Components-Signetics
; Wed Feb 11 16:56:12 CET 2004 RP
;	- hack for TINIm400
; rm *hex *lst *rel *sym *map *ihx *hex *lnk *mem *rst
;
;../../tools/sdcc/bin/asx8051 -plosgff saa1064.s;../../tools/sdcc/bin/sdcc -mds400 --model-flat24 --stack-10bit -Wl-r --xram-loc 0x140000 --code-loc 0x490000 saa1064.rel;../../tools/sdcc/bin/packihx saa1064.ihx > saa1064.hex
;
;
;on ne peut pas choisir le banc mémoire avec a390 
;../../tini1.12/native/bin/Linux/a390 -l -s -Fhex -p 400 -f 1.12 saa1064.s
;
;****************************************************************************



I2CLED     =     0x3F           ;Slave address for SAA1064 LED driver.
SCLPin     =     P1.1           ;I2C serial clock line.
SDAPin     =     P1.0           ;I2C serial data line.

        .area REG_BANK_0        (REL,OVR,DATA)
        .ds 8
        .area REG_BANK_3        (REL,OVR,DATA)
       .ds 8
       
	.area SSEG (DATA)
__start__stack:
        .ds     1
	
	.area BSEG (BIT)
NoAck:	    .ds 1
BusFault:   .ds 1        ;I2C bus fault flag.
I2CBusy:    .ds 1        ;I2C busy flag.
		
	.area XSEG (XDATA)
BitCnt:     .db    1            ;Bit counter for I2C routines.
ByteCnt:    .db    2            ;Byte counter for I2C routines.
SlvAdr:     .db    3            ;Slave address for I2C routines.
XmtDat:     		 	;I2C transmit buffer, 8 bytes max.
	    .db   0
	    .db	  6
	    .db	  255
	    .db	  0
	    .db	  127
	    .db	  1
	    .db	  0
	    .db	  0
RcvDat:     .ascii    "        " ;I2C receive buffer, 8 bytes max.
AltRcv:     .ascii    "        " ;Alternate I2C receive buffer, 8 bytes max.

	.area ROMSEG (ABS,CON,DATA)
	.ds 24 ; 24 bytes of directs used starting at 0x68
	
	.area CSEG (CODE)
            sjmp Init
            .ascii "TINI400"
            .db 0
            .ascii "1.12" ; this is optional with the -f switch
            .db 0

Init: 
TestLoop:  
           ;clr    NoAck
	   ;SETB    NoAck
	   ;clr SDAPin
	   ;lcall BitDly
;loop:	   JB      NoAck,loop
	   ;setb SDAPin
	   ;lcall BitDly
	   ;LJMP    TestLoop
	
           MOV     SlvAdr,#I2CLED ;Write data to SAA1064 LED driver.
           MOV     R0,#XmtDat     ;Start of data.
           MOV     ByteCnt,#6     ;Send 6 bytes (subaddress, control, data).
           LCALL   SendData

	   LJMP    TestLoop       ;Repeat operation for scope watchers.


; BitDly - insures minimum high and low clock times on I2C bus.
; This routine must be tuned for the actual oscilator frequency used, shown 
; here tuned for a 12MHz clock. Note that the CALL instruction that invokes 
; BitDly already uses 2 machine cycles.

BitDly:    NOP         ;NOPs to delay 5 microseconds (minus 4
           NOP         ;  machine cycles for CALL and RET).
           NOP
           NOP
           NOP
           NOP
           NOP
	   RET


; SCLHigh - sends SCL pin high and waits for any clock stretching peripherals.

SCLHigh:   SETB    SCLPin         ;Set SCL from our end.
           JNB     SCLPin,$       ;Wait for pin to actually go high.
           RET


; SendStop - sends an I2C stop, releasing the bus.

SendStop:  CLR     SDAPin         ;Get SDA ready for stop.
           lCALL   SCLHigh        ;Set clock for stop.
           LCALL   BitDly
           SETB    SDAPin         ;Send I2C stop.
           LCALL   BitDly
           CLR     I2CBusy        ;Clear I2C busy status.
           RET                    ;Bus should now be released.


; SendByte - sends one byte of data to an I2C slave device.
; Enter with:
;   ACC = data byte to be sent.

SendByte:  MOV     BitCnt,#8      ;Set bit count.
SBloop:    RLC     A              ;Send one data bit.
           MOV     SDAPin,C       ;Put data bit on pin.
           LCALL   SCLHigh        ;Send clock.
           LCALL   BitDly
           CLR     SCLPin
           LCALL   BitDly
;loop:	   ljmp loop
;           DJNZ    BitCnt,SBloop  ;Repeat until all bits sent.

;loop:	   ljmp loop
           SETB    SDAPin         ;Release data line for acknowledge.
           LCALL   SCLHigh        ;Send clock for acknowledge.
           LCALL   BitDly
           JNB     SDAPin,SBEX    ;Check for valid acknowledge bit.
           SETB    NoAck          ;Set status for no acknowledge.
SBEX:      CLR     SCLPin         ;Finish acknowledge bit.
           LCALL   BitDly
           RET


; GoMaster - sends an I2C start and slave address.
; Enter with:
;   SlvAdr = slave address.

GoMaster:  SETB    I2CBusy        ;Indicate that I2C frame is in progress.
           CLR     NoAck          ;Clear error status flags.
           CLR     BusFault
           JNB     SCLPin,Fault   ;Check for bus clear.
           JNB     SDAPin,Fault
           CLR     SDAPin         ;Begin I2C start.
           LCALL   BitDly
           CLR     SCLPin
           LCALL   BitDly         ;Complete I2C start.
           MOV     A,SlvAdr       ;Get slave address.
           LCALL   SendByte       ;Send slave address.
           RET

Fault:     SETB    BusFault       ;Set fault status
           RET                    ;  and exit.


; SendData - sends one or more bytes of data to an I2C slave device.
; Enter with:
;   ByteCnt = count of bytes to be sent.
;   SlvAdr  = slave address.
;   @R0     = data to be sent (the first data byte will be the 
;             subaddress, if the I2C device expects one).

SendData:  LCALL   GoMaster       ;Acquire bus and send slave address.
           JB      NoAck,SDEX     ;Check for slave not responding.

SDLoop:    MOV     A,@R0          ;Get data byte from buffer.
           LCALL   SendByte       ;Send next data byte.
;never
           INC     R0             ;Advance buffer pointer.
           JB      NoAck,SDEX     ;Check for slave not responding.
           DJNZ    ByteCnt,SDLoop ;All bytes sent?

SDEX:      LCALL   SendStop       ;Done, send an I2C stop.
           RET


;RcvByte - receives one byte of data from an I2C slave device.
; Returns:
;   ACC = data byte received.

RcvByte:   MOV     BitCnt,#8      ;Set bit count.

RBLoop:    LCALL   SCLHigh        ;Read one data bit.
           LCALL   BitDly
           MOV     C,SDAPin       ;Get data bit from pin.
           RLC     A              ;Rotate bit into result byte.
           CLR     SCLPin
           LCALL   BitDly
           DJNZ    BitCnt,RBLoop  ;Repeat until all bits received.

           PUSH    ACC            ;Save accumulator
           MOV     A,ByteCnt
           CJNE    A,#1,RBAck     ;Check for last byte of frame.
           SETB    SDAPin         ;Send no acknowledge on last byte.
           SJMP    RBAClk

RBAck:     CLR     SDAPin         ;Send acknowledge bit.
RBAClk:    LCALL   SCLHigh        ;Send acknowledge clock.
           POP     ACC            ;Restore accumulator
           LCALL   BitDly
           CLR     SCLPin
           SETB    SDAPin         ;Clear acknowledge bit.
           LCALL   BitDly
           RET


;RcvData - receives sends one or more bytes of data from an I2C slave device.
; Enter with:
;   ByteCnt = count of bytes to be sent.
;   SlvAdr  = slave address.
; Returns:
;   @R0     = data received.

; Note: to receive with a subaddress, use SendData to set the subaddress
;   first (no provision for repeated start).

RcvData:   INC     SlvAdr         ;Set for READ of slave.
           LCALL   GoMaster       ;Acquire bus and send slave address.
           JB      NoAck,RDEX     ;Check for slave not responding.

RDLoop:    LCALL   RcvByte        ;Recieve next data byte.
           MOV     @R0,A          ;Save data byte in buffer.
           INC     R0             ;Advance buffer pointer.
           DJNZ    ByteCnt,RDLoop ;Repeat untill all bytes received.

RDEX:      LCALL   SendStop       ;Done, send an I2C stop.
           RET

	.area DSEG (DATA)
	
	.area ISEG (DATA)
	
	
	
