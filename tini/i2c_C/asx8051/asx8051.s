
;****************************************************************************

; Mon Feb 23 01:13:28 CET 2004 RP
;	- test asm with asx8051
;
; rm *hex *lst *rel *sym *map *ihx *hex *lnk *mem *rst
;
;../../tools/sdcc/bin/asx8051 -los asx8051.s
;../../tools/sdcc/bin/sdcc -mds400 --model-flat24 --stack-10bit -Wl-r --xram-loc 0x140000 --code-loc 0x490000 asx8051.rel
;../../tools/sdcc/bin/packihx asx8051.ihx > asx8051.hex
;
;../../tools/sdcc/bin/sdcc -mds400 --model-flat24 --stack-10bit -Wl-r --data-loc 0x01 --xram-loc 0x140000 --code-loc 0x490000 asx8051.rel
;
;****************************************************************************



.flat24 on              ; 24 bit flat addressing

;****************************************************************************
;                               Main Program
;****************************************************************************
	.area CSEG    (CODE)
            sjmp Init
            .ascii "TINI400"
            .db 0
            .ascii "1.12" ; this is optional with the -f switch
            .db 0


Init: ; your init routine starts here

TestLoop:  

           SETB    P1.1
	   LCALL   BitDly
	   CLR	   P1.1
	   LCALL   BitDly
           LJMP    TestLoop       ;Repeat operation for scope watchers.

BitDly:    NOP                    ;NOPs to delay 5 microseconds (minus 4 machine cycles for CALL and RET).
	   NOP			  
	   NOP			  
	   NOP			  
	   NOP			  
	   NOP			  
	   NOP			  
	   NOP			  
           RET

	.area SSEG	(DATA)
	
	.area XSEG	(XDATA)
	
	.area ISEG
	
	.area ROMSEG
	
	.area DSEG
	
	


