;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.3.7 Thu Feb  5 13:47:56 2004

;--------------------------------------------------------
	.module i2ctest
	.optsdcc -mds400 --model-flat24
	
.flat24 on		; 24 bit flat addressing
dpx = 0x93		; dpx register unknown to assembler
dps = 0x86		; dps register unknown to assembler
dpl1 = 0x84		; dpl1 register unknown to assembler
dph1 = 0x85		; dph1 register unknown to assembler
dpx1 = 0x95		; dpx1 register unknown to assembler
ap = 0x9C		; ap register unknown to assembler
_ap = 0x9C		; _ap register unknown to assembler
mcnt0 = 0xD1		; mcnt0 register unknown to assembler
mcnt1 = 0xD2		; mcnt1 register unknown to assembler
ma = 0xD3		; ma register unknown to assembler
mb = 0xD4		; mb register unknown to assembler
mc = 0xD5		; mc register unknown to assembler
F1 = 0xD1		; F1 user flag unknown to assembler
esp = 0x9B		; ESP user flag unknown to assembler
;--------------------------------------------------------
; Public variables in this module
;--------------------------------------------------------
	.globl _main
;--------------------------------------------------------
; special function registers
;--------------------------------------------------------
_P4	=	0x0080
_SP	=	0x0081
_DPL	=	0x0082
_DPH	=	0x0083
_DPL1	=	0x0084
_DPH1	=	0x0085
_DPS	=	0x0086
_PCON	=	0x0087
_TCON	=	0x0088
_TMOD	=	0x0089
_TL0	=	0x008a
_TL1	=	0x008b
_TH0	=	0x008c
_TH1	=	0x008d
_CKCON	=	0x008e
_P1	=	0x0090
_EXIF	=	0x0091
_P4CNT	=	0x0092
_DPX	=	0x0093
_DPX1	=	0x0095
_SCON0	=	0x0098
_SBUF0	=	0x0099
_ESP	=	0x009b
_AP	=	0x009c
_ACON	=	0x009d
_P2	=	0x00a0
_P5	=	0x00a1
_P5CNT	=	0x00a2
_IE	=	0x00a8
_P3	=	0x00b0
_IP	=	0x00b8
_SCON1	=	0x00c0
_SBUF1	=	0x00c1
_PMR	=	0x00c4
_MCON	=	0x00c6
_TA	=	0x00c7
_T2CON	=	0x00c8
_T2MOD	=	0x00c9
_RCAP2L	=	0x00ca
_RTL2	=	0x00ca
_RCAP2H	=	0x00cb
_RTH2	=	0x00cb
_TL2	=	0x00cc
_TH2	=	0x00cd
_PSW	=	0x00d0
_MCNT0	=	0x00d1
_MCNT1	=	0x00d2
_MA	=	0x00d3
_MB	=	0x00d4
_MC	=	0x00d5
_WDCON	=	0x00d8
_ACC	=	0x00e0
_EIE	=	0x00e8
_MXAX	=	0x00ea
_B	=	0x00f0
_EIP	=	0x00f8
;--------------------------------------------------------
; special function bits 
;--------------------------------------------------------
_IT0	=	0x0088
_IE0	=	0x0089
_IT1	=	0x008a
_IE1	=	0x008b
_TR0	=	0x008c
_TF0	=	0x008d
_TR1	=	0x008e
_TF1	=	0x008f
_T2	=	0x0090
_T2EX	=	0x0091
_RXD1	=	0x0092
_TXD1	=	0x0093
_INT2	=	0x0094
_INT3	=	0x0095
_INT4	=	0x0096
_INT5	=	0x0097
_RI_0	=	0x0098
_TI_0	=	0x0099
_RB8_0	=	0x009a
_TB8_0	=	0x009b
_REN_0	=	0x009c
_SM2_0	=	0x009d
_SM1_0	=	0x009e
_SM0_0	=	0x009f
_FE_0	=	0x009f
_EX0	=	0x00a8
_ET0	=	0x00a9
_EX1	=	0x00aa
_ET1	=	0x00ab
_ES0	=	0x00ac
_ET2	=	0x00ad
_ES1	=	0x00ae
_EA	=	0x00af
_RXD0	=	0x00b0
_TXD0	=	0x00b1
_INT0	=	0x00b2
_INT1	=	0x00b3
_T0	=	0x00b4
_T1	=	0x00b5
_WR	=	0x00b6
_RD	=	0x00b7
_PX0	=	0x00b8
_PT0	=	0x00b9
_PX1	=	0x00ba
_PT1	=	0x00bb
_PS0	=	0x00bc
_PT2	=	0x00bd
_PS1	=	0x00be
_RI_1	=	0x00c0
_TI_1	=	0x00c1
_RB8_1	=	0x00c2
_TB8_1	=	0x00c3
_REN_1	=	0x00c4
_SM2_1	=	0x00c5
_SM1_1	=	0x00c6
_SM0_1	=	0x00c7
_FE_1	=	0x00c7
_CP_RL	=	0x00c8
_C_T	=	0x00c9
_TR2	=	0x00ca
_EXEN2	=	0x00cb
_TCLK	=	0x00cc
_RCLK	=	0x00cd
_EXF2	=	0x00ce
_TF2	=	0x00cf
_P	=	0x00d0
_F1	=	0x00d1
_OV	=	0x00d2
_RS0	=	0x00d3
_RS1	=	0x00d4
_F0	=	0x00d5
_AC	=	0x00d6
_CY	=	0x00d7
_RWT	=	0x00d8
_EWT	=	0x00d9
_WDRF	=	0x00da
_WDIF	=	0x00db
_PFI	=	0x00dc
_EPFI	=	0x00dd
_POR	=	0x00de
_SMOD_1	=	0x00df
_EX2	=	0x00e8
_EX3	=	0x00e9
_EX4	=	0x00ea
_EX5	=	0x00eb
_EWDI	=	0x00ec
_C1IE	=	0x00ed
_C0IE	=	0x00ee
_CANBIE	=	0x00ef
_PX2	=	0x00f8
_PX3	=	0x00f9
_PX4	=	0x00fa
_PX5	=	0x00fb
_PWDI	=	0x00fc
_C1IP	=	0x00fd
_C0IP	=	0x00fe
_CANBIP	=	0x00ff
;--------------------------------------------------------
; overlayable register banks 
;--------------------------------------------------------
	.area REG_BANK_0	(REL,OVR,DATA)
	.ds 8
	.area REG_BANK_3	(REL,OVR,DATA)
	.ds 8
;--------------------------------------------------------
; internal ram data
;--------------------------------------------------------
	.area DSEG    (DATA)
;--------------------------------------------------------
; overlayable items in internal ram 
;--------------------------------------------------------
	.area OSEG    (OVR,DATA)
;--------------------------------------------------------
; Stack segment in internal ram 
;--------------------------------------------------------
	.area	SSEG	(DATA)
__start__stack:
	.ds	1

;--------------------------------------------------------
; indirectly addressable internal ram data
;--------------------------------------------------------
	.area ISEG    (DATA)
;--------------------------------------------------------
; bit data
;--------------------------------------------------------
	.area BSEG    (BIT)
;--------------------------------------------------------
; external ram data
;--------------------------------------------------------
	.area XSEG    (XDATA)
_main_buffer_1_1::
	.ds 25
;--------------------------------------------------------
; external initialized ram data
;--------------------------------------------------------
	.area XISEG   (XDATA)
;--------------------------------------------------------
; the direct data area used by the DS80c400 ROM code.
;--------------------------------------------------------
.area ROMSEG (ABS,CON,DATA)

.ds 24 ; 24 bytes of directs used starting at 0x68

;--------------------------------------------------------
; interrupt vector 
;--------------------------------------------------------
	.area CSEG    (CODE)
__interrupt_vect:
;	DS80C400 IVT must be generated at runtime.
	sjmp	__sdcc_400boot
	.ascii	'TINI'	; required signature for 400 boot loader.
	.db	0	; selected bank: zero *should* work...
	__sdcc_400boot:	ljmp	__sdcc_gsinit_startup
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area GSINIT  (CODE)
	.area GSFINAL (CODE)
	.area GSINIT  (CODE)
__sdcc_gsinit_startup:
	lcall	__sdcc_external_startup
	mov	a,dpl
	jz	__sdcc_init_data
	ljmp	__sdcc_program_startup
__sdcc_init_data:
;	_ds390_genXINIT() start
	mov	a,#l_XINIT
	orl	a,#l_XINIT>>8
	jz	00003$
	mov	a,#s_XINIT
	add	a,#l_XINIT
	mov	r1,a
	mov	a,#s_XINIT>>8
	addc	a,#l_XINIT>>8
	mov	r2,a
	mov	dptr,#s_XINIT
	mov	dps,#0x21
	mov	dptr,#s_XISEG
00001$:	clr	a
	movc	a,@a+dptr
	movx	@dptr,a
	inc	dptr
	inc	dptr
00002$:	mov	a,dpl
	cjne	a,ar1,00001$
	mov	a,dph
	cjne	a,ar2,00001$
	mov	dps,#0
00003$:
;	_ds390_genXINIT() end
	.area GSFINAL (CODE)
	ljmp	__sdcc_program_startup
;--------------------------------------------------------
; Home
;--------------------------------------------------------
	.area HOME    (CODE)
	.area CSEG    (CODE)
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CSEG    (CODE)
__sdcc_program_startup:
	lcall	_main
;	return from main will lock up
	sjmp .
;------------------------------------------------------------
;Allocation info for local variables in function 'main'
;------------------------------------------------------------
;i                         Allocated to registers 
;s                         Allocated to registers 
;buffer                    Allocated with name '_main_buffer_1_1'
;------------------------------------------------------------
;	i2ctest.c:26: void main()
;	genFunction 
;	-----------------------------------------
;	 function main
;	-----------------------------------------
_main:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	i2ctest.c:31: printf("i2ctest LGM V0.1.1 !!!\r\n"); 
;	genIpush 
	mov	a,#__str_0
	push	acc
	mov	a,#(__str_0 >> 8)
	push	acc
	mov	a,#(__str_0 >> 16)
	push	acc
	mov	a,#0x02
	push	acc
;	genCall 
	lcall	_printf
;	stack adjustment for parms
	pop	acc
	pop	acc
	pop	acc
	pop	acc
;	i2ctest.c:32: romInit('n',SPEED_2X);
;	genAssign 
;	genAssign: resultIsFar = TRUE
	mov	dptr,#_romInit_PARM_2
	mov	a,#0x01
	movx	@dptr,a
;	genCall 
;	genSend argreg = 1, size = 1 
	mov	dpl,#0x6E
	lcall	_romInit
;	i2ctest.c:33: printf("after init_rom!!!\r\n");
;	genIpush 
	mov	a,#__str_1
	push	acc
	mov	a,#(__str_1 >> 8)
	push	acc
	mov	a,#(__str_1 >> 16)
	push	acc
	mov	a,#0x02
	push	acc
;	genCall 
	lcall	_printf
;	stack adjustment for parms
	pop	acc
	pop	acc
	pop	acc
	pop	acc
;	i2ctest.c:34: getchar();
;	genCall 
	lcall	_getchar
;	i2ctest.c:35: printf("read i2c bus!!!\r\n");
;	genIpush 
	mov	a,#__str_2
	push	acc
	mov	a,#(__str_2 >> 8)
	push	acc
	mov	a,#(__str_2 >> 16)
	push	acc
	mov	a,#0x02
	push	acc
;	genCall 
	lcall	_printf
;	stack adjustment for parms
	pop	acc
	pop	acc
	pop	acc
	pop	acc
;	i2ctest.c:36: buffer[0]=0x00;
;	genPointerSet 
	mov	dptr,#_main_buffer_1_1
; Peephole 180   changed mov to clr
	clr  a
	movx	@dptr,a
;	i2ctest.c:37: buffer[1]=0x06;
;	genPointerSet 
	mov	dptr,#(_main_buffer_1_1 + 0x0001)
	mov	a,#0x06
	movx	@dptr,a
;	i2ctest.c:38: buffer[2]=0x0F;
;	genPointerSet 
	mov	dptr,#(_main_buffer_1_1 + 0x0002)
	mov	a,#0x0F
	movx	@dptr,a
;	i2ctest.c:39: buffer[3]=0xF0;
;	genPointerSet 
	mov	dptr,#(_main_buffer_1_1 + 0x0003)
	mov	a,#0xF0
	movx	@dptr,a
;	i2ctest.c:40: buffer[4]=0xA0;
;	genPointerSet 
	mov	dptr,#(_main_buffer_1_1 + 0x0004)
	mov	a,#0xA0
	movx	@dptr,a
;	i2ctest.c:41: buffer[5]=0x0E;
;	genPointerSet 
	mov	dptr,#(_main_buffer_1_1 + 0x0005)
	mov	a,#0x0E
	movx	@dptr,a
;	i2ctest.c:42: while(1) {
;	genLabel 
00102$:
;	i2ctest.c:44: buffer[0]=0xaa;
;	genPointerSet 
	mov	dptr,#_main_buffer_1_1
	mov	a,#0xAA
	movx	@dptr,a
;	i2ctest.c:45: buffer[1]=0xaa;
;	genPointerSet 
	mov	dptr,#(_main_buffer_1_1 + 0x0001)
	mov	a,#0xAA
	movx	@dptr,a
;	i2ctest.c:47: i2c_select(0x72);
;	genCall 
;	genSend argreg = 1, size = 1 
	mov	dpl,#0x72
	lcall	_i2c_select
;	i2ctest.c:48: i2c_stop();
;	genCall 
	lcall	_i2c_stop
;	i2ctest.c:49: s=i2c_readbyte(1);
;	genCall 
;	genSend argreg = 1, size = 1 
	mov	dpl,#0x01
	lcall	_i2c_readbyte
;	genAssign 
;	genAssign: resultIsFar = FALSE
	mov	r2,dpl
;	i2ctest.c:50: printf("read %x\n\r",s & 0xFF);
;	genCast 
	mov	r3,#0
;	genIpush 
	push	ar2
	push	ar3
;	genIpush 
	mov	a,#__str_3
	push	acc
	mov	a,#(__str_3 >> 8)
	push	acc
	mov	a,#(__str_3 >> 16)
	push	acc
	mov	a,#0x02
	push	acc
;	genCall 
	lcall	_printf
;	stack adjustment for parms
	pop	acc
	pop	acc
	pop	acc
	pop	acc
	pop	acc
	pop	acc
;	genGoto 
;	genLabel 
; Peephole 132   changed ljmp to sjmp
	sjmp 00102$
00104$:
;	genEndFunction 
	ret
	.area CSEG    (CODE)
__str_0:
	.ascii "i2ctest LGM V0.1.1 !!!"
	.db 0x0D
	.db 0x0A
	.db 0x00
__str_1:
	.ascii "after init_rom!!!"
	.db 0x0D
	.db 0x0A
	.db 0x00
__str_2:
	.ascii "read i2c bus!!!"
	.db 0x0D
	.db 0x0A
	.db 0x00
__str_3:
	.ascii "read %x"
	.db 0x0A
	.db 0x0D
	.db 0x00
	.area XINIT   (CODE)
