;****************************************************************************
;*
;*  Copyright (C) 2004 Dallas Semiconductor Corporation. 
;*  All rights Reserved. Printed in U.S.A.
;*  This software is protected by copyright laws of
;*  the United States and of foreign countries.
;*  This material may also be protected by patent laws of the United States 
;*  and of foreign countries.
;*  This software is furnished under a license agreement and/or a
;*  nondisclosure agreement and may only be used or copied in accordance
;*  with the terms of those agreements.
;*  The mere transfer of this software does not imply any licenses
;*  of trade secrets, proprietary technology, copyrights, patents,
;*  trademarks, maskwork rights, or any other form of intellectual
;*  property whatsoever. Dallas Semiconductor retains all ownership rights.
;*
;*     Module Name: AudioPlayer
;*
;*     Description: Plays canned audio messages from a memory address
;*
;*        Filename: audioplayer.a51
;*
;* Dependant Files: tini.inc, ds80c390.inc, tinimacro.inc apiequ.inc
;*
;*          Author: KLA
;*
;*           Tools: macro + a390
;*
;*         Version: 0.01 
;*
;*         Created: 03/25/2004
;*
;*           Notes: Native library rules:
;*                  - No ACalls or AJmps -> this module can span segments.
;*                  - The module must be contiguous. No ORG statements
;*                    allowed. The module offsets are automatically
;*                    set to zero by the assembler.
;*                  - There are absolutely no directs that persist
;*                    across method calls. Certain ranges of directs are
;*                    reserved for global system variables. You can be
;*                    process swapped at any time. Touch them and you
;*                    scrog the system!!!
;*                    At the time this was written this includes 
;*                    20h-21h and 68h and above.
;*                    All other directs MUST be returned to the
;*                    state that they were in before the method was called.
;*                  - Indirects are reserved for system use only. Touch them
;*                    and you scrog the system!!! As of release 1.02 a handful
;*                    of "indirects" have been reserved for native libraries.
;*                    See Native_Methods.txt for details.
;*                  - Register banks 0, 1 and 3 are free for library use.
;*                    Register bank 2 can only be used after method
;*                    parameters are removed from the Java stack.
;*                  - The file tini.inc contains the address of all
;*                    exported systems functions. These are static linkages
;*                    that are tied to a specific firmware release. Your
;*                    module MUST be reassembled for other firmware revisions.
;*                  - The file apiequ.inc contains the numbers that represent
;*                    exceptions and class numbers. These are static linkages
;*                    tied to a specific firmware release. Your module MUST
;*                    be reassembled with this file for each release.
;*                  - Threads in native methods are not automatically
;*                    swapped by the thread scheduler. Details on swapping
;*                    from native methods are contained in Native_Methods.txt. 
;*                  - We reserve the right to change these rules in future
;*                    releases.
;*                  - See the native method Readme file in the distribution
;*                    for more details.
;*
;****************************************************************************
$include(tini_400.inc)
$include(ds80c400.inc)
$include(tinimacro.inc)
$include(apiequ.inc)


;
; Should this be 1338?
;
RELOAD_COUNT equ 1000

TIMER_RELOAD equ 65535 - RELOAD_COUNT
TIMER_RELOAD_H equ High(TIMER_RELOAD)     
TIMER_RELOAD_L equ Low(TIMER_RELOAD)

;;; TIMER DEFINITIONS *****************************************************
;
; We are using timer 3 for the 11.025 KHz clock
;
TIMER_LOW     equ tl3
TIMER_HIGH    equ th3
TIMER_VECTOR  equ 04Bh


SETUP_TIMER  MACRO
   ; use timer 3 for our timer
   ; we are running at 18.432 MHz
   mov t3cm, #21h
   ENDM

TIMER_PRIORITY_HIGH  MACRO
  setb pt3                       ; set timer 3 priority high
  ENDM

ENABLE_TIMER_INTERRUPT  MACRO
  setb et3                       ; enable timer 3 interrupt
  ENDM

DISABLE_TIMER_INTERRUPT  MACRO
  clr et3                        ; disable timer 3 interrupt
  ENDM

ENABLE_TIMER  MACRO
  orl  t3cm, #40h                ; enable timer 3, 0100 0000
  ENDM

DISABLE_TIMER  MACRO
  anl  t3cm, #0BFh                ; enable timer 3, 1011 1111
  ENDM

CLEAR_TIMER_INTERRUPT  MACRO
   ; The clear of the timer 3 bit should be handled by hardware
   anl t3cm, #7Fh        ; timer 3, shouldn't need to do nothin'
   ENDM


;;; DAC SIGNALING *****************************************************

CLOCK_HIGH  MACRO
   setb P1.0
   ENDM

CLOCK_LOW  MACRO
   clr P1.0
   ENDM

DATA_BIT equ P1.1



;
; Turns DAC chip select ON
;
SELECT_CHIP_ON  MACRO
   ;clr      P1.0
   anl  P5, #7fh
   ENDM

;
; Turns DAC chip select OFF
;
SELECT_CHIP_OFF  MACRO
;   setb     P1.0
   orl P5, #80h
   ENDM

;
; Turns LoadDAC signal OFF
;
LOADDAC_OFF  MACRO
;   setb     P1.1
   orl P5, #40h
   ENDM

;
; Turns LoadDAC signal ON
;
LOADDAC_ON  MACRO
;   clr      P1.1
   anl P5, #0BFh
   ENDM

;;; (IN)DIRECT MEMORY ALLOCATIONS *****************************************

DataPointer_L           equ 0F0h
DataPointer_H           equ 0F1h
DataPointer_X           equ 0F2h

LengthRemaining_L       equ 0F3h 
LengthRemaining_H       equ 0F4h
LengthRemaining_X       equ 0F5h


; select data pointer 2 (pointer 3 of 4) for auto-inc
DPS_DPTR2_AUTO_INC   equ 018h   

;
;***************************************************************************
;*
;* Function Name: audioplayer_Init
;*
;*   Description: Initialization routine
;*
;*      Input(s): None
;*
;*    Outputs(s): A - 0 if success
;*                
;*         Notes: Every TINI library is required to have an initialization
;*                function and the initialization routine MUST be located
;*                at offset zero into the file. This function is called at the
;*                end of the Java load/loadLibrary method invocation. If
;*                this init routine returns with any non-zero value in ACC
;*                the load will fail with an UnsatisfiedLinkError. This 
;*                function should be used to malloc memory and/or initialize
;*                data/hardware.
;*                This function will be called each time load/loadLibrary
;*                is called. State should be checked in order to determine
;*                whether or not initialization is required.
;*
;*                The minimum init routine must include:
;*                  clr a
;*                  ret
;*
;* *** This routine must exist ***
;* *** This routine must exist ***
;* *** This routine must exist ***
;* *** This routine must be at offset 0 ***
;* *** This routine must be at offset 0 ***
;* *** This routine must be at offset 0 ***
;*
;****************************************************************************
audioplayer_Init:
    ;
    ; Install our interrupt handler.  Copy 'ljmp MY_INTERRUPT_HANDLER' 
    ; to the proper vector.
    ;
    mov     dptr, #INT_HANDLER
    movx    a, @dptr
    inc     dptr
    mov     r0, a
    movx    a, @dptr
    inc     dptr
    mov     r1, a
    movx    a, @dptr
    inc     dptr
    mov     r2, a
    movx    a, @dptr
    inc     dptr
    mov     r3, a

    mov     dpx, #0
    mov     dph, #0
    mov     dpl, #TIMER_VECTOR                ; address for timer 1 interrupt vector


    ;
    ; Now copy the handler...
    ;
    mov     a, r0
    movx    @dptr, a
    inc     dptr
    mov     a, r1
    movx    @dptr, a
    inc     dptr
    mov     a, r2
    movx    @dptr, a
    inc     dptr
    mov     a, r3
    movx    @dptr, a
    inc     dptr

    mov     scon2, #20h               ; setup up serial port 2 for mode 0, divide by 4
    anl     scon2, #0FDh              ; clear the ti2 bit

    ;
    ; make sure timer 3 starts disabled
    ;
    DISABLE_TIMER_INTERRUPT
    ;
    ; make sure timer 3 runs as high priority
    ;
    TIMER_PRIORITY_HIGH

    ;
    ; Make sure DAC signals are in a known state
    ;
    SELECT_CHIP_OFF
    LOADDAC_OFF

    push    ie                      ; Save interrupt state.
    clr     ea                      ; Disable interrupts.

    mov     TA,#0AAH
    mov     TA,#055H

    anl     P5CNT,#000h     ; Set P5.4-P5.7 for use as port pins.
    pop     ie                      ; Restore interrupt state.



    clr     a
    ret

;****************************************************************************

LibraryID:      ;*
db "rdrocket"   ;*

INT_HANDLER:
  ljmp mytimer3interrupt

;****************************************************************************
;*
;* Function Name: Native_getNumberOfAudios
;*
;*   Description: public static native int getNumberOfAudios(int address);
;*
;*      Input(s): address of flash audio image
;*
;*    Outputs(s): A - zero on success, exception number on failure.
;*                returns an int in R3:R2:R1:R0, msb:...:lsb.
;*                
;****************************************************************************
Native_getNumberOfAudios:
  clr    a                       ; request first parameter
  lcall  NatLib_LoadPrimitive    ; get address in r3:r2:r1:r0

  ; ignore whatever r3 had
  mov    dpx, r2
  mov    dph, r1
  mov    dpl, r0

  mov    r3, #0                  ; top 2 bytes of number always 0
  mov    r2, #0                  ; 
  movx   a, @dptr                ; get high byte of number
  inc    dptr
  mov    r1, a
  movx   a, @dptr                ; get low byte of number
  mov    r0, a
  clr  a                         ; indicate "no exception"
  ret                            ;




;****************************************************************************
;*
;* Function Name: PointToAudioRecord
;*
;*   Description: On the stack: (int address, int index);
;*
;*      Input(s): address of flashed audio image
;*                index of audio record to get the address for
;*
;*    Outputs(s): dptr - point to 6 byte audio record
;*                
;*         Notes: The format for the audio table is...
;*                [u2] number of audio entries
;*                [u5] * NUM_ENTRIES
;*                     where the first u3 is the address (x:h:l)
;*                     and the next u2 is the length (h:l)
;*
;****************************************************************************
PointToAudioRecord:
  clr    a                       ; request first parameter
  lcall  NatLib_LoadPrimitive    ; get address in r3:r2:r1:r0

  ; ignore whatever r3 had
  mov    dpx, r2
  mov    dph, r1
  mov    dpl, r0
  inc    dptr                    ; inc past the 2 bytes of 'number of entries'
  inc    dptr

  mov    a, #1                   ; request second parameter
  lcall  NatLib_LoadPrimitive    ; get index in r3:r2:r1:r0

  ;
  ; mulitply r1:r0 by 5
  ;
  mov    b, #6
  mov    a, r0
  mul    ab
  mov    r0, a                   ; store the low byte
  push   b                       ; preserve the high byte
  mov    a, r1
  mov    b, #6
  mul    ab
  mov    r2, b                   ; xhigh byte or result
  mov    r1, a
  pop    acc                     ; restore the earlier high byte
  add    a, r1                   ; and add to result
  mov    r1, a
  jnc    nat_getaa_nocarry
  inc    r2
nat_getaa_nocarry:
  ;
  ; now r2:r1:r0 has the result of 6 * index.  Add to dptr
  ;
  mov    a, dpl
  add    a, r0
  mov    dpl, a
  mov    a, dph
  addc   a, r1 
  mov    dph, a
  mov    a, dpx
  addc   a, r2 
  mov    dpx, a
  ;
  ; now we are pointing to our 5 byte record
  ;
  ret

;****************************************************************************
;*
;* Function Name: Native_getAudioAddress
;*
;*   Description: public static native int getAudioAddress(int address, int index);
;*
;*      Input(s): address of flashed audio image
;*                index of audio record to get the address for
;*
;*    Outputs(s): A - zero on success, exception number on failure.
;*                returns an address R3:R2:R1:R0, msb:...:lsb.
;*                
;*         Notes: The format for the audio table is...
;*                [u2] number of audio entries
;*                [u6] * NUM_ENTRIES
;*                     where the first u3 is the address (x:h:l)
;*                     and the next u2 is the length (h:l)
;*
;****************************************************************************
Native_getAudioAddress:
  lcall  PointToAudioRecord      ; get the audio record
  mov    r3, #0                  ; top byte meaningless
  movx   a, @dptr
  inc    dptr
  mov    r2, a                   ; x byte of address
  movx   a, @dptr
  inc    dptr
  mov    r1, a                   ; h byte of address
  movx   a, @dptr
  mov    r0, a                   ; L byte of address

  clr   a                        ; indicate "no exceptions"
  ret                            ;

;****************************************************************************
;*
;* Function Name: Native_getAudioLength
;*
;*   Description: public static native int getAudioLength(int address, int index);
;*
;*      Input(s): address of flashed audio image
;*                index of audio record to get the address for
;*
;*    Outputs(s): A - zero on success, exception number on failure.
;*                returns length in R3:R2:R1:R0, msb:...:lsb.
;*                
;*         Notes: The format for the audio table is...
;*                [u2] number of audio entries
;*                [u6] * NUM_ENTRIES
;*                     where the first u3 is the address (x:h:l)
;*                     and the next u2 is the length (h:l)
;*
;****************************************************************************
Native_getAudioLength:
  lcall  PointToAudioRecord      ; get the audio record
  inc    dptr                    ; point past the address
  inc    dptr
  inc    dptr
  mov    r3, #0                  ; top byte meaningless
  movx   a, @dptr
  inc    dptr
  mov    r2, a                   ; x byte of length
  movx   a, @dptr
  inc    dptr
  mov    r1, a                   ; h byte of length
  movx   a, @dptr
  mov    r0, a                   ; L byte of length

  clr   a                        ; indicate "no exceptions"
  ret                            ;


;****************************************************************************
;*
;* Function Name: Native_isAudioPlaying
;*
;*   Description: public static native boolean isAudioPlaying();
;*
;*      Input(s): None 
;*
;*    Outputs(s): A - zero on success, exception number on failure.
;*                Determine if audio is playing (timer 3 interrupt enabled)
;*                Boolean returned in r3:r2:r1:r0
;*                
;****************************************************************************
Native_isAudioPlaying:
  mov   r3, #0
  mov   r2, #0
  mov   r1, #0
  jb    et3, nat_iap_timerenabled 
  mov   r0, #0                   ; timer is not enabled
  sjmp  nat_iap_exit

nat_iap_timerenabled:
  mov   r0, #1                   ; timer IS enabled

nat_iap_exit:
  clr   a                        ; no error
  ret                            ;

 ;****************************************************************************
;*
;* Function Name: Native_startAudio
;*
;*   Description: public static native int startAudio(int address, int length);
;*
;*      Input(s): address to start playing audio data at
;*                length of audio data to play
;*
;*    Outputs(s): A - zero on success, exception number on failure.
;*                returns an int in R3:R2:R1:R0, msb:...:lsb.
;*                
;****************************************************************************
Native_startAudio:
  clr   a                        ;
  lcall NatLib_LoadPrimitive     ; load address
  mov   a, r0                    ; low byte of the data pointer
  mov   r0, #DataPointer_L     
  mov   @r0, a
  mov   a, r1                    ; high byte of the data pointer
  mov   r0, #DataPointer_H
  mov   @r0, a
  mov   a, r2                    ; extended byte of the data pointer
  mov   r0, #DataPointer_X 
  mov   @r0, a

  mov   a, #1                    ;
  lcall NatLib_LoadPrimitive     ; load length
  mov   a, r0                    ; low byte of length

  mov   r0, #LengthRemaining_L
  mov   @r0, a
  mov   a, r1                    ; high byte of length
  mov   r0, #LengthRemaining_H
  mov   @r0, a
  mov   a, r2                    ; extended byte of length
  mov   r0, #LengthRemaining_X
  mov   @r0, a
  ;
  ; initialize timer 3 reloads.  this will give it a little time before it starts.
  ;
  mov   TIMER_LOW, #0
  mov   TIMER_HIGH, #0
  
  ;
  ; enabling the timer 3 interrupt should start playing audio!
  ;
  CLEAR_TIMER_INTERRUPT
  ENABLE_TIMER_INTERRUPT
  ENABLE_TIMER

  clr   a                        ; no error 
  ret                            ;


;****************************************************************************


mytimer3interrupt:
        push    acc
        push    psw
        push    dps
        push    r0_b0
        push    dpx2
        push    dph2
        push    dpl2

        ;
        ; extract our data pointer
        ;
        mov     r0, #DataPointer_L
        mov     a, @r0
        mov     dpl2, a
        mov     r0, #DataPointer_H
        mov     a, @r0
        mov     dph2, a
        mov     r0, #DataPointer_X
        mov     a, @r0
        mov     dpx2, a

        ;
        ; Reload the timer
        ;
        mov     TIMER_LOW, #TIMER_RELOAD_L
        mov     TIMER_HIGH, #TIMER_RELOAD_H
        CLEAR_TIMER_INTERRUPT                ; clear timer interrupt flag

        ;
        ; read our audio data
        ;
        mov     dps, #DPS_DPTR2_AUTO_INC
        movx    a, @dptr                  ; read lsbyte left
        mov     r0, a                     ; R0 now holds lsbyte left
        movx    a, @dptr                  ; read msbyte left

        ;
        ; Now load the DAC's
        ;
        CLOCK_LOW
        SELECT_CHIP_ON

        cpl acc.7                          ; toggle top bit to get -1 to 1 range
        ;
        ; Wait for the byte to be shifted out on the port pins.
        ;
        mov c,0E0h.7
        mov     DATA_BIT, c
        CLOCK_HIGH
        mov     c, acc.6
        CLOCK_LOW
        mov     DATA_BIT, c
        CLOCK_HIGH
        mov     c, acc.5
        CLOCK_LOW
        mov     DATA_BIT, c
        CLOCK_HIGH
        mov     c, acc.4
        CLOCK_LOW
        mov     DATA_BIT, c
        CLOCK_HIGH
        mov     c, acc.3
        CLOCK_LOW
        mov     DATA_BIT, c
        CLOCK_HIGH
        mov     c, acc.2
        CLOCK_LOW
        mov     DATA_BIT, c
        CLOCK_HIGH
        mov     c, acc.1
        CLOCK_LOW
        mov     DATA_BIT, c
        CLOCK_HIGH
        mov     c, acc.0
        CLOCK_LOW
        mov     DATA_BIT, c
        CLOCK_HIGH
        nop
        CLOCK_LOW


        mov     a, r0
        ;
        ; Send other byte to DAC
        ;
        mov     c, acc.7
        mov     DATA_BIT, c
        CLOCK_HIGH
        mov     c, acc.6
        CLOCK_LOW
        mov     DATA_BIT, c
        CLOCK_HIGH
        mov     c, acc.5
        CLOCK_LOW
        mov     DATA_BIT, c
        CLOCK_HIGH
        mov     c, acc.4
        CLOCK_LOW
        mov     DATA_BIT, c
        CLOCK_HIGH
        mov     c, acc.3
        CLOCK_LOW
        mov     DATA_BIT, c
        CLOCK_HIGH
        mov     c, acc.2
        CLOCK_LOW
        mov     DATA_BIT, c
        CLOCK_HIGH
        mov     c, acc.1
        CLOCK_LOW
        mov     DATA_BIT, c
        CLOCK_HIGH
        mov     c, acc.0
        CLOCK_LOW
        mov     DATA_BIT, c
        CLOCK_HIGH
        nop
        CLOCK_LOW
        nop

        SELECT_CHIP_OFF

        mov     r0, #DataPointer_L
        mov     a, dpl2
        mov     @r0, a
        mov     r0, #DataPointer_H
        mov     a, dph2
        mov     @r0, a
        mov     r0, #DataPointer_X
        mov     a, dpx2
        mov     @r0, a

        ;
        ; Chip select if off, now we need to pulse LDAC.
        ; Also check to see if we're at the end of our clip.
        ; These instructions are intermingled to make sure 
        ; the LDAC signal stays on for long enough.
        ;
        mov     r0, #LengthRemaining_L
        mov     a, @r0                  
        LOADDAC_ON

        ;
        ; if a is zero, we need to dec the high length
        ;
        jz      interrupt_dechighlength
        dec     a
        mov     @r0, a 
        ;
        ; if a is zero now, we need to check for zero length.
        ; else we just exit
        ;
        jnz     interrupt_exit
        mov     r0, #LengthRemaining_H
        mov     a, @r0
        ;
        ; if the high byte is not zero, there is more data.  exit now
        ;
        jnz     interrupt_exit
        ;
        ; else check the xhigh bits
        ;
        mov     r0, #LengthRemaining_X
        mov     a, @r0
        ;
        ; if the high byte is not zero, there is more data.  exit now
        ;
        jnz     interrupt_exit

        ;
        ; else there is no more data.  disable the timer 3 interrupt.
        ;
        DISABLE_TIMER_INTERRUPT
        DISABLE_TIMER
        sjmp    interrupt_exit

interrupt_dechighlength:
        mov     @r0, #0FFh
        mov     r0, #LengthRemaining_H
        mov     a, @r0
        jz      interrupt_decxhighlength
        dec     a
        mov     @r0, a
        ; 
        ; No threat of running into 0 length here, just exit
        ;
        sjmp    interrupt_exit

interrupt_decxhighlength:
        mov     @r0, #0FFh
        mov     r0, #LengthRemaining_X
        mov     a, @r0
        dec     a
        mov     @r0, a

interrupt_exit:
        LOADDAC_OFF
        pop     dpl2
        pop     dph2
        pop     dpx2
        pop     r0_b0
        pop     dps
        pop     psw 
        pop     acc
        reti




END
