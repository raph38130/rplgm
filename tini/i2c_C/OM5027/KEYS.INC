;******************************************************************************
;*                                                                            *
;*                       KEYPAD INTERFACE ROUTINES                            *
;*                                                                            *
;******************************************************************************
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;
;CONTENTS       init_switches                                                 ;
;               restart_key_scan                                              ;
;               scan_keys                                                     ;
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;
;         ÄÄÄÄ¿
;            7ÃÄÄÄÄÄÄÄ led
;             ³
;            6ÃÄÄÄÄÄÄÄ led
;      P      ³
;      C     5ÃÄÄÄÄÄÄÄÄÄÄÄ¿
;      F      ³           ³
;      8     4ÃÄÄÄÄÄ¿     ³
;      5      ³     ³     ³
;      7     3ÃÄÄÄÄÄ3ÄÄÄÄÄ7ÄÄ
;      4      ³     ³     ³
;      A     2ÃÄÄÄÄÄ2ÄÄÄÄÄ6ÄÄ
;             ³     ³     ³
;            1ÃÄÄÄÄÄ1ÄÄÄÄÄ5ÄÄ
;             ³     ³     ³
;            0ÃÄÄÄÄÄ0ÄÄÄÄÄ4ÄÄ
;         ÄÄÄÄÙ     ³     ³
;

phase1          equ     00100000b
phase2          equ     00010000b
keypadAdr       equ     7eh
debounce        equ     03

        bseg
keyvalid:       dbit    1
onekey:         dbit    1
bseg_end        set     $

        dseg
ledvalue:       ds      1
keybits:        ds      1
scancount:      ds      1
dseg_end        set     $

        cseg

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;Init_Switches          initialize the ScanSwitches variables                 ;
;Assumptions                                                                  ;
;       Repitition      20mS    scan repitition rate                          ;
;       ScanCount       3       scans until valid  210mS                      ;
;       LedValue        8Fh     the initial LED, key port value               ;
;       KeyBits         00h     value of the key just pressed                 ;
;       flags           clear   cleared by clearing all the ram               ;
;ENTRY  nil                                                                   ;
;EXIT   scancount                                                             ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
Init_Switches:
        mov     scancount,#debounce
        mov     ledvalue,#08Fh
        mov     keybits,#000h
        clr     onekey
        clr     keyvalid
        ret
;
;-------------- end of Init_Switches

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;Scan_Keys              scan the keys, flag when valid                        ;
;NOTE:  this should be called early on in the main loop                       ;
;ENTRY  LEDVALUE        value to control the two LED's                        ;
;       KEYCOUNT        number of scans remaining                             ;
;       KEYBITS         the bits from the last scan                           ;
;USES   R2              IIC, bit counter                                      ;
;       R3              IIC                                                   ;
;       R4              IIC                                                   ;
;       R5                                                                    ;
;       R7              holds the current scan key bits                       ;
;       IICBUFF                                                               ;
;STACK  2 bytes                                                               ;
;CALLS  READ_KEYS       local to this routine                                 ;
;       IIC                                                                   ;
;EXIT   KEYVALID        set if key valid, else clear                          ;
;       ONEKEY          set if one key detected, else clear                   ;
;       KEYBITS         represents the keys pressed                           ;
;       KEYCOUNT        number of scans left until valid, user can ignore     ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
read_keys:
        orl     a,ledvalue
        mov     r5,a
rk0:    mov     a,r5
        mov     iicbuff,r5
        mov     r2,#1                   ;tx = 1
        mov     r3,#1                   ;rx = 1
        mov     r4,#keypadAdr           ;address
        lcall   iic
        inc     a
        jz      rk0
        mov     a,iicbuff
        anl     a,#00001111b
        clr     c
        ret
r_k_error:
        setb    c
        ret

scan_keys:
        jb      keyvalid,s_s_exit       ;exit, last key not used yet
s_s_phase1:
        setb    p1.0                    ;scope trigger
        mov     a,#phase1               ;first phase of switch scan
        call    read_keys
        clr     p1.0
        jc      restart_key_scan
        mov     r7,a
s_s_phase2:
        mov     a,#phase2               ;second phase of switch scan
        call    read_keys
        jc      restart_key_scan
        xch     a,r7                    ;MV
        swap    a
        orl     a,r7                    ;all 8 switches in r7
        cpl     a
        mov     r7,a                    ;now active high
        jz      restart_key_scan
        mov     r2,#8                   ;must always complete 8 loops
s_s_loop:
        mov     a,keybits
        rr      a
        mov     keybits,a
        mov     a,r7
        mov     c,acc.0
        rr      a
        mov     r7,a
        jnc     s_s_nokey               ;look for ones as active
        mov     a,keybits               ;the last key value
        jz      s_s_first
        cjne    a,7,s_s_twokeys
        djnz    scancount,s_s_nokey
        setb    onekey
s_s_twokeys:                            ;if two keys only flag as valid
        setb    keyvalid
        sjmp    s_s_nokey               ;continue with all 8 bits
s_s_first:
        mov     keybits,r7              ;remember the matching pattern
s_s_nokey:
        djnz    r2,s_s_loop
s_s_exit:
        ret

restart_key_scan:
        mov     keybits,#00
        mov     scancount,#debounce
        clr     onekey
        clr     keyvalid                ;scanning resumes
        ret
;
;-------------- end of Scan_Switches

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;altled         Compliments LED configuration once every second.              ;
;                                                                             ;
;ENTRY          newsec                                                        ;
;               ledvalue                                                      ;
;                                                                             ;
;USES           acc                                                           ;
;                                                                             ;
;EXIT           ledvalue                                                      ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
altled: jnb     newsec,altx
        mov     a,ledvalue
        cpl     acc.7
        cpl     acc.6
        mov     ledvalue,a
altx:   ret

;----------------- end of altled

