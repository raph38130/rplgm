;* mar mar 29 16:21:41 UTC 2005 RP
;* - T3 for stepper motor clock
;* - T2 for quadrature
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
  
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;TIMER T2 quadrature decoding
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
Native_readT2:
; TH2 TL2  
  mov a,TH2
  mov R1_B0,a                            
  mov a,TL2            
  mov R0_B0,a                
  clr a
  mov R3_B0,a           
  mov R2_B0,a          
  ret 

Native_startCountT2:
   mov a, #00000001b	;DCEN=1
   mov T2MOD, a	
   mov a, #00000110b	;C/T2=1 TR2=1
   mov T2CON, a	
   mov a, #80h
   mov TH2,a
   clr a
   mov TL2,a
   ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; STEPPER CMD VIA P1.7 (J5.1)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
wait:
   mov b,a
   push b
   mov a, R0_B0
l:
   mov b,a
   push b
   mov a,#0FFh
delay:
   dec a
   jnz delay
   pop b
   mov a,b
   dec a
   jnz l   
   pop b
   mov a,b
   ret

   
;R0 delay 0x0B 0xFF
;R1 steps 0x00 0xFF
;
;
Native_stepper:
   clr a
   lcall NatLib_LoadPrimitive     ; load parameter
   mov a, R1_B0   
loop:
   setb P1.7
   call wait
   clr P1.7
   call wait
   dec a
   jnz loop  
   ret
                             
 END 
