;*******************************************************************************
;*                                                                             *
;*                    OM5027 RS232 DEMONSTRATION MODULE                        *
;*                                                                             *
;*******************************************************************************
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
; THE 0M5027 MAY BE CONNECTED VIA THE RS232 PORT TO A PC RUNNING A TERMINAL   ;
; EMULATION PROGRAM AT 9600,8,1,N.                                            ;
;                                                                             ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;
;CONTENTS       uart_init                                                     ;
;               restart_uart                                                  ;
;               uart_isr                                                      ;
;                                                                             ;
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;

t1_reload       equ     255             ;for 9600 baud, f=3.579545MHz, smod=0
CR              equ     0Dh             ;carriage return
LF              equ     0Ah             ;line feed
EOT             equ     04h             ;end of text
BS              equ     08h             ;back space
ESC             equ     1bh             ;escape character
SP_             equ     20h             ;space bar

PB1             equ     00000001b       ;encoded push-buttons for keybits
PB2             equ     00000010b
PB3             equ     00000100b
PB4             equ     00001000b
PB5             equ     00010000b
PB6             equ     00100000b
PB7             equ     01000000b
PB8             equ     10000000b
maxmodule       equ     17              ;number of module creens
upcase          equ     11011111b       ;upper case mask

        bseg
newsec_request: dbit    1
rs232inactive:  dbit    1
infomode:       dbit    1
illegal_char:   dbit    1
bseg_end        set     $

        dseg
uartbuff:       ds      10
dseg_end        set     $

        cseg

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;uart_init      Initialises 80cl580 uart to 9600,8,1,n using timer 1,         ;
;               divide by 32, and a 3.579545MHz crystal. Initialises          ;
;               uart_isr variables.                                           ;
;                                                                             ;
;EXIT           th1             t1_reload                                     ;
;               tmod            8 bit auto-reload (t1)                        ;
;               sm1             set                                           ;
;               ren             set                                           ;
;               es0             set                                           ;
;               tr1             set                                           ;
;               newsec_request  set                                           ;
;               rs232inactive   set                                           ;
;               infomode        set                                           ;
;               Reg Bank 2                                                    ;
;                                                                             ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
uart_init:
;assume registers registers are in reset state
        mov     th1,#t1_reload
        mov     tl1,#t1_reload
        mov     a,tmod                  ;set timer 1 to auto-reload timer mode.
        anl     a,#0Fh
        orl     a,#00100000b            ;t1 mode: 8 bit auto-reload
        mov     tmod,a
        setb    sm1                     ;serial mode 1
        setb    ren                     ;enable serial reception
        setb    es0                     ;enable uart interrupt
        setb    tr1                     ;start timer 1
        clr     newsec_request
        setb    rs232inactive
        clr     infomode
        push    psw
        clr     rs0                     ;change to reg bank 2
        setb    rs1                     ;initialise reg bank 2 for rs232 demo
        mov     r0,#low(rs232intro)
        mov     r1,#high(rs232intro)
        mov     r2,#00                  ;rsmode = 0
        mov     r5,#00                  ;phase = 0
        mov     r6,#00
        pop     psw                     ;change back to previous reg bank
        ret

;---------- end of uart_init

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;restart_uart   If requested by the background process, this routine places   ;
;               a character into s0buf to cause an interrupt which passes     ;
;               control back to the background process.                       ;
;                                                                             ;
;ENTRY          newsec_request                                                ;
;               newsec                                                        ;
;                                                                             ;
;EXIT           sends a back space if a new second and rs232 active           ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
restart_uart:
        jnb     newsec_request,rutx
        jnb     newsec,rutx
        mov     s0buf,#08
        clr     newsec_request
rutx:   ret

;------- end of reset_uart

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;uart_isr       This routine coordinates the display of information on the    ;
;               terminal emulator, as well as coordinating the response to    ;
;               incoming character commands. The routine is invoked by        ;
;               interrupt upon which it processes any recieved key (if it     ;
;               was an ri interrupt), or sends the next character if it was   ;
;               a ti interrupt. A small buffer is used to store the current   ;
;               time, and time info is resent once per second (the forground  ;
;               re-initiates transmission).                                   ;
;                                                                             ;
;               There are 4 modes of operation with the present mode denoted  ;
;               by the value in RSMODE. Each mode has several phases of       ;
;               transmission with the current phase denoted by the value PHASE;
;                                                                             ;
;DISPLAY        RSMODE  PHASE           ACTION                                ;
;                                                                             ;
;MAIN MENU        0       0             send mode 0 "still" text              ;
;+ TIME                                 phase = 1                             ;
;                                                                             ;
;                         1             send time                             ;
;                                       phase = 2                             ;
;                                                                             ;
;                         2             If new RX then                        ;
;                                              If  0 < RX < 4 then            ;
;                                                  rsmode = RX                ;
;                                                  phase = 0                  ;
;                                                                             ;
;                                        Else                                 ;
;                                            If new second goto phase 1       ;
;                                                                             ;
;                                                                             ;
;                                                                             ;
;MODULE MENU      1       0             send mode 2 "still" text              ;
;+ TIME                                 phase = 1                             ;
;                                                                             ;
;                         1             send time                             ;
;                                       phase = 2                             ;
;                                                                             ;
;                         2             If new RX then                        ;
;                                          Case RX of                         ;
;                                             Esc   Phase = 0,rsmode = 0      ;
;                                             A     Phase = 0,dptr = #module0 ;
;                                             :       :     :  :        :     ;
;                                             P     Phase = 0,dptr = #module15;
;                                        Else                                 ;
;                                            If new second goto phase 1       ;
;                                                                             ;
;                                                                             ;
;REMOTE CONTROL   2       0             send mode 3 "still" text              ;
;MENU + TIME                            phase = 1                             ;
;                                                                             ;
;                         1             send time                             ;
;                                       phase = 2                             ;
;                                                                             ;
;                         2             If new RX then                        ;
;                                          Case RX of                         ;
;                                             Esc   Phase = 0, rsmode = 0     ;
;                                             1     keybits =  pb1            ;
;                                             2     keybits =  pb2            ;
;                                             3     keybits =  pb3            ;
;                                             4     keybits =  pb4            ;
;                                             5     keybits =  pb5            ;
;                                             6     keybits =  pb6            ;
;                                             7     keybits =  pb7            ;
;                                             8     keybits =  pb8            ;
;                                                                             ;
;                                                   keyvalid = true           ;
;                                        Else                                 ;
;                                            If new second goto phase 1       ;
;                                                                             ;
;PRODUCTION       3       0             send mode 1 "still" text              ;
;INFO + TIME                            phase = 1                             ;
;                                                                             ;
;                         1             send time                             ;
;                                       phase = 2                             ;
;                                                                             ;
;                         2             If new RX then                        ;
;                                              If RX = Esc then               ;
;                                                 Phase = 0                   ;
;                                                 rsmode = 0                  ;
;                                        Else                                 ;
;                                            If new second goto phase 1       ;
;                                                                             ;
;                                                                             ;
;ENTRY          invoked by ri and ti interrupt                                ;
;               rs232inactive                                                 ;
;                                                                             ;
;                                                                             ;
;USES           reg bank 2 is reserved for rs232 routines                     ;
;               r(2)0           dpl, and buffer data pointer                  ;
;               r(2)1           dph                                           ;
;               r(2)2           rsmode; determines current display            ;
;               r(2)5           phase; determines phase of transmission       ;
;               r(2)6           current screen (when displaying module info)  ;
;               uartbuff                                                      ;
;               dptr                                                          ;
;               psw                                                           ;
;               acc                                                           ;
;               infomode                                                      ;
;               illegal_char                                                  ;
;                                                                             ;
;STACK          4 bytes + 2 for local calls + 2 for the INT call              ;
;                                                                             ;
;CALLS          split1                                                        ;
;               loadbuff                                                      ;
;               processRx0                                                    ;
;               processRx1                                                    ;
;               processRx2                                                    ;
;               processRx3                                                    ;
;                                                                             ;
;                                                                             ;
;EXIT           newsec_request  set when waiting for restart from the forgrnd ;
;               rs232inactive   set when transmission initiated               ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
uart_isr:
;store forground status
        push    psw
        push    acc
        push    dph
        push    dpl
        clr     rs0
        setb    rs1                     ;use register bank 2
        clr     ren

        jbc     rs232inactive,uir1
        jb      ri,uir15

;if phase 1 then use @r0 to point into the uart buffer
uir1:   cjne    r5,#01,uir8
        mov     a,@r0                   ;get next char to send
        sjmp    uir11

;if phase 0 or 2 use dptr to point into code embeded data
uir8:   mov     dph,r1
        mov     dpl,r0
        clr     a
        movc    a,@a+dptr               ;get next char to send

;if EOT then end of current phase - so prepare data for next phase
uir11:  cjne    a,#EOT,uir2

        cjne    r5,#00,uir13            ;if was phase 0 then
        call    loadbuff                ;prepare data buffer for phase 1
        inc     r5
        sjmp    uir1

uir13:  cjne    r5,#01,uir14            ;if was phase 1 then
        mov     r0,#low(backSp)         ;prepare data buffer for phase 2
        mov     r1,#high(backSp)
        inc     r5
        setb    newsec_request
        clr     ti
        setb    ren                     ;re-enable rx
        sjmp    uir15

uir14:  cjne    r5,#02,uirx             ;if was phase 2 then
        mov     r5,#01                  ;prepare data for phase 1 again
        call    loadbuff                ;ie. prepare to resend time
        sjmp    uir1                    ;go and resend time

;send character and increment pointer to next char
uir2:   clr     ti
        mov     s0buf,a
        inc     r0
        mov     a,r0
        jnz     uirx
        inc     r1
        sjmp    uirx

;process data if data recieved
uir15:  jnb     ri,uirx
        clr     newsec_request
        clr     ri
        clr     ren
        clr     illegal_char
        mov     a,r2
        jnz     uir16
        call    processRx0
        sjmp    uir20
uir16:  djnz    acc,uir17
        call    processRx1
        sjmp    uir20
uir17:  djnz    acc,uir18
        call    processRx2
        sjmp    uir20
uir18:  djnz    acc,uirx
        call    processRx3

uir20:  jnb     illegal_char,uir8
        setb    ren
        clr     illegal_char
        setb    newsec_request
        sjmp    uirx

;restore forground and exit
uirx:   pop     dpl
        pop     dph
        pop     acc
        pop     psw
        reti

;subroutine to update uartbuff
loadbuff:
        mov     r0,#clk2
        mov     r1,#uartbuff+8
        mov     @r1,#EOT        ;sentinal
        dec     r1
        call    split1          ;seconds
        mov     @r1,#':'
        dec     r1
        call    split1          ;minutes
        mov     @r1,#':'
        dec     r1
        call    split1          ;hours
        mov     r0,#uartbuff
        ret

;subroutine to process rsmode 0 RX data
processRx0:
        mov     a,s0buf
        cjne    a,#ESC,pr00             ;test for escape request
        mov     r5,#00                  ;set phase to 0
        mov     r2,#00                  ;reset rsmode
        mov     r0,#low(rs232intro)
        mov     r1,#high(rs232intro)
        sjmp    pr0x
pr00:   subb    a,#30h
        anl     a,#11111100b            ;test legal data between 1 and 3
        jnz     pr0x1
        mov     r5,#00                  ;set phase to 0
        mov     a,s0buf
        subb    a,#30h
        mov     r2,a                    ;set new rsmode
        rl      a                       ;now set new dptr
        add     a,#pr02-pr01
        movc    a,@a+pc
pr01:   mov     r1,a
        mov     a,r2
        rl      a
        add     a,#pr02-pr03
        inc     a
        movc    a,@a+pc
pr03:   mov     r0,a
pr0x:   ret

pr02:   dw      rs232intro
        dw      RSmoduleinfo
        dw      remotecontrol
        dw      productinfo

pr0x1:  setb    illegal_char
        ret

;subroutine to process rsmode 1 RX data
processRx1:
        mov     a,s0buf
        cjne    a,#ESC,pr10             ;test for escape request
        mov     r5,#00                  ;set phase to 0
        jbc     infomode,pr14
        mov     r2,#00                  ;reset rsmode
        mov     r0,#low(rs232intro)
        mov     r1,#high(rs232intro)
        clr     infomode
        sjmp    pr1x

pr14:   mov     r0,#low(rsmoduleinfo)
        mov     r1,#high(rsmoduleinfo)
        sjmp    pr1x

pr10:   cjne    a,#SP_,pr15            ;test for space (next screen request)
        mov     r5,#00                 ;set phase to 0
        inc     r6                     ;next module screen
        mov     a,r6
        cjne    a,#maxmodule,$+3
        jc      pr16
        clr     a
        sjmp    pr16

pr15:   anl     a,#upcase
        subb    a,#41h
        cjne    a,#maxmodule,$+3
        jnc     pr1x1
pr16:   setb    infomode
        mov     r6,a
        rl      a                       ;now set new dptr
        add     a,#pr12-pr11
        movc    a,@a+pc
pr11:   mov     r1,a
        mov     a,r6
        rl      a
        add     a,#pr12-pr13
        inc     a
        movc    a,@a+pc
pr13:   mov     r0,a
pr1x:   ret

pr12:   dw      A_
        dw      B_
        dw      C_
        dw      D_
        dw      E_
        dw      F_
        dw      G_
        dw      H_
        dw      I_
        dw      J_
        dw      K_
        dw      L_
        dw      M_
        dw      N_
        dw      O_
        dw      P_
        dw      Q_

pr1x1:  setb    illegal_char
        ret

;subroutine to process rsmode 2 RX data
processRx2:
        mov     a,s0buf
        cjne    a,#ESC,pr20             ;test for escape request
        mov     r5,#00                  ;set phase to 0
        mov     r2,#00                  ;reset rsmode
        mov     r0,#low(rs232intro)
        mov     r1,#high(rs232intro)
        sjmp    pr2x

pr20:   cjne    a,#'1',pr21
        mov     keybits,#pb1
        sjmp    pr2c
pr21:   cjne    a,#'2',pr22
        mov     keybits,#pb2
        sjmp    pr2c
pr22:   cjne    a,#'3',pr23
        mov     keybits,#pb3
        sjmp    pr2c
pr23:   cjne    a,#'4',pr24
        mov     keybits,#pb4
        sjmp    pr2c
pr24:   cjne    a,#'5',pr25
        mov     keybits,#pb5
        sjmp    pr2c
pr25:   cjne    a,#'6',pr26
        mov     keybits,#pb6
        sjmp    pr2c
pr26:   cjne    a,#'7',pr27
        mov     keybits,#pb7
        sjmp    pr2c
pr27:   cjne    a,#'8',pr28
        mov     keybits,#pb8
        sjmp    pr2c
pr28:   anl     a,#Upcase
        cjne    a,#'Q',pr29
        mov     keybits,#pb5
        sjmp    pr2c
pr29:   cjne    a,#'W',pr2A
        mov     keybits,#pb6
        sjmp    pr2c
pr2a:   cjne    a,#'E',pr2B
        mov     keybits,#pb7
        sjmp    pr2c
pr2b:   cjne    a,#'R',pr2x1
        mov     keybits,#pb8

pr2c:   setb    keyvalid
pr2x1:  setb    illegal_char
pr2x:   ret

;subroutine to process rsmode 3 RX data
processRx3:
        mov     a,s0buf
        cjne    a,#ESC,pr3x1            ;test for escape request
        mov     r5,#00                  ;set phase to 0
        mov     r2,#00                  ;reset rsmode
        mov     r0,#low(rs232intro)
        mov     r1,#high(rs232intro)
pr3x:   ret

pr3x1:  setb    illegal_char
        ret


rs232intro:
DB  CR,LF,LF,LF,LF,LF,LF
DB  '********************************************************************',CR,LF
DB  '*                                                                  *',CR,LF
DB  '*                 OM5027 RS232 PORT DEMONSTRATION                  *',CR,LF
DB  '*                                                                  *',CR,LF
DB  '********************************************************************',CR,LF,LF

DB  'This RS232 demostration has 3 modes of operation which are listed',CR,LF
DB  'below. They verify the full duplex RS232 serial communication ',CR,LF
DB  'capability of the 80CL580 on-board UART together with the RS232',CR,LF
DB  'interface module. For correct operation your terminal should not',CR,LF
DB  'echo your key strokes.',CR,LF

DB  'Type the number corresponding to your choice:',CR,LF,LF

DB  'MAIN MENU ',CR,LF
DB  '--------------------------------------------------------------------',CR,LF,LF

DB  '1. OM5027 Module Information',CR,LF,LF

DB  '2. OM5027 Remote Control',CR,LF,LF

DB  '3. OM5027 Product Information',CR,LF,LF

DB  '--------------------------------------------------------------------',CR,LF
DB  'Time: ',EOT


rsmoduleinfo:
DB  CR,LF,LF,LF,LF,LF,LF
DB  '********************************************************************',CR,LF
DB  '*                                                                  *',CR,LF
DB  '*                    OM5027 MODULE INFORMATION                     *',CR,LF
DB  '*                                                                  *',CR,LF
DB  '********************************************************************',CR,LF,LF

DB  'This function displays information on the various OM5027 evaluation',CR,LF
DB  'board modules.',CR,LF,LF
DB  'Select a module from the list below by pressing the letter key ',CR,LF
DB  'corresponding to your choice, or...',CR,LF
DB  '              PRESS <ESC> TO RETURN TO THE MAIN MENU.',CR,LF

DB  '--------------------------------------------------------------------',CR,LF
DB  'A. 80CL580 Microcontroller Module     J. RAM Module',CR,LF
DB  'B. Clock Module                       K. IIC Bus Extender Module',CR,LF
DB  'C. Keypad I/O Module                  L. IIC to Parallel Bus Module',CR,LF
DB  'D. Starburst and 7-Segment LCD Module M. RS232 Serial Port Module',CR,LF
DB  'E. Dot-Matrix LCD Module              N. 87C51 Socket Module',CR,LF
DB  'F. DTMF Module                        O. Voltage Regulator Module',CR,LF
DB  'G. ADC and DAC Module                 P. Introduction to IIC Bus',CR,LF
DB  'H. Digital I/O Expander Module        Q. Switch & Jumper Settings.',CR,LF
DB  'I. EEPROM Module',CR,LF
DB  '--------------------------------------------------------------------',CR,LF
DB  'Time: ',EOT

remotecontrol:
DB  CR,LF,LF,LF,LF,LF,LF
DB  '********************************************************************',CR,LF
DB  '*                                                                  *',CR,LF
DB  '*                      OM5027 REMOTE CONTROL                       *',CR,LF
DB  '*                                                                  *',CR,LF
DB  '********************************************************************',CR,LF,LF

DB  'Remote control of the OM5027 evaluation board is now possible via ',CR,LF
DB  'this terminal by using the following key substitutions.',CR,LF,LF

DB  'The terminal key positioning is analogous to the the OM5027 keypad.',CR,LF
DB  '       <1>   <2>   <3>   <4>    <PB1> <PB2> <PB3> <PB4>',CR,LF
DB  '       <Q>   <W>   <E>   <R>    <PB5> <PB6> <PB7> <PB8>',CR,LF

DB  'Terminal Keyboard      OM5027 Keypad',CR,LF
DB  '--------------------------------------------------------------------',CR,LF
DB  '<1>                =     PB1             <Esc> = Return to main menu',CR,LF
DB  '<2>                =     PB2',CR,LF
DB  '<3>                =     PB3',CR,LF
DB  '<4>                =     PB4',CR,LF
DB  '<5> or <Q>         =     PB5',CR,LF
DB  '<6> or <W>         =     PB6',CR,LF
DB  '<7> or <E>         =     PB7',CR,LF
DB  '<8> or <R>         =     PB8',CR,LF
DB  '--------------------------------------------------------------------',CR,LF

DB  'Time: ',EOT


productinfo:
DB  CR,LF,LF,LF,LF,LF,LF
DB  '********************************************************************',CR,LF
DB  '*                                                                  *',CR,LF
DB  '*                   OM5027 PRODUCTION INFORMATION                  *',CR,LF
DB  '*                                                                  *',CR,LF
DB  '********************************************************************',CR,LF,LF

DB  '                OM5027 Low Voltage, Low Power',CR,LF
DB  '                   I2C Bus Evaluation Board',CR,LF
DB  '              Demonstration Software Revision '
                                revision
DB  CR,LF,LF,LF

DB  '                    Philips Semiconductors',CR,LF
DB  '                     Zurich, Switzerland.',CR,LF,LF
DB  '                     Made in New Zealand.',CR,LF,LF,LF,LF

DB  '              PRESS <ESC> TO RETURN TO MAIN MENU',CR,LF,LF,LF,LF
DB  '--------------------------------------------------------------------',CR,LF

DB  'Time: ',EOT


backSp: db      BS,BS,BS,BS,BS,BS,BS,EOT     ;first BS sent from forground

;-------------- end of uart routines

A_:
DB  CR,LF,LF,LF,LF
DB  'A)   80CL580 CONTROL MICROCONTROLLER',CR,LF,LF

DB  'The 83CL580 is an 8-bit general purpose microcontroller based on the 8051',CR,LF
DB  'architecture and manufactured in advanced CMOS. It is suited for use in',CR,LF
DB  'real-time applications such as cordless telephones and mobile communications.',CR,LF
DB  'The device has low power consumption and a supply voltage of 2.5-6.0 volts.',CR,LF
DB  'It has two software selectable modes of reduced activity for further power',CR,LF
DB  'reduction: Idle and Power-down. The 83CL580 also functions as an arithmatic',CR,LF
DB  'processor having facilities for both binary and BCD arithmetic plus bit-',CR,LF
DB  'handling capabilities. The instruction set consists of over 100 instructions.',CR,LF
DB  'The 83CL580 has 6K bytes of internal ROM (expandable to 64K externally) plus',CR,LF
DB  '256 bytes of internal RAM (expandable to 64K externally). It has 40 I/0 lines,',CR,LF
DB  '3 16-bit timer/counters, a 15 source 2 priority level interrupt structure,',CR,LF
DB  'a full duplex serial uart, an IIC bus serial interface, an 8-bit',CR,LF
DB  '4-channel A/D, 1 8-bit PWM, a watchdog timer, and operates from 0 to 12MHz.',CR,LF
DB  LF
DB  'The OM5027 utilises an externally expanded 80CL580 with a low voltage EPROM.',CR,LF
DB  'AD0 is used to measure the supply rail voltage which gives the "feed forward"',CR,LF
DB  'necessary to regulate the LCD contrast via the PWM output. AD1 is used to',CR,LF
DB  'measure the ambient temperature. All remaining ports are brought out to',CR,LF
DB  'headers with the IIC interface used to control a number of peripherals.',CR,LF,LF

DB  '   PRESS <ESC> TO RETURN TO MENU   or   <SPACE BAR> FOR NEXT MODULE',CR,LF
DB  'Time: ',EOT

B_:
DB  CR,LF,LF,LF,LF
DB  'B)   LOW POWER CLOCK CALENDAR MODULE',CR,LF,LF

DB  'The PCF8593 is a low power CMOS Real-time Clock / Calendar optimised for low',CR,LF
DB  'power consumption. Addresses and data are transferred serially via ',CR,LF
DB  'the IIC bus. The built-in word address register is incremented',CR,LF
DB  'automatically after each written or read data byte. The built-in 32.768 kHz',CR,LF
DB  'oscillator circuit and the first 8 registers are used for the clock/calendar',CR,LF
DB  'and counter functions. The next 8 registers may be programmed as alarm',CR,LF
DB  'registers or used as free RAM space. Alarm modes include daily alarm, dated',CR,LF
DB  'alarm, weekday alarm and timer alarm. The PCF8593 may be placed in the Event',CR,LF
DB  'Counter Mode in which it counts externally pulses applied to the OSC input.',CR,LF
DB  'The event counter stores up to 6 digits of data, stored as hexidecimal values,',CR,LF
DB  'therefore storing up to 1 million events. An event counter alarm occurs when',CR,LF
DB  'the event counter registers match a pre-programmed value, and may be used to',CR,LF
DB  'generate an external interrupt signal. The operating supply voltage is',CR,LF
DB  '1.0-6.0 volts however the IIC interface requires a minimum of 2.5 volts.',CR,LF,LF,LF,LF

DB  '   PRESS <ESC> TO RETURN TO MENU   or   <SPACE BAR> FOR NEXT MODULE',CR,LF,LF,LF,LF
DB  'Time: ',EOT

C_:
DB  CR,LF,LF,LF,LF,LF
DB  'C)   KEYPAD I/O MODULE',CR,LF,LF

DB  'The PCF8574 is a single chip silicon gate CMOS circuit. It provides I/O',CR,LF
DB  'expansion for microcontrollers via the IIC bus.  This slave device consists',CR,LF
DB  'of an 8 bit quasi-bidirectional port and an IIC interface. The PCF8574 has',CR,LF
DB  'low current consumption and includes latched outputs with high current drive',CR,LF
DB  'capability for directly driving LEDs. The supply voltage range is 2.5-6.0 V.',CR,LF
DB  'It also possesses an open drain interrupt output which may be used to signal',CR,LF
DB  'the host microcontroller when incoming data has changed. The PCF8574 and 74A',CR,LF
DB  'differ only by their address.',CR,LF,LF

DB  'The OM5027 utilises the PCF8574A as a keyboard interface. The 8 push buttons',CR,LF
DB  'are multiplexed into 4 I/O lines by 2 control lines, therefore using a total',CR,LF
DB  'of 6 I/O lines. The remaining 2 I/O lines are used to drive the 2 LEDs.',CR,LF,LF,LF
DB  LF,LF,LF
DB  '   PRESS <ESC> TO RETURN TO MENU   or   <SPACE BAR> FOR NEXT MODULE',CR,LF,LF,LF,LF
DB  'Time: ',EOT

D_:
DB  CR,LF,LF,LF,LF,LF
DB  'D)    STARBURST AND SEVEN SEGMENT DISPLAY MODULE',CR,LF,LF

DB  'The LPH3802-1 is a Twisted Nematic Liquid Crystal Display module with',CR,LF
DB  'integrated driver in Chip-On-Glass Technology. The display consists of 10',CR,LF
DB  'Starburst and 10 7-Segment characters, plus several icons. The transflective',CR,LF
DB  'positive image has an optimum viewing angle of 270 degrees. The integrated',CR,LF
DB  'driver is a PCF8578 and provides the required 1:8 multiplexing. Display data',CR,LF
DB  'is transferred to the module via a partial IIC-bus which is in all aspects',CR,LF
DB  'the same as IIC except there is no acknowledge bit from the slave. The supply',CR,LF
DB  'voltage range is 2.5-6.0 volts.',CR,LF,LF

DB  'The OM5027 varies the contrast via the 80CL580 PWM output and an inverting',CR,LF
DB  'dc-dc converter. This voltage (Vlcd) is also compensated by the 80CL580 to',CR,LF
DB  'maintain LCD contrast in case of supply voltage fluctuation.',CR,LF,LF,LF,LF,LF,LF

DB  '   PRESS <ESC> TO RETURN TO MENU   or   <SPACE BAR> FOR NEXT MODULE',CR,LF,LF,LF,LF
DB  'Time: ',EOT


E_:
DB  CR,LF,LF,LF,LF,LF,LF,LF
DB  'E)   DOT-MATRIX LCD MODULE',CR,LF,LF

DB  'The LPH3827-1 is a low power Super Twisted Nematic, 12 character x 3 line',CR,LF
DB  '5 x 7 dot matrix LCD module, with integrated driver in Chip-On-Glass',CR,LF
DB  'Technology. The transflective positive image has an optimum viewing',CR,LF
DB  'angle of 270 degrees. The integrated driver is a PCF2116 which provides ',CR,LF
DB  'the required 1:32 multiplexing and contains a character generator with',CR,LF
DB  '240 fixed and 16 write by program characters. The module interfaces via',CR,LF
DB  'the IIC bus. The supply voltage range is 2.5-6.0 volts.',CR,LF
DB  LF,LF,LF,LF,LF,LF,LF,LF,LF,LF

DB  '   PRESS <ESC> TO RETURN TO MENU   or   <SPACE BAR> FOR NEXT MODULE',CR,LF,LF,LF,LF
DB  'Time: ',EOT

F_:
DB  CR,LF,LF,LF,LF,LF
DB  'F)   DTMF MODULE',CR,LF,LF

DB  'The PCD3312 is a single chip silicon gate CMOS integrated circuit. It is',CR,LF
DB  'intended to provide dual-tone multi-frequency (DTMF) combinations required',CR,LF
DB  'for tone dialing systems in telephone sets which contain a microcontroller',CR,LF
DB  'for the control functions. The various audio output frequencies are generated',CR,LF
DB  'from an on-chip 3.58 MHz quartz crystal control oscillator. With its on-chip',CR,LF
DB  'voltage reference the PCD3312 provides constant output amplitudes which are',CR,LF
DB  'independent of the operating supply voltage and ambient temperature. An',CR,LF
DB  'on-chip filtering system ensures a very low total harmonic distortion in',CR,LF
DB  'accordance with the CEPT T/CS46-03 recommendations. In addition to the',CR,LF
DB  'standard frequencies the device provides 12 modem frequencies (300 to',CR,LF
DB  '1200 bits per second) used in simplex modem applications, and two octaves',CR,LF
DB  'of musical tones in steps of semitones. The supply voltage is 2.5-6.0 V.',CR,LF
DB  LF
DB  'The OM5027 utilises the PCD3312 as a single tone generator for Audio Alarm',CR,LF
DB  'Chimes and for generating musical melodies. The PCD3312 output signal is',CR,LF
DB  'amplified by a TDA7050 (140 mW Low Voltage Mono/Stereo Audio Amplifier)',CR,LF
DB  'connected in mono bridge-tied load mode and driving a piezo buzzer.',CR,LF,LF

DB  '   PRESS <ESC> TO RETURN TO MENU   or   <SPACE BAR> FOR NEXT MODULE',CR,LF,LF

DB  'Time: ',EOT

G_:
DB  CR,LF,LF,LF,LF
DB  'G)   ADC AND DAC MODULE',CR,LF,LF

DB  'The PCF8591 is a single chip, single supply low power 8-bit CMOS data',CR,LF
DB  'acquisition device with four analogue inputs, one analogue output and a',CR,LF
DB  'serial IIC bus interface. Three address pins are used for programming the',CR,LF
DB  'hardware address, allowing the use of up to 8 devices connected without',CR,LF
DB  'additional hardware. Address control and data to and from the device are',CR,LF
DB  'transferred serially via the IIC bus. The supply voltage range is 2.5-6 V.',CR,LF
DB  LF
DB  'The functions of the device include analogue input multiplexing, on-chip',CR,LF
DB  'track and hold function, 8-bit analogue-to-digital conversion and an',CR,LF
DB  '8-bit digital-to-analogue conversion. The maximum conversion rate is',CR,LF
DB  'given by the maximum speed of the IIC bus. Possible applications include',CR,LF
DB  'closed loop control systems, low power converter for remote data',CR,LF
DB  'aquisition, battery operated equipment, acquisition of analogue values in',CR,LF
DB  'automotive, audio and TV applications.',CR,LF
DB  LF
DB  'The OM5027 exercises PCF8591 functions by continuously incrementing the DAC',CR,LF
DB  'output. This saw-tooth waveform is aquired by an adjacent ADC input and',CR,LF
DB  'read back to the control microcontroller for display.',CR,LF,LF

DB  '   PRESS <ESC> TO RETURN TO MENU   or   <SPACE BAR> FOR NEXT MODULE',CR,LF,LF
DB  'Time: ',EOT

H_:
DB  CR,LF,LF,LF,LF,LF,LF

DB  'H)   DIGITAL I/O EXPANDER',CR,LF,LF

DB  'The PCF8574 is a single chip silicon gate CMOS circuit. It provides I/O',CR,LF
DB  'expansion for microcontrollers via the IIC bus.  This slave device consists',CR,LF
DB  'of an 8 bit quasi-bidirectional port and an IIC interface. The PCF8574 has',CR,LF
DB  'low current consumption and includes latched outputs with high current drive',CR,LF
DB  'capability for directly driving LEDs. The supply voltage range is 2.5-6.0 V.',CR,LF
DB  'It also possesses an open drain interrupt output which may be used to signal',CR,LF
DB  'the host microcontroller when incoming data has changed. The PCF8574 and 74A',CR,LF
DB  'differ only by their address.',CR,LF,LF

DB  'The OM5027 software utilises the PCF8574A as a keyboard interface.',CR,LF
DB  'The other PCF8574 I/O module is fully functional and available for user',CR,LF
DB  'evaluation and prototyping.',CR,LF
DB  LF,LF,LF,LF,LF

DB  '   PRESS <ESC> TO RETURN TO MENU   or   <SPACE BAR> FOR NEXT MODULE',CR,LF,LF,LF,LF
DB  'Time: ',EOT


I_:
DB  CR,LF,LF,LF,LF,LF,LF
DB  'I)    128 x 8-BIT EEPROM MODULE',CR,LF,LF

DB  'The PCF8581C is a low powered CMOS EEPROM with a wide operating voltage of 2.5',CR,LF
DB  'to 6.0 V. The PCF8581 is organised as 128 words by 8 bits. Addresses and data',CR,LF
DB  'are transferred serially via IIC bus. The built-in word address register is',CR,LF
DB  'incremented automatically after each written or read data byte. All bytes can',CR,LF
DB  'be read in a single operation. Up to 8 bytes can be written in 1 operation,',CR,LF
DB  'reducing the total write time per byte. Three address pins are used to define',CR,LF
DB  'the hardware address, allowing the use of up to 8 devices connected to the',CR,LF
DB  'bus without additional hardware. No external components are required. There',CR,LF
DB  'is an automatic erase before write. The chip is designed for at least 10,000',CR,LF
DB  'write cycles per byte, an infinite number of read cycles, and 10 years minimum',CR,LF
DB  'non-volatile data retention.',CR,LF,LF

DB  'The OM5027 EEPROM module is not exercised by the demonstration software so',CR,LF
DB  'as to preserve the device for user evaluation or prototyping. Note that source',CR,LF
DB  'code for a software driver is provided on disk (Metalink Assembler).',CR,LF,LF,LF,LF

DB  '   PRESS <ESC> TO RETURN TO MENU   or   <SPACE BAR> FOR NEXT MODULE',CR,LF,LF,LF
DB  'Time: ',EOT


J_:
DB  CR,LF,LF,LF,LF,LF
DB  'J)    256 x 8-BIT STATIC RAM MODULE',CR,LF,LF

DB  'The PCF8570 is a low powered static CMOS RAM organised into 256 words by',CR,LF
DB  '8-bits. Addresses and data are transferred serially via the IIC bus.',CR,LF
DB  'The built-in word address register is incremented automatically after ',CR,LF
DB  'each written or read data byte. Three address pins are used for',CR,LF
DB  'address, allowing the use of up to 8 devices connected to the bus',CR,LF
DB  'without additional hardware. The PCF8570C has an alternative slave address',CR,LF
DB  'for memory extension up to 16 devices when used in conjunction with the',CR,LF
DB  'PCF8570. No external components are required. The operating supply voltage',CR,LF
DB  'is 2.5 - 6.0 volts with data retension down to 1.0 volts.',CR,LF,LF

DB  'Possible applications include:',CR,LF
DB  'Telephony (RAM expansion for stored numbers in repertory dialing),',CR,LF
DB  'Radio, Television and VCR (channel presets),',CR,LF
DB  'General Purpose (RAM expansion for microcontrollers with IIC interfaces).',CR,LF,LF

DB  'The OM5027 demonstration software does not exercise this module however',CR,LF
DB  'it is fully functional for user evaluation and prototyping.',CR,LF,LF

DB  '   PRESS <ESC> TO RETURN TO MENU   or   <SPACE BAR> FOR NEXT MODULE',CR,LF,LF,LF
DB  'Time: ',EOT


K_:
DB  CR,LF,LF,LF,LF,LF
DB  'K)    IIC BUS EXTENDER',CR,LF,LF

DB  'The 82B715 is a bipolar integrated circuit intended for application in',CR,LF
DB  'IIC bus systems. While retaining all the operating modes and features of',CR,LF
DB  'the IIC bus system, it permits extension of the practical separation',CR,LF
DB  'distance between the components on the IIC bus by buffering both the data',CR,LF
DB  '(SDA) and the clock (SCL) lines. The nominal bus capacitance limit of 400pF',CR,LF
DB  'restricts practical communications distances to a few meters. Using one',CR,LF
DB  '82B715 at each end of longer cables reduces the cable loading capacitance',CR,LF
DB  'on the IIC bus by a factor of 10 times and may allow the use of low cost',CR,LF
DB  'general purpose wiring to extend the bus lengths (4000 pF).',CR,LF,lf

DB  'Thus if connecting two OM5027 boards together via the bus extenders, assuming',CR,LF
DB  'the total local bus capacity of each isolated board is 120pF, then the maximum',CR,LF
DB  'permissible capacitance of cable connecting the extenders would be:',CR,LF
DB  'C cable (max) = 4000pF - (2 x 10 x 120pF) = 1.6nF.',CR,LF
DB  'Remove a pair of 330 ohm jumpers from one of the boards (only) so as not to',CR,LF
DB  'exceed the IIC current specifications of 3mA (30mA on the high power side).',CR,LF
DB  'The 82B715 requires a minimum voltage of 4.5 volts. When operating',CR,LF
DB  'below this voltage ensure the bus extender is switched OFF the bus to avoid',CR,LF
DB  'bus corruption.',CR,LF

DB  '   PRESS <ESC> TO RETURN TO MENU   or   <SPACE BAR> FOR NEXT MODULE',CR,LF,LF
DB  'Time: ',EOT


L_:
DB  CR,LF,LF,LF,LF,LF,LF
DB  'L)     IIC TO PARALLEL BUS MODULE',CR,LF,LF

DB  'The PCF8584 is an integrated circuit designed in CMOS technology which',CR,LF
DB  'serves as an interface between most standard parallel-bus microcontrollers',CR,LF
DB  'or processors and the serial IIC bus. The PCF8584 provides both master and',CR,LF
DB  'slave functions. Communication with the IIC-bus is carried out on a',CR,LF
DB  'byte-wise basis using interrupt or polled handshake. It controls all the',CR,LF
DB  'IIC-bus specific sequencing, protocol, arbitration and timing. The PCF8584',CR,LF
DB  'allows parallel-bus sytems to communicate bi-directionally with the IIC-bus.',CR,LF
DB  LF
DB  'An external clock must be applied to the clock pin in order to reset the',CR,LF
DB  'PCF8584, therefore the module must be switched OFF the evaluation board',CR,LF
DB  'unless an external system is connected. The I.C requires a Vdd of 4.5-5.5 V',CR,LF
DB  'and should be removed from the bus during low voltage demonstrations.',CR,LF

DB  LF,LF,LF,LF,LF

DB  '   PRESS <ESC> TO RETURN TO MENU   OR   <SPACE BAR> FOR NEXT MODULE',CR,LF,LF,LF,LF
DB  'Time: ',EOT



M_:
DB  CR,LF,LF,LF,LF,LF,LF
DB  'M)    RS232 SERIAL PORT MODULE',CR,LF,LF

DB  'A MAX232 is provided to allow RS232 communications between the 80CL580,',CR,LF
DB  'or the 8051 socket, and an external P.C or terminal unit. The MAX232',CR,LF
DB  'drives the lines with the voltage level and polarity specified in the',CR,LF
DB  'RS232 physical protocol. The handshaking lines RTS and CTS are not used',CR,LF
DB  'and should be disconnected when running this RS232 demonstration.',CR,LF
DB  'The MAX232 requires a supply voltage of 4.5-5.5 volts and should be',CR,LF
DB  'removed from the bus during low voltage demonstrations.',CR,LF

DB  LF,LF,LF,LF,LF,LF,LF,LF,LF,LF

DB  '   PRESS <ESC> TO RETURN TO MENU   or   <SPACE BAR> FOR NEXT MODULE',CR,LF,LF,LF,LF
DB  'Time: ',EOT



N_:
DB  CR,LF,LF,LF,LF,LF
DB  'N)    87C51 FAMILY MICROCONTROLLER SOCKET',CR,LF,LF

DB  'The 40-pin DIL socket is provided for insertion of suitable 87CL51 derivatives',CR,LF
DB  'or emulator footprints. Suitable microcontrollers or emulators must be pin ',CR,LF
DB  'compatible with an 87CL51 (DIL 40) as the associated pins are connected to',CR,LF
DB  'the IIC bus, RS232 interface, Vdd/Gnd, and crystal socket. All other pins ',CR,LF
DB  'are brought to headers.',CR,LF,LF,LF
DB  LF,LF,LF,LF,LF,LF,LF,LF,LF,LF

DB  '   PRESS <ESC> TO RETURN TO MENU   or   <SPACE BAR> FOR NEXT MODULE',CR,LF,LF,LF,LF
DB  'Time: ',EOT



O_:
DB  CR,LF,LF,LF,LF,LF,LF,LF,LF,LF
DB  'O)    VOLTAGE REGULATOR MODULE',CR,LF,LF

DB  'The LM317L is configured to allow a 2.5 to 5.2 V Vdd supply which ',CR,LF
DB  'powers all evaluation board components. The supply may be varied between',CR,LF
DB  'these bounds using the VDD ADJUST potentiometer. The actual voltage may',CR,LF
DB  'be displayed on the Dot-Matrix LCD module along with the LCD contrast control',CR,LF
DB  'variables (PWM count etc.). The Voltage Regulator Module may be connected ',CR,LF
DB  'to a "Plug Pack" or similar power supply of between 7 and 12 volts.',CR,LF,LF
DB  LF,LF,LF,LF,LF,LF,LF,LF,LF,LF


DB  '   PRESS <ESC> TO RETURN TO MENU   or   <SPACE BAR> FOR NEXT MODULE',CR,LF,LF,LF,LF
DB  'Time: ',EOT

P_:
DB  CR,LF,LF,LF,LF,LF,LF
DB  'P)   INTRODUCTION TO THE IIC-BUS',CR,LF,LF

DB  'The Inter-I.C (IIC) Bus is a standard bi-directional two wire bus carrying data',CR,LF
DB  'and clock signals at speeds of up to 100 Kbits per second. It is intended for',CR,LF
DB  'chip to chip and board to board communication with control, address and data',CR,LF
DB  'information carried serially over the bus. It allows for bi-directional data',CR,LF
DB  'transfer without any extra lines, and can be a multi-master system with',CR,LF
DB  'automatic bus arbitration. Each I.C has a unique 7-bit address eliminating the',CR,LF
DB  'need for separate address or selection lines. Only the I.C addressed generates',CR,LF
DB  'an acknowledge signal, allowing for extension of the bus at any time without',CR,LF
DB  'new software. Every byte transferred is acknowledged by the receiver. The',CR,LF
DB  'protocol is standard for a wide range of speeds, functions and applications.',CR,LF,LF

DB  'The IIC bus may be extended to FAST-MODE with an operating speed of up to',CR,LF
DB  '400 Kbits per second and 10-bit addressing with specified devices.',CR,LF,LF

DB  'To reduce power consumption on the OM5027, the 80CL580 is run at the',CR,LF
DB  'reduced frequency of 3.58 MHz. Therefore the IIC bus clock line (SCL) has a',CR,LF
DB  'corresponding maximum frequency of 60 KHz.',CR,LF,LF,LF

DB  '   PRESS <ESC> TO RETURN TO MENU   or   <SPACE BAR> FOR NEXT MODULE',CR,LF,LF

DB  'Time: ',EOT

Q_:
DB  CR,LF,LF,LF,LF,LF,LF
DB  'Q)    SWITCH AND JUMPER SETTINGS',CR,LF,LF

DB  'DIP Switch Assignments are as follows:',CR,LF,LF

DB  'POWER AND IIC DIP SWITCHES',CR,LF,LF

DB  '        Switch number:        1       2       3       4',CR,LF
DB  '        Function:            SDA     SCL     VDD     GND',CR,LF,LF


DB  'RS232 DIP SWITCHES',CR,LF,LF

DB  '        Switch number:        1       2       3       4',CR,LF
DB  '        Function:            CTS     RTS     TXD     RXD  ',CR,LF,LF


DB  'During normal operation of the OM5027 demonstration software all DIP switches',CR,LF
DB  'should be turned ON with the exception of the PCF8584 (which requires an',CR,LF
DB  'external crystal) and the 82B715 (which requires a minimum Vdd of 4.5 V) which',CR,LF
DB  'must both be switched OFF the bus.',CR,LF,LF

DB  '   PRESS <ESC> TO RETURN TO MENU   or   <SPACE BAR> FOR NEXT MODULE',CR,LF,LF,LF,LF

DB  'Time: ',EOT




