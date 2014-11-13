;******************************************************************************
;*                                                                            *
;*                            DTMF ROUTINES                                   *
;*                                                                            *
;******************************************************************************
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;
;CONTENTS     beep                                                            ;
;             musicmode                                                       ;
;             dtmf_off                                                        ;
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;
dtmfadr         equ     01001010b       ;PCD3312 address
dtmft           equ     00111111b       ;alarm tone 1768Hz
dtmfb           equ     00110101b       ;tone for seconds beep 832Hz
dtmfo           equ     00000001b       ;turns the dtmf oscillator off

;musical tone codes for the dtmf
Dsh5            equ     30h
E5              equ     31h
F5              equ     32h
Fsh5            equ     33h
G5              equ     34h
Gsh5            equ     35h
A5              equ     36h
Ash5            equ     37h
B5              equ     38h
C6              equ     39h
Csh6            equ     3Ah
D6              equ     29h
Dsh6            equ     3Bh
E6              equ     3ch
F6              equ     3dh
Fsh6            equ     0eh
G6              equ     3eh
Gsh6            equ     2Ch
A6              equ     3fh
Ash6            equ     04h
B6              equ     05h
C7              equ     25h
Csh7            equ     2Fh
D7              equ     06h
o               equ     00h

        bseg
dtmf_in_use:     dbit    1      ;set if in use
shortbeep:       dbit    1      ;key beep in progress
bseg_end         set     $

        dseg
tone:            ds      1      ;holds a tone code
tunepointer:     ds      1      ;pointer into a tune block
toneCount:       ds      1      ;delay between notes
tuneNo:          ds      1      ;current tune
dseg_end         set     $

        cseg

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
;BEEP           Checks the seconds and outputs a tone accordingly.
;               If seconds = 00 turns on minute beep, if seconds = 01 turns off
;               beep. If the alarm is running it plays a melody.
;
;ENTRY          aring   set     alarm is ring, play melody
;               clk2            current seconds
;               dtmf_in_use     if set then no minute beep
;
;USES           acc
;               tone            to ramp the melody
;               iicbuff         for IIC transfer
;
;CALLS          iic
;
;EXIT           aring   clr     if end of melody reached
;               tone    dtmft   first tone value
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
beep:   jb      aring,ring              ;if ringing jump
        jb      dtmf_in_use,beepx
        mov     a,clk2                  ;get seconds
        jnz     sec01                   ;not 00 go test if 01
        mov     iicbuff,#dtmfb          ;is zero so tone for seconds beep
        sjmp    beepop

sec01:  cjne    a,#01h,beepx            ;exit if not 01
beepof: mov     iicbuff,#dtmfo          ;dtmf off value
        sjmp    beepop                  ;output it

ring:   mov     a,clk2                  ;get the seconds
        cjne    a,#12,$+3               ;exit if not 10
        jc      ringx
        clr     aring                   ;turn off alarm ringing flag
        sjmp    beepof

ringx:  jnb     acc.0,rgoff
        mov     iicbuff,#dtmft           ;move tone to buffer
        sjmp    beepop
rgoff:  mov     iicbuff,#dtmfo           ;pulse off tone

beepop: setb    rs0                     ;select register bank 1
        mov     r2,#01                  ;tx = 1 always send 1
        mov     r3,#00                  ;rx = 0
        mov     r4,#dtmfadr             ;address = dtmf
        call    iic                     ;call the general interface
        inc     a
        jz      beepof
        clr     rs0                     ;select register bank 0
beepx:  ret                             ;done

;----------------- end of beep

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
;musicMode      Plays one of a selection of tunes using the dtmf. Each tune
;               comprises of a series of notes each followed by delay. The
;               delay component is a multiple of the main loop time.
;
;ENTRY          newlcd2         refresh screen and init variables
;               tuneNo          current tune
;               tunepointer     pointer into tune
;               tonecount       delay count (note length)
;               keyvalid
;               keybits
;
;
;USES           iicbuff
;               a,r(1)2,3,4
;               dptr
;
;
;CALLS          restart_key_scan
;               iic
;
;EXIT           dtmf_in_use
;               newlcd2
;               tuneNo          current tune
;               tunepointer     pointer into tune
;               tonecount       delay count (note length)
;               keyvalid
;               keybits
;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
musicMode:
;initialise screen if first pass
        jnb     newlcd2,msc0
        setb    modmode
        mov     dptr,#muscr
        call    lcd2_WrScr
        mov     tuneNo,#00
        clr     newlcd2

;get dptr for current tune (0 = no tune)
msc0:   mov     a,tuneNo
        rl      a
        add     a,#tunetbl-mark3
        movc    a,@a+pc
mark3:  mov     dph,a
        mov     a,tuneNo
        rl      a
        add     a,#05
        movc    a,@a+pc
        mov     dpl,a
        sjmp    msc8
tunetbl:
        dw      nulltune
        dw      tune1
        dw      tune2
        dw      tune3
        dw      tune4
        dw      tune5
      
;process key data
msc8:   jnb     keyvalid,msc3
        mov     tunepointer,#00
        mov     tonecount,#10
        mov     a,keybits
        mov     tuneNo,#00
        jnb     acc.1,$+6
        mov     tuneNo,#01
        jnb     acc.2,$+6
        mov     tuneNo,#02
        jnb     acc.4,$+6
        mov     tuneNo,#03
        jnb     acc.5,$+6
        mov     tuneNo,#04
        jnb     acc.6,$+6
        mov     tuneNo,#05
        jb      acc.3,msc6
        setb    dtmf_in_use
        jnb     acc.7,msc5

msc6:   setb    newlcd2                 ;prepare to exit
        clr     modmode
        clr     dtmf_in_use
        mov     iicbuff,#dtmfo
        sjmp    msc4

msc5:   call    restart_key_scan


;if previous note has expired then play next note for next delay period
msc3:   mov     a,tuneNo
        jz      mscx
        jb      aring,mscx              ;dont play if alarm ringing
        djnz    tonecount,mscx
        mov     a,tunepointer
        movc    a,@a+dptr               ;get new tone
        cjne    a,#0ffh,msc7            ;if end of tune then exit
        clr     dtmf_in_use
        mov     iicbuff,#dtmfo
        mov     tuneNo,#00
        mov     tonecount,#00
        sjmp    msc4
msc7:   mov     iicbuff,a               ;play note and inc pointer
        inc     tunepointer
        mov     a,tunepointer
        movc    a,@a+dptr               ;get duration count
        mov     tonecount,a
msc4:   setb    rs0                     ;send note
        mov     r2,#01
        mov     r3,#00
        mov     r4,#dtmfadr
        call    iic
        inc     a
        jz      msc4
        clr     rs0
        inc     tunepointer            ;inc pointer for next tone
        mov     a,tonecount
        jnz     mscx
        mov     tonecount,#01
        ljmp    msc0
mscx:   ret

muscr:  db      '  PCD3312',04
        db      '  Melodies',04
        db      0c9h,0c9h,0c9h,' ',0c9h,0c9h,0c9h,' ',0c9h,0c9h,0c9h,04

nulltune:
        db      0ffh

tune1:  db      CSH6,3,A5,3,B5,3,CSH6,3,E6,3,D6,3,O,0,D6,3,FSH6,3,E6,3,O,0,E6,3
        db      A6,3,GSH6,3,A6,3,E6,3,CSH6,3,A5,3,B5,3,CSH6,3
        db      D6,3,FSH6,3,E6,3,D6,3,CSH6,3,A5,3,B5,3,CSH6,3,A5,3
        db      GSH5,3,A5,3,B5,3,E5,3,GSH5,3,B5,3,D6,3,CSH6,3,B5,3

        db      CSH6,3,A5,3,B5,3,CSH6,3,E6,3,D6,3,O,0,D6,3,FSH6,3,E6,3,O,0,E6,3
        db      A6,3,GSH6,3,A6,3,E6,3,CSH6,3,A5,3,B5,3,CSH6,3
        db      FSH5,3,E6,3,D6,3,CSH6,3,B5,3,A5,3,E5,3,A5,3,GSH5,3,A5,3
        db      CSH6,3,E6,3,A6,3,E6,3,CSH6,3,A5,6,0FFH


tune2:  db      B5,8,D6,4,A6,12,G6,8,D6,4,C6,12
        db      B5,8,o,0,B5,4,o,0,B5,4,C6,4,D6,4,E6,12,D6,12
        db      B5,8,D6,4,A6,12,G6,8,D6,4,C6,12
        db      B5,8,D6,4,o,0,D6,4,E6,4,FSH6,4,G6,12,o,0,G6,12

        db      A6,8,D6,1,o,0,D6,4,FSH6,4,E6,4,D6,4
        db      B5,8,D6,4,G6,12,E6,8,G6,4,A6,8
        db      G6,4,FSH6,12,D6,12

        db      B5,8,D6,4,A6,12,G6,8,D6,4,C6,12
        db      B5,8,D6,4,D6,4,E6,4,FSH6,4,G6,12,o,0,G6,12,0FFH


tune3:  db      A5,1,C6,1,E6,1,A5,1,C6,1,E6,1,A5,1,C6,1,E6,1,A5,1,C6,1,E6,1,A5,1
        db      C6,1,F6,1,A5,1,C6,1,F6,1,A5,1,C6,1,F6,1,A5,1,C6,1,F6,1,B5,1,D6,1
        db      F6,1,B5,1,D6,1,F6,1,B5,1,D6,1,F6,1,B5,1,D6,1,F6,1,B5,1,D6,1,G6,1
        db      B5,1,D6,1,G6,1,B5,1,D6,1,C6,1,E6,1,G6,1,C6,1,E6,1,G6,1,C6,1,E6,1
        db      G6,1,C6,1,E6,1,G6,1,C6,1,DSH6,1,GSH6,1,C6,1,DSH6,1,GSH6,1,C6,1
        db      DSH6,1,GSH6,1,C6,1,DSH6,1,ASH6,1,C6,1,DSH6,1,ASH6,1,D6,1,F6,1
        db      ASH6,1,D6,1,F6,1,ASH6,1,D6,1,F6,1,ASH6,1,D6,1,F6,1,ASH6,1,D6,1
        db      F6,1,C7,1,E6,1,G6,1,C7,1,E6,1,G6,1,C7,1,E6,1,G6,1,C7,1,E6,1,G6,1
        db      C7,1,F6,1,GSH6,1,Csh7,1,E6,1,G6,1,C7,1,F6,1,GSH6,1,Csh7,1,E6,1
        db      G6,1,C7,1,F6,1,GSH6,1,Csh7,1,E6,1,G6,1,C7,1,F6,1,GSH6,1,Csh7,1
        db      E6,1,G6,1,C7,4,0ffh

tune4:  db      F5,2,A5,2,D6,2,G6,2,C7,2,G6,2,D6,2,A5,2
        db      F5,2,C6,2,E6,2,A6,2,C7,2,A6,2,E6,2,C6,2
        db      F5,2,A5,2,DSH6,2,G6,2,C7,2,G6,2,DSH6,2,A5,2
        db      F5,2,ASH5,2,D6,2,F6,2,ASH6,2,F6,2,D6,2,ASH5,2
        db      F5,2,ASH5,2,CSH6,2,F6,2,ASH6,2,F6,2,CSH6,2,ASH5,2
        db      F5,2,A5,2,D6,2,G6,2,C7,2,G6,2,D6,2,A5,2
        db      G5,2,B5,2,FSH6,2,A6,2,D7,2,A6,2,FSH6,2,B5,2
        db      C6,2,E6,2,G6,2,ASH6,2,C7,2,ASH6,2,G6,2,E6,2,
        db      F5,2,A5,2,C6,2,F6,2,G6,2,F6,2,C6,2,A5,2,F5,4,0FFH

tune5:  db      E6,1,FSH6,1,G6,1,GSH6,1,G6,1,FSH6,1,F6,1,E6,1
        db      A6,1,GSH6,1,G6,1,GSH6,1,G6,1,FSH6,1,F6,1
        db      E6,1,FSH6,1,G6,1,GSH6,1,G6,1,FSH6,1,F6,1,E6,1
        db      A6,1,GSH6,1,G6,1,GSH6,1,G6,1,FSH6,1,F6,1
        db      E6,1,FSH6,1,G6,1,GSH6,1,G6,1,FSH6,1,F6,1,FSH6,1,F6,1,E6,1,DSH6,1,E6,1,F6,1
        db      FSH6,1,G6,1,GSH6,1,A6,1,GSH6,1,G6,1,GSH6,8

        db      A6,1,B6,1,C7,1,CSH7,1,C7,1,B6,1,ASH6,1,A6,1
        db      D7,1,CSH7,1,C7,1,CSH7,1,C7,1,B6,1,ASH6,1
        db      A6,1,B6,1,C7,1,CSH7,1,C7,1,B6,1,ASH6,1,A6,1
        db      D7,1,CSH7,1,C7,1,CSH7,1,C7,1,B6,1,ASH6,1
        db      A6,1,B6,1,C7,1,CSH7,1,C7,1,B6,1,ASH6,1,B6,1,ASH6,1,A6,1,GSH6,1,A6,1,ASH6,1
        db      B6,1,C7,1,CSH7,1,D7,1,CSH7,1,C7,1,CSH7,8,0FFH

;The following is a spare tune
;tune5:  db      F5,2,G5,2,A5,2,ASH5,2,C6,2,D6,2,E6,2,F6,2,G6,2,A6,2,C7,2,A6,2
;        db      G6,2,F6,2,E6,2,D6,2,C6,2,ASH5,2,A5,2,G5,2,F5,2,E5,2,F5,8
;
;        db      F6,2,C7,2,A6,2,C7,2
;        db      E6,2,C7,2,A6,2,C7,2
;        db      D6,2,C7,2,A6,2,C7,2
;        db      C6,2,C7,2,A6,2,C7,2
;        db      F6,2,C7,2,A6,2,C7,2
;        db      E6,2,C7,2,A6,2,C7,2
;        db      D6,2,C7,2,A6,2,C7,2
;        db      C6,2,C7,2,A6,2,C7,2
;
;        db      E6,2,ASH6,2,G6,2,ASH6,2
;        db      D6,2,ASH6,2,G6,2,ASH6,2
;        db      C6,2,ASH6,2,G6,2,ASH6,2
;        db      ASH5,2,ASH6,2,G6,2,ASH6,2
;
;        db      D6,2,A6,2,F6,2,A6,2
;        db      C6,2,A6,2,F6,2,A6,2
;        db      ASH5,2,A6,2,F6,2,A6,2
;        db      A5,2,A6,2,F6,2,A6,2
;
;        db      F5,2,G5,2,A5,2,ASH5,2,C6,2,D6,2,E6,2,F6,2,G6,2,A6,2,C7,2,A6,2
;        db      G6,2,F6,2,E6,2,D6,2,C6,2,ASH5,2,A5,2,G5,2,F5,2,E5,2,F5,8,0FFH

;--------- end of musicmode

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
;dtmf_off       Ensures dtmf is turn off.
;
;USES           R(1)2,3,4
;               IIC
;
;CALLS          iic
;
;EXIT           dtmf_in_use
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
dtmf_off:
        mov     iicbuff,#dtmfo
        setb    rs0
        mov     r2,#01
        mov     r3,#00
        mov     r4,#dtmfadr
        call    iic
        inc     a
        jz      dtmf_off
        clr     dtmf_in_use
        clr     rs0
        ret

;---------- end of dtmf_off

;---------- end of DTMF Routines

