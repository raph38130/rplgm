;******************************************************************************
;*                                                                            *
;*                  DRIVER ROUTINES FOR LPH3802-1 LCD MODULE                  *
;*                                                                            *
;******************************************************************************
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;
;CONTENTS:      lcd1_init                                                     ;
;               lcd1_dispinit                                                 ;
;               lcd1_lcd1                                                     ;
;               lcd1_addstr                                                   ;
;               lcd1_send                                                     ;
;               lcd1_convchar   (internal only)                               ;
;               lcd1_convicon       "      "                                  ;
;               lcd1_lcd1demo                                                 ;
;                                                                             ;
;An image of the lcd1 display is located in ram. First modify the image and   ;
;then send it. To add a string the image, place the string in the iicbuff     ;
;and end it with the sentinal 04. Then call lcd1_addstr which will add the    ;
;string to the image and then compute the bounds of new data so that only a   ;
;minimal amount of data be sent to the display. To modify icons, set or reset ;
;the image bits directly, and then set refresh_all so the entire image will   ;
;be sent. To send, call lcd1_send which will encode and send the image        ;
;between the bounds specified by lcd1_addstr (or all if refresh_all = 1).     ;
;                                                                             ;
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;

LCD1adr        equ     078h     ;LPH3802-1 address
LCD1mode       equ     0D5h     ;set mode (with command to follow)
LCD1sel        equ     0E0h     ;define device subaddress (command to follow)
LCD1bank       equ     0FCh     ;start bank 0 (command to follow)
LCD1topram     equ     089h     ;reset RAM data pointer to 9 (command to follow)
LCD1rinc       equ     070h     ;auto increment RAM access (last command)

        bseg
iconbits       set   (8 - ($ MOD 8)) + $

        org     iconbits

lcd1_N:         dbit     1      ;image bits of lcd1 icons
lcd1_SVC:       dbit     1
lcd1_A:         dbit     1
lcd1_B:         dbit     1
lcd1_RM:        dbit     1
lcd1_x1:        dbit     1
lcd1_LB:        dbit     1
lcd1_x2:        dbit     1
lcd1_x3:        dbit     1
lcd1_x4:        dbit     1
lcd1_x5:        dbit     1
lcd1_x6:        dbit     1
lcd1_x7:        dbit     1
lcd1_x8:        dbit     1
dummy1:         dbit     1
dummy2:         dbit     1


lcd1line:       dbit     1       ;0=line1, 1=line2 of LCD1
dataToSend:     dbit     1       ;set if new data in image ram not yet sent
refresh_all:    dbit     1       ;set to refresh all positions in lcd1
bseg_end        set      $

        dseg

iconbyte        data    iconbits/8 + 20h

lcd_line1:      ds      10      ;image of LCD1 line 1 in ascii format
lcd_line2:      ds      10      ;image of  "     "  2  "   "     "
firstpos:       ds      1       ;position on line of first char of string
pos:            ds      1       ;horizontal position of first new data in image
endpos:         ds      1       ;   "           "     " last   "    "   "   "
dseg_end        set     $

        cseg

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;LCD1_init      Initialise LPH3802-1                                          ;
;                                                                             ;
;ENTRY          nil                                                           ;
;                                                                             ;
;USES           iicbuff                                                       ;
;               r0,r1                                                         ;
;               r(1)1,2,3,4                                                   ;
;               ignack                                                        ;
;                                                                             ;
;CALLS          IIC                                                           ;
;                                                                             ;
;EXIT           nil                                                           ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
LCD1_init:
        mov     IICbuff,#LCD1mode
        mov     IICbuff+1,#LCD1sel
        mov     IICbuff+2,#LCD1topram
        mov     IICbuff+3,#LCD1bank
        mov     IICbuff+4,#LCD1rinc
        setb    rs0             ;bank 1
        mov     r1,#IICbuff
        mov     r2,#05          ;tx = 4
        mov     r3,#0           ;rx = 0
        mov     r4,#LCD1adr     ;address = lcd
        setb    ignack
        call    iic             ;send to display
        inc     a
        jz      lcd1_init
        clr     ignack
        clr     rs0             ;select register bank 0
;initialise driver routine variables
        clr     lcd1line
        clr     dataToSend
        mov     firstpos,#00
        mov     pos,#10
        mov     endpos,#01
        setb    refresh_all
lcdinx: ret
;--------end of lcd1_init

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;lcd1DispInit   Refreshes the entire lcd1 date/time display                   ;
;                                                                             ;
;ENTRY          nil                                                           ;
;                                                                             ;
;USES           r0                                                            ;
;                                                                             ;
;EXIT           refresh_all   set                                             ;
;               image cleared                                                 ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
lcd1DispInit:
        mov     r0,#lcd_line1
        mov     r1,#20
ldi0:   mov     @r0,#' '
        inc     r0
        djnz    r1,ldi0
        mov     pos,#01
        mov     endpos,#10
        setb    refresh_all            ;force a full screen refresh
;clear all icon image bits
        mov     iconbyte+1,#00
        mov     iconbyte,#00
        jnb     aset,lcdinx
        mov     iconbyte,#00100000b
        ret

;-------- end of lcd1DispInit


;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;Lcd1_addStr    Adds a string to lcd1 (LPH3802-1) image ram.                  ;
;               Prepares parameters for LCD1_send.                            ;
;                                                                             ;
;ENTRY  IICBUFF         ascii string with sentinal #04                        ;
;       lcd1line        0 for line 1, or 1 for line 2                         ;
;       firstpos        position of first char on line (between 1 and 10)     ;
;                                                                             ;
;USES   r0,r1                                                                 ;
;       ACC                                                                   ;
;                                                                             ;
;EXIT   lcd_line1,                                                            ;
;       lcd_line2       hold LCD image data                                   ;
;       datatosend      set                                                   ;
;       pos             position of first new char nearest the LHS of the lcd ;
;       endpos          position of new char nearest the RHS of the lcd       ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
lcd1_addstr:
;transfer string to appropriate line of image ram
        mov     a,firstpos
        mov     r0,#IICbuff             ;pointer into iicbuff in r0
        jb      lcd1line,lstr0
        add     a,#lcd_line1-1
        mov     r1,a
        sjmp    lstr1
lstr0:  add     a,#lcd_line2-1
        mov     r1,a                    ;pointer into image in r1
lstr1:  cjne    @r0,#04,lstr2           ;check for end of string
        sjmp    lstr3
lstr2:  mov     a,@r0                   ;transfer char
        mov     @r1,a
        inc     r0
        inc     r1
        sjmp    lstr1
;now compare length and position to that of any previous data not yet sent, and
;re-adjust absolute pos and endpos to encompass all new data to be sent
lstr3:  mov     a,r0
        clr     c
        subb    a,#iicbuff              ;calculate last position
        jb      dataToSend,lstr6
        mov     pos,firstpos
        add     a,firstpos
        dec     a
        mov     endpos,a
        sjmp    lstrx
lstr6:  cjne    a,endpos,$+3
        jc      lstr4
        mov     endpos,a
lstr4:  mov     a,firstpos
        cjne    a,pos,$+3
        jnc     lstrx
        mov     pos,firstpos
lstrx:  setb    dataToSend
        ret
;---------end of LCD1_addstr

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;LCD1_SEND     Sends encoded lcd image data corresponding to the character    ;
;              columns between pos and endpos inclusive.                      ;
;                                                                             ;
;ENTRY         lcd_line1,                                                     ;
;              lcd_line2         hold LCD image data                          ;
;              pos               position of 1st new char nearest LHS         ;
;              endpos            position of new char nearest the RHS         ;
;              datatosend        must be set to send anything                 ;
;              refresh_all       set if required to send all positions        ;
;                                (overrides pos, endpos and datatosend)       ;
;USES          iicbuff                                                        ;
;              lcd1line                                                       ;
;              acc,b,r0,r1,r2                                                 ;
;                                                                             ;
;CALLS         LCD1_convChar                                                  ;
;              IIC                                                            ;
;                                                                             ;
;EXIT          datatosend        reset                                        ;
;              pos               set to #10                                   ;
;              endpos            set to #01                                   ;
;              refresh_all       reset                                        ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
LCD1_SEND:
        jnb     refresh_all,l1snd0
        mov     pos,#01
        mov     endpos,#10
        clr     refresh_all
        sjmp    l1snd5

l1snd0: jnb     datatosend,l1sndx
        mov     a,endpos
        cjne    a,pos,$+3
        jc      l1sndx

;reset required amount of iicbuff to all 00h
l1snd5: mov     r1,#30
        mov     r0,#iicbuff+1
        mov     @r0,#00
        inc     r0
        djnz    r1,$-3

;place control byte into iicbuff to specify lcd display RAM address
        mov     a,pos
        dec     a
        mov     b,#03
        mul     ab
        add     a,#lcd1topram
        clr     acc.7           ;last command bit, next bytes are data
        mov     iicbuff,a

;convert and place data into iicbuff for line 1
        mov     r0,#iicbuff+1   ;pointer to data destination
        mov     a,#lcd_line1-1
        add     a,pos
        mov     r1,a            ;pointer to data origin
        clr     lcd1line        ;select line 1
        mov     a,endpos
        clr     c
        subb    a,pos
        inc     a
        mov     r2,a            ;counter initialised
l1snd1: call    lcd1_convChar
        djnz    r2,l1snd1       ;do until all new char converted

;convert and place data into iicbuff for line 2
        mov     r0,#iicbuff+1   ;pointer to data destination
        mov     a,#lcd_line2-1
        add     a,pos
        mov     r1,a            ;pointer to data origin
        setb    lcd1line        ;select line 2
        mov     a,endpos
        clr     c
        subb    a,pos
        inc     a
        mov     r2,a            ;counter initialised
l1snd2: call    lcd1_convChar
        djnz    r2,l1snd2       ;do until all new char converted

;-------------------------------
;convert and place data into iicbuff for icon bits
        mov     r0,#iicbuff+1   ;pointer to data destination
        mov     r1,pos          ;first position to update on lcd
        dec     r1
        mov     a,endpos
        clr     c
        subb    a,pos
        inc     a
        mov     r2,a            ;counter initialised
l1snd3: call    lcd1_convIcon
        djnz    r2,l1snd3       ;do until all new char converted

;-------------------------------

;send iicbuff to lcd1
l1snd20:setb    rs0
        mov     a,endpos
        clr     c
        subb    a,pos
        inc     a
        mov     b,#03
        mul     ab
        inc     a
        mov     r2,a            ;no. of bytes to Tx
        mov     r3,#00          ;no. of bytes to Rx
        mov     r4,#lcd1adr     ;LCD1 address
        setb    ignack          ;IIC to ignore acknolledge for
        call    iic
        inc     a
        clr     ignack
        clr     rs0
        JZ      L1SNDX
        SETB    REFRESH_ALL

l1sndx: clr     datatosend
        mov     pos,#10
        mov     endpos,#01
        ret
;--------end of lcd1_send

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;LCD1_convchar  Converts a single image char into the format required by the  ;
;               LPH3802-1 LCD module, and OR's it with iicbuff data.          ;
;                                                                             ;
;Assumptions    Only the following legal ascii characters are used:-          ;
;               Line 1: '0'-'9', 'A'-'Z', ' ','*','+','-'                     ;
;               Line 2: '0'-'9',' ','-'                                       ;
;               Writing '#' to either line turns on all segments associated   ;
;               with that position.                                           ;
;                                                                             ;
;ENTRY          r0              pointer to iicbuff (for output)               ;
;               r1              pointer to image ram (for input)              ;
;               lcd1line        specifies line of image ram                   ;
;                                                                             ;
;USES           iicbuff                                                       ;
;               acc                                                           ;
;               b,r0,r1,r3                                                    ;
;                                                                             ;
;EXIT           r0              incremented by 3                              ;
;               r1              incremented by 1                              ;
;               iicbuff         (converted char data) AND (iicbuff data)      ;
;                                                                             ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
LCD1_convChar:
        jb      lcd1line,l1cnv1
;convert char from 1st line
        mov     a,@r1
;test for non alpha/numeric characters
        mov     dptr,#lcd1tbl2   ;else specify appropriate table
        cjne    a,#' ',l1cnv2
        mov     a,#26
        sjmp    l1cnv8
l1cnv2: cjne    a,#'*',l1cnv3
        mov     a,#27
        sjmp    l1cnv8
l1cnv3: cjne    a,#'+',l1cnv4
        mov     a,#28
        sjmp    l1cnv8
l1cnv4: cjne    a,#'-',l1cnv20
        mov     a,#29
        sjmp    l1cnv8
l1cnv20:cjne    a,#'#',l1cnv6
        mov     a,#30
        sjmp    l1cnv8

;test for numeric character
l1cnv6: cjne    a,#':',$+3
        jnc     l1cnv7          ;jmp if not 0-9
        mov     dptr,#lcd1tbl1   ;else specify 0-9 table
        clr     c
        subb    a,#30h          ;subtract offset
        sjmp    l1cnv8

;test for alphabetic character
l1cnv7: cjne    a,#'[',$+3
        jnc     l1cnv5          ;jmp if not A-Z
        mov     dptr,#lcd1tbl2   ;else specify A-Z table
        clr     c
        subb    a,#41h          ;subtract offset
        sjmp    l1cnv8

l1cnv5: mov     a,#26            ;' ' for illegal chars
        sjmp    l1cnv8

l1cnv1:
;convert char from line 2
        mov     dptr,#lcd1tbl3
        mov     a,@r1
        cjne    a,#'-',l1cnv11
        mov     a,#10
        sjmp    l1cnv8
l1cnv11:cjne    a,#' ',l1cnv12
        mov     a,#11
        sjmp    l1cnv8

l1cnv12:
;assume character is 0-9
        clr     c
        subb    a,#30h

;multiply by 3 to get table index
l1cnv8: clr     c
        mov     b,#03
        mul     ab
        mov     r3,a
        mov     b,#03
;logical 'OR' 3 bytes from iicbuff with 3 bytes from table
l1cnv10:movc    a,@a+dptr
        orl     a,@r0
        mov     @r0,a
        inc     r0
        inc     r3
        mov     a,r3
        djnz    b,l1cnv10
l1cnvx: inc     r1
        ret
;--------end of lcd1_conv

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;Lcd1_convIcon  Converts any icon bits associated with the current position   ;
;               and OR's them with the appropriate destination in iicbuff.    ;
;                                                                             ;
;ENTRY          r0      destination pointer into iicbuff                      ;
;               r1      points to current position                            ;
;               acc     current position on lcd                               ;
;               iicbuff                                                       ;
;                                                                             ;
;USES           acc                                                           ;
;               dptr                                                          ;
;               r0                                                            ;
;               r1                                                            ;
;                                                                             ;
;EXIT           iicbuff holds formatted data to send                          ;
;               r0      points to next icon bit column in iicbuff             ;
;               r1      incremented to point to next position                 ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
Lcd1_convIcon:
        mov     dptr,#posjmp
        mov     a,r1
        cjne    a,#10,$+3
        jnc     $
        rl      a
        add     a,r1
        jmp     @a+dptr
posjmp:
        ljmp    pos1
        ljmp    pos2
        ljmp    pos3
        ljmp    pos4
        ljmp    pos5
        ljmp    pos6
        ljmp    pos7
        ljmp    pos8
        ljmp    pos9
        ljmp    pos10


pos1:   jb      lcd1_N,lci1
        sjmp    lci2
pos2:   jb      lcd1_SVC,lci1
        sjmp    lci2
pos3:   jb      lcd1_A,lci1
        sjmp    lci2
pos4:   jb      lcd1_B,lci1
        sjmp    lci2
pos5:   jb      lcd1_RM,lci1
        sjmp    lci2
pos6:   jb      lcd1_x1,lci1
        sjmp    lci2
;the rest of the positions are associted with 2 icons each
pos7:   jb      lcd1_LB,ps72
        call    lci5
        sjmp    ps71
ps72:   call    lci4
ps71:   jb      lcd1_x2,lci8
        sjmp    lci9
pos8:   jb      lcd1_x3,ps82
        call    lci5
        sjmp    ps81
ps82:   call    lci4
ps81:   jb      lcd1_x4,lci8
        sjmp    lci9
pos9:   jb      lcd1_x5,ps92
        call    lci5
        sjmp    ps91
ps92:   call    lci4
ps91:   jb      lcd1_x6,lci8
        sjmp    lci9
pos10:  jb      lcd1_x7,ps102
        call    lci5
        sjmp    ps101
ps102:  call    lci4
ps101:  jb      lcd1_x8,lci8
        sjmp    lci9

;set/reset the appropriate bit and superimpose on the formatted data in iicbuff
;then increment the the pointers to point to the next source and destination

;single icon positions
lci1:   mov     a,#10000000b
        orl     a,@r0
        sjmp    lci6
lci2:   mov     a,#01111111b
        anl     a,@r0
lci6:   mov     @r0,a
        inc     r0
        inc     r0
        inc     r0
        inc     r1
        ret

;double icon positions
lci4:   mov     a,#10000000b
        orl     a,@r0
        sjmp    lci7
lci5:   mov     a,#01111111b
        anl     a,@r0
lci7:   mov     @r0,a
        inc     r0
        inc     r0
        ret
lci8:   mov     a,#10000000b
        orl     a,@r0
        sjmp    lci10
lci9:   mov     a,#01111111b
        anl     a,@r0
lci10:  mov     @r0,a
        inc     r0
        inc     r1
        ret
;----------- end of lcd1_convicon

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;lcd1Demo       Controls a display demonstration of characters and icons      ;
;               on lcd1. A long string is pulled accross the top line         ;
;               starburst characters, the 2nd line of 7-seg shows the time,   ;
;               the icons are highlighted one by one.                         ;
;                                                                             ;
;ENTRY          newlcd2         set first time round                          ;
;               demopointer     pointer into string                           ;
;               count           scroll rate counter                           ;
;                                                                             ;
;USES           acc,r1,r2,r3                                                  ;
;                                                                             ;
;CALLS          lcd2_wrScr                                                    ;
;                                                                             ;
;EXIT           lcd_line1       modified                                      ;
;               iconbyte        modified                                      ;
;               pos                                                           ;
;               endpos                                                        ;
;               refresh_all                                                   ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
        bseg
divby2:                 dbit    1       ;halfs rate icon display changes
bseg_end                set     $

        dseg
demoPointer:            ds      1       ;points into string
count:                  ds      1       ;determines rate starburst display moves
dseg_end                set     $

        cseg

lcd1Demo:
        clr     modmode
        jnb     newlcd2,ldd0
        mov     dptr,#demoscr
        call    lcd2_wrscr
        mov     demopointer,#00
        clr     newlcd2
        mov     count,#04
        mov     iconbyte+1,#10000001b
        mov     iconbyte,#00010000b

ldd0:   djnz    count,lddx
        mov     count,#04
        mov     r1,#lcd_line1
        mov     r2,#10
        mov     r3,#00
        mov     dptr,#lcd1msg
ldd2:   mov     a,demopointer
        add     a,r3
        movc    a,@a+dptr
        cjne    a,#EOT,ldd1
        mov     demopointer,#0ffh
ldd1:   mov     @r1,a
        inc     r1
        inc     r3
        djnz    r2,ldd2
        mov     a,demopointer
        inc     a
        mov     demopointer,a

;rotate icons at half the rate
        jb      divby2,ldd3
        clr     c
        mov     a,iconbyte
        rlc     a
        mov     a,iconbyte+1
        rlc     a
        mov     iconbyte+1,a
        mov     a,iconbyte
        rlc     a
        mov     iconbyte,a

ldd3:   cpl     divby2
        mov     pos,#01
        mov     endpos,#10
        setb    datatosend
        setb    refresh_all
lddx:   ret


lcd1msg:
        db      '         *** LPH3802-1 *** STARBURST AND ICON DISPLAY - '
        db      'A  B  C  D  E  F  G  H  I  J  K  L  M  N  O  P  Q  R  S  T  U  '
        db      'V  W  X  Y  Z - - - '
        db      '0  1  2  3  4  5  6  7  8  9  *  +  -            '
        db      'THE QUICK BROWN FOX JUMPED OVER THE LAZY DOGS BACK  ',EOT
demoscr:
        db      '  OM5027    ',04
        db      ' LPH3802-1  ',04
        db      ' EXHIBITION ',04



;-----------------------------------------

LCD1TBL1:
        db      00010101b       ;0
        db      00011000b
        db      00001101b

        db      00000000b       ;1
        db      00000101b
        db      00000000b

        db      00000011b       ;2
        db      00011000b
        db      00000110b

        db      00000010b       ;3
        db      00011000b
        db      00000111b

        db      00000110b       ;4
        db      00000101b
        db      00000010b

        db      00000110b       ;5
        db      00011000b
        db      00000011b

        db      00000111b       ;6
        db      00011000b
        db      00000011b

        db      00000000b       ;7
        db      00001001b
        db      00001000b

        db      00000111b       ;8
        db      00011000b
        db      00000111b

        db      00000110b       ;9
        db      00011000b
        db      00000111b
;---------------------------------------
lcd1tbl2:
        db      00000111b       ;A
        db      00001000b
        db      00000111b

        db      00000000b       ;B
        db      00011101b
        db      00000111b

        db      00000101b       ;C
        db      00011000b
        db      00000000b

        db      00000000b       ;D
        db      00011101b
        db      00000101b

        db      00000111b       ;E
        db      00011000b
        db      00000000b

        db      00000111b       ;F
        db      00001000b
        db      00000000b

        db      00000101b       ;G
        db      00011000b
        db      00000011b

        db      00000111b       ;H
        db      00000000b
        db      00000111b

        db      00000000b       ;I
        db      00000101b
        db      00000000b

        db      00000001b       ;J
        db      00010000b
        db      00000101b

        db      00000111b       ;K
        db      00000000b
        db      00011000b

        db      00000101b       ;L
        db      00010000b
        db      00000000b

        db      00001101b       ;M
        db      00000001b
        db      00001101b

        db      00001101b       ;N
        db      00000000b
        db      00010101b

        db      00000101b       ;O
        db      00011000b
        db      00000101b

        db      00000111b       ;P
        db      00001000b
        db      00000110b

        db      00000101b       ;Q
        db      00011000b
        db      00010101b

        db      00000111b       ;R
        db      00001000b
        db      00010110b

        db      00000110b       ;S
        db      00011000b
        db      00000011b

        db      00000100b       ;T
        db      00001101b
        db      00000100b

        db      00000101b       ;U
        db      00010000b
        db      00000101b

        db      00001000b       ;V
        db      00000001b
        db      00001000b

        db      00010101b       ;W
        db      00000100b
        db      00010101b

        db      00011000b       ;X
        db      00000000b
        db      00011000b

        db      00000110b       ;Y
        db      00000001b
        db      00000110b

        db      00010000b       ;Z
        db      00011000b
        db      00001000b

        db      00000000b       ;SPACE
        db      00000000b
        db      00000000b

        db      00011010b       ;*
        db      00000101b
        db      00011010b

        db      00000010b       ;+
        db      00000101b
        db      00000010b

        db      00000010b       ;-
        db      00000000b
        db      00000010b

        db      11111111b       ;all on
        db      11111111b
        db      11111111b


;-------------------------------------- 7 SEGMENT
LCD1TBL3:
        db      01100000b       ;0
        db      10100000b
        db      01100000b

        db      00000000b       ;1
        db      00000000b
        db      01100000b

        db      01000000b       ;2
        db      11100000b
        db      00100000b

        db      00000000b       ;3
        db      11100000b
        db      01100000b

        db      00100000b       ;4
        db      01000000b
        db      01100000b

        db      00100000b       ;5
        db      11100000b
        db      01000000b

        db      01100000b       ;6
        db      11100000b
        db      01000000b

        db      00000000b       ;7
        db      00100000b
        db      01100000b

        db      01100000b       ;8
        db      11100000b
        db      01100000b

        db      00100000b       ;9
        db      11100000b
        db      01100000b

        db      00000000b       ;-
        db      01000000b
        db      00000000b

        db      00000000b       ;SPACE
        db      00000000b
        db      00000000b

;-------- end of LCD1 routines
