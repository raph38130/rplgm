;******************************************************************************
;*                                                                            *
;*                   OM5027 MODULE INFORMATION MODE ROUTINE                   *
;*                                                                            *
;******************************************************************************

ramadr          equ     0AEh    ;PCF8570 address
eepromadr       equ     0A6h    ;PCF8581 address
ioadr           equ     04Eh    ;PCF8574 address

        dseg
module:         ds      1       ;currently selected module
dseg_end        set     $

        cseg
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;ModuleInfo     Coordinates the display of module information on lcd2.        ;
;               On first pass the screen is initialised. PB3 and PB7 are      ;
;               used to cycle around - one screen per module. Information     ;
;               constists of I.C name, descriptive name, presence on IIC bus. ;
;                                                                             ;
;ENTRY          module                                                        ;
;               newlcd2                                                       ;
;               keybits                                                       ;
;               keyvalid                                                      ;
;               modmode                                                       ;
;                                                                             ;
;USES           acc,dptr,b                                                    ;
;                                                                             ;
;CALLS          restart_key_scan                                              ;
;               lcd2_WrPage                                                   ;
;                                                                             ;
;EXIT           module                                                        ;
;               newlcd2                                                       ;
;               modmode                                                       ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
moduleInfo:
;Update screen if necessary
        jnb     newlcd2,mio0
        setb    modmode
        clr     newlcd2

;first get module page base for dptr
        mov     a,module
        rl      a
        add     a,#infotbl-mark
        movc    a,@a+pc
mark:   mov     dph,a
        mov     a,module
        rl      a
        add     a,#05
        movc    a,@a+pc
        mov     dpl,a
        sjmp    mio6
infotbl:dw      info0
        dw      info1
        dw      info2
        dw      info3
        dw      info4
        dw      info5
        dw      info6
        dw      info7
        dw      info8
        dw      info9
        dw      info10
        dw      info11

mio6:   mov     r2,#00
        call    lcd2_WrPage

;first set up lcd2 ddram address to on/off location
mio0:   mov     iicbuff+1,#01
        call    lcd2_newline
        mov     iicbuff+1,#09
        call    lcd2_indent

;test for presence of currently selected module by reading a byte
        setb    rs0
        mov     iicbuff,#00
        mov     r2,#00
        mov     r3,#01
        mov     a,module
        mov     dptr,#miojmp
        rl      a
        jmp     @a+dptr
miojmp: sjmp    mij13
        sjmp    mij1
        sjmp    mij2
        sjmp    mij3
        sjmp    mij4
        sjmp    mij5
        sjmp    mij6
        sjmp    mij13
        sjmp    mij8
        sjmp    mij9
        sjmp    mij15
        sjmp    mij15

mij1:   mov     r4,#clkadr
        sjmp    mij12
mij2:   mov     r4,#ramadr
        sjmp    mij12
mij3:   mov     r4,#eepromadr
        sjmp    mij12
mij4:   mov     r4,#dtmfadr
        mov     iicbuff,#dtmfo      ;write a byte to dtmf
        mov     r2,#01
        mov     r3,#00
        sjmp    mij12
mij5:   mov     r4,#ioadr
        sjmp    mij12
mij6:   mov     r4,#atodadr
        sjmp    mij12
mij8:   mov     r4,#keypadadr
        sjmp    mij12
mij9:   mov     r4,#lcd1adr
        mov     r2,#01
        mov     r3,#00

mij12:  call    iic
        mov     dptr,#on
        jz      mij14
        mov     dptr,#off
        dec     a
        jz      mij14
mij13:  mov     dptr,#blank
        sjmp    mij14
mij15:  mov     dptr,#on
mij14:  clr     rs0
        call    lcd2_WrCodeStr

;process key data
        jnb     keyvalid,miox
        mov     a,keybits
        jb      acc.3,mio8
        jnb     acc.7,mio5
mio8:   clr     modmode
        setb    newlcd2
        mov     module,#00
        sjmp    miox
mio5:   jnb     acc.6,mio1
        mov     a,module
        inc     a
        cjne    a,#12,$+3
        jc      mio4
        clr     a
        sjmp    mio4
mio1:   jnb     acc.2,mio2
        mov     a,module
        dec     a
        cjne    a,#12,$+3
        jc      mio4
        mov     a,#11
mio4:   setb    newlcd2
        mov     module,a
mio2:   call    restart_key_scan
miox:   ret


on:     db      'ON ',04
off:    db      'OFF',04
blank:  db      '   ',04

info0:  db      '  OM5027    ',04
        db      '  Module    ',04
        db      'Information ',04

info1:  db      'PCF8593     ',04
        db      'Clock/Alarm ',04
        db      'and Calendar',04

info2:  db      'PCF8570     ',04
        db      'Static RAM  ',04
        db      '256 bytes   ',04

info3:  db      'PCF8581C    ',04
        db      'EEPROM with ',04
        db      '128 bytes   ',04

info4:  db      'PCD3312     ',04
        db      'DTMF tone   ',04
        db      'generator   ',04

info5:  db      'PCF8574     ',04
        db      'Digital Port',04
        db      '8 bit I/O   ',04

info6:  db      'PCF8591     ',04
        db      '4 ADC Inputs',04
        db      '1 DAC Output',04

info7:  db      'PCF8584  OFF',04
        db      'IIC/parallel',04
        db      'interface   ',04

info8:  db      'PCF8574A    ',04
        db      'Keypad/leds ',04
        db      '(8 bit I/O) ',04

info9:  db      'LPH3802     ',04
        db      'LCD#1 7-seg.',04
        db      '& Starburst ',04

info10: db      'LPH3827     ',04
        db      'LCD#2 5x7   ',04
        db      'Dot Matrix  ',04

info11: db      '80CL580     ',04
        db      'Low voltage ',04
        db      'microcontrol',04

;--------- end of moduleinfo
