;******************************************************************************
;*                                                                            *
;*               OM5027 CLOCK MAINTENANCE AND DISPLAY MODULE                  *
;*                                                                            *
;******************************************************************************
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;
;CONTENTS       clk_init          split1         dispModData                  ;
;               rdclk             timeMode       modOps                       ;
;               lcd1              alarmMode      update                       ;
;               secUpdate         getalarm                                    ;
;               timeUpdate        alarmtog                                    ;
;               dateUpdate        datemode                                    ;
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;


clkadr          equ     10100010b ;PCF8583 address
clkctl          equ     00001100b ;clock control,masked,alarm active
alctl1          equ     00000000b ;alarm control,no clock alarm
alctl2          equ     00010000b ;alarm control,clock daily alarm

        bseg
aset:           dbit    1               ;alarm is set flag
aring:          dbit    1               ;alarm ringing flag
newsec:         dbit    1               ;set when second changed
newmin:         dbit    1               ;set when minute changed
newdata:        dbit    1               ;set when data has been modified
flashMod:       dbit    1               ;set when display to flash
bseg_end        set     $

        dseg
clk1:           ds      1               ;.10:.01 seconds
clk2:           ds      1               ; 10:1 seconds
clk3:           ds      1               ; 10:1 minutes
clk4:           ds      1               ; 10:1 hours
dat1:           ds      1               ; 10 days:unit days
dat2:           ds      1               ; 10 months:unit months
mod1:           ds      1               ; holds data for modification
mod2:           ds      1               ; holds data for modification
flashCount:     ds      1               ; counter for display flash rate
dseg_end        set     $
        cseg

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;clk_init       Initialises the PCF8593 peripheral. Does not affect operation ;
;               if already initialised correctly.                             ;
;                       clock mode: masked, alarm active                      ;
;                       alarm mode: no clock alarm,no timer.                  ;
;               If illegal values in time or date registers then initialise   ;
;               to legal values.                                              ;
;                                                                             ;
;ENTRY          aset            if set then set alarm                         ;
;                                                                             ;
;USES           r(1)0,1,2,3,4                                                 ;
;               acc,iicbuff                                                   ;
;                                                                             ;
;CALLS          iic                                                           ;
;               testbcd         (internal)                                    ;
;                                                                             ;
;EXIT           initialise control/status (+time/date registers if required)  ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
clk_init:
;initialise control/status register
        setb    rs0                     ;select register bank 1
ci20:   mov     iicbuff,#00h            ;word address to 0
        mov     iicbuff+1,#clkctl       ;clock control
        mov     r2,#02                  ;tx = 2
        mov     r3,#00                  ;rx = 0
        mov     r4,#clkadr              ;address = clock
        call    iic                     ;send control to clock

;initialise alarm control register
ci21:   mov     iicbuff,#08h            ;word address to 8
        mov     iicbuff+1,#alctl2       ;alarm control, daily clock alarm
        jb      aset,ci25
        mov     iicbuff+1,#alctl1       ;alarm control, no clock alarm
ci25:   mov     r2,#02                  ;tx = 2
        mov     r3,#00                  ;rx = 0
        mov     r4,#clkadr              ;address = clock
        call    iic                     ;send alarm control to clock

;get clock and date data
ci22:   mov     iicbuff,#02h            ;word address to 1
        mov     r2,#01                  ;tx = 2
        mov     r3,#05                  ;rx = 5
        mov     r4,#clkadr              ;address = clock
        call    iic                     ;send alarm control to clock
        jb      buserror,cix

;test for legality and correct if necessary
        mov     r0,#iicbuff             ;secs
        call    testbcd
        jnz     ci10
        cjne    @r0,#60h,$+3
        jnc     ci10
ci2:    inc     r0                      ;mins
        call    testbcd
        jnz     ci10
        cjne    @r0,#60h,$+3
        jnc     ci10
ci4:    inc     r0                      ;hours
        call    testbcd
        jnz     ci10
        cjne    @r0,#24h,$+3
        jnc     ci10
ci5:    inc     r0                      ;days
        call    testbcd
        jnz     ci10
        cjne    @r0,#00h,ci7
        sjmp    ci10
ci7:    cjne    @r0,#32h,$+3            ;corrected from 32 decimal for ver 1.1
        jnc     ci10
ci8:    inc     r0                      ;months
        call    testbcd
        jnz     ci10
        cjne    @r0,#00h,ci9
        sjmp    ci10
ci9:    cjne    @r0,#13h,$+3
        jc      cix

;if illegal then prepare for rewriting the clock chip
ci10:   mov     iicbuff+1,#01
        mov     iicbuff+2,#00
        mov     iicbuff+3,#00
        mov     iicbuff+4,#01
        mov     iicbuff+5,#01

ci23:   mov     iicbuff,#02h            ;word address to 8
        mov     r2,#06                  ;tx = 6
        mov     r3,#00                  ;rx = 0
        mov     r4,#clkadr              ;address = clock
        call    iic                     ;send alarm control to clock

cix:    clr     rs0                     ;select register bank 0
        ret

testbcd:                                ;if not bcd then reset all
        mov     a,@r0
        clr     c
        add     a,#9Ah
        da      a
        xrl     a,@r0
        ret

;-------- end of clk_init

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;RDCLK  Reads all the relevant data required from the clock chip.             ;
;       The time is stored in clk1-clk4. The date is stored in dat1-dat2      ;
;       If the alarm is to ring it will set the alarm ringing flag and clear  ;
;       the alarm flag in the chip. The alarm mode is checked so the lcd icon ;
;       shows correct status.                                                 ;
;                                                                             ;
;ENTRY  clk2            the last read seconds                                 ;
;USES   iicbuff                                                               ;
;       r0                                                                    ;
;       r1                                                                    ;
;                                                                             ;
;CALLS  iic                                                                   ;
;       beep                                                                  ;
;                                                                             ;
;EXIT   clk[1..4]       loaded with the current time                          ;
;       dat[1..2]       loaded with current date                              ;
;       aring   set     if alarm ringing, else cleared                        ;
;       newsec          set if new second                                     ;
;       newmin          set if new miniute                                    ;
;       aset            reflects status of alarm mode                         ;
;       refresh_all     set if icon changed (refreshes lcd1)                  ;
;       lcd1_x1         reflects status of alarm mode                         ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
rdclk:  mov     iicbuff,#00h            ;set internal address to 00
        setb    rs0                     ;select register bank 1
        mov     r2,#01                  ;tx = 1
        mov     r3,#09                  ;rx = 9
        mov     r4,#clkadr               ;address = clock
        call    iic
        jnz     clkoff
        clr     rs0                     ;select register bank 0
        setb    newsec
        setb    newmin
        mov     a,clk2                  ;get old seconds
        cjne    a,iicbuff+2,rdc3        ;compare old with new jump if not equal
        clr     newsec                  ;clear newsec as not a new second
        clr     newmin
        sjmp    rdc0
rdc3:   mov     a,clk3
        cjne    a,iicbuff+3,rdc0        ;compare old with new jump if not equal
        clr     newmin

;move the time and date to it's storage area
rdc0:   mov     r0,#clk1                ;pointer to destination
        mov     r1,#iicbuff+1           ;pointer to source
        mov     r2,#06h                 ;move 6 bytes
rdc1:   mov     a,@r1
        mov     @r0,a
        inc     r1
        inc     r0
        djnz    r2,rdc1

;check if the alarm is set and adjust alarm icon if different
        mov     a,iicbuff+8             ;get alarm status reg
        anl     a,#00110000b            ;mask out all except alarm mode bits
        jnz     rdc6
        jnb     aset,rdc5
        clr     aset
        clr     lcd1_x1
        setb    refresh_all
        sjmp    rdc5
rdc6:   jb      aset,rdc5
        setb    aset
        setb    lcd1_x1
        setb    refresh_all

;now look at the status word and test for alarm ring.
rdc5:   mov     a,iicbuff               ;get status
        jnb     acc.1,rdc2              ;not set so exit via beep if needed

;now set the alarm ringing flag and clear the flag in the chip
        setb    aring                   ;make the alarm ring
        mov     tone,#0
        mov     iicbuff,#00h            ;control word address
        mov     iicbuff+1,#clkctl       ;control word
        setb    rs0                     ;select register bank 1
 rdc20: mov     r2,#02                  ;tx = 2
        mov     r3,#00                  ;rx = 0
        mov     r4,#clkadr              ;address for clock
        call    iic
        inc     a
        jz      rdc20
        clr     rs0                     ;select register bank 0

;if a new second then beep
rdc2:   jnb     newsec,rdclkx
        call    beep
rdclkx: ret

clkoff:                         ;if clk off bus then use tempory LEGAL values
        mov     clk2,#00
        mov     clk3,#00
        mov     clk4,#00
        mov     dat1,#01
        mov     dat2,#01
        clr     newsec
        clr     newmin
        clr     aset
        clr     lcd1_x1
        setb    refresh_all
        ret
;----------- end of rdclk

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;LCD1           This routine controls the display of date, time, and icon     ;
;               information on lcd1. The seconds are updated every second     ;
;               and the rest is updated every minute or when otherwise        ;
;               requested by the refresh_all flag. If in lcd1_demo mode then  ;
;               display time only.                       (mode = 08)          ;
;                                                                             ;
;ENTRY          newsec                                                        ;
;               newmin                                                        ;
;               refresh_all                                                   ;
;                                                                             ;
;USES           acc,c                                                         ;
;                                                                             ;
;CALLS          secUpdate                                                     ;
;               timeUpdate                                                    ;
;               dateUpdate                                                    ;
;               lcd1_send                                                     ;
;                                                                             ;
;EXIT           nil                                                           ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
LCD1:   mov     a,mode
        cjne    a,#08,lcd13
        sjmp    lcd12                   ;if mode 8 show time only
lcd13:  jb      refresh_all,lcd14
        jnb     newsec,lcd11
        jb      newmin,lcd14
        call    secUpdate               ;update seconds in image ram
        sjmp    lcd11
lcd14:  call    dateUpdate              ;update date in image ram
lcd12:  call    timeUpdate              ;update time in image ram
lcd11:  call    lcd1_send               ;send new data in image ram
        ret

;-------- end of lcd1

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;SecUpdate      Updates seconds display in the lcd1 image                     ;
;                                                                             ;
;ENTRY          clk2                                                          ;
;                                                                             ;
;USES           acc                                                           ;
;               r0,r1                                                         ;
;                                                                             ;
;CALLS          split1                                                        ;
;               lcd1_addstr                                                   ;
;                                                                             ;
;EXIT           lcd_line2                                                     ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
SecUpdate:
        setb    lcd1line        ;select line 2 of lcd1
        mov     firstpos,#08    ;position of 1's of seconds
        mov     r0,#clk2
        mov     r1,#iicbuff+1
        call    split1
        mov     iicbuff+2,#04   ;sentinal
        call    lcd1_addstr     ;update image
        ret

;--------end of secUpdate

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;TimeUpdate     Updates all time data in the lcd1 image                       ;
;                                                                             ;
;ENTRY          clk[2..4]                                                     ;
;               dat[1..2]                                                     ;
;                                                                             ;
;USES           r0,r1                                                         ;
;               iicbuff                                                       ;
;                                                                             ;
;CALLS          split1                                                        ;
;               lcd1_addstr                                                   ;
;                                                                             ;
;EXIT           lcd_line2                                                     ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
timeUpdate:
        setb    lcd1line        ;select line 2 of lcd1
        mov     firstpos,#02    ;position of 10's of hours on lcd1
        mov     r0,#clk2
        mov     r1,#iicbuff+8
        mov     @r1,#04         ;sentinal
        dec     r1
        call    split1          ;seconds
        mov     @r1,#'-'
        dec     r1
        call    split1          ;minutes
        mov     @r1,#'-'
        dec     r1
        call    split1          ;hours
        call    lcd1_addstr     ;update image
        ret

;-------- end of timeupdate

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;DateUpdate     Decodes raw date information and updates the lcd1 image       ;
;                                                                             ;
;ENTRY          dat[1..2]                                                     ;
;                                                                             ;
;USES           acc,b,c                                                       ;
;               dptr                                                          ;
;                                                                             ;
;CALLS          lcd1_addstr                                                   ;
;                                                                             ;
;EXIT           nil                                                           ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;dat1 =      year,10 days : unit days
;dat2 = weekday,10 months : unit months
;
DateUpdate:
        mov     dptr,#datetbl
        mov     r1,#iicbuff
        mov     a,dat2          ;raw month data
        mov     c,acc.4         ;store 10's of months
        anl     a,#0Fh          ;leave only 1's
        jnc     dud1            ;jump if < 10 months
        add     a,#10           ;convert to binary
dud1:   cjne    a,#13,$+3
        jc      dud2            ;jump if legal
dud7:   mov     a,#13
dud2:   jz      dud7
        dec     a
        mov     b,#11
        mul     ab              ;calc. pointer value
        cjne    a,#133,$+3      ;check legal
        jc      dud6
        mov     a,#12
dud6:   mov     r0,a
dud4:   mov     a,r0
        movc    a,@a+dptr
        mov     @r1,a
        inc     r1
        inc     r0
        cjne    a,#04,dud4
        mov     r1,#iicbuff-1
dud3:   inc     r1
        cjne    @r1,#'X',dud3
        mov     a,dat1
        swap    a
        anl     a,#03h
        add     a,#30h
        mov     @r1,a
        inc     r1
        mov     a,dat1
        anl     a,#0fh
        add     a,#30h
        mov     @r1,a
        mov     firstpos,#01
        clr     lcd1line       ;select line 2 of lcd1
        call    lcd1_addstr
        ret
datetbl:
        db      'XX JANUARY',04
        db      '  XX FEB  ',04
        db      ' XX MARCH ',04
        db      ' XX APRIL ',04
        db      '  XX MAY  ',04
        db      ' XX JUNE  ',04
        db      ' XX JULY  ',04
        db      'XX AUGUST ',04
        db      ' XX SEPT  ',04
        db      'XX OCTOBER',04
        db      '  XX NOV  ',04
        db      '  XX DEC  ',04
        db      'XX ERROR  ',04

;----------------- end of DateUpdate

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;SPLIT1         takes the data pointed to by R0 and splits the nibbles        ;
;               up. They are converted to ascii and stored in the ram         ;
;               pointed to by R1,R1-1.                                        ;
;                                                                             ;
;ENTRY          R0              points to data to split                       ;
;               R1              points to where to put split data             ;
;USES           ACC                                                           ;
;EXIT           @R1             split data                                    ;
;               r0              increment to point to next packed bcd         ;
;               r1              decremented twice                             ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
split1: mov     a,@r0
        anl     a,#0fh
        add     a,#30h
        mov     @r1,a           ;units
        dec     r1
        mov     a,@r0
        anl     a,#0f0h
        swap    a
        add     a,#30h
        mov     @r1,a           ;tens
        dec     r1
        inc     r0
        ret

;-------- end of split1

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;TimeMode       Processes key information to coordinate between the time and  ;
;               set-time displays and the modification of clock data          ;
;                                                                             ;
;ENTRY          modmode         set if in "modify data" mode                  ;
;               keyvalid                                                      ;
;               keybits         PB8     update if data changed and exit       ;
;               newlcd2                                                       ;
;                                                                             ;
;USES           r0,acc                                                        ;
;                                                                             ;
;CALLS          modOps                                                        ;
;               update                                                        ;
;               dispmoddata                                                   ;
;               restart_key_scan                                              ;
;                                                                             ;
;EXIT           newlcd2                                                       ;
;               modmode                                                       ;
;               newdata                                                       ;
;               refresh_all                                                   ;
;                                                                             ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
TimeMode:
;initialise screen if first pass
        jnb     newlcd2,tm10
        mov     dptr,#timscr0           ;"Current Time" or ...
        jnb     modmode,tm0
        jb      newdata,tm20
        mov     mod1,clk3
        mov     mod2,clk4
tm20:   mov     dptr,#timscr1           ;..."Alter-Time" if modmode
tm0:    call    lcd2_WrScr
        clr     newlcd2

;display data
tm10:   jb      modmode,tm11
        mov     mod1,clk3
        mov     mod2,clk4
tm11:   call    dispModData             ;display data in mod variables

;process key data
        jnb     keyvalid,tm7

;perform necessary data modification
        call    modOps

;if data modified test for roll over
        jnb     newdata,tm5
        mov     r0,#mod1                ;minutes

        cjne    @r0,#99h,tm3
        mov     @r0,#59h
tm3:    cjne    @r0,#60h,$+3
        jc      tm4
        mov     @r0,#00
tm4:    inc     r0                      ;hours
        cjne    @r0,#99h,tm6
        mov     @r0,#23h
tm6:    cjne    @r0,#24h,$+3
        jc      tm5
        mov     @r0,#00

tm5:    mov     a,keybits
        jnb     acc.7,tm8               ;test for exit request
        jnb     newdata,tm9
        mov     a,#03
        call    update                  ;update hours and minutes
        mov     mod1,#00
        mov     mod2,#00
        mov     a,#01                   ;internal address
        call    update                  ;zero the seconds
        setb    refresh_all
        clr     newdata
tm9:    setb    newlcd2
        clr     modmode

tm8:    call    restart_key_scan
tm7:    ret


timscr0:db      '  PCF8593',04
        db      'Current Time',04
        db      04

timscr1:db      '  PCF8593',04
        db      ' Alter Time',04
        db      04

;------------ end of timemode

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;alarmMode      Processes key information to coordinate between the alarm and ;
;               set-alarm displays and the modification of alarm data         ;
;                                                                             ;
;ENTRY          modmode         set if in "modify data" mode                  ;
;               keyvalid                                                      ;
;               keybits         PB8     update if data changed and exit       ;
;               newlcd2                                                       ;
;                                                                             ;
;USES           r0,acc                                                        ;
;                                                                             ;
;CALLS          getAlarmData                                                  ;
;               modOps                                                        ;
;               update                                                        ;
;               restart_key_scan                                              ;
;                                                                             ;
;EXIT           newlcd2                                                       ;
;               modmode                                                       ;
;               newdata                                                       ;
;                                                                             ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
alarmMode:
;get alarm data and initialise screen if first pass
        jnb     newlcd2,alm10
        jb      newdata,alm20
        call    getAlarmData
alm20:  mov     dptr,#almscr0           ;"Alarm Time" or ...
        jnb     modmode,alm0
        mov     dptr,#almscr1           ;..."Alter-Alarm" if modmode
alm0:   call    lcd2_WrScr
        clr     newlcd2

;display data in mod variables
alm10:  call    dispModData

;process key data
        jnb     keyvalid,alm7

;perform necessary data modification
        call    modOps

;if data modified test for roll over
        jnb     newdata,alm5
        mov     r0,#mod1                ;minutes
        cjne    @r0,#99h,alm3
        mov     @r0,#59h
alm3:   cjne    @r0,#60h,$+3
        jc      alm4
        mov     @r0,#00
alm4:   inc     r0                      ;hours
        cjne    @r0,#99h,alm6
        mov     @r0,#23h
alm6:   cjne    @r0,#24h,$+3
        jc      alm5
        mov     @r0,#00

alm5:   mov     a,keybits
        jnb     acc.7,alm8              ;test for exit request
        jnb     newdata,alm9
        mov     a,#11                   ;internal address
        call    update                  ;update hours and minutes
        mov     mod1,#00
        mov     mod2,#00
        mov     a,#09                   ;internal address
        call    update                  ;zero the seconds
        clr     aset
        call    alarmtog
        clr     newdata
alm9:   setb    newlcd2
        clr     modmode

alm8:   call    restart_key_scan
alm7:   ret


almscr0:
        db      '  PCF8593',04
        db      ' Alarm Time',04
        db      04
almscr1:
        db      '  PCF8593',04
        db      'Alter Alarm',04
        db      04

;----------- end of alarmmode

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;getAlarmData   Obtains alarm data from clock/calendar I.C.                   ;
;                                                                             ;
;ENTRY          nil                                                           ;
;                                                                             ;
;USES           r(1)2,3,4                                                     ;
;               iicbuff                                                       ;
;                                                                             ;
;CALLS          IIC                                                           ;
;                                                                             ;
;EXIT           mod1            ;minutes                                      ;
;               mod2            ;hours                                        ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
getAlarmData:
        mov     iicbuff,#0bh    ;internal address for alarm
        setb    rs0             ;select register bank 1
        mov     r2,#01h         ;tx = 1
        mov     r3,#02          ;rx = 2
        mov     r4,#clkadr      ;address = clock
        call    iic             ;from the clock
        inc     a
        jz      getalarmdata
        clr     rs0             ;select register bank 0
        mov     mod1,iicbuff    ;save alarm minutes
        mov     mod2,iicbuff+1  ;save alarm hours
        ret

;--------- end of getAlarmData

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;AlarmTog       Toggles the alarm setting on or off, or kills alarm if ringing;
;                                                                             ;
;ENTRY          aring                                                         ;
;                                                                             ;
;USES           r(1)2,3,4                                                     ;
;               iicbuff                                                       ;
;                                                                             ;
;CALLS          IIC                                                           ;
;                                                                             ;
;EXIT           nil                                                           ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
alarmtog:
        jbc     aring,almt1             ;make sure the alarm stops ringing
        mov     iicbuff,#08h            ;alarm control register
        mov     iicbuff+1,#alctl1       ;get ready to turn alarm off
        jb      aset,almt0              ;jump if turning alarm on
        mov     iicbuff+1,#alctl2       ;get ready to turn alarm on

almt0:  setb    rs0                     ;select register bank 1
        mov     r2,#02                  ;tx = 2
        mov     r3,#00                  ;rx = 0
        mov     r4,#clkadr              ;address = clock
        call    iic                     ;send two bytes
        inc     a
        jz      almt0
        clr     rs0                     ;select register bank 0
        sjmp    almtx

almt1:  mov     iicbuff,#dtmfo          ;turn off any tone
        setb    rs0                     ;select register bank 1
        mov     r2,#01                  ;tx = 1
        mov     r3,#00                  ;rx = 0
        mov     r4,#dtmfadr             ;address = dtmf
        call    iic                     ;turn tone off
        inc     a
        jz      almt1
        clr     rs0                     ;select register bank 0

almtx:  ret

;--------- end of alarmtog

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;DateMode       Processes key information to coordinate between the date and ;
;               set-date displays and the modification of clock data         ;
;                                                                            ;
;ENTRY          modmode         set if in "modify data" mode                 ;
;               keyvalid                                                     ;
;               keybits         PB8     update if data changed and exit      ;
;               newlcd2                                                      ;
;                                                                            ;
;USES           r0,acc                                                       ;
;               dptr                                                         ;
;                                                                            ;
;CALLS          modOps                                                       ;
;               update                                                       ;
;               restart_key_scan                                             ;
;                                                                            ;
;EXIT           newlcd2                                                      ;
;               modmode                                                      ;
;               newdata                                                      ;
;               clk3            causes a full refresh of lcd1                ;
;                                                                            ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
DateMode:
;initialise screen if first pass
        jnb     newlcd2,dt10
        mov     dptr,#datscr0           ;"Current Date" or ...
        jnb     modmode,dt0
        jb      newdata,dt20
        mov     mod1,dat1
        mov     mod2,dat2
dt20:   mov     dptr,#datscr1           ;..."Alter-Date" if modmode
dt0:    call    lcd2_WrScr
        clr     newlcd2

;display data
dt10:   jb      modmode,dt11
        mov     mod1,dat1
        mov     mod2,dat2
dt11:   call    dispModData

;process key data
        jnb     keyvalid,dt7
        call    modOps                  ;perform necessary data modification

        jnb     newdata,dt5             ;if data modified test for roll over
        mov     dptr,#month
        mov     a,mod2
        movc    a,@a+dptr
        mov     r0,#mod1                ;days
        cjne    @r0,#00h,dt3
        mov     @r0,a
dt3:    cjne    a,mod1,$+3
        jnc     dt4
        mov     @r0,#01
dt4:    inc     r0                      ;months
        cjne    @r0,#00h,dt6
        mov     @r0,#12h
dt6:    cjne    @r0,#13h,$+3
        jc      dt5
        mov     @r0,#01

dt5:    mov     a,keybits
        jnb     acc.7,dt8               ;test for exit request
        jnb     newdata,dt9
        mov     a,#05
        call    update                  ;update date
        setb    refresh_all
        clr     newdata
dt9:    setb    newlcd2
        clr     modmode

dt8:    call    restart_key_scan
dt7:    ret


datscr0:db      '  PCF8593',04
        db      'Current Date',04
        db      ' M       D',04

datscr1:db      '  PCF8593',04
        db      ' Alter Date',04
        db      ' M       D',04

month:  db      00    ;dummy
        db      31h   ;january
        db      28h   ;february (leap year)
        db      31h   ;march
        db      30h   ;april
        db      31h   ;may
        db      30h   ;june
        db      31h   ;july
        db      31h   ;august
        db      30h   ;september
        db      00
        db      00
        db      00
        db      00
        db      00
        db      00
        db      31h   ;october
        db      30h   ;november
        db      31h   ;december

;--------------- end of DateMode

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;DispModData    Displays on the 3rd line of LCD2 the packed BCD data contained;
;               in mod1,mod2 in the format:  "   m1m1:m2m2". The display is   ;
;               made to flash on and off during modmode.                      ;
;                                                                             ;
;ENTRY          mod1            holds most significant packed BCD for display ;
;               mod2            holds least significant packed BCD for display;
;               flashMod        1 = disp. on, 0 = disp. off                   ;
;               flashCount      number of passes before flashmod complimented ;
;                                                                             ;
;USES           iicbuff                                                       ;
;               r0,r1                                                         ;
;                                                                             ;
;CALLS          lcd2_newline                                                  ;
;               lcd2_indent                                                   ;
;               split1                                                        ;
;               lcd2_wrdatastr                                                ;
;                                                                             ;
;EXIT           mod1,mod2       unchanged                                     ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
DispModData:

;check for display flash
        jnb     modmode,dmd4            ;dont flash if not in modmode
        mov     a,keybits
        jnz     dmd4                    ;dont flash if key pressed
        mov     a,flashcount
        anl     a,#07h
        djnz    acc,dmd0
        cpl     flashMod
        mov     a,#04
dmd0:   mov     flashCount,a
        sjmp    dmd2
dmd4:   setb    flashmod                ;turn on

;go to beginning of line 3 of lcd2 and indent 3 spaces
dmd2:   mov     iicbuff+1,#03
        call    lcd2_newline
        mov     iicbuff+1,#03
        call    lcd2_indent
        jnb     modmode,dmd3
        jnb     flashMod,dmd1

;split the packed bcd into the iicbuff and send to lcd2
 dmd3:  mov     r0,#mod1
        mov     r1,#iicbuff+6
        mov     @r1,#04
        dec     r1
        call    split1
        mov     @r1,#':'
        dec     r1
        call    split1
        call    lcd2_wrdatastr
        ret

dmd1:   mov     dptr,#flashOff          ;flash off
        call    lcd2_wrCodeStr
        ret

flashOff:
        db      '  :  ',04

;----------- end of DispModData

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;ModOps         increments/decrements packed BCD data in mod1/mod2            ;
;                                                                             ;
;ENTRY          mod1,mod2,keybits       PB2     increment mod2                ;
;                                       PB3     increment mod1                ;
;USES           acc                     PB6     decrement mod2                ;
;               r0                      PB7     decrement mod1                ;
;                                                                             ;
;EXIT           mod1                                                          ;
;               mod2                                                          ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
ModOps:
        mov     a,keybits
        jb      acc.1,mo0       ;PB2
        jb      acc.2,mo1       ;PB3
        jb      acc.5,mo2       ;PB6
        jb      acc.6,mo3       ;PB7
        ret

mo0:    mov     r0,#mod2        ;inc MSD's
        sjmp    mo6
mo1:    mov     r0,#mod1        ;inc LSD's
mo6:    mov     a,@r0
        add     a,#01
        da      a
        mov     @r0,a
        setb    newdata
        ret

mo2:    mov     r0,#mod2        ;dec MSD's
        sjmp    mo7
mo3:    mov     r0,#mod1        ;dec MSD's
mo7:    mov     a,@r0
        add     a,#99h
        da      a
        mov     @r0,a
        setb    newdata
        ret

;--------- end of ModOps

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;update         Writes modified data to clock calendar peripheral.            ;
;                                                                             ;
;ENTRY          acc             holds specified internal address              ;
;               mod1            packed BCD data                               ;
;               mod2            packed BCD data                               ;
;                                                                             ;
;USES           r(1)1,2,3,4                                                   ;
;                                                                             ;
;EXIT           nil                                                           ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
update: setb    rs0
        mov     r1,#iicbuff
        mov     r2,#03
        mov     r3,#00
        mov     r4,#clkadr
        mov     iicbuff,a
        mov     iicbuff+1,mod1
        mov     iicbuff+2,mod2
        call    iic
        inc     a
        jz      update
        clr     rs0
        ret
;-------- end of update

;------------- end of clock associated routines
