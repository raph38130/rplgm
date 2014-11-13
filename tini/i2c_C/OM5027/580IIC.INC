;******************************************************************************
;580IIC.INC contains
;INITIALIZE_IIC         initialize the IIC interface
;IIC                    execute the transfer request
;******************************************************************************

;-----------------------------------------------------------------------------;
;                    IIC routines for the 8051 derivatives                    ;
;                        with byte wide interfaces.                           ;
;                               07 March 1994                                 ;
;
;               This code is presented as is and therefore it is              ;
;               for the user to decide if these routines are                  ;
;               applicable to his application. No correspondance              ;
;               will be entered into.                                         ;
;                                                                             ;
;               G.D.Moss        Philips N.Z. Ltd.                             ;
;-----------------------------------------------------------------------------;
;                            MODIFICATIONS                                    ;
;                                                                             ;
;1) These routines cater for single master systems only. Slave reciever and   ;
;   Slave Transmitter modes are not implimeneted.                             ;
;                                                                             ;
;2) These routines have been modified to ignore the "acknollage bit" when     ;
;   communicating with the LPH3802-1 LCD module. Therefore the IGNACK flag    ;
;   must be set prior to invoking IIC to communicate with the LPH3802-1, and  ;
;   reset immediately afterwards for normal communication with other devies.  ;
;                                                                             ;
;-----------------------------------------------------------------------------;
;
;define bits for s1con,# instructions
bcr2            equ     80h             ;s1con, serial bit rate flag 2
bens1           equ     40h             ;s1con, enable serial interface
bsta            equ     20h             ;s1con, start request flag
bsto            equ     10h             ;s1con, stop request flag
bsi             equ     08h             ;s1con, serial task complete
baa             equ     04h             ;s1con, arbitration lost flag
bcr1            equ     02h             ;s1con, serial bit rate flag 1
bcr0            equ     01h             ;s1con, serial bit rate flag 0
;
;define constants for i2c rouitines
speed           equ     bcr1+bcr2       ;iic bus speed 60 KHz at 3.58 Mhz

        bseg
ignack:         dbit    1
buserror:       dbit    1
bseg_end        set     $

        dseg
iicbuff:        ds      40      ;iic buffer
dseg_end        set     $
        cseg
;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
;INITIALIZE_IIC         initialize the IIC interface lines
;Generate a stop condition to free the bus and check for stuck port lines
;at the same time.
;ENTRY  nil
;STACK  nil
;EXIT   ACC     00      interface lines OK
;               01      interface lines stuck
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
initialize_iic:
        orl     p1,#11000000b           ;set scl/sda hi
        CLR     IGNACK                  ;IGNORE ACKNOWLEDGE FLAG
        CLR     BUSERROR
        clr     scl
        nop
        jb      scl,i_iic_error
        clr     sda
        nop
        jb      sda,i_iic_error
        nop
        nop
        nop
        nop
        setb    scl
        nop
        nop
        nop
        nop
        jnb     scl,i_iic_error
        setb    sda                     ;stop condition
        nop
        jnb     sda,i_iic_error
        clr     a
        mov     s1con,#bens1+bsto+baa+speed ;sets up the iic hardware
        ret
i_iic_error:
        mov     a,#001h
        ret
;
;-------------- end of INITIALIZE_IIC
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
;       I2C SERIAL INTERFACE SOFTWARE ROUTINES. They emulate
;       the routines used on the 1987 evaluation board.
;
;Note:	The register bank selected prior to the call will be used
;       exclusively for IIC SIO routines.
;       8 data bytes is the maximum that can be sent or recieved
;       in its present form. This is due to the size of IICbuff but
;       can be easily altered as the application requires.
;
;ENTRY  R'2     holds the number to transmit
;       R'3     holds the number to receive
;       R'4     slave address, with r/w bit = 0
;USES   R'1     points to the tx/rx buffer
;       ACC
;STACK  0 bytes
;EXIT   ACC     00 if transfer ok
;               01 if not acknowledged
;               FF if a bus error occured
;       R'1     points to last read byte
;       R'2     lost
;       R'3     lost
;       R'4     preserved
;       DPTR    lost
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
iic:    mov     r1,#IICbuff
        mov     s1con,#bens1+bsto+baa+speed
        setb    sta
        jmp     wsi1
;08H    A START CONDITION WAS SENT
;       SLA+R/W SENT, ACK BIT RECEIVED
iics:   clr     sta                     ;so we don't generate repeated starts
        mov     a,r4                    ;get address
        cjne    r2,#0,rdadd             ;transmit first
        setb    acc.0                   ;receive only
rdadd:
        mov     s1dat,a                 ;load address to send
        clr     si                      ;this will now transmit the address
        jmp     wsi1                    ;wait for si = 1,status 18h 20h 40h 48h
;10H    A REPEATED START WAS GENERATED
;       RESEND THE ADDRESS AS SLA+R, ACK BIT RECEIVED
iicrs:  clr     sta                     ;not repeated repeated starts
        mov     a,r4                    ;get the address
        setb    acc.0                   ;make it a read
        mov     s1dat,a                 ;ready it for sending
        clr     si                      ;send the address on its way
        jmp     wsi1                    ;wait for si = 1,status 40h 48h
;18H    SLA+W SENT, ACK RECEIVED
;       SEND DATA FIRST BYTE, ACK RECEIVED
mtxa:
        mov     a,@r1                   ;get data to send
        mov     s1dat,a                 ;put it where it will be sent from
        clr     si                      ;send it on its way
        jmp     wsi1                    ;wait for si = 1,status 28h 30h
;28H    DATA SENT, ACK RECEIVED
;       SEE IF MORE TO SEND, ELSE SEND A STOP
mtxd:   djnz    r2,mtxd1                ;jump if multiple bytes to send
        mov     a,r3                    ;get number to read
        jz      stopok                  ;none so stop
        setb    sta                     ;generate a repeated start
        clr     si                      ;start the repeated start
        jmp     wsi1                    ;wait for si = 1,status 10h
mtxd1:  inc     r1                      ;point to next byte to send
        mov     a,@r1
        mov     s1dat,a                 ;ready it for sending
        clr     si
        jmp     wsi1                    ;wait for si = 1,status 28h 30h
;40H    SLA+R SENT,ACK RECEIVED
;       STORE RECEIVED DATA, ACK SENT
mrxa:   mov     r1,#IICbuff             ;point to buffer
        djnz    r3,rdmlti               ;jump if more than 1 byte to read
rdlast: clr     aa                      ;set interface to not acknowledge
rdmlti: clr     si                      ;start it on its way
        jmp    wsi1                     ;wait for si = 1,status 50h 58h
;50H    DATA RECEIVED, ACK SENT
;       STORE RECEIVED DATA, IF THE NEXT IS THE LAST SEND NACK  ELSE SEND ACK
mrxd:   mov     a,s1dat                 ;get the received byte
        mov     @r1,a                   ;put it into IICbuff
        inc     r1                      ;point to next
        djnz    r3,rdmlti               ;loop back to read more
        sjmp    rdlast                  ;read the last with no acknowledge
;as some af the iic software modules are longer than 8 bytes (optimum)
;we vector to each routine via a jump table.
wsi1:   clr     a
wsi2:   jb      si,wsi3
        djnz    acc,wsi2
        sjmp    buserr

wsi3:   mov     a,s1sta                 ;get the status
        rr      a                       ;/2
        rr      a                       ;/4
        cjne    a,#23,$+3
        jnc     buserr                  ;protect against any multimaster states
        mov     dptr,#iictbl            ;point to the table
        jmp     @a+dptr
;master mode jump table s1sta codes and discription
iictbl: sjmp    buserr                  ;00h    bus error during mst or sla modes
        sjmp    iics                    ;08h    start has been sent
        sjmp    iicrs                   ;10h    repeated start has been sent
        sjmp    mtxa                    ;18h    sla_w sent, ack recieved
        sjmp    noack20                 ;20h    sla_w sent, nack recieved
        sjmp    mtxd                    ;28h    data sent, ack recieved
        sjmp    noack30                 ;30h    data sent, nack recieved
        sjmp    buserr                  ;38h    arbitration lost in sla and rw or data
        sjmp    mrxa                    ;40h    sla_r sent ack recieved
        sjmp    noack48                 ;48h    sla_r sent nack recieved
        sjmp    mrxd                    ;50h    data and ack recieved
        sjmp    mrxna                   ;58h    data and nack recieved

;STATES ASSOCIATED WITH SLAVE RECIEVER AND SLAVE MASTER ARE TO RESULT
;IN A BUS ERROR, AS THIS IS A SINGLE MASTER ROUTINE.

;58H    DATA RECEIVED, NACK SENT
;       STORE RECEIVED DATA, GENERATE A STOP
mrxna:  mov     a,s1dat                 ;get the last byte
        mov     @r1,a                   ;put it into IICbuff
stopok: clr     a                       ;return the accu = 00
        setb    aa                      ;set acknowledge bit
stop:   setb    sto                     ;issue a stop
        clr     si                      ;start a stop
        ret                       ;iic routine finished so return

;20H    30H     48H
;       SLA+R/W SENT, NACK RECEIVED
;       GENERATE A STOP, RETURN WITH ACC = 01H
noack20:JB      IGNACK,MTXA             ;IGNORE NOT ACKNOWLEDGE
        mov     a,#01h                  ;return the accu = 01
        sjmp    stop                    ;issue a stop and exit

noack30:JB      IGNACK,MTXD             ;IGNORE NOT ACKNOWLEDGE
        mov     a,#01h                  ;return the accu = 01
        sjmp    stop                    ;issue a stop and exit

noack48:JB      IGNACK,MRXA             ;IGNORE NOT ACKNOWLEDGE
        mov     a,#01h                  ;return the accu = 01
        sjmp    stop                    ;issue a stop and exit

;00H    38H
;       BUS ERROR, FREE THE BUS AND RETURN ACC = FFH
;       CALLER TO DECIDE ON RETRY
buserr: mov     s1con,#bens1+bsto+baa+speed ;release the bus
        mov     a,#0ffh                 ;return with accu = ff
        setb    buserror                ;
        ret                             ;done
;
;-------------- end of IIC routine
