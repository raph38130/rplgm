;******************************************************************************
;*                                                                            *
;*               PCF8591 ADC/DAC PERIPHERAL ROUTINES FOR OM5027               *
;*                                                                            *
;******************************************************************************
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;
;CONTENTS       dac                                                           ;
;               dacadcMode                                                    ;
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;

atodadr         equ     10011110b       ;PCF8591 address
adcctl          equ     01000100b       ;output on,4 inputs,inc,a/d channel 0
dacctl          equ     01000000b       ;output on
dactime         equ     05h             ;sets rate of dac ramping

        dseg
daCount:        ds      1               ;counter for dac ramping rate
dtoa:           ds      1               ;outgoing digital to analogue value
atod:           ds      1               ;incoming analoque to digital value
dseg_end        set     $

        cseg

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;DAC            Decrements and checks the frequency counter daCount before    ;
;               incrementing and outputing the DAC value to the PCF8591.      ;
;                                                                             ;
;ENTRY          daCount                                                       ;
;               dtoa                                                          ;
;                                                                             ;
;USES           iicbuff                                                       ;
;               r(1)2,3,4                                                     ;
;               acc                                                           ;
;                                                                             ;
;CALLS          iic                                                           ;
;                                                                             ;
;EXIT           daCount                                                       ;
;               dtoa                                                          ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
DAC:    djnz    daCount,dac0
        mov     daCount,#dacTime
        inc     dtoa                    ;increment output value
        mov     iicbuff,#dacctl         ;set digital control word
        mov     iicbuff+1,dtoa          ;output value
        setb    rs0                     ;select register bank 1
dac1:   mov     r2,#02                  ;tx = 2
        mov     r3,#00                  ;rx = 0
        mov     r4,#atodadr             ;address = atod
        call    iic
        inc     a
        jz      dac1
        clr     rs0                     ;select register bank 0
dac0:   ret

;--------- end of DAC

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;dacadcMode     Coordinates display of the DAC and ADC values on lcd2         ;
;                                                                             ;
;ENTRY          dtoa                                                          ;
;                                                                             ;
;USES           dptr                                                          ;
;                                                                             ;
;CALLS          lcd2_wrScr                                                    ;
;               dispByteLcd2                                                  ;
;               lcd2_newline                                                  ;
;               lcd2_indent                                                   ;
;                                                                             ;
;EXIT           nil                                                           ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
dacadcMode:
;initialise screen if first pass
        clr     modmode
        jnb     newlcd2,dcd0
        mov     dptr,#dacscr
        call    lcd2_WrScr
        clr     newlcd2

;now read dtoa value
dcd0:   mov     iicbuff,#adcctl ;set analogue control word
        setb    rs0             ;select register bank 1
dcd2:   mov     r2,#01          ;tx = 1
        mov     r3,#01          ;rx = 1
        mov     r4,#atodadr     ;address = atod
        call    iic
        inc     a
        jz      dcd2
        clr     rs0             ;select register bank 0
        mov     atod,iicbuff       ;get data

;display DAC value
        mov     iicbuff+1,#02
        call    lcd2_newline
        mov     iicbuff+1,#07
        call    lcd2_indent
        mov     a,dtoa
        call    DispByteLCD2

;display ADC value
        mov     iicbuff+1,#03
        call    lcd2_newline
        mov     iicbuff+1,#07
        call    lcd2_indent
        mov     a,atod
        call    DispByteLCD2

;check for exit key PB8
        jnb     keyvalid,dcdx
        mov     a,keybits
        jnb     acc.7,dcd1
        setb    newlcd2
dcd1:   call    restart_key_scan
dcdx:   ret

dacscr: db      '  PCF8591',04
        db      ' DAC = xxx',04
        db      ' ADC = xxx',04

;--------- end of DACADC

