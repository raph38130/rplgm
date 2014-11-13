;******************************************************************************
;*                                                                            *
;*                         OM5027 GENERAL ROUTINES                            *
;*                                                                            *
;******************************************************************************
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;
;CONTENTS       T0_process                                                    ;
;               T0_init                                                       ;
;               processkeys                                                   ;
;               modecontrol                                                   ;
;               handle_bus_error                                              ;
;               signon                                                        ;
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;

T0_reload       equ     -(20880-12)     ;70 ms timebase (including ISR)
                                        ; @ f = 3.579545 MHz
        bseg
timesup:        dbit    1               ;set when T0 has overflowed
newlcd2:        dbit    1               ;set when first pass of new mode
modmode:        dbit    1               ;set when in modify data mode
bseg_end        set     $

        dseg
mode:           ds      1               ;specifies mode (0-8)
dseg_end        set     $

        cseg

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;T0_init        Initialises T0 to 16 bit timer with 70ms reload and           ;
;               enabled interrupt.                                            ;
;                                                                             ;
;EXIT           T0              70ms                                          ;
;               tmod            mode 1                                        ;
;               et0             set                                           ;
;               ea              set                                           ;
;               timesup         reset                                         ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
T0_init:
       mov     th0,#high(T0_reload)
       mov     tl0,#low(T0_reload)
       orl     tmod,#00000001b
       setb    tr0                     ;start the timer
       setb    et0                     ;enable T0 interrupt
       setb    ea                      ;enable global interrupts
       clr     timesup
       ret

;-------- end of T0_init

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;T0_Process     Reload the timer, set flag to show times up                   ;
;                                                                             ;
;ENTRY          by interrupt only                                             ;
;                                                                             ;
;EXIT           TIMER 0         running again                                 ;
;               timesup         set                                           ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
T0_process:
        clr     tr0
        setb    timesup
        mov     th0,#high(T0_reload)
        mov     tl0,#low(T0_reload)
        setb    tr0
        reti
;-------------- end of T0_process

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;ProcessKeys    Decodes key data and actions accordingly                      ;
;               If not modmode then     PB1     HELP                          ;
;                                       PB2     lcd contrast up               ;
;                                       PB3     N/F                           ;
;                                       PB4     select previous mode          ;
;                                       PB5     N/F                           ;
;                                       PB6     lcd contrast down             ;
;                                       PB7     toggle alarm on/off           ;
;                                       PB8     select next mode              ;
;               If modmode then PB1 only is recognised and the other keybits  ;
;               are left to be decoded by the subsequent mode routines.       ;
;               A special service routine is called when exiting mode 8       ;
;               or lcd1demo mode.                                             ;
;                                                                             ;
;ENTRY          keybits                                                       ;
;               keyvalid                                                      ;
;               modmode                                                       ;
;                                                                             ;
;USES           acc,c                                                         ;
;                                                                             ;
;CALLS          contrastup                                                    ;
;               contrastdn                                                    ;
;               modecontrol                                                   ;
;               almtog                                                        ;
;               help                                                          ;
;               restart_key_scan                                              ;
;               exit_lcd1demo (internal)                                      ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
processKeys:
        jnb     keyvalid,pks4
        mov     a,keybits
        jnb     acc.0,pks0
        call    help
        sjmp    pks5
pks0:   jnb     helpmode,pks9
        setb    newlcd2
        clr     helpmode
        sjmp    pks5
pks9:   jb      modmode,pks4
        jnb     acc.1,pks1
        call    contrastup
        sjmp    pks5
pks1:   jnb     acc.5,pks2
        call    contrastdn
        sjmp    pks5
pks2:   jnb     acc.6,pks6
        call    alarmtog
        sjmp    pks5
pks6:   jnb     acc.4,pks7
        setb    modmode
        setb    newlcd2
        sjmp    pks5
pks7:   jnb     acc.3,pks8
        mov     a,mode
        dec     a
        cjne    a,#0ffh,pks21
        mov     a,#08
pks21:  mov     mode,a
        setb    newlcd2         ;new mode so flag to refresh LCD2
        setb    refresh_all
        cjne    a,#07,pks5
        call    exit_lcd1demo
        sjmp    pks5
pks8:   jnb     acc.7,pks5
        mov     a,mode
        inc     a
        cjne    a,#09,pks20
        clr     a
        call    exit_lcd1demo
pks20:  mov     mode,a
        setb    newlcd2         ;new mode so flag to refresh LCD2
        setb    refresh_all
pks5:   call    restart_key_scan
pks4:   jb      helpmode,pksx
        call    modecontrol
pksx:   ret


exit_lcd1demo:
        mov     iconbyte+1,#00
        mov     iconbyte,#00
        jnb     aset,xl1dx
        mov     iconbyte,#00100000b
        mov     Vdd,#00         ;force a refresh of the Vdd bar graph
xl1dx:  ret

;------- end processkeys

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;ModeControl    Decodes mode byte and invokes appropriate mode.               ;
;                                                                             ;
;ENTRY          mode                                                          ;
;                                                                             ;
;USES           acc                                                           ;
;               dptr                                                          ;
;                                                                             ;
;CALLS          tempMode                                                      ;
;               timeMode                                                      ;
;               alarMmode                                                     ;
;               dateMode                                                      ;
;               dacadcMode                                                    ;
;               trackingMode                                                  ;
;               manualMode                                                    ;
;               musicMode                                                     ;
;               moduleInfo                                                    ;
;               lcd1demo                                                      ;
;                                                                             ;
;EXIT           nil                                                           ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
ModeControl:
        mov     dptr,#return
        push    dpl                 ;Push the return address on the stack to
        push    dph                 ;simulate a call.
        mov     dptr,#jmplcd
        mov     a,mode
        rl      a
        add     a,mode
        jmp     @a+dptr

jmplcd: ljmp    tempMode
        ljmp    timeMode
        ljmp    alarMmode
        ljmp    dateMode
        ljmp    dacadcMode
        ljmp    trackingMode
        ljmp    musicMode
        ljmp    moduleInfo
        ljmp    lcd1Demo

return: ret

;------- end of modecontrol

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;handle_bus_error       Reinitialise clock, lcds and dtmf in case of corrupt- ;
;                                                                      ion    ;
;ENTRY          buserror                                                      ;
;                                                                             ;
;CALLS          clk_init                                                      ;
;               lcd1_init                                                     ;
;               lcd2_init                                                     ;
;               dtmf_off                                                      ;
;                                                                             ;
;EXIT           newlcd2         refresh lcd2                                  ;
;               refresh_all     refresh lcd1                                  ;
;               buserror        cleared                                       ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
handle_bus_error:
        clr     buserror
;        call    clk_init
        call    lcd1_init
        call    lcd2_init
        call    dtmf_off
        setb    newlcd2
        setb    refresh_all
        ret

;------- end of handle_bus_error

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;signon         Dislays title and firmware revision on lcd2, and lcd1 all     ;
;               segments on.                                                  ;
;                                                                             ;
;USES           r0,r1                                                         ;
;               lcd_line1                                                     ;
;               iconbyte                                                      ;
;                                                                             ;
;CALLS          lcd1_send                                                     ;
;               lcd2_wrScr                                                    ;
;               delayms                                                       ;
;                                                                             ;
;EXIT           lcd1 image      all segments on                               ;
;                                                                             ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
signon: mov     r0,#lcd_line1           ;lcd1 all segments on
        mov     r1,#10
so3:    mov     @r0,#'#'
        inc     r0
        djnz    r1,so3
        mov     iconbyte,#0ffh
        mov     iconbyte+1,#0ffh        ;lcd1 al icons on
        setb    refresh_all
        call    lcd1_send               ;refresh lcd1
        mov     dptr,#introscr
        call    lcd2_WrScr              ;write title on lcd2
        mov     r0,#04
son0:   mov     a,#255
        call    delayms                 ;wait a second
        djnz    r0,son0
        mov     dptr,#revscr
        call    lcd2_WrScr              ;write revision on lcd2
        mov     r0,#04
son1:   mov     a,#255
        call    delayms                 ;wait a second
        djnz    r0,son1
        ret

introscr:
        db      '   OM5027   ',04
        db      ' EVALUATION ',04
        db      '   BOARD    ',04
revscr:
        db      '  FIRMWARE  ',04
        db      '  REVISION  ',04
        db      '    '
        REVISION
        db      04

;---------- end of signon

;---------------- end of general routines
