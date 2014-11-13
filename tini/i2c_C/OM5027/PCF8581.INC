;******************************************************************************
;*                                                                            *
;*                         PCF8581 EEPROM ROUTINE                             *
;*                                                                            *
;******************************************************************************
;NOT INCORPORATED IN OM5027 DEMONSTRATION PROGRAM AS THE I.C HAS A LIMITED
;NUMBER OF WRITE CYCLES.

        DSEG
ebuff:          ds      22              ;eeprom buffer
dseg_end        set     $

        CSEG
;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
;ROMWR write to the PCF8581 eeprom chip.
;Due to the added complexity of the eeprom write routine, any data
;to be transfered is loaded into a special buffer. This buffer is
;22 bytes long but could be made longer by altering the equate table.
;
;The first byte in the buffer is the number of data bytes. The next
;byte is the internal word address and the rest of the buffer, upto 20
;bytes, is filled with data for consecutive memory locations in the chip.
;
;The PCB8581 takes several milliseconds to process each written byte.
;If the PCB8581 is still busy processing the last data written to it,
;it will not acknowledge its write address so the routine will exit
;without altering the buffer contents. If it is sucessful in writing
;upto 7 bytes to the chip then the buffer contents will be altered to
;reflect this fact.
;
;On exit,the first byte in the buffer will be the number of bytes
;still to be sent. This will be zero when the buffer is free.
;The second byte is the address of the next memory location to be written
;to. The buffer data contents are moved to the start of the buffer ready
;for the next time the routine is called.
;ENTRY  EBUFF[0]        number of bytes to send
;       EBUFF[1]        device internal sub address
;       EBUFF[2..22]    up to 20 data bytes
;USES   R0              pointer to EBUFF
;       R1              pointer to IICBUFF
;       R2              counter of bytes sent, IIC Tx count
;       R3              IIC Rx count
;       R4              IIC address
;       IICBUFF
;CALLS  IIC
;STACK  2 bytes
;EXIT   EBUFF[0]        number of bytes left to send
;       EBUFF[1]        next internal sub address
;       EBUFF[2..22]    data left to send
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
romwr:  mov     r0,#ebuff               ;point to eeprom buffer
        mov     r1,#iicbuff             ;point to transfer buffer
        mov     r2,#00h                 ;number sent counter, 7 max
        mov     a,@r0                   ;get the number to send
        jz      romwr_exit              ;exit if none left to send

        mov     r3,a                    ;save the number to send
        inc     r0                      ;point to word address
        mov     a,@r0                   ;get the word address
        mov     @r1,a                   ;put word address into buffer

romwr_loop:
        inc     r0                      ;point to a data byte
	inc	r1
        mov     a,@r0                   ;get the data byte
        mov     @r1,a                   ;put the data byte
        inc     r2                      ;up the number sent counter
        djnz    r3,romwr_l1             ;down the number to send counter
        sjmp    romwr_send
romwr_l1:
        cjne    r2,#7,romwr_loop

romwr_send:
        mov     a,r2                    ;get the number to send
        inc     a                       ;make allowance for word address
        setb    rs0                     ;select register bank 1
        mov     r2,a                    ;tx = bytes+1
        mov     r3,#00h                 ;rx = 0
        mov     r4,#eeprom              ;set the address
        call    iic
        clr     rs0                     ;select register bank 0
        jnz     romwr_exit              ;non zero is a failure
        mov     r1,#ebuff               ;point to start of ebuff
        mov     a,r3                    ;get new number to send
        mov     @r1,a                   ;save it for next time
        jz      romwr_exit              ;if zero exit
        inc     r1                      ;point to word address
        mov     a,@r1                   ;get the old address
        add     a,r2                    ;add in the number sent
        mov     @r1,a                   ;save it for next time
romwr_move:
        inc     r1                      ;point to first data byte
        inc     r0                      ;point to first non sent byte
	mov	a,@r0
	mov	@r1,a
        djnz    r3,romwr_move           ;jump back more to move
romwr_exit:
        ret                             ;done until next time
;
;-------------- end of ROMWR

