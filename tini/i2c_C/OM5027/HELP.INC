;******************************************************************************
;*                                                                            *
;*                         OM5027 HELP ROUTINES                               *
;*                                                                            *
;******************************************************************************

        bseg
helpmode:       dbit    1       ;set if in help mode
lastScreen:     dbit    1       ;set if last screen of help
bseg_end        set     $

        dseg
linepointer:    ds      1       ;points to line in help text block
dseg_end        set     $

        cseg
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
;HELP        Displays on LCD2 a very brief description of each buttons        ;
;            function depending on the present Mode and whether in Modmode.   ;
;            Each mode has a block of text with its own base pointer in       ;
;            a table. A line pointer is used to specify the top of the screen ;
;            to be displayed. Each block of text must be < 256 and each line  ;
;            must end with the sentinal 04. Each block must be terminated     ;
;            with 00.                                                         ;
;                                                                             ;
;ENTRY       Mode            contains current mode                            ;
;            Modmode         set if in a modify mode (any data entry mode)    ;
;            helpmode        set if already in help mode                      ;
;            lastScreen      set if last help screen is being displayed       ;
;            linepointer     pointer to top of help screen                    ;
;                                                                             ;
;USES        acc,dptr,r1,r2                                                   ;
;                                                                             ;
;CALLS       lcd2_WrPage                                                      ;
;                                                                             ;
;EXIT        helpmode        set while in help mode                           ;
;            lastScreen      cleared after last help screen                   ;
;            newlcd2         signals mother mode for LCD2 refresh after help  ;
;            linepointer     incremented by 3 to page down                    ;
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^;
help:   jb      lastscreen,hlpx
        jb      helpmode,hlp1
        setb    helpmode
        mov     linepointer,#00

;if music mode turn off dtmf
        mov     a,mode
        cjne    a,#06,hlp1
        call    dtmf_off

;get help base for dptr
hlp1:   jb      modmode,hlp3
        mov     a,mode
        rl      a
        add     a,#helptbl-mark1
        movc    a,@a+pc
mark1:  mov     dph,a
        mov     a,mode
        rl      a
        add     a,#05
        movc    a,@a+pc
        mov     dpl,a
        sjmp    hlp0
helptbl:
        dw      tmphlp
        dw      tmhlp
        dw      almhlp
        dw      dthlp
        dw      adchlp
        dw      trkhlp
        dw      melhlp
        dw      modhlp
        dw      demhlp

hlp3:   mov     a,mode
        rl      a
        add     a,#modhelptbl-mark2
        movc    a,@a+pc
mark2:  mov     dph,a
        mov     a,mode
        rl      a
        add     a,#05
        movc    a,@a+pc
        mov     dpl,a
        sjmp    hlp0

modhelptbl:
        dw      tmphlp
        dw      mtmhlp
        dw      malmhlp
        dw      mdthlp
        dw      adchlp
        dw      trkhlp
        dw      melhlp
        dw      modhlp
        dw      demhlp

hlp0:   mov     r2,linepointer
        call    lcd2_WrPage
        mov     a,r1
        movc    a,@a+dptr
        jnz     hlp2
        setb    lastScreen
hlp2:   mov     a,linepointer
        add     a,#03
        mov     linepointer,a
        ret

hlpx:   setb    newlcd2
        mov     linepointer,#00
        clr     helpmode
        clr     lastScreen
        ret

tmphlp:
adchlp:
demhlp:
        db      'PB1 Help ',7eh,04
        db      'PB2 LCD Up  ',04
        db      'PB6 LCD Down',04

        db      'PB1 Help ',7eh,04
        db      'PB4 Mode Up ',04
        db      'PB8 Mode Dn ',04,00

tmhlp:
        db      'PB1 Help ',7eh,04
        db      'PB2 LCD Up  ',04
        db      'PB6 LCD Down',04

        db      'PB1 Help ',7eh,04
        db      'PB5 Set time',04
        db      'PB7 Alarm On',04

        db      'PB1 Help ',7eh,04
        db      'PB4 Mode Up ',04
        db      'PB8 Mode Dn ',04,00

mtmhlp:
malmhlp:
        db      'PB1 Help ',7eh,04
        db      'PB2 Inc. hrs',04
        db      'PB6 Dec. hrs',04

        db      'PB1 Help ',7eh,04
        db      'PB3 Inc. min',04
        db      'PB7 Dec. min',04

        db      'PB1 Help ',7eh,04
        db      'PB8 Enter it',04,04,00


almhlp:
        db      'PB1 Help ',7eh,04
        db      'PB2 LCD Up  ',04
        db      'PB6 LCD Down',04

        db      'PB1 Help ',7eh,04
        db      'PB7 On/Off  ',04
        db      'PB5 Alter it',04

        db      'PB1 Help ',7eh,04
        db      'PB4 Mode Up ',04
        db      'PB8 Mode Dn ',04,00

dthlp:
        db      'PB1 Help ',7eh,04
        db      'PB2 LCD Up  ',04
        db      'PB6 LCD Down',04

        db      'PB5 Set date',04
        db      'PB4 Mode Up ',04
        db      'PB8 Mode Dn ',04,00

mdthlp:
        db      'PB1 Help ',7eh,04
        db      'PB2 Inc. mon',04
        db      'PB6 Dec. mon',04

        db      'PB1 Help ',7eh,04
        db      'PB3 Inc. day',04
        db      'PB7 Dec. day',04

        db      'PB1 Help ',7eh,04
        db      'PB8 Enter   ',04,04,00

trkhlp:
        db      'PB1 Help ',7eh,04
        db      'PB2 LCD Up  ',04
        db      'PB6 LCD Down',04

        db      'PB1 Help ',7eh,04
        db      'PB7 Tracking',04
        db      '    On/Off  ',04

        db      'PB1 Help ',7eh,04
        db      'PB4 Mode Up ',04
        db      'PB8 Mode Dn ',04,00

melhlp:
        db      'PB1 Help ',7eh,04
        db      'PB4 Mode Up ',04
        db      'PB8 Mode Dn ',04

        db      'PB2,3,5,6,7 ',04
        db      'Each give a ',04
        db      'unique tune.',04,00

modhlp:
        db      'PB1 Help ',7eh,04
        db      'PB4 Mode Up ',04
        db      'PB8 Mode Dn ',04

        db      'PB3 Previous',04
        db      'PB7 Next    ',04
        db      '    Module  ',04,00

;---------- end of help
