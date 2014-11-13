                              1 ;--------------------------------------------------------
                              2 ; File Created by SDCC : FreeWare ANSI-C Compiler
                              3 ; Version 2.3.7 Thu Feb  5 13:47:56 2004
                              4 
                              5 ;--------------------------------------------------------
                              6 	.module i2ctest
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
   0000                     221 _main_buffer_1_1::
   0000                     222 	.ds 25
                            223 ;--------------------------------------------------------
                            224 ; external initialized ram data
                            225 ;--------------------------------------------------------
                            226 	.area XISEG   (XDATA)
                            227 ;--------------------------------------------------------
                            228 ; the direct data area used by the DS80c400 ROM code.
                            229 ;--------------------------------------------------------
                            230 .area ROMSEG (ABS,CON,DATA)
                            231 
   0068                     232 .ds 24 ; 24 bytes of directs used starting at 0x68
                            233 
                            234 ;--------------------------------------------------------
                            235 ; interrupt vector 
                            236 ;--------------------------------------------------------
                            237 	.area CSEG    (CODE)
   0000                     238 __interrupt_vect:
                            239 ;	DS80C400 IVT must be generated at runtime.
   0000 80 05               240 	sjmp	__sdcc_400boot
   0002 54 49 4E 49         241 	.ascii	'TINI'	; required signature for 400 boot loader.
   0006 00                  242 	.db	0	; selected bank: zero *should* work...
   0007 02 49 39 41         243 	__sdcc_400boot:	ljmp	__sdcc_gsinit_startup
                            244 ;--------------------------------------------------------
                            245 ; global & static initialisations
                            246 ;--------------------------------------------------------
                            247 	.area GSINIT  (CODE)
                            248 	.area GSFINAL (CODE)
                            249 	.area GSINIT  (CODE)
   3941                     250 __sdcc_gsinit_startup:
   3941 12 49 01 34         251 	lcall	__sdcc_external_startup
   3945 E5 82               252 	mov	a,dpl
   3947 60 04               253 	jz	__sdcc_init_data
   3949 02 49 00 0B         254 	ljmp	__sdcc_program_startup
   394D                     255 __sdcc_init_data:
                            256 ;	_ds390_genXINIT() start
   394D 74 05               257 	mov	a,#l_XINIT
   394F 44 00               258 	orl	a,#l_XINIT>>8
   3951 60 27               259 	jz	00003$
   3953 74 7E               260 	mov	a,#s_XINIT
   3955 24 05               261 	add	a,#l_XINIT
   3957 F9                  262 	mov	r1,a
   3958 74 39               263 	mov	a,#s_XINIT>>8
   395A 34 00               264 	addc	a,#l_XINIT>>8
   395C FA                  265 	mov	r2,a
   395D 90 49 39 7E         266 	mov	dptr,#s_XINIT
   3961 75 86 21            267 	mov	dps,#0x21
   3964 90 14 05 9B         268 	mov	dptr,#s_XISEG
   3968 E4                  269 00001$:	clr	a
   3969 93                  270 	movc	a,@a+dptr
   396A F0                  271 	movx	@dptr,a
   396B A3                  272 	inc	dptr
   396C A3                  273 	inc	dptr
   396D E5 82               274 00002$:	mov	a,dpl
   396F B5 01 F6            275 	cjne	a,ar1,00001$
   3972 E5 83               276 	mov	a,dph
   3974 B5 02 F1            277 	cjne	a,ar2,00001$
   3977 75 86 00            278 	mov	dps,#0
   397A                     279 00003$:
                            280 ;	_ds390_genXINIT() end
                            281 	.area GSFINAL (CODE)
   397A 02 49 00 0B         282 	ljmp	__sdcc_program_startup
                            283 ;--------------------------------------------------------
                            284 ; Home
                            285 ;--------------------------------------------------------
                            286 	.area HOME    (CODE)
                            287 	.area CSEG    (CODE)
                            288 ;--------------------------------------------------------
                            289 ; code
                            290 ;--------------------------------------------------------
                            291 	.area CSEG    (CODE)
   000B                     292 __sdcc_program_startup:
   000B 12 49 00 11         293 	lcall	_main
                            294 ;	return from main will lock up
   000F 80 FE               295 	sjmp .
                            296 ;------------------------------------------------------------
                            297 ;Allocation info for local variables in function 'main'
                            298 ;------------------------------------------------------------
                            299 ;i                         Allocated to registers 
                            300 ;s                         Allocated to registers 
                            301 ;buffer                    Allocated with name '_main_buffer_1_1'
                            302 ;------------------------------------------------------------
                            303 ;	i2ctest.c:26: void main()
                            304 ;	genFunction 
                            305 ;	-----------------------------------------
                            306 ;	 function main
                            307 ;	-----------------------------------------
   0011                     308 _main:
                    0002    309 	ar2 = 0x02
                    0003    310 	ar3 = 0x03
                    0004    311 	ar4 = 0x04
                    0005    312 	ar5 = 0x05
                    0006    313 	ar6 = 0x06
                    0007    314 	ar7 = 0x07
                    0000    315 	ar0 = 0x00
                    0001    316 	ar1 = 0x01
                            317 ;	i2ctest.c:31: printf("i2ctest LGM V0.1.1 !!!\r\n"); 
                            318 ;	genIpush 
   0011 74 EB               319 	mov	a,#__str_0
   0013 C0 E0               320 	push	acc
   0015 74 00               321 	mov	a,#(__str_0 >> 8)
   0017 C0 E0               322 	push	acc
   0019 74 49               323 	mov	a,#(__str_0 >> 16)
   001B C0 E0               324 	push	acc
   001D 74 02               325 	mov	a,#0x02
   001F C0 E0               326 	push	acc
                            327 ;	genCall 
   0021 12 49 0B B3         328 	lcall	_printf
                            329 ;	stack adjustment for parms
   0025 D0 E0               330 	pop	acc
   0027 D0 E0               331 	pop	acc
   0029 D0 E0               332 	pop	acc
   002B D0 E0               333 	pop	acc
                            334 ;	i2ctest.c:32: romInit('n',SPEED_2X);
                            335 ;	genAssign 
                            336 ;	genAssign: resultIsFar = TRUE
   002D 90 14 04 1A         337 	mov	dptr,#_romInit_PARM_2
   0031 74 01               338 	mov	a,#0x01
   0033 F0                  339 	movx	@dptr,a
                            340 ;	genCall 
                            341 ;	genSend argreg = 1, size = 1 
   0034 75 82 6E            342 	mov	dpl,#0x6E
   0037 12 49 04 2F         343 	lcall	_romInit
                            344 ;	i2ctest.c:33: printf("after init_rom!!!\r\n");
                            345 ;	genIpush 
   003B 74 04               346 	mov	a,#__str_1
   003D C0 E0               347 	push	acc
   003F 74 01               348 	mov	a,#(__str_1 >> 8)
   0041 C0 E0               349 	push	acc
   0043 74 49               350 	mov	a,#(__str_1 >> 16)
   0045 C0 E0               351 	push	acc
   0047 74 02               352 	mov	a,#0x02
   0049 C0 E0               353 	push	acc
                            354 ;	genCall 
   004B 12 49 0B B3         355 	lcall	_printf
                            356 ;	stack adjustment for parms
   004F D0 E0               357 	pop	acc
   0051 D0 E0               358 	pop	acc
   0053 D0 E0               359 	pop	acc
   0055 D0 E0               360 	pop	acc
                            361 ;	i2ctest.c:34: getchar();
                            362 ;	genCall 
   0057 12 49 02 F5         363 	lcall	_getchar
                            364 ;	i2ctest.c:35: printf("read i2c bus!!!\r\n");
                            365 ;	genIpush 
   005B 74 18               366 	mov	a,#__str_2
   005D C0 E0               367 	push	acc
   005F 74 01               368 	mov	a,#(__str_2 >> 8)
   0061 C0 E0               369 	push	acc
   0063 74 49               370 	mov	a,#(__str_2 >> 16)
   0065 C0 E0               371 	push	acc
   0067 74 02               372 	mov	a,#0x02
   0069 C0 E0               373 	push	acc
                            374 ;	genCall 
   006B 12 49 0B B3         375 	lcall	_printf
                            376 ;	stack adjustment for parms
   006F D0 E0               377 	pop	acc
   0071 D0 E0               378 	pop	acc
   0073 D0 E0               379 	pop	acc
   0075 D0 E0               380 	pop	acc
                            381 ;	i2ctest.c:36: buffer[0]=0x00;
                            382 ;	genPointerSet 
   0077 90 14 00 00         383 	mov	dptr,#_main_buffer_1_1
                            384 ; Peephole 180   changed mov to clr
   007B E4                  385 	clr  a
   007C F0                  386 	movx	@dptr,a
                            387 ;	i2ctest.c:37: buffer[1]=0x06;
                            388 ;	genPointerSet 
   007D 90 14 00 01         389 	mov	dptr,#(_main_buffer_1_1 + 0x0001)
   0081 74 06               390 	mov	a,#0x06
   0083 F0                  391 	movx	@dptr,a
                            392 ;	i2ctest.c:38: buffer[2]=0x0F;
                            393 ;	genPointerSet 
   0084 90 14 00 02         394 	mov	dptr,#(_main_buffer_1_1 + 0x0002)
   0088 74 0F               395 	mov	a,#0x0F
   008A F0                  396 	movx	@dptr,a
                            397 ;	i2ctest.c:39: buffer[3]=0xF0;
                            398 ;	genPointerSet 
   008B 90 14 00 03         399 	mov	dptr,#(_main_buffer_1_1 + 0x0003)
   008F 74 F0               400 	mov	a,#0xF0
   0091 F0                  401 	movx	@dptr,a
                            402 ;	i2ctest.c:40: buffer[4]=0xA0;
                            403 ;	genPointerSet 
   0092 90 14 00 04         404 	mov	dptr,#(_main_buffer_1_1 + 0x0004)
   0096 74 A0               405 	mov	a,#0xA0
   0098 F0                  406 	movx	@dptr,a
                            407 ;	i2ctest.c:41: buffer[5]=0x0E;
                            408 ;	genPointerSet 
   0099 90 14 00 05         409 	mov	dptr,#(_main_buffer_1_1 + 0x0005)
   009D 74 0E               410 	mov	a,#0x0E
   009F F0                  411 	movx	@dptr,a
                            412 ;	i2ctest.c:42: while(1) {
                            413 ;	genLabel 
   00A0                     414 00102$:
                            415 ;	i2ctest.c:44: buffer[0]=0xaa;
                            416 ;	genPointerSet 
   00A0 90 14 00 00         417 	mov	dptr,#_main_buffer_1_1
   00A4 74 AA               418 	mov	a,#0xAA
   00A6 F0                  419 	movx	@dptr,a
                            420 ;	i2ctest.c:45: buffer[1]=0xaa;
                            421 ;	genPointerSet 
   00A7 90 14 00 01         422 	mov	dptr,#(_main_buffer_1_1 + 0x0001)
   00AB 74 AA               423 	mov	a,#0xAA
   00AD F0                  424 	movx	@dptr,a
                            425 ;	i2ctest.c:47: i2c_select(0x72);
                            426 ;	genCall 
                            427 ;	genSend argreg = 1, size = 1 
   00AE 75 82 72            428 	mov	dpl,#0x72
   00B1 12 49 07 4E         429 	lcall	_i2c_select
                            430 ;	i2ctest.c:48: i2c_stop();
                            431 ;	genCall 
   00B5 12 49 06 A2         432 	lcall	_i2c_stop
                            433 ;	i2ctest.c:49: s=i2c_readbyte(1);
                            434 ;	genCall 
                            435 ;	genSend argreg = 1, size = 1 
   00B9 75 82 01            436 	mov	dpl,#0x01
   00BC 12 49 06 C1         437 	lcall	_i2c_readbyte
                            438 ;	genAssign 
                            439 ;	genAssign: resultIsFar = FALSE
   00C0 AA 82               440 	mov	r2,dpl
                            441 ;	i2ctest.c:50: printf("read %x\n\r",s & 0xFF);
                            442 ;	genCast 
   00C2 7B 00               443 	mov	r3,#0
                            444 ;	genIpush 
   00C4 C0 02               445 	push	ar2
   00C6 C0 03               446 	push	ar3
                            447 ;	genIpush 
   00C8 74 2A               448 	mov	a,#__str_3
   00CA C0 E0               449 	push	acc
   00CC 74 01               450 	mov	a,#(__str_3 >> 8)
   00CE C0 E0               451 	push	acc
   00D0 74 49               452 	mov	a,#(__str_3 >> 16)
   00D2 C0 E0               453 	push	acc
   00D4 74 02               454 	mov	a,#0x02
   00D6 C0 E0               455 	push	acc
                            456 ;	genCall 
   00D8 12 49 0B B3         457 	lcall	_printf
                            458 ;	stack adjustment for parms
   00DC D0 E0               459 	pop	acc
   00DE D0 E0               460 	pop	acc
   00E0 D0 E0               461 	pop	acc
   00E2 D0 E0               462 	pop	acc
   00E4 D0 E0               463 	pop	acc
   00E6 D0 E0               464 	pop	acc
                            465 ;	genGoto 
                            466 ;	genLabel 
                            467 ; Peephole 132   changed ljmp to sjmp
   00E8 80 B6               468 	sjmp 00102$
   00EA                     469 00104$:
                            470 ;	genEndFunction 
   00EA 22                  471 	ret
                            472 	.area CSEG    (CODE)
   00EB                     473 __str_0:
   00EB 69 32 63 74 65 73   474 	.ascii "i2ctest LGM V0.1.1 !!!"
        74 20 4C 47 4D 20
        56 30 2E 31 2E 31
        20 21 21 21
   0101 0D                  475 	.db 0x0D
   0102 0A                  476 	.db 0x0A
   0103 00                  477 	.db 0x00
   0104                     478 __str_1:
   0104 61 66 74 65 72 20   479 	.ascii "after init_rom!!!"
        69 6E 69 74 5F 72
        6F 6D 21 21 21
   0115 0D                  480 	.db 0x0D
   0116 0A                  481 	.db 0x0A
   0117 00                  482 	.db 0x00
   0118                     483 __str_2:
   0118 72 65 61 64 20 69   484 	.ascii "read i2c bus!!!"
        32 63 20 62 75 73
        21 21 21
   0127 0D                  485 	.db 0x0D
   0128 0A                  486 	.db 0x0A
   0129 00                  487 	.db 0x00
   012A                     488 __str_3:
   012A 72 65 61 64 20 25   489 	.ascii "read %x"
        78
   0131 0A                  490 	.db 0x0A
   0132 0D                  491 	.db 0x0D
   0133 00                  492 	.db 0x00
                            493 	.area XINIT   (CODE)
