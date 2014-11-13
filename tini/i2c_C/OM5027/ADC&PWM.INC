;******************************************************************************
;*                                                                            *
;*                       ADC AND PWM MAINTENANCE ROUTINES                     *
;*                                                                            *
;******************************************************************************
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;
;CONTENTS       adc_isr              Vdd_bargraph                             ;
;               pwm_init             tempmode                                 ;
;               getTemp              trackingmode                             ;
;               getVdd               signDisp                                 ;
;               lcdcontrast          HtoD                                     ;
;               getPWM               HtoD_ascii                               ;
;               CalcMaxtrim          dispByteLcd2                             ;
;               absPWM               manual_contrastUp                        ;
;               contrastUp           manual_contrstDn                         ;
;               contrstDn            calibrate_PWM                            ;
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;
        bseg
newtemp:        dbit    1       ;set if temp changed
newVdd:         dbit    1       ;set if vdd changed (beyond 0.1v resolution)
newtrim:        dbit    1       ;set if trim setting changed by user
tracking:       dbit    1       ;set if automatic tracking switched on
bseg_end        set     $

        dseg
adc_temp:       ds      1       ;raw adc temp value
temp:           ds      1       ;scaled temp value
adc_Vdd:        ds      1       ;raw adc vdd value
Vdd:            ds      1       ;scaled vdd value
rawPWM:         ds      1       ;pwm value when trim = 0
trim:           ds      1       ;ajdustable relative offset for pwm (msb = sign)
maxtrim:        ds      1       ;maximum adjustable relative offset
tempcount:      ds      1       ;controls rate of display of temp
vddoffset:      ds      1       ;constant for calibration of PWM (msb = sign)
dseg_end        set     $

        cseg
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;adc_isr        ADC interrupt service routine. Gets adc_Vdd and adc_temp on   ;
;               alternate passes from channels AD0 and AD1 respectively.      ;
;               Generally this routine is invoked once per pass of the main   ;
;               program loop, each time checking alternate channels and       ;
;               setting flags if the values have changed. The adc is restarted;
;               as the program passes through the beginning of the main loop. ;
;                                                                             ;
;ENTRY          by adc interrupt                                              ;
;                                                                             ;
;USES           saves acc                                                     ;
;               psw                                                           ;
;               adcon                                                         ;
;                                                                             ;
;STACK          2                                                             ;
;                                                                             ;
;EXIT           adc_temp                                                      ;
;               adc_Vdd                                                       ;
;               newtemp         set if changed                                ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
adc_isr:
        push    acc
        push    psw
        mov     a,adcon
        jnb     acc.0,adisr1     ;jmp if vdd channel
        setb    newtemp
        mov     a,adc_temp
        cjne    a,adch,adisr2    ;check if temp changed
        clr     newtemp
adisr2: mov     adc_temp,adch
        sjmp    adisrx
adisr1: mov     adc_vdd,adch
adisrx: anl     adcon,#11101111b ; clr adci
        xrl     adcon,#00000001b ; swap channel for next time
        pop     psw
        pop     acc
        reti

;-------- end of adc_isr

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;pwm_init       Initialises the PWM and variables associated with contrast    ;
;               control. Therefore Vdd must first be sampled.                 ;
;               The EEPROM is checked for a valid preset vddoffset which will ;
;               then be used to calibrate the LCD pwm for optimum contrast.   ;
;                                                                             ;
;ENTRY          nil                                                           ;
;                                                                             ;
;USES           acc                                                           ;
;               adcon                                                         ;
;               pwm0                                                          ;
;               vddoffset                                                     ;
;                                                                             ;
;CALLS          getPWM                                                        ;
;               getVdd                                                        ;
;               calcMaxtrim                                                   ;
;               absPWM                                                        ;
;                                                                             ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
pwm_init:
        orl     adcon,#00001000b        ;start adc conversion
        mov     pwmp,#00                ;initialise pwm0
        mov     trim,#0
        mov     vddoffset,#00
        mov     a,#5
        call    delayms
        call    rdeeprom
        clr     newtrim
        call    getVdd
        call    getPWM
        call    calcMaxtrim
        call    absPWM
        ret

;read preset vddoffset from eeprom
rdeeprom:
        mov     r4,#eepromAdr
        mov     r2,#01
        mov     r3,#01
        mov     iicbuff,#0ffh           ;read from last internal address
        call    iic
        mov     a,iicbuff
        clr     acc.7
        cjne    a,#06,$+3
        jnc     rdex
        mov     vddoffset,iicbuff
rdex:   ret

;--------- end of pwm_init

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;calibrate_pwm  This routine loops internally reading the keys and setting the;
;               contrast. It is used to set the optimum contrast for Trim = 0 ;
;               and need only be done once during factory testing. It does    ;
;               this by inc/decrementing the vddoffset constant which is later;
;               combined with Vdd in getPWM to reference the base PWM setting.;
;               PB8 writes the new value to the EEPROM chip in location FFh   ;
;               which is scanned on power up, and exits to main program loop. ;
;                                                                             ;
;ENTRY          p3.7 held low at power up                                     ;
;                                                                             ;
;USES           acc                                                           ;
;               adcon                                                         ;
;               pwm0                                                          ;
;               iicbuff                                                       ;
;               keyvalid                                                      ;
;               vddoffset                                                     ;
;                                                                             ;
;CALLS          getPWM                                                        ;
;               getVdd                                                        ;
;               calcMaxtrim                                                   ;
;               absPWM                                                        ;
;               scan_keys                                                     ;
;               restart_key_scan                                              ;
;               upoffset  (internal)                                          ;
;               dnoffset  (internal)                                          ;
;               iic                                                           ;
;               delayms                                                       ;
;                                                                             ;
;EXIT           Vddoffset                                                     ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
calibrate_pwm:
cpm4:   orl     adcon,#00001000b        ;start adc conversion
        mov     a,#70
        call    delayms                 ;delay for background & key scan rate
        call    scan_keys

        jnb     keyvalid,cpm0
        jnb     acc.1,cpm1              ;if PB2 pressed then
        call    dnoffset                ;decrement Vddoffset to increase contrast
        sjmp    cpm3

cpm1:   jnb     acc.5,cpm2              ;if PB5 pressed then
        call    upoffset                ;increment Vddoffset to decrease contrast
        sjmp    cpm3

cpm2:   jnb     acc.7,cpm3              ;if PB8 pressed then
        call    restart_key_scan        ;prepare for new key press
        setb    keyvalid
        sjmp    cpmx

cpm3:   call    restart_key_scan

cpm0:   call    getVdd
        call    getPWM
        call    calcMaxtrim
        call    absPWM
        sjmp    cpm4
cpmx:   call    restart_key_scan
        setb    keyvalid

;write new vddoffset to the EEPROM
        mov     r4,#eepromAdr
        mov     r2,#02
        mov     r3,#00
        mov     iicbuff,#0ffh
        mov     iicbuff+1,vddoffset
        call    iic
        ret

upoffset:
        mov     a,vddoffset
        jb      acc.7,uof3             ;if sign (+) then inc until max
        inc     a
        cjne    a,#05,uof5
        sjmp    uof4
uof5:   jc      uof4                   ;if less than max then save
        mov     a,#05                  ;else vddoffset:= max
        sjmp    uof4
uof3:   dec     a                      ;sign (-) so dec until 0 and
        cjne    a,#10000000b,uof6
        clr     a
uof6:   jnc     uof4
        clr     a                      ;then change sign
uof4:   mov     vddoffset,a
        ret

dnoffset:
        mov     a,vddoffset
        jnb     acc.7,dof3             ;if sign (+) then jmp
        inc     a
        clr     acc.7
        cjne    a,#05,dof5
dof5:   jc      dof7                   ;if less than max then sav
        mov     a,#05                  ;else vddoffset:= max
dof7:   setb    acc.7                  ;correct sign
        sjmp    dof4
dof3:   dec     a                      ;sign (+) so dec until 0 and
        cjne    a,#0FFh,dof6           ;(until counter rolled over to FF)
        mov     a,#10000001b
dof6:   jc      dof4
        mov     a,#10000001b           ;then change sign
dof4:   mov     vddoffset,a
        ret

;--------- Calibrate_PWM

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;GetTemp        Uses the raw adc count to extract the the scaled temp value   ;
;               in from a table. The table corresponds to the OM5027 temp     ;
;               sensing circuit employing a 10K NTC and a 1.2V reference, and ;
;               produces an output between -15 and +50 deg Celcius.           ;
;                                                                             ;
;ENTRY          adc_Temp                                                      ;
;                                                                             ;
;USES           dptr,acc                                                      ;
;                                                                             ;
;CALLS          nil                                                           ;
;                                                                             ;
;EXIT           Temp                                                          ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
GetTemp:
        mov     a,adc_temp
        clr     c
        subb    a,#30           ;subtract table offset
        jc      gt_error
        jz      gt_error        ;test for legality
        cjne    a,#160,$+3
        jnc     gt_error
        mov     dptr,#temptbl   ;temperature table address
        movc    a,@a+dptr
        mov     temp,a
        ret

gt_error:
        mov     temp,#'-'
        ret

$include(temptbl.ref)           ;table of temerature values

;------------ end of GetTemp

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;GetVdd         Uses the raw adc count to extract the the scaled Vdd value    ;
;               from a table. Flags if scaled value has changed.              ;
;                                                                             ;
;ENTRY          adc_Vdd                                                       ;
;                                                                             ;
;USES           dptr,acc                                                      ;
;                                                                             ;
;CALLS          nil                                                           ;
;                                                                             ;
;EXIT           Vdd                                                           ;
;               newVdd                                                        ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
GetVdd:
        mov     a,adc_Vdd
        mov     dptr,#Vdd_Ref   ;Vdd table address
        movc    a,@a+dptr
        setb    newVdd
        cjne    a,Vdd,gvdx      ;check if Vdd changed
        clr     newVdd
gvdx:   mov     vdd,a
        ret

$include(om5027.ref)            ;table of Vdd values

;-------- end of GetVdd

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;lcdContrast    Controls the LCD automatic contrast adjustment. The PWM output;
;               is regulated to maintain the contrast (Vlcd) at a set point   ;
;               which is determined by both Vdd and the Trim offset.          ;
;                                                                             ;
;ENTRY          newVdd          set if Vdd has changed                        ;
;               newtrim         set if trim setting has changed               ;
;               trim            trim setting                                  ;
;                                                                             ;
;CALLS          getPWM                                                        ;
;               calcMaxtrim                                                   ;
;               absPWM                                                        ;
;                                                                             ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
lcdContrast:
        jnb     newVdd,lc0      ;If Vdd changed
        call    getPWM          ;get pwm(vdd) from table and then
        call    calcMaxtrim     ;calculate new maximum absolute trim.
        jb      newVdd,lc1      ;If Vdd changed
lc0:    jb      newtrim,lc1     ;OR trim changed then
        sjmp    lcx
lc1:    call    absPWM          ;calculate new absolute pwm value
lcx:    ret

;------- end of lcdContrast

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;getPWM         Using (Vdd + VddOffset) as a pointer, this extracts the       ;
;               corresponding PWM value from a table. This value will         ;
;               maintain the specified optimum contrast of Vlcd = 6.5 volts   ;
;               (w.r.t. Vdd) via the dc-dc converter on the OM5027.           ;                     ;
;               (the VddOffset value is determined while in calibration mode  ;
;               to compensate for variation in component tolerances)          ;
;                                                                             ;
;ENTRY          Vdd                                                           ;
;               VddOffset                                                     ;
;                                                                             ;
;USES           acc,b                                                         ;
;               dptr                                                          ;
;                                                                             ;
;EXIT           rawPWM                                                        ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
getPWM: mov     dptr,#pwmtbl
        mov     b,vddoffset
        mov     a,vdd
        jbc     b.7,gpm1        ;jmp if negative
        add     a,b             ;add vddoffset for calibration
        sjmp    gpm2
gpm1:   clr     c
        subb    a,b             ;subtract vdd offset for calibration
gpm2:   clr     c
        subb    a,#20           ;subtract table offset
        cjne    a,#35,$+3
        jc      gpm0
        mov     a,#35
gpm0:   movc    a,@a+dptr       ;get raw PWM for optimum contrast
        mov     rawPWM,a
        ret

$include(pwmtbl.ref)

;------- end of getPWM

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;CalcMaxtrim    Calculates the maximum offset (maxtrim), which may be added to;
;               rawPWM, as a function of the current Vdd. The maximum has been;
;               defined such that lcd2 will still be readable at maximum trim.;
;               This has been defined as maxtrim = rawPWM ö 3 for simplicity. ;
;                                                                             ;
;ENTRY          rawPWM                                                        ;
;                                                                             ;
;USES           acc                                                           ;
;               b                                                             ;
;                                                                             ;
;EXIT           maxtrim                                                       ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
CalcMaxtrim:
        mov     a,rawPWM
        mov     b,#03           ;divide rawPWM by 3 to get new maxtrim
        div     ab
        mov     maxtrim,a
        mov     a,b             ;get remainder
        cjne    a,#02,cmx       ;if remainder = 2
        inc     maxtrim         ;then round up

cmx:    ret

;------- end of CalcMaxtrim

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;absPWM         Calculates the absolute trim offset and adds/subtracts it from;
;               the raw PWM value, using the result to actuate PWM0.          ;
;               i.e. PWM0 = rawPWM ñ (trim*maxtrim)/4                         ;
;                                                                             ;
;ENTRY          trim                                                          ;
;               maxtrim                                                       ;
;               rawPWM                                                        ;
;                                                                             ;
;USES           acc                                                           ;
;               b,c                                                           ;
;                                                                             ;
;EXIT           PWM0                                                          ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
absPWM: mov     a,trim
        clr     acc.7
        mov     b,maxtrim
        mul     ab
        mov     b,#04
        clr     c
        div     ab
        xch     a,b             ;get remainder
        cjne    a,#02,ap0       ;if remainder = 2
        inc     b               ;then round up
                                ;rounded (trim*maxtrim)/4 in b
ap0:    mov     a,trim
        rlc     a               ;get sign bit
        mov     a,rawPWM
        jc      ap1             ;jump if -
        add     a,b
        sjmp    ap2
ap1:    clr     c
        subb    a,b
ap2:    mov     pwm0,a
        ret

;-------- end of absPWM

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;contrastup     Increments the signed variable Trim, which implies a ratio    ;
;               used to determine the lcd contrast.                           ;
;               An upper limit of +4 is enforced.                             ;
;                                                                             ;
;ENTRY          trim                                                          ;
;                                                                             ;
;USES           acc                                                           ;
;                                                                             ;
;EXIT           trim                                                          ;
;               newtrim                                                       ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
contrastup:
        mov     a,trim
        jb      acc.7,cup3             ;if sign (+) then inc until maxtrim
        inc     a
        cjne    a,#04,cup5
        sjmp    cup4
cup5:   jc      cup4                   ;if less than max then save trim
        mov     a,#04                  ;else trim:= max
        sjmp    cup4
cup3:   dec     a                      ;sign (-) so dec until 0 and
        cjne    a,#10000000b,cup6
        clr     a
cup6:   jnc     cup4
        clr     a                      ;then change sign
cup4:   mov     trim,a
        setb    newtrim
        ret
;---------- end of contrastUp

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;contrastDn     decrements the signed variable Trim, which implies a ratio   ;
;               used to determine the lcd contrast.                          ;
;               A lower limit of -3 is enforced.                             ;
;                                                                            ;
;ENTRY          trim                                                         ;
;                                                                            ;
;USES           acc                                                          ;
;                                                                            ;
;EXIT           trim                                                         ;
;               newtrim                                                      ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
contrastdn:
        mov     a,trim
        jnb     acc.7,cnd3             ;if sign (+) then jmp
        inc     a
        clr     acc.7
        cjne    a,#03,cnd5
cnd5:   jc      cnd7                   ;if less than max then save trim
        mov     a,#03                  ;else trim:= maxtrim
cnd7:   setb    acc.7                  ;correct sign
        sjmp    cnd4
cnd3:   dec     a                      ;sign (+) so dec until 0 and
        cjne    a,#0FFh,cnd6           ;(until counter rolled over to FF)
        mov     a,#10000001b
cnd6:   jc      cnd4
        mov     a,#10000001b           ;then change sign
cnd4:   mov     trim,a
        setb    newtrim
        ret
;---------- end of contrastDn

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;manual_contrastup      Increments pwm0 (when tracking turned off)            ;
;                                                                             ;
;ENTRY          nil                                                           ;
;                                                                             ;
;USES           acc                                                           ;
;                                                                             ;
;EXIT           pwm0                                                          ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
manual_contrastup:
        mov     a,pwm0
        clr     c
        add     a,#01h
        cjne    a,#255,mcup2
        sjmp    mcup1
mcup2:  mov     pwm0,a
mcup1:  ret

;------- end of manual_contrastup

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;manual_contrastDn      Decrements pwm0 (when tracking turned off)            ;
;                                                                             ;
;ENTRY          nil                                                           ;
;                                                                             ;
;USES           acc                                                           ;
;                                                                             ;
;EXIT           pwm0                                                          ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
manual_contrastdn:
        mov     a,pwm0
        clr     c
        subb    a,#01h
        jc      mcdn1
        mov     pwm0,a
mcdn1:  ret

;------- end of manual_contrastup

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;Vdd_bargraph   Maintains Vdd bargraph icons. Flags a rewrite of lcd1 if bar  ;
;               graph icons have changed.                                     ;
;                                                                             ;
;ENTRY          mode                                                          ;
;               newvdd                                                        ;
;                                                                             ;
;USES           acc                                                           ;
;                                                                             ;
;EXIT           iconbyte+1                                                    ;
;               pos                                                           ;
;               endpos                                                        ;
;               datatosend                                                    ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;bargraph length equates
v50     equ     00111110b       ;vdd > 5.0v
v45     equ     00111110b       ;5.0v > vdd > 4.5v
v40     equ     00011110b       ;4.5v > vdd > 4.0v
v35     equ     00001110b       ;4.0v > vdd > 3.5v
v30     equ     00000110b       ;3.5v > vdd > 3.0v
v25     equ     00000010b       ;3.0v > vdd > 2.5v

vdd_bargraph:
        jnb     newvdd,vbgx             ;exit if vdd not changed
        mov     a,mode                  ;exit if in lcd1 demo mode (8)
        xrl     a,#08
        jz      vbgx

;compare vdd to threshholds and get appropriate bar length
;if bar length has changed then update
        mov     a,vdd
        cjne    a,#50,$+3
        jc      vbg0
        mov     a,iconbyte+1
        xrl     a,#v50
        jz      vbgx
        mov     iconbyte+1,#v50
        sjmp    vbg5

vbg0:   cjne    a,#45,$+3
        jc      vbg1
        mov     a,iconbyte+1
        xrl     a,#v45
        jz      vbgx
        mov     iconbyte+1,#v45
        sjmp    vbg5

vbg1:   cjne    a,#40,$+3
        jc      vbg2
        mov     a,iconbyte+1
        xrl     a,#v40
        jz      vbgx
        mov     iconbyte+1,#v40
        sjmp    vbg5

vbg2:   cjne    a,#35,$+3
        jc      vbg3
        mov     a,iconbyte+1
        xrl     a,#v35
        jz      vbgx
        mov     iconbyte+1,#v35
        sjmp    vbg5

vbg3:   cjne    a,#30,$+3
        jc      vbg4
        mov     a,iconbyte+1
        xrl     a,#v30
        jz      vbgx
        mov     iconbyte+1,#v30
        sjmp    vbg5

vbg4:   mov     a,iconbyte+1
        xrl     a,#v25
        jz      vbgx
        mov     iconbyte+1,#v25

vbg5:   mov     pos,#8                  ;prepare for lcd1 update
        mov     endpos,#10
        setb    datatosend
vbgx:   ret

;----------- end of Vdd_bargraph

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;TrackingMode   Coordinates display of tracking parameters                    ;
;                                                                             ;
;ENTRY          newlcd2                                                       ;
;               tracking                                                      ;
;                                                                             ;
;USES           r0,r1                                                         ;
;               iicbuff                                                       ;
;               dptr                                                          ;
;                                                                             ;
;CALLS          lcd2_WrScr                                                    ;
;               lcd2_newline                                                  ;
;               lcd2_wrDataStr                                                ;
;               htod_ascii                                                    ;
;               signDisp                                                      ;
;                                                                             ;
;EXIT           newlcd2                                                       ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
TrackingMode:
;initialise screen if first pass
        jnb     newlcd2,tk0
        setb    modmode
        mov     dptr,#tstmsg0
        jb      tracking,tk1
        mov     dptr,#tstmsg1
tk1:    call    lcd2_WrScr
        clr     newlcd2

tk0:    mov     iicbuff+1,#03   ;goto line 3
        call    lcd2_newline
        mov     r0,#iicbuff+1

;load Vdd into display string
        mov     a,vdd
        mov     b,#10
        div     ab
        add     a,#30h
        mov     @r0,a
        inc     r0
        mov     @r0,#'.'
        mov     a,b
        add     a,#30h
        inc     r0
        mov     @r0,a

;leave a space
        inc     r0
        mov     @r0,#' '
        inc     r0
        inc     r0

;load PWM into display string
        mov     a,pwm0
        call    htod_ascii

;leave a space
        inc     r0
        mov     @r0,#' '

;load TRIM into display string
        mov     a,r0
        inc     a
        mov     r1,a
        mov     r0,#trim
        call    SignDisp

;send string to lcd2
        mov     @r1,#04           ;sentinal
        call    lcd2_wrdatastr

;process key data
        jnb     keyvalid,tkx
        mov     a,keybits
        jnb     acc.1,tk4
        jnb     tracking,tk7
        call    contrastUp
        sjmp    tk3
tk7:    call    manual_contrastUp
        sjmp    tk3
tk4:    jnb     acc.5,tk5
        jnb     tracking,tk8
        call    contrastDn
        sjmp    tk3
tk8:    call    manual_contrastDn
        sjmp    tk3
tk5:    jnb     acc.6,tk6
        cpl     tracking
        setb    newlcd2
tk6:    jb      acc.3,tk9
        jnb     acc.7,tk3
tk9:    setb    tracking
        mov     vdd,#0ffh               ;force contrast adjust
        setb    newlcd2
        clr     modmode
        sjmp    tkx
tk3:    call    restart_key_scan
tkx:    ret

tstmsg0:db      'Tracking ON ',04
        db      'Vdd PWM Trim',04
        db      04

tstmsg1:db      'Tracking OFF',04
        db      'Vdd PWM Trim',04
        db      04

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;tempmode       Controls the display of temperature on lcd2 for mode 0.       ;
;                                                                             ;
;                                                                             ;
;ENTRY          tempcount                                                     ;
;               temp                                                          ;
;               newlcd2                                                       ;
;                                                                             ;
;USES           acc,r1                                                        ;
;                                                                             ;
;CALLS          lcd2_WrScr                                                    ;
;               SignDisp                                                      ;
;               lcd2_wrDataStr                                                ;
;               lcd2_wrCodeScr                                                ;
;               lcd2_newline                                                  ;
;               lcd2_indent                                                   ;
;                                                                             ;
;EXIT           tempcount                                                     ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
tempmode:
;initialise screen if first pass
        clr     modmode
        jnb     newlcd2,tmp0
        mov     dptr,#tempscr
        call    lcd2_WrScr
        clr     newlcd2
        mov     tempcount,#05
        sjmp    tmp3

tmp0:   mov     a,tempcount
        djnz    acc,tmp1
        mov     tempcount,#05
tmp3:   mov     iicbuff+1,#03
        call    lcd2_newline            ;select line 3
        mov     r0,#temp
        cjne    @r0,#'-',tmp2           ;test if illegal
        sjmp    tmp_error
tmp2:   mov     iicbuff+1,#02           ;select position
        call    lcd2_indent
        mov     r1,#iicbuff+1
        call    SignDisp                ;format data into iicbuff
        mov     @r1,#04
        call    lcd2_wrdatastr          ;send to lcd2
        sjmp    tmpx
tmp1:   mov     tempcount,a
tmpx:   ret

tmp_error:
        mov     dptr,#tmpscr1
        call    lcd2_wrcodestr
        ret

tempscr:db      '- PHILIPS -',04
        db      'Temperature',04
tmpscr1:db      '   --',0dfh,'C',04

;------------ end of tempmode

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;SignDisp       Takes a signed number pointed to by R0 and separates into     ;
;               the 3 byte format:  sign (ascii), MSD (ascii), LSD (ascii),   ;
;               to by r1, r1+1, r1+2 respectively for display purposes.       ;
;                                                                             ;
;Assumptions    The signed number is a single byte with the MSB = sign (0+,1-);
;               THIS ROUTINE IS ONLY DESIGNED TO HANDLE SIGNED NUMBERS WITH   ;
;               ABSOLUTE VALUE =< 99 !!!                                      ;
;                                                                             ;
;ENTRY          r0       points to signed byte                                ;
;               r1       points to destination of first (sign) byte           ;
;                                                                             ;
;USES           acc,b,c                                                       ;
;               r0,r1                                                         ;
;                                                                             ;
;EXIT           @r1      sign ('-'if -ve and ' ' if +ve )                     ;
;               @r1+1    MSD  (ascii)                                         ;
;               @r1+2    LSD  (ascii)                                         ;
;               r1       incremented 3 times                                  ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
SignDisp:
        mov     @r1,#' '
        mov     a,@r0
        jnb     acc.7,sdp0
        mov     @r1,#'-'
        clr     acc.7
sdp0:   inc     r1
        mov     b,#10
        div     ab
        add     a,#30h
        mov     @r1,a
        inc     r1
        mov     a,b
        add     a,#30h
        mov     @r1,a
        inc     r1
        ret
;---------- end of SignDisp

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;HTOD           Converts the byte in the acc into 3 unpacked bcd numbers.     ;
;                                                                             ;
;ENTRY          R0      points to where the tens digit is to be stored        ;
;                       the hundreds go in R0-1 and the units in R0+1.        ;
;USES           R1      points to hundreds                                    ;
;               ACC                                                           ;
;               B                                                             ;
;EXIT           @R0     unpacked BCD placed here                              ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
htod:   xch     a,r0                    ;get tens pointer
        mov     r1,a                    ;hundreds point to tens
        xch     a,r0                    ;r0 points to tens
        dec     r1                      ;r1 points to hundreds
;registers set up now do the conversion
        mov     b,#100                  ;divide by 100
        div     ab                      ;to determine hundreds
        mov     @r1,a                   ;save hundreds
        mov     a,#10                   ;divide remainder by 10
        xch     a,b
        div     ab                      ;10's in acc, remainder in b
        mov     @r0,a                   ;save 10's
        mov     a,b                     ;get 1's
        inc     r0
        mov     @r0,a                   ;save 1's
        ret
;
;-------------- end of HTOD

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;HTOD_ascii     Converts the byte in the acc into decimal ascii               ;
;               (same as htod but result in ascii)                            ;
;                                                                             ;
;ENTRY          R0      points to where the tens digit is to be stored        ;
;                       the hundreds go in R0-1 and the units in R0+1.        ;
;USES           R1      points to hundreds                                    ;
;               ACC                                                           ;
;               B                                                             ;
;EXIT           @R0     decimal ascii                                         ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
htod_ascii:
        xch     a,r0                    ;get tens pointer
        mov     r1,a                    ;hundreds point to tens
        xch     a,r0                    ;r0 points to tens
        dec     r1                      ;r1 points to hundreds
;registers set up now do the conversion
        mov     b,#100                  ;divide by 100
        div     ab                      ;to determine hundreds
        add     a,#30h
        mov     @r1,a                   ;save hundreds
        mov     a,#10                   ;divide remainder by 10
        xch     a,b
        div     ab                      ;10's in acc, remainder in b
        add     a,#30h
        mov     @r0,a                   ;save 10's
        mov     a,b                     ;get 1's
        inc     r0
        add     a,#30h
        mov     @r0,a                   ;save 1's
        ret
;
;-------------- end of HTOD_ascii


;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;DispByteLCD2   Converts a binary byte to BCD in the iic buffer and displays  ;
;               the on lcd2                                                   ;
;                                                                             ;
;ENTRY          acc     binary byte to display                                ;
;                                                                             ;
;USES           r0,iicbuff                                                    ;
;                                                                             ;
;CALLS          htod_ascii                                                    ;
;               lcd2_wrStr                                                    ;
;                                                                             ;
;EXIT           nil                                                           ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
DispByteLCD2:
        mov     r0,#iicbuff+2
        call    htod_ascii
        inc     r0
        mov     @r0,#04
        call    lcd2_wrdatastr
        ret

;------- end of DispByteLCD2


