                              1 ;--------------------------------------------------------
                              2 ; File Created by SDCC : FreeWare ANSI-C Compiler
                              3 ; Version 2.3.7 Thu Feb  5 13:36:48 2004
                              4 
                              5 ;--------------------------------------------------------
                              6 	.module essai
                              7 	.optsdcc -mds400 --model-flat24
                              8 	
                              9 .flat24 on		; 24 bit flat addressing
                    0093     10 dpx = 0x93		; dpx register unknown to assembler
                    0086     11 dps = 0x86		; dps register unknown to assembler
                    0084     12 dpl1 = 0x84		; dpl1 register unknown to assembler
                    0085     13 dph1 = 0x85		; dph1 register unknown to assembler
                    0095     14 dpx1 = 0x95		; dpx1 register unknown to assembler
                    009C     15 ap = 0x9C		; ap register unknown to assembler
                    009C     16 _ap = 0x9C		; _ap register unknown to assembler
                    00D1     17 mcnt0 = 0xD1		; mcnt0 register unknown to assembler
                    00D2     18 mcnt1 = 0xD2		; mcnt1 register unknown to assembler
                    00D3     19 ma = 0xD3		; ma register unknown to assembler
                    00D4     20 mb = 0xD4		; mb register unknown to assembler
                    00D5     21 mc = 0xD5		; mc register unknown to assembler
                    00D1     22 F1 = 0xD1		; F1 user flag unknown to assembler
                    009B     23 esp = 0x9B		; ESP user flag unknown to assembler
                             24 ;--------------------------------------------------------
                             25 ; Public variables in this module
                             26 ;--------------------------------------------------------
                             27 	.globl _main
                             28 ;--------------------------------------------------------
                             29 ; special function registers
                             30 ;--------------------------------------------------------
                    0080     31 _P4	=	0x0080
                    0081     32 _SP	=	0x0081
                    0082     33 _DPL	=	0x0082
                    0083     34 _DPH	=	0x0083
                    0084     35 _DPL1	=	0x0084
                    0085     36 _DPH1	=	0x0085
                    0086     37 _DPS	=	0x0086
                    0087     38 _PCON	=	0x0087
                    0088     39 _TCON	=	0x0088
                    0089     40 _TMOD	=	0x0089
                    008A     41 _TL0	=	0x008a
                    008B     42 _TL1	=	0x008b
                    008C     43 _TH0	=	0x008c
                    008D     44 _TH1	=	0x008d
                    008E     45 _CKCON	=	0x008e
                    0090     46 _P1	=	0x0090
                    0091     47 _EXIF	=	0x0091
                    0092     48 _P4CNT	=	0x0092
                    0093     49 _DPX	=	0x0093
                    0095     50 _DPX1	=	0x0095
                    0098     51 _SCON0	=	0x0098
                    0099     52 _SBUF0	=	0x0099
                    009B     53 _ESP	=	0x009b
                    009C     54 _AP	=	0x009c
                    009D     55 _ACON	=	0x009d
                    00A0     56 _P2	=	0x00a0
                    00A1     57 _P5	=	0x00a1
                    00A2     58 _P5CNT	=	0x00a2
                    00A8     59 _IE	=	0x00a8
                    00B0     60 _P3	=	0x00b0
                    00B8     61 _IP	=	0x00b8
                    00C0     62 _SCON1	=	0x00c0
                    00C1     63 _SBUF1	=	0x00c1
                    00C4     64 _PMR	=	0x00c4
                    00C6     65 _MCON	=	0x00c6
                    00C7     66 _TA	=	0x00c7
                    00C8     67 _T2CON	=	0x00c8
                    00C9     68 _T2MOD	=	0x00c9
                    00CA     69 _RCAP2L	=	0x00ca
                    00CA     70 _RTL2	=	0x00ca
                    00CB     71 _RCAP2H	=	0x00cb
                    00CB     72 _RTH2	=	0x00cb
                    00CC     73 _TL2	=	0x00cc
                    00CD     74 _TH2	=	0x00cd
                    00D0     75 _PSW	=	0x00d0
                    00D1     76 _MCNT0	=	0x00d1
                    00D2     77 _MCNT1	=	0x00d2
                    00D3     78 _MA	=	0x00d3
                    00D4     79 _MB	=	0x00d4
                    00D5     80 _MC	=	0x00d5
                    00D8     81 _WDCON	=	0x00d8
                    00E0     82 _ACC	=	0x00e0
                    00E8     83 _EIE	=	0x00e8
                    00EA     84 _MXAX	=	0x00ea
                    00F0     85 _B	=	0x00f0
                    00F8     86 _EIP	=	0x00f8
                             87 ;--------------------------------------------------------
                             88 ; special function bits 
                             89 ;--------------------------------------------------------
                    0088     90 _IT0	=	0x0088
                    0089     91 _IE0	=	0x0089
                    008A     92 _IT1	=	0x008a
                    008B     93 _IE1	=	0x008b
                    008C     94 _TR0	=	0x008c
                    008D     95 _TF0	=	0x008d
                    008E     96 _TR1	=	0x008e
                    008F     97 _TF1	=	0x008f
                    0090     98 _T2	=	0x0090
                    0091     99 _T2EX	=	0x0091
                    0092    100 _RXD1	=	0x0092
                    0093    101 _TXD1	=	0x0093
                    0094    102 _INT2	=	0x0094
                    0095    103 _INT3	=	0x0095
                    0096    104 _INT4	=	0x0096
                    0097    105 _INT5	=	0x0097
                    0098    106 _RI_0	=	0x0098
                    0099    107 _TI_0	=	0x0099
                    009A    108 _RB8_0	=	0x009a
                    009B    109 _TB8_0	=	0x009b
                    009C    110 _REN_0	=	0x009c
                    009D    111 _SM2_0	=	0x009d
                    009E    112 _SM1_0	=	0x009e
                    009F    113 _SM0_0	=	0x009f
                    009F    114 _FE_0	=	0x009f
                    00A8    115 _EX0	=	0x00a8
                    00A9    116 _ET0	=	0x00a9
                    00AA    117 _EX1	=	0x00aa
                    00AB    118 _ET1	=	0x00ab
                    00AC    119 _ES0	=	0x00ac
                    00AD    120 _ET2	=	0x00ad
                    00AE    121 _ES1	=	0x00ae
                    00AF    122 _EA	=	0x00af
                    00B0    123 _RXD0	=	0x00b0
                    00B1    124 _TXD0	=	0x00b1
                    00B2    125 _INT0	=	0x00b2
                    00B3    126 _INT1	=	0x00b3
                    00B4    127 _T0	=	0x00b4
                    00B5    128 _T1	=	0x00b5
                    00B6    129 _WR	=	0x00b6
                    00B7    130 _RD	=	0x00b7
                    00B8    131 _PX0	=	0x00b8
                    00B9    132 _PT0	=	0x00b9
                    00BA    133 _PX1	=	0x00ba
                    00BB    134 _PT1	=	0x00bb
                    00BC    135 _PS0	=	0x00bc
                    00BD    136 _PT2	=	0x00bd
                    00BE    137 _PS1	=	0x00be
                    00C0    138 _RI_1	=	0x00c0
                    00C1    139 _TI_1	=	0x00c1
                    00C2    140 _RB8_1	=	0x00c2
                    00C3    141 _TB8_1	=	0x00c3
                    00C4    142 _REN_1	=	0x00c4
                    00C5    143 _SM2_1	=	0x00c5
                    00C6    144 _SM1_1	=	0x00c6
                    00C7    145 _SM0_1	=	0x00c7
                    00C7    146 _FE_1	=	0x00c7
                    00C8    147 _CP_RL	=	0x00c8
                    00C9    148 _C_T	=	0x00c9
                    00CA    149 _TR2	=	0x00ca
                    00CB    150 _EXEN2	=	0x00cb
                    00CC    151 _TCLK	=	0x00cc
                    00CD    152 _RCLK	=	0x00cd
                    00CE    153 _EXF2	=	0x00ce
                    00CF    154 _TF2	=	0x00cf
                    00D0    155 _P	=	0x00d0
                    00D1    156 _F1	=	0x00d1
                    00D2    157 _OV	=	0x00d2
                    00D3    158 _RS0	=	0x00d3
                    00D4    159 _RS1	=	0x00d4
                    00D5    160 _F0	=	0x00d5
                    00D6    161 _AC	=	0x00d6
                    00D7    162 _CY	=	0x00d7
                    00D8    163 _RWT	=	0x00d8
                    00D9    164 _EWT	=	0x00d9
                    00DA    165 _WDRF	=	0x00da
                    00DB    166 _WDIF	=	0x00db
                    00DC    167 _PFI	=	0x00dc
                    00DD    168 _EPFI	=	0x00dd
                    00DE    169 _POR	=	0x00de
                    00DF    170 _SMOD_1	=	0x00df
                    00E8    171 _EX2	=	0x00e8
                    00E9    172 _EX3	=	0x00e9
                    00EA    173 _EX4	=	0x00ea
                    00EB    174 _EX5	=	0x00eb
                    00EC    175 _EWDI	=	0x00ec
                    00ED    176 _C1IE	=	0x00ed
                    00EE    177 _C0IE	=	0x00ee
                    00EF    178 _CANBIE	=	0x00ef
                    00F8    179 _PX2	=	0x00f8
                    00F9    180 _PX3	=	0x00f9
                    00FA    181 _PX4	=	0x00fa
                    00FB    182 _PX5	=	0x00fb
                    00FC    183 _PWDI	=	0x00fc
                    00FD    184 _C1IP	=	0x00fd
                    00FE    185 _C0IP	=	0x00fe
                    00FF    186 _CANBIP	=	0x00ff
                            187 ;--------------------------------------------------------
                            188 ; overlayable register banks 
                            189 ;--------------------------------------------------------
                            190 	.area REG_BANK_0	(REL,OVR,DATA)
   0000                     191 	.ds 8
                            192 	.area REG_BANK_3	(REL,OVR,DATA)
   0018                     193 	.ds 8
                            194 ;--------------------------------------------------------
                            195 ; internal ram data
                            196 ;--------------------------------------------------------
                            197 	.area DSEG    (DATA)
                            198 ;--------------------------------------------------------
                            199 ; overlayable items in internal ram 
                            200 ;--------------------------------------------------------
                            201 	.area OSEG    (OVR,DATA)
                            202 ;--------------------------------------------------------
                            203 ; Stack segment in internal ram 
                            204 ;--------------------------------------------------------
                            205 	.area	SSEG	(DATA)
   002A                     206 __start__stack:
   002A                     207 	.ds	1
                            208 
                            209 ;--------------------------------------------------------
                            210 ; indirectly addressable internal ram data
                            211 ;--------------------------------------------------------
                            212 	.area ISEG    (DATA)
                            213 ;--------------------------------------------------------
                            214 ; bit data
                            215 ;--------------------------------------------------------
                            216 	.area BSEG    (BIT)
                            217 ;--------------------------------------------------------
                            218 ; external ram data
                            219 ;--------------------------------------------------------
                            220 	.area XSEG    (XDATA)
                            221 ;--------------------------------------------------------
                            222 ; external initialized ram data
                            223 ;--------------------------------------------------------
                            224 	.area XISEG   (XDATA)
                            225 ;--------------------------------------------------------
                            226 ; the direct data area used by the DS80c400 ROM code.
                            227 ;--------------------------------------------------------
                            228 .area ROMSEG (ABS,CON,DATA)
                            229 
   0068                     230 .ds 24 ; 24 bytes of directs used starting at 0x68
                            231 
                            232 ;--------------------------------------------------------
                            233 ; interrupt vector 
                            234 ;--------------------------------------------------------
                            235 	.area CSEG    (CODE)
   0000                     236 __interrupt_vect:
                            237 ;	DS80C400 IVT must be generated at runtime.
   0000 80 05               238 	sjmp	__sdcc_400boot
   0002 54 49 4E 49         239 	.ascii	'TINI'	; required signature for 400 boot loader.
   0006 00                  240 	.db	0	; selected bank: zero *should* work...
   0007 02 49 35 19         241 	__sdcc_400boot:	ljmp	__sdcc_gsinit_startup
                            242 ;--------------------------------------------------------
                            243 ; global & static initialisations
                            244 ;--------------------------------------------------------
                            245 	.area GSINIT  (CODE)
                            246 	.area GSFINAL (CODE)
                            247 	.area GSINIT  (CODE)
   3519                     248 __sdcc_gsinit_startup:
   3519 12 49 01 97         249 	lcall	__sdcc_external_startup
   351D E5 82               250 	mov	a,dpl
   351F 60 04               251 	jz	__sdcc_init_data
   3521 02 49 00 0B         252 	ljmp	__sdcc_program_startup
   3525                     253 __sdcc_init_data:
                            254 ;	_ds390_genXINIT() start
   3525 74 05               255 	mov	a,#l_XINIT
   3527 44 00               256 	orl	a,#l_XINIT>>8
   3529 60 27               257 	jz	00003$
   352B 74 56               258 	mov	a,#s_XINIT
   352D 24 05               259 	add	a,#l_XINIT
   352F F9                  260 	mov	r1,a
   3530 74 35               261 	mov	a,#s_XINIT>>8
   3532 34 00               262 	addc	a,#l_XINIT>>8
   3534 FA                  263 	mov	r2,a
   3535 90 49 35 56         264 	mov	dptr,#s_XINIT
   3539 75 86 21            265 	mov	dps,#0x21
   353C 90 14 05 62         266 	mov	dptr,#s_XISEG
   3540 E4                  267 00001$:	clr	a
   3541 93                  268 	movc	a,@a+dptr
   3542 F0                  269 	movx	@dptr,a
   3543 A3                  270 	inc	dptr
   3544 A3                  271 	inc	dptr
   3545 E5 82               272 00002$:	mov	a,dpl
   3547 B5 01 F6            273 	cjne	a,ar1,00001$
   354A E5 83               274 	mov	a,dph
   354C B5 02 F1            275 	cjne	a,ar2,00001$
   354F 75 86 00            276 	mov	dps,#0
   3552                     277 00003$:
                            278 ;	_ds390_genXINIT() end
                            279 	.area GSFINAL (CODE)
   3552 02 49 00 0B         280 	ljmp	__sdcc_program_startup
                            281 ;--------------------------------------------------------
                            282 ; Home
                            283 ;--------------------------------------------------------
                            284 	.area HOME    (CODE)
                            285 	.area CSEG    (CODE)
                            286 ;--------------------------------------------------------
                            287 ; code
                            288 ;--------------------------------------------------------
                            289 	.area CSEG    (CODE)
   000B                     290 __sdcc_program_startup:
   000B 12 49 00 11         291 	lcall	_main
                            292 ;	return from main will lock up
   000F 80 FE               293 	sjmp .
                            294 ;------------------------------------------------------------
                            295 ;Allocation info for local variables in function 'main'
                            296 ;------------------------------------------------------------
                            297 ;i                         Allocated to registers 
                            298 ;------------------------------------------------------------
                            299 ;	essai.c:18: void main()
                            300 ;	genFunction 
                            301 ;	-----------------------------------------
                            302 ;	 function main
                            303 ;	-----------------------------------------
   0011                     304 _main:
                    0002    305 	ar2 = 0x02
                    0003    306 	ar3 = 0x03
                    0004    307 	ar4 = 0x04
                    0005    308 	ar5 = 0x05
                    0006    309 	ar6 = 0x06
                    0007    310 	ar7 = 0x07
                    0000    311 	ar0 = 0x00
                    0001    312 	ar1 = 0x01
                            313 ;	essai.c:22: printf("hello LGM V0.2.5!!!\r\n"); 
                            314 ;	genIpush 
   0011 74 2E               315 	mov	a,#__str_0
   0013 C0 E0               316 	push	acc
   0015 74 01               317 	mov	a,#(__str_0 >> 8)
   0017 C0 E0               318 	push	acc
   0019 74 49               319 	mov	a,#(__str_0 >> 16)
   001B C0 E0               320 	push	acc
   001D 74 02               321 	mov	a,#0x02
   001F C0 E0               322 	push	acc
                            323 ;	genCall 
   0021 12 49 08 55         324 	lcall	_printf
                            325 ;	stack adjustment for parms
   0025 D0 E0               326 	pop	acc
   0027 D0 E0               327 	pop	acc
   0029 D0 E0               328 	pop	acc
   002B D0 E0               329 	pop	acc
                            330 ;	essai.c:23: romInit('n',SPEED_2X);
                            331 ;	genAssign 
                            332 ;	genAssign: resultIsFar = TRUE
   002D 90 14 04 01         333 	mov	dptr,#_romInit_PARM_2
   0031 74 01               334 	mov	a,#0x01
   0033 F0                  335 	movx	@dptr,a
                            336 ;	genCall 
                            337 ;	genSend argreg = 1, size = 1 
   0034 75 82 6E            338 	mov	dpl,#0x6E
   0037 12 49 04 92         339 	lcall	_romInit
                            340 ;	essai.c:24: printf("after init_rom!!!\r\n");
                            341 ;	genIpush 
   003B 74 44               342 	mov	a,#__str_1
   003D C0 E0               343 	push	acc
   003F 74 01               344 	mov	a,#(__str_1 >> 8)
   0041 C0 E0               345 	push	acc
   0043 74 49               346 	mov	a,#(__str_1 >> 16)
   0045 C0 E0               347 	push	acc
   0047 74 02               348 	mov	a,#0x02
   0049 C0 E0               349 	push	acc
                            350 ;	genCall 
   004B 12 49 08 55         351 	lcall	_printf
                            352 ;	stack adjustment for parms
   004F D0 E0               353 	pop	acc
   0051 D0 E0               354 	pop	acc
   0053 D0 E0               355 	pop	acc
   0055 D0 E0               356 	pop	acc
                            357 ;	essai.c:25: getchar();
                            358 ;	genCall 
   0057 12 49 03 58         359 	lcall	_getchar
                            360 ;	essai.c:26: printf("                    go!!!\r\n");
                            361 ;	genIpush 
   005B 74 58               362 	mov	a,#__str_2
   005D C0 E0               363 	push	acc
   005F 74 01               364 	mov	a,#(__str_2 >> 8)
   0061 C0 E0               365 	push	acc
   0063 74 49               366 	mov	a,#(__str_2 >> 16)
   0065 C0 E0               367 	push	acc
   0067 74 02               368 	mov	a,#0x02
   0069 C0 E0               369 	push	acc
                            370 ;	genCall 
   006B 12 49 08 55         371 	lcall	_printf
                            372 ;	stack adjustment for parms
   006F D0 E0               373 	pop	acc
   0071 D0 E0               374 	pop	acc
   0073 D0 E0               375 	pop	acc
   0075 D0 E0               376 	pop	acc
                            377 ;	essai.c:27: while(i<100) {
                            378 ;	genAssign 
                            379 ;	genAssign: resultIsFar = FALSE
   0077 7A 00               380 	mov	r2,#0x00
   0079 7B 00               381 	mov	r3,#0x00
                            382 ;	genLabel 
   007B                     383 00103$:
                            384 ;	genCmpLt 
                            385 ;	genCmp
   007B C3                  386 	clr	c
   007C EA                  387 	mov	a,r2
   007D 94 64               388 	subb	a,#0x64
   007F EB                  389 	mov	a,r3
   0080 64 80               390 	xrl	a,#0x80
   0082 94 80               391 	subb	a,#0x80
                            392 ;	genIfxJump
   0084 40 04               393 	jc	00115$
   0086 02 49 01 07         394 	ljmp	00105$
   008A                     395 00115$:
                            396 ;	essai.c:28: printf("%02d ",i);
                            397 ;	genIpush 
   008A C0 02               398 	push	ar2
   008C C0 03               399 	push	ar3
   008E C0 02               400 	push	ar2
   0090 C0 03               401 	push	ar3
                            402 ;	genIpush 
   0092 74 74               403 	mov	a,#__str_3
   0094 C0 E0               404 	push	acc
   0096 74 01               405 	mov	a,#(__str_3 >> 8)
   0098 C0 E0               406 	push	acc
   009A 74 49               407 	mov	a,#(__str_3 >> 16)
   009C C0 E0               408 	push	acc
   009E 74 02               409 	mov	a,#0x02
   00A0 C0 E0               410 	push	acc
                            411 ;	genCall 
   00A2 12 49 08 55         412 	lcall	_printf
                            413 ;	stack adjustment for parms
   00A6 D0 E0               414 	pop	acc
   00A8 D0 E0               415 	pop	acc
   00AA D0 E0               416 	pop	acc
   00AC D0 E0               417 	pop	acc
   00AE D0 E0               418 	pop	acc
   00B0 D0 E0               419 	pop	acc
   00B2 D0 03               420 	pop	ar3
   00B4 D0 02               421 	pop	ar2
                            422 ;	essai.c:29: if ((i % 10)==9) printf("\r\n");
                            423 ;	genAssign 
                            424 ;	genAssign: resultIsFar = TRUE
   00B6 90 14 04 07         425 	mov	dptr,#__modsint_PARM_2
   00BA 74 0A               426 	mov	a,#0x0A
   00BC F0                  427 	movx	@dptr,a
   00BD E4                  428 	clr	a
   00BE A3                  429 	inc	dptr
   00BF F0                  430 	movx	@dptr,a
                            431 ;	genCall 
   00C0 C0 02               432 	push	ar2
   00C2 C0 03               433 	push	ar3
                            434 ;	genSend argreg = 1, size = 2 
   00C4 8A 82               435 	mov	dpl,r2
   00C6 8B 83               436 	mov	dph,r3
   00C8 12 49 06 5C         437 	lcall	__modsint
   00CC D0 03               438 	pop	ar3
   00CE D0 02               439 	pop	ar2
                            440 ;	genCmpEq 
                            441 ;	gencjneshort
   00D0 E5 82               442 	mov	a,dpl
                            443 ; Peephole 132   changed ljmp to sjmp
                            444 ; Peephole 197   optimized misc jump sequence
   00D2 B4 09 29            445 	cjne a,#0x09,00102$
   00D5 E5 83               446 	mov  a,dph
   00D7 B4 00 24            447 	cjne a,#0x00,00102$
                            448 ;00116$:
                            449 ; Peephole 200   removed redundant sjmp
   00DA                     450 00117$:
                            451 ;	genIpush 
   00DA C0 02               452 	push	ar2
   00DC C0 03               453 	push	ar3
   00DE 74 7A               454 	mov	a,#__str_4
   00E0 C0 E0               455 	push	acc
   00E2 74 01               456 	mov	a,#(__str_4 >> 8)
   00E4 C0 E0               457 	push	acc
   00E6 74 49               458 	mov	a,#(__str_4 >> 16)
   00E8 C0 E0               459 	push	acc
   00EA 74 02               460 	mov	a,#0x02
   00EC C0 E0               461 	push	acc
                            462 ;	genCall 
   00EE 12 49 08 55         463 	lcall	_printf
                            464 ;	stack adjustment for parms
   00F2 D0 E0               465 	pop	acc
   00F4 D0 E0               466 	pop	acc
   00F6 D0 E0               467 	pop	acc
   00F8 D0 E0               468 	pop	acc
   00FA D0 03               469 	pop	ar3
   00FC D0 02               470 	pop	ar2
                            471 ;	genLabel 
   00FE                     472 00102$:
                            473 ;	essai.c:30: i++;
                            474 ;	genPlus 
   00FE 0A                  475 	inc	r2
   00FF BA 00 01            476 	cjne	r2,#0,00118$
   0102 0B                  477 	inc	r3
   0103                     478 00118$:
                            479 ;	did genPlusIncr
                            480 ;	genGoto 
   0103 02 49 00 7B         481 	ljmp	00103$
                            482 ;	genLabel 
   0107                     483 00105$:
                            484 ;	essai.c:32: getchar();
                            485 ;	genCall 
   0107 12 49 03 58         486 	lcall	_getchar
                            487 ;	essai.c:33: printf("                    end\n\r");
                            488 ;	genIpush 
   010B 74 7D               489 	mov	a,#__str_5
   010D C0 E0               490 	push	acc
   010F 74 01               491 	mov	a,#(__str_5 >> 8)
   0111 C0 E0               492 	push	acc
   0113 74 49               493 	mov	a,#(__str_5 >> 16)
   0115 C0 E0               494 	push	acc
   0117 74 02               495 	mov	a,#0x02
   0119 C0 E0               496 	push	acc
                            497 ;	genCall 
   011B 12 49 08 55         498 	lcall	_printf
                            499 ;	stack adjustment for parms
   011F D0 E0               500 	pop	acc
   0121 D0 E0               501 	pop	acc
   0123 D0 E0               502 	pop	acc
   0125 D0 E0               503 	pop	acc
                            504 ;	essai.c:34: getchar();
                            505 ;	genCall 
   0127 12 49 03 58         506 	lcall	_getchar
                            507 ;	essai.c:35: while(1);
                            508 ;	genLabel 
   012B                     509 00107$:
                            510 ;	genGoto 
                            511 ;	genLabel 
                            512 ; Peephole 132   changed ljmp to sjmp
   012B 80 FE               513 	sjmp 00107$
   012D                     514 00109$:
                            515 ;	genEndFunction 
   012D 22                  516 	ret
                            517 	.area CSEG    (CODE)
   012E                     518 __str_0:
   012E 68 65 6C 6C 6F 20   519 	.ascii "hello LGM V0.2.5!!!"
        4C 47 4D 20 56 30
        2E 32 2E 35 21 21
        21
   0141 0D                  520 	.db 0x0D
   0142 0A                  521 	.db 0x0A
   0143 00                  522 	.db 0x00
   0144                     523 __str_1:
   0144 61 66 74 65 72 20   524 	.ascii "after init_rom!!!"
        69 6E 69 74 5F 72
        6F 6D 21 21 21
   0155 0D                  525 	.db 0x0D
   0156 0A                  526 	.db 0x0A
   0157 00                  527 	.db 0x00
   0158                     528 __str_2:
   0158 20 20 20 20 20 20   529 	.ascii "                    go!!!"
        20 20 20 20 20 20
        20 20 20 20 20 20
        20 20 67 6F 21 21
        21
   0171 0D                  530 	.db 0x0D
   0172 0A                  531 	.db 0x0A
   0173 00                  532 	.db 0x00
   0174                     533 __str_3:
   0174 25 30 32 64 20      534 	.ascii "%02d "
   0179 00                  535 	.db 0x00
   017A                     536 __str_4:
   017A 0D                  537 	.db 0x0D
   017B 0A                  538 	.db 0x0A
   017C 00                  539 	.db 0x00
   017D                     540 __str_5:
   017D 20 20 20 20 20 20   541 	.ascii "                    end"
        20 20 20 20 20 20
        20 20 20 20 20 20
        20 20 65 6E 64
   0194 0A                  542 	.db 0x0A
   0195 0D                  543 	.db 0x0D
   0196 00                  544 	.db 0x00
                            545 	.area XINIT   (CODE)
