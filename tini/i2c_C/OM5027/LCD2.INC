;******************************************************************************
;*                                                                            *
;*                   LCD DRIVER ROUTINES FOR LPH3827-1                        *
;*                    WITH CHIP-ON-GLASS 2116A DRIVER                         *
;*                           AND IIC INTERFACE                                *                                                                 *
;*                                                                            *
;*                    M.Vivanti            Philips N.Z. Ltd.                  *
;******************************************************************************
;LCD2 equates
LCD2ctrl        equ     0h              ;lcd2 control byte
LCD2adr         equ     74h             ;lcd2 address
initCmnd        equ     00110000b       ;initilaise command
fnsetCmnd       equ     00110100b       ;function set command
dispsetCmnd     equ     00001110b       ;display set command
readbfCtrl      equ     00100000b       ;read busy flag
dispOffCmnd     equ     00001000b       ;display off command
dispClrCmnd     equ     00000001b       ;clear display command
EntryCmnd       equ     00000110b       ;
homeCmnd        equ     00000010b       ;home cursor command
wrdataCtrl      equ     01000000b       ;write data
clrdispcmnd     equ     00000001b       ;

        bseg
lcd2_noack:     dbit    1               ;set if no ack bit from lcd2
bseg_end        set     $

        cseg

;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;
;The following is a collection of procedures for the control of the LPH3827-1 ;
;LCD module.                                                                  ;
;                                                                             ;
;LCD2_init         initialise LCD after powerup                               ;
;LCD2_clrdisplay   clear display                                              ;
;LCD2_WrCodeStr    send string from code space pointed to by dptr, sentinal 04;
;LCD2_WrDataStr    send string from IIC buffer starting at IICbuff+1, "  04   ;
;LCD2_singledata   send single ascii char from IICbuff at IICbuff+1           ;
;LCD2_home         home                                                       ;
;LCD2_newline      line no.in iicbuff+1                                       ;
;LCD2_indent       indents cursor, no. of spaces to indent in IICbuff+1       ;
;LCD2_clrline      clears present line and sets address to 1st space on line  ;
;readbf            delays until LCD not busy. Returns current DDRAM address   ;
;send1Cmnd         sends a single command                                     ;
;LCD2_shift        shifts display left (iicbuff+1 = 0), right (iicbuff+1 = 1) ;
;                              visible address range                          ;
;                      line 1:       00 to 13h                                ;
;                      line 2:       20 to 33h                                ;
;                      line 3:       40 to 53h                                ;
;                      line 4:       60 to 73h  (only visible when scrolled)  ;
;                                                                             ;
;lcd2_wrScr            writes a page of text (3 lines)  see below             ;
;lcd2_wrPage           "         "               "       "    "               ;
;lcd2_wrPageStr                                                               ;
;delayms                                                                      ;
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;LCD2_init      Software initialisation of LPH3827-1                          ;
;                                                                             ;
;ENTRY          nil                                                           ;
;                                                                             ;
;USES           acc                                                           ;
;               r(1)2,3,4                                                     ;
;               IICbuff                                                       ;
;                                                                             ;
;CALLS          Send1Cmnd                                                     ;
;               delayms                                                       ;
;               readbf                                                        ;
;               IIC                                                           ;
;                                                                             ;
;EXIT           nil                                                           ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
LCD2_init:
        setb    rs0
        clr     lcd2_noack
        mov     a,#15
        lcall   delayms                 ;wait 15ms
        mov     IICbuff,#lcd2ctrl
        mov     IICbuff+1,#fnsetCmnd
        lcall   send1Cmnd               ;send function set
        mov     a,#5                    ;wait 5ms
        lcall   delayms
        mov     IICbuff,#lcd2ctrl
        mov     IICbuff+1,#fnsetCmnd
        lcall   send1Cmnd               ;send function set
        mov     a,#01
        lcall   delayms                 ;wait 1ms
        mov     IICbuff,#lcd2ctrl
        mov     IICbuff+1,#fnsetCmnd
        lcall   send1Cmnd
        mov     a,#05
        lcall   delayms                 ;wait 5ms
        mov     IICbuff,#lcd2ctrl
        mov     iicbuff+1,#fnsetcmnd
        lcall   send1Cmnd
        mov     iicbuff,#lcd2ctrl
        mov     iicbuff+1,#dispOffcmnd
        lcall   send1cmnd
;        mov     iicbuff,#lcd2ctrl      ;not required for OM5027 as whole
;        mov     iicbuff+1,#dispclrcmnd ;display is rewritten
;        lcall   send1cmnd
        lcall   readbf
        mov     iicbuff,#lcd2ctrl
        mov     iicbuff+1,#entrycmnd
        lcall   send1Cmnd
        lcall   readbf
;display on, blink off
l2i0:   mov     IICbuff,#lcd2ctrl
        mov     IICbuff+1,#dispsetCmnd
        mov     r2,#2
        mov     r3,#0
        mov     r4,#LCD2adr
        lcall   IIC
        djnz    acc,$+4                 ;if no acknowledge set buserrror
        setb    lcd2_noack              ;so lcd2 will be reinitialised
        inc     a
        inc     a
        jz      l2i0
        clr     rs0
        ret
;--------end of LCD2init

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;LCD2_clrdisplay                                                              ;
;                                                                             ;
;ENTRY          nil                                                           ;
;                                                                             ;
;USES           acc                                                           ;
;               iicbuff                                                       ;
;                                                                             ;
;CALLS          delayms                                                       ;
;               send1cmnd                                                     ;
;                                                                             ;
;EXIT           nil                                                           ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
LCD2_clrdisplay:
        setb    rs0
        mov     iicbuff,#00
        mov     iicbuff+1,#clrdispcmnd
        lcall   send1cmnd
        mov     a,#04
        lcall   delayms
        clr     rs0
        ret
;--------end of clrdisplay

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;LCD2_WrDataStr  Writes data string to ddram starting at current ddram address;
;                Writes from left to right without the screen scrolling       ;
;                                                                             ;
;ENTRY          IICbuff         holds string which starts at #IICbuff+1 and   ;
;                               ends with sentinal #04                        ;
;                                                                             ;
;USES           r(1)1,2,3,4                                                   ;
;                                                                             ;
;CALLS          IIC                                                           ;
;                                                                             ;
;EXIT           nil                                                           ;
;                                                                             ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
LCD2_WrDataStr:
        setb    rs0
l2wds0: mov     r2,#00h
        mov     iicbuff,#wrdatactrl
        mov     r1,#IICbuff
        inc     r1
        inc     r2
        cjne    @r1,#04,$-2             ;count how many to send in r2
        mov     r3,#0
        mov     r4,#LCD2adr
        lcall   IIC
        djnz    acc,$+4                 ;if no acknowledge set buserrror
        setb    lcd2_noack                ;so lcd2 will be reinitialised
        inc     a
        inc     a
        jz      l2wds0
        clr     rs0
        ret
;--------end of WrDataStr

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;LCD2_WrCodeStr Write string from code space pointed to by dptr, and ending    ;
;               with sentinal #04. Writes from left to right without scrolling.;
;                                                                              ;
;ENTRY          dptr           points to string in code space with sentinal #04;
;                                                                              ;
;USES           acc                                                            ;
;               r(1)1,2,3,4,5,6                                                ;
;                                                                              ;
;STACK          2                                                              ;
;                                                                              ;
;CALLS          IIC                                                            ;
;                                                                              ;
;EXIT           dptr            points to 1st code byte after sentinal         ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
LCD2_WrCodeStr:
        setb    rs0
        mov     r5,dpl          ;store initial dptr in case need to re-transmit
        mov     r6,dph
wcs0:   mov     r1,#IICbuff
        mov     r2,#00h
wcs1:   inc     r1
        inc     r2
        clr     a
        movc    a,@a+dptr
        mov     @r1,a
        inc     dptr
        cjne    a,#04,wcs1
        push    dpl
        push    dph
        mov     IICbuff,#wrdatactrl
        mov     r3,#00
        mov     r4,#LCD2adr
        lcall   IIC
        pop     dph
        pop     dpl
        djnz    acc,$+4                 ;if no acknowledge set buserrror
        setb    lcd2_noack                ;so lcd2 will be reinitialised
        inc     a
        inc     a
        jz      wcserr
        clr     rs0
        ret

wcserr: mov     dpl,r5
        mov     dph,r6
        sjmp    wcs1
;--------end of wrcodestr

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;LCD2_newline   Resets ddram address to beginning of specified line.          ;
;                                                                             ;
;ENTRY          IICbuff+1       holds line number 01..04                      ;
;                                                                             ;
;USES           acc                                                           ;
;                                                                             ;
;CALLS          Send1Cmnd                                                     ;
;                                                                             ;
;EXIT           nil                                                           ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
LCD2_newline:
        setb    rs0
        mov     a,iicbuff+1
        jz      nlx
        dec     a
        swap    a
        mov     iicbuff+1,a
        add     a,iicbuff+1
        setb    acc.7                   ;complete command byte
        mov     iicbuff+1,a
        mov     iicbuff,#00
        lcall   send1Cmnd
nlx:    clr     rs0
        ret
;--------end of newline_LCD2

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;LCD2_indent    Resets DDRAM address corresponding to the specified           ;
;               indentation on the current line                               ;
;                                                                             ;
;Assumption     The current ddram address is at the beginning of the line     ;
;               and the number to indent is just an offset added to it.       ;
;                                                                             ;
;ENTRY          IICbuff+1       holds number of spaces to indent              ;
;                                                                             ;
;USES           acc                                                           ;
;                                                                             ;
;CALLS          Send1Cmnd                                                     ;
;               readbf                                                        ;
;EXIT           nil                                                           ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
LCD2_indent:
        setb    rs0
        lcall   readbf
        clr     acc.7
        add     a,iicbuff+1
        setb    acc.7                   ;complete command byte
        mov     iicbuff+1,a
        mov     iicbuff,#00
        lcall   send1Cmnd
        clr     rs0
        ret
;--------end of indent_LCD2

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;LCD2_home      Resets DDRAM address to #00 and returns a shifted display to  ;
;               its original position.                                        ;
;                                                                             ;
;ENTRY          nil                                                           ;
;                                                                             ;
;USES           acc                                                           ;
;                                                                             ;
;CALLS          Send1Cmnd                                                     ;
;               delayms                                                       ;
;                                                                             ;
;EXIT           nil                                                           ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
LCD2_home:
        setb    rs0
        mov     iicbuff,#00
        mov     iicbuff+1,#homeCmnd
        lcall   send1Cmnd
        mov     a,#02
        lcall   delayms
        clr     rs0
        ret
;--------end of home

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;LCD2_singledata  Sends a single ascii character from IICbuff+1               ;
;                                                                             ;
;ENTRY          iicbuff+1       holds char to send                            ;
;                                                                             ;
;USES           acc                                                           ;
;                                                                             ;
;CALLS          Send1Cmnd                                                     ;
;                                                                             ;
;EXIT           nil                                                           ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
LCD2_singledata:
        setb    rs0
        mov     iicbuff,#wrdatactrl
        lcall   send1cmnd
        clr     rs0
        ret
;--------end of singledata_LCD2:

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;LCD2_clrline   Clears current line and sets ddram address to first position  ;
;                                                                             ;
;ENTRY          nil                                                           ;
;                                                                             ;
;USES           r(1)1,2,3,4                                                   ;
;               acc                                                           ;
;               iicbuff                                                       ;
;                                                                             ;
;CALLS          Send1Cmnd                                                     ;
;               IIC                                                           ;
;                                                                             ;
;EXIT           nil                                                           ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
LCD2_clrline:
        setb    rs0
        mov     a,#13h
        mov     r1,#iicbuff+3
cl4:    mov     @r1,#' '
        inc     r1
        djnz    acc,cl4
        lcall   readbf
        clr     acc.7
        clr     c
        cjne    a,#20h,$+3
        jnc     cl1
        mov     iicbuff+1,#80h
        ljmp    cl3
cl1:    cjne    a,#40h,$+3
        jnc     cl2
        mov     iicbuff+1,#0A0h
        ljmp    cl3
cl2:    cjne    a,#60h,$+3
        jnc     cl5
        mov     iicbuff+1,#0C0h
        ljmp    cl3
cl5:    mov     iicbuff+1,#0E0h
cl3:    mov     a,#lcd2ctrl
        setb    acc.7              ;command following
        mov     iicbuff,a
        mov     iicbuff+2,#wrdatactrl
        mov     r2,#17h
        mov     r3,#00
        mov     r4,#lcd2adr
        lcall   iic
        djnz    acc,$+4            ;if no acknowledge set buserrror
        setb    lcd2_noack         ;so lcd2 will be reinitialised
        inc     a
        inc     a
        jz      lcd2_clrline
        lcall   send1Cmnd          ;reset address to first char
clx:    clr     rs0
        ret
;--------end of clrline_LCD2

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;LCD2_shift     Shifts display left or right by 1 position                    ;
;                                                                             ;
;ENTRY          iicbuff+1     holds #00 for left shift, or #01 for right shift;
;                                                                             ;
;USES           r(1)1,2,3,4                                                   ;
;               acc                                                           ;
;               iicbuff                                                       ;
;                                                                             ;
;CALLS          Send1Cmnd                                                     ;
;               IIC                                                           ;
;                                                                             ;
;EXIT           nil                                                           ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
LCD2_shift:
        setb    rs0
        mov     iicbuff,#0
        mov     a,iicbuff+1
        jnz     shft1
        mov     iicbuff+1,#011000b
        ljmp    shft2
shft1:  mov     iicbuff+1,#011100b
shft2:  lcall   send1cmnd
        clr     rs0
        ret
;--------end of shift_LCD2:

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;LCD2_WrScr     Clears screen and writes 3 lines of text from code space.     ;
;                                                                             ;
;Assumptions    3 consecutive lines are expected, each ending with sentinal#04;
;                                                                             ;
;ENTRY          dptr            enter with address of first line in code space;
;                                                                             ;
;CALLS          lcd2_clrScr                                                   ;
;               lcd2_newline                                                  ;
;               lcd2_wrcodestr                                                ;
;                                                                             ;
;EXIT                                                                         ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
LCD2_WrScr:
        call    lcd2_clrDisplay
        mov     iicbuff+1,#1
        call    lcd2_newline
        call    lcd2_wrcodestr
        mov     iicbuff+1,#2
        call    lcd2_newline
        call    lcd2_wrcodestr
        mov     iicbuff+1,#3
        call    lcd2_newline
        call    lcd2_wrcodestr
        ret
;--------- end of LCD2_WrStr


;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;LCD2_WrPage    Clears the screen and then writes a page (3 lines) of text to ;
;               lcd2. DPTR locates the beginning of the block of text from    ;
;               which the page (line number of page base in r2) is extracted. ;
;               Hence text block must be < 256 bytes. Each line must be ended ;
;               with the sentinal 04.                                         ;
;                                                                             ;
;ENTRY          dptr       point to beginning of block of text                ;
;               r2         line number                                        ;
;                                                                             ;
;USES           r0,r1,r2                                                      ;
;               acc                                                           ;
;                                                                             ;
;CALLS          lcd2_WrPageStr                                                ;
;               lcd2_clrScreen                                                ;
;                                                                             ;
;EXIT           dptr       preserved                                          ;
;               r2         linepointer preserved                              ;
;               r1         current char pointer                               ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
LCD2_WrPage:
;first work out the byte offset corresponding to the line number
        mov     r0,#00
        mov     r1,#00
        mov     a,r2
        jz      wrp1
        clr     a
wrp0:   mov     a,r1
        movc    a,@a+dptr
        inc     r1                      ;byte offset in r1
        cjne    a,#04,wrp0
        inc     r0
        mov     a,r0
        xrl     a,r2
        jnz     wrp0

wrp1:   call    lcd2_clrDisplay
        mov     iicbuff+1,#01
        call    lcd2_newline
        mov     a,r1
        call    lcd2_wrPageStr
        mov     r1,a
        mov     iicbuff+1,#02
        call    lcd2_newline
        mov     a,r1
        call    lcd2_wrPageStr
        mov     r1,a
        mov     iicbuff+1,#03
        call    lcd2_newline
        mov     a,r1
        call    lcd2_wrPageStr
        mov     r1,a
        ret
;--------- end oflcd2_WrPageStr

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;LCD2_WrPageStr DPTR locates the base of the block of text from               ;
;               which the page string (offset pointed to by acc) is extracted.;
;               Hence text block must be < 256 bytes. The page string must be ;
;               terminated with the sentinal 04.                              ;
;                                                                             ;
;                                                                             ;
;ENTRY          dptr       point to beginning of block of text                ;
;               acc        points to page string within block                 ;
;                                                                             ;
;USES           r(1)0,1,2,3,4,5                                                 ;
;               iicbuff                                                       ;
;               acc                                                           ;
;                                                                             ;
;CALLS          iic                                                           ;
;                                                                             ;
;EXIT           dptr       preserved                                          ;
;               acc        pointer returned incremented by number of chars    ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
LCD2_WrPageStr:
        setb    rs0
        mov     r5,a            ;store in r5 in case of need to retransmit
wps1:   mov     a,r5
        mov     r0,a
        mov     r1,#IICbuff
        mov     r2,#00h
wps0:   mov     a,r0
        inc     r1
        inc     r2
        movc    a,@a+dptr
        mov     @r1,a
        inc     r0
        cjne    a,#04,wps0
        push    dpl
        push    dph
        mov     IICbuff,#wrdatactrl
        mov     r3,#00
        mov     r4,#LCD2adr
        lcall   IIC
        pop     dph
        pop     dpl
        djnz    acc,$+4                 ;if no acknowledge set buserrror
        setb    lcd2_noack              ;so lcd2 will be reinitialised
        inc     a
        inc     a
        jz      wps1
        mov     a,r0
        clr     rs0
        ret
;------ end of lcd2_wrPageStr

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;send1Cmnd      Sends a single command                                        ;
;                                                                             ;
;ENTRY          iicbuff         holds control byte                            ;
;               iicbuff+1       holds command byte                            ;
;                                                                             ;
;USES           R2,R3,R4                                                      ;
;                                                                             ;
;STACK          2                                                             ;
;                                                                             ;
;CALLS          IIC                                                           ;
;                                                                             ;
;EXIT           dptr            preserved                                     ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
send1cmnd:                              ;sends one command
        mov     r2,#2
        mov     r3,#0
        mov     r4,#LCD2adr
        push    dpl
        push    dph
        lcall   iic
        pop     dph
        pop     dpl
        djnz    acc,$+4                 ;if no acknowledge set buserrror
        setb    lcd2_noack              ;so lcd2 will be reinitialised
        inc     a
        inc     a
        jz      send1cmnd
        ret
;--------end of send1cmnd

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;readbf         Read busy flag and proceed only when not busy.                ;
;               Returns ddram address counter.                                ;
;                                                                             ;
;ENTRY          nil                                                           ;
;                                                                             ;
;USES           R2,R3                                                         ;
;                                                                             ;
;CALLS          IIC                                                           ;
;                                                                             ;
;EXIT           acc     holds current ddram address counter                   ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
readbf:
        mov     iicbuff,#lcd2ctrl
        mov     r2,#01
        mov     r3,#01
        lcall   iic
        djnz    acc,$+4                 ;if no acknowledge set buserrror
        setb    lcd2_noack              ;so lcd2 will be reinitialised
        inc     a
        inc     a
        jz      readbf
        mov     a,iicbuff
        jb      acc.7,readbf
        ret
;--------end of readbf

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;delayms         Delays for a specified number milliseconds (approx only)     ;
;                                                                             ;
;ENTRY           ACC            number of millisecond units in Acc;           ;
;                                                                             ;
;USES            R2                                                           ;
;                ACC                                                          ;
;                                                                             ;
;EXIT            nil                                                          ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
delayms:
        mov     r2,#149
        djnz    r2,$
        djnz    acc,delayms
        ret
;--------end of delayms

;--------end of LPH3827-1 routines
