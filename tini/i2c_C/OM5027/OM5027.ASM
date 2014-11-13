$title(OM5027)
$date(20-DEC-94)
$pagewidth(132)
$symbols
$object
;
;     The information presented in this document does not form part
;     of any quotation or contract, is believed to be accurate and
;     reliable and may be changed without notice.  No liability will be
;     accepted by the publisher for any consequence of its use.
;     Publication thereof does not convey nor imply any license under
;     patent- or other industrial or intellectual property rights.
;
;                                               PHILIPS COMPONENTS
;
;******************************************************************************
;*                                                                            *
;*                                 OM5027                                     *
;*                                                                            *
;*                      LOW POWER, LOW VOLTAGE, IIC BUS                       *
;*                                                                            *
;*                            EVALUATION BOARD                                *
;*                                                                            *
;******************************************************************************
;
;******************************************************************************
;*                                                                            *
;*                         DEMONSTRATION SOFTWARE                             *
;*                                                                            *
;*                              SOURCE CODE                                   *
;*                                                                            *
;*                           - MAIN PROGRAM -                                 *
;*                                                    by  M.Vivanti           *
;*                                                        Philips New Zealand *
;*                                                                            *
;******************************************************************************
;
;This source code was produced for the METALINK ASSEMBLER and is segmented
;into 16 files as below.
;
;        OM5027.asm             - main program (this file)
;        general.inc            - general and program flow routines
;        clk.inc                - clock peripheral routines
;        keys.inc               - keyboard peripheral routines
;        adc&pwm.inc            - 80CL580 adc and pwm routines
;        atod.inc               - ADC/DAC peripheral routines
;        dtmf.inc               - dtmf peripheral routines
;        moduleIn.inc           - module information mode routines
;        help.inc               - help routines
;        lcd1.inc               - lcd1 display driver routines
;        lcd2.inc               - lcd2 display driver routines
;        580iic.inc             - 80CL580 iic interface driver routines
;        uart.inc               - uart demonstration routines
;        OM5027.ref             - Vdd scaling reference table *
;        temptbl.ref            - temperature scaling reference table *
;        pwmtbl.ref             - pwm/contrast reference table *
;        PCA8581.inc            - EEPROM driver routine
;                               * all .ref files are included into adc&pwm.inc
;
;******************************************************************************
;*                                                                            *
;*                          REVISION HISTORY                                  *
;*                                                                            *
;******************************************************************************
;
;REVSION         DATE                   MODIFICATION
;
;1.0             5 August, 1994         1st release
;
;1.1             9 September, 1994     1) Clk_init was modified to corrected bug:
;                                       "clock chip reset to 1 January after
;                                        powerup if day greater than 20th".
;
;                                      2) Musicmode now has a new tune.
;
;                                      3) Uart_isr has been modified to include
;                                         a menu with options:
;                                                       Module information,
;                                                       OM5027 remote control,
;                                                       Production information
;
;1.2            15 December, 1994      1) Allow calibration of lcd voltage at
;                                         power up. Invoke setup mode by
;                                         grounding p3.7. Use PB2/PB5 to set
;                                         optimum contrast. Use PB8 to save
;                                         and exit. The calibration offset is
;                                         stored in the EEPROM and read on
;                                         power up. The Calibrate_PWM routine
;                                         has been added to ADC&PWM.INC with
;                                         slight adjustments to PWM_init and
;                                         getPWM. PWMtbl.ref has an extra entry
;******************************************************************************
;*                                                                            *
;*                          SOFTWARE OPERATION                                *
;*                                                                            *
;******************************************************************************
;
;In all modes, except data entry modes e.g. Alter Time (where modmode is set):
;PB2   Increase contrast for both LCD modules
;PB6   Decrease contrast for both LCD modules
;
;0) Temperature Display Mode:
;PB1   Help
;PB8   Exit and go to next mode
;
;1) Time Display Mode                 Alter Time Mode
;PB1   Help                           PB1   Help
;PB4   Exit and go to previous mode   PB8   Enter new value and return
;PB8   Exit and go to next mode       PB2   Increment hours
;PB5   Enter Alter Time mode          PB3   Increment minutes
;                                     PB6   Decrement hours
;                                     PB7   Decrement minutes
;
;2) Alarm Display Mode                Alter Alarm Mode
;PB1   Help                           PB1   Help
;PB4   Exit and go to previous mode   PB8   Enter new value and return
;PB8   Exit and go to next mode       PB2   Increment hours
;PB7   Toggle alarm set ON/OFF        PB3   Increment minutes
;PB5   Enter Alter Alarm mode         PB6   Decrement hours
;                                     PB7   Decrement minutes
;
;3) Date Display Mode                 Alter Date Mode
;PB1   Help                           PB1   Help
;PB4   Exit and go to previous mode   PB5   Enter new value and return
;PB8   Exit and go to next mode       PB2   Increment month
;PB5   Enter Alter Date mode          PB3   Increment day
;                                     PB6   Decrement month
;                                     PB7   Decrement day
;
;4) ADC/DAC Display Mode
;PB1   Help
;PB4   Exit and go to previous mode
;PB8   Exit and go to next mode
;
;A ramping value is written to the DAC output of the PCF8591 to produce a
;sawtooth waveform. This signal is capacitively coupled to the adjacent ADC input
;(if unjumpered) an is susequently read back and displayed to demonstrate the
;functioning of the PCF8591
;
;5)  Vdd and Contrast Parameter Display Mode
;PB1   Help
;PB4   Exit and go to previous mode
;PB8   Exit and go to next mode
;PB7   Toggles contrast control between automatic and manual
;PB2   Contrast up
;PB6   Contrast down
;
;This mode demonstrates the automatic LCD voltage compensation with variations
;in supply voltage. The supply voltage, PWM count, and adjustable trim
;setting may be observed as Vdd or contrast settings are changed. Tracking may
;be turned off and the contrast adjusted manually (in this mode only).
;
;6) Melody Mode
;PB1   Help
;PB4   Exit and go to previous mode
;PB8   Exit and go to next mode
;PB2   Start melody #2
;PB3   Start melody #3
;PB5   Start melody #4
;PB6   Start melody #5
;PB7   Start melody #6
;
;Select one of five melodies to be output by the DTMF/piezo module.
;
;7) Module Information Mode
;PB1   Help
;PB4   Exit and go to previous mode
;PB8   Exit and go to next mode
;PB3   Go to previous module display
;PB7   Go to next module display
;
;Information is provided on each module of the OM5027 including primary I.C name,
;status of connection to the bus, and a descriptive name. Cycle from one module
;screen to the next.
;
;8) Starburst/7-segment LCD Demonstration Mode
;PB1   Help
;PB4   Exit and go to previous mode
;PB8   Exit and go to next mode
;
;An active presentation on the LPH3802-1 LCD showing the variety of characters
;achievable with the starburst display.
;
;HELP
;In any mode context sensitive help is available by pressing PB8. Once in help
;mode keep pressing PB1 to page through the screens until you are returned to
;the original mode. This may take several keypresses depending on the amount
;of help text to display.
;
;RS232
;To demonstrate the RS232 module, information may be displayed on a personal
;computer running a terminal emulation at 9600,8,1,n. Type a single character
;from the terminal emulator to start transmission.
;
;
;
;******************************************************************************
;*                                                                            *
;*                      DEVELOPMENT AND EMULATION                             *
;*                                                                            *
;******************************************************************************
;This source code was produced for the METALINK ASSEMBLER and was
;developed using the PDS51 Develpment System emulating a 87C552 derivative.
;The OM5027 control microcontroller is the 80CL580.
;THE SYSTEM CLOCK IS AT  3.579545 MHz

FALSE           equ     0
TRUE            equ     NOT FALSE

;WHEN USING THE 80CL580 SET EMULATION TO FALSE

EMULATION       equ     False

        IF EMULATION
$mod552
_ead            equ     ead
adc_vector      equ     53h
        ELSE
$modcl580
_ead            equ     ien1.7
adc_vector      equ     73h
        ENDIF

;******************************************************************************
;*                                                                            *
;*                        INTERNAL RAM ORGANIZATION                           *
;*                                                                            *
;******************************************************************************
;
;	LOCATION	USE		COMMENTS
;        0FFh                           top of ram 80CL580
;	 :
;        0C8h         stack             stack starts here
;        :         indirects
;        80h                            80h-0FFh is Indirectly Addressed only
;        :           directs
;        2Ah
;        :             bits
;        20h                            20h to 2Fh are bit addressable
;        1Fh
;        :           bank 3
;        :              8x8             free
;        18h
;        17h
;        :           bank 2
;        :              8x8             used by RS232 routines
;        10h
;        0Fh
;	 :	     bank 1		used by I2C routines
;	 :		8x8
;        08h
;        07h
;	 :	     bank 0		for general use
;	 :		8x8		during programme
;        00h                            operation
;
;
;******************************************************************************
;*                                                                            *
;*                           MACRO DEFINITIONS                                *
;*                                                                            *
;******************************************************************************

revision        macro
                        db      '1.2'   ;firmware revision
                endm

;******************************************************************************
;*                                                                            *
;*                           SYSTEM EQUATES                                   *
;*                                                                            *
;******************************************************************************

bytes_for_bseg  equ     10      ;no. of bit addressable bytes allocated

;Other system equates are defined within the 'include' files and therefore
;close to the routines they are associated with. Refer to the beginning of
;each include file for system equate definitions.

;******************************************************************************
;*                                                                            *
;*                        BIT ADDRESS DEFINITIONS                             *
;*                                                                            *
;******************************************************************************

;The bit addressable segment starts at 20h and 6 bytes have been used out of
;10 allocated. Bit variables are defined within the 'include' files and
;therefore close to the routines they are associated with. Refer to the
;beginning of each include file for bit variable definitions.

;******************************************************************************
;*                                                                            *
;*                        RESERVE AREA FOR STACK                              *
;*                                                                            *
;******************************************************************************

;Put the stack deep into the indirectly addressable segment at location 200
;therefore allowing 56 bytes or 28 CALL levels, and freeing up directly
;addressable locations < 128.

        DSEG
        org     200
stack:          ds      32

;******************************************************************************
;*                                                                            *
;*                        DATA BYTE LOCATIONS                                 *
;*                                                                            *
;******************************************************************************

;10 bytes have been allocated for the bit addressable variables.
;Therefore place directly addressable byte varables above them:

        DSEG
dataorg         equ     bytes_for_bseg + 20h

       org dataorg

;Byte variables are defined within the 'include' files and therefore close
;to the routines they are associated with. Refer to the beginning of each
;include file for byte variable definitions.

;******************************************************************************
;*                                                                            *
;*                          INTERRUPT VECTORS                                 *
;*                                                                            *
;******************************************************************************

        CSEG

        org     0000h           ;reset vector
reset:  sjmp    start
        org     0003h
        reti
        org     000bh           ;T0 vector
        ljmp    T0_process
        org     0013h
        reti
        org     001bh
        reti
        org     0023h
        ljmp    uart_isr        ;S0 vector
        org     adc_vector
        ljmp    adc_isr         ;adc vector

;******************************************************************************
;******************************************************************************
;**                                                                          **
;**                          START FROM RESET                                **
;**                                                                          **
;******************************************************************************
;******************************************************************************

        org     0080h

start:  mov     sp,#stack-1     ;initialize stack pointer
        mov     r0,#00          ;set all ram to 00
st0:    mov     @r0,#00
        djnz    r0,st0
        mov     a,#10           ;10ms delay for modules to power up
        call    delayms
        call    initialize_iic  ;initialise IIC interface
        call    lcd1_init       ;initialise LPH3802-1 LCD module (LCD1)
        call    lcd2_init       ;initialise LPH3827-1 LCD module (LCD2)
        call    init_switches   ;initialise PCF8574A I/O keypad
        setb    aset            ;alarm set
        call    clk_init        ;initialise PCF8593 clock calendar
        setb    _ead            ;enable adc interrupt
        call    uart_init       ;initialise RS232 communications
        setb    ea              ;enable interrupts
        call    pwm_init        ;initialise contrast control
        setb    tracking        ;automatic contrast adjustment ON
        setb    newlcd2         ;flag first mode to refresh LCD2
        mov     module,#00      ;initialise module mode to default
        mov     daCount,#05     ;initialise DAC increment rate counter
        mov     mode,#00        ;set mode to default
        clr     modmode
        setb    keyvalid
        clr     helpmode
        clr     dtmf_in_use
        call    T0_init         ;initialise wake-up-from-idle timer
        mov     vdd,#0ffh       ;force bargraph refresh
        call    signon          ;show sign-on i.e. version number etc.

IF EMULATION
        setb    p4.0
        jb      p4.0,st1
ELSE
        setb    p3.7            ;test request for LCD setup mode
        jb      p3.7,st1        ;issued by holding P3.7 low at power up
ENDIF
        call    Calibrate_PWM   ;invoke LCD setup mode
st1:    call    lcd1dispInit    ;initialise lcd1 image RAM
        setb    refresh_all


;******************************************************************************
;******************************************************************************
;**                                                                          **
;**                             MAIN LOOP                                    **
;**                                                                          **
;******************************************************************************
;******************************************************************************

;------- Main program loop

main:   clr     timesup
        orl     adcon,#00001000b      ;start adc conversion
        call    rdclk                 ;read clock/calendar peripheral
        call    altled                ;alternate LEDs for keyboard peripheral
        call    dac                   ;send ramping value to DAC peripheral
        call    restart_uart          ;restart uart if uart active
        jnb     tracking,mn0
        call    lcdcontrast           ;maintain LCD contrast
mn0:    call    getTemp               ;obtain scaled temperature
        call    getVdd                ;obtain scaled supply voltage
        call    vdd_bargraph          ;service Vdd bargraph on LCD1
        call    lcd1                  ;maintain clock display on LCD1
        call    scan_keys             ;scan keyboard peripheral
mn4:    call    processKeys           ;process key data and invoke selected mode
mn5:    jnb     buserror,mn2
        call    handle_bus_error      ;error recovery
mn2:    jnb     lcd2_noack,mn3
        call    lcd2_init             ;error recovery
        setb    newlcd2
mn3:

IF EMULATION
        mov     a,#50                 ;if emulating use a delay to pad loop
        call    delayms
ELSE
        jb      timesup,main          ;if timer has overflowed loop back
mn1:    orl     pcon,#00000001b       ;else go into idle mode and await
        nop                           ;an interrupt
        nop
        jnb     timesup,mn1
ENDIF
        sjmp    main

;------- end of main

;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
$include(general.inc)
$include(clk.inc)
$include(keys.inc)
$include(adc&pwm.inc)
$include(atod.inc)
$include(dtmf.inc)
$include(moduleIn.inc)
$include(help.inc)
$include(lcd1.inc)
$include(lcd2.inc)
;place the following last as they do not require directly addressable byte ram
$include(580iic.inc)
$include(uart.inc)
;$include(pca8581.inc)
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        if       bseg_end gt bytes_for_bseg * 8
        WARNING  bseg has overlapped into dseg !!!!
	endif

        if       iicbuff gt 080h
        WARNING  variables have been pushed into ISEG !!!!
	endif

        if       dseg_end gt 0C8h
        WARNING  dseg has overlapped into stack !!!!
	endif

endprg	equ	$

        if      endprg gt 7FFFh
        WARNING program is too long !!!!
	endif

        end
