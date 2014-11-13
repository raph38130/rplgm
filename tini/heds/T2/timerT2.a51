;* 
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
                             
 END 
