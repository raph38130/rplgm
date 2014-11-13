;* Sat May 13 23:21:36 CEST 2006 RP
;* - pwm is ok
;* ven jan 21 13:01:12 UTC 2005 RP
;* - overflow flag needs to be cleared
;* 
;* 
$include(tini_400.inc)
$include(ds80c400.inc)
$include(tinimacro.inc)
$include(apiequ.inc)

Native_Init:
	clr a
	ret


Native_stopT2:
  	clr ET2                                  
   	mov T2MOD, #000000000b	
   	mov T2CON, #000000000b
	mov RCAP2L, #000h
	mov RCAP2H, #000h

	clr a
	ret

Native_startpwmT2:
  	clr   ET2      			; turn off interrupts                            
  	mov   a, #2Bh                   ; install isr handler          
  	mov   dps, #1                              
  	mov   dptr, #T2_ISR                        
  	mov   dps, #0                              
  	lcall System_InstallInterrupt              

	clr   a                         ; load parameter n° 0
  	lcall NatLib_LoadPrimitive     
	
	mov   dps, #1		        ; change RCAP2H reload value in isr
	mov RCAP2L, #000h		
	mov dptr, #adr1 
	inc dptr
	inc dptr
	mov a,R0_B0			; high level duration
	movx @dptr, a
	mov dptr, #adr2
	inc dptr
	inc dptr
	mov a,R1_B0			; low level duration
	movx @dptr, a
	mov   dps, #0
	
   	mov T2MOD, #000000000b		; T2 timer with autoreload
   	mov T2CON, #000000100b
	
  	setb   ET2 
  	clr a
  	ret


T2_ISR:	
	clr 0C8h.7			;timer 2 it ack
	
	jb P1.0, l1			
  	setb P1.0
adr1:	mov RCAP2H, #080h		; value overload during init	
	reti
	
l1:  	clr P1.0
adr2:	mov RCAP2H, #080h		; value overload during init
	reti


  
END 
