;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.3.7 Mon Feb 23 01:52:21 2004

;--------------------------------------------------------
	.module i2c400
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
	.globl _i2c_recv
	.globl _I2CByteIn
	.globl _ByteOutI2C
	.globl _BitInI2C
	.globl _BitOutI2C
	.globl _I2CSendReceive_PARM_3
	.globl _I2CSendReceive_PARM_2
	.globl _i2c_recv_PARM_2
	.globl _I2CSendStop_PARM_3
	.globl _I2CSendStop_PARM_2
	.globl _i2cReceiveBuffer
	.globl _i2cTransmitBuffer
	.globl _I2CReset
	.globl _I2CStart
	.globl _I2CStop
	.globl _I2CSendStop
	.globl _I2CSendReceive
	.globl _I2CDumpError
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
_i2cTransmitBuffer::
	.ds 128
_i2cReceiveBuffer::
	.ds 128
_I2CSendStop_PARM_2::
	.ds 1
_I2CSendStop_PARM_3::
	.ds 1
_i2c_recv_PARM_2::
	.ds 1
_I2CSendReceive_PARM_2::
	.ds 1
_I2CSendReceive_PARM_3::
	.ds 1
;--------------------------------------------------------
; external initialized ram data
;--------------------------------------------------------
	.area XISEG   (XDATA)
_i2cError:
	.ds 1
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area GSINIT  (CODE)
	.area GSFINAL (CODE)
	.area GSINIT  (CODE)
;--------------------------------------------------------
; Home
;--------------------------------------------------------
	.area HOME    (CODE)
	.area CSEG    (CODE)
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CSEG    (CODE)
;------------------------------------------------------------
;Allocation info for local variables in function 'I2CReset'
;------------------------------------------------------------
;------------------------------------------------------------
;	i2c400.c:84: char I2CReset(void)
;	genFunction 
;	-----------------------------------------
;	 function I2CReset
;	-----------------------------------------
_I2CReset:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	i2c400.c:86: SDA_LOW;
;	genAnd 
;	i2c400.c:87: SCL_LOW;
;	genAnd 
; Peephole 183   avoided anl during execution
	anl  _P1,#(0xFE & 0xFD)
;	i2c400.c:88: SCL_HIGH;
;	genOr 
	orl	_P1,#0x02
;	i2c400.c:89: SDA_HIGH;
;	genOr 
	orl	_P1,#0x01
;	i2c400.c:90: i2cError = 0;
;	genAssign 
;	genAssign: resultIsFar = TRUE
	mov	dptr,#_i2cError
; Peephole 180   changed mov to clr
	clr  a
	movx	@dptr,a
;	i2c400.c:91: return (SCL_IN && SDA_IN);
;	genAnd 
;	better literal AND.
	mov	a,_P1
	anl	a, #0x01
	mov	r2,a
;	genRightShift 
;	genRightShiftLiteral (1), size 1
;	genrshOne
	mov	a,_P1
	clr	c
	rrc	a
	mov	r3,a
;	genAnd 
	anl	ar3,#0x01
;	genAndOp 
	mov	a,r2
	jz	00103$
	mov	a,r3
00103$:
	jz	00104$
	mov	a,#1
00104$:
	mov	r2,a
;	genRet 
	mov	dpl,r2
;	genLabel 
00101$:
;	genEndFunction 
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'I2CStart'
;------------------------------------------------------------
;------------------------------------------------------------
;	i2c400.c:100: char I2CStart(void)
;	genFunction 
;	-----------------------------------------
;	 function I2CStart
;	-----------------------------------------
_I2CStart:
;	i2c400.c:102: SDA_HIGH;
;	genOr 
	orl	_P1,#0x01
;	i2c400.c:103: SCL_HIGH;
;	genOr 
	orl	_P1,#0x02
;	i2c400.c:104: I2CDelay(I2CDELAY);
;	genInline 
	 nop nop nop nop nop nop nop nop nop nop nop nop nop nop 
;	i2c400.c:105: SDA_LOW;        /* Pull SDA down... */
;	genAnd 
	anl	_P1,#0xFE
;	i2c400.c:106: I2CDelay(I2CDELAY);
;	genInline 
	 nop nop nop nop nop nop nop nop nop nop nop nop nop nop 
;	i2c400.c:107: SCL_LOW;        /* ...and then SCL -> start condition. */
;	genAnd 
	anl	_P1,#0xFD
;	i2c400.c:108: I2CDelay(I2CDELAY);
;	genInline 
	 nop nop nop nop nop nop nop nop nop nop nop nop nop nop 
;	i2c400.c:109: return 0;
;	genRet 
	mov	dpl,#0x00
;	genLabel 
00101$:
;	genEndFunction 
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'I2CStop'
;------------------------------------------------------------
;------------------------------------------------------------
;	i2c400.c:118: char I2CStop(void)
;	genFunction 
;	-----------------------------------------
;	 function I2CStop
;	-----------------------------------------
_I2CStop:
;	i2c400.c:120: SDA_LOW;
;	genAnd 
	anl	_P1,#0xFE
;	i2c400.c:121: SCL_HIGH;        /* Let SCL go up */
;	genOr 
	orl	_P1,#0x02
;	i2c400.c:122: I2CDelay(I2CDELAY);
;	genInline 
	 nop nop nop nop nop nop nop nop nop nop nop nop nop nop 
;	i2c400.c:123: SDA_HIGH;        /* ...and then SDA up -> stop condition. */
;	genOr 
	orl	_P1,#0x01
;	i2c400.c:124: I2CDelay(I2CDELAY);
;	genInline 
	 nop nop nop nop nop nop nop nop nop nop nop nop nop nop 
;	i2c400.c:126: return (SCL_IN && SDA_IN);  /* Both will be up, if everything is fine */
;	genAnd 
;	better literal AND.
	mov	a,_P1
	anl	a, #0x01
	mov	r2,a
;	genRightShift 
;	genRightShiftLiteral (1), size 1
;	genrshOne
	mov	a,_P1
	clr	c
	rrc	a
	mov	r3,a
;	genAnd 
	anl	ar3,#0x01
;	genAndOp 
	mov	a,r2
	jz	00103$
	mov	a,r3
00103$:
	jz	00104$
	mov	a,#1
00104$:
	mov	r2,a
;	genRet 
	mov	dpl,r2
;	genLabel 
00101$:
;	genEndFunction 
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'BitOutI2C'
;------------------------------------------------------------
;bout                      Allocated to registers r2 
;------------------------------------------------------------
;	i2c400.c:135: char BitOutI2C(char bout)
;	genFunction 
;	-----------------------------------------
;	 function BitOutI2C
;	-----------------------------------------
_BitOutI2C:
;	genReceive 
	mov	r2,dpl
;	i2c400.c:137: printf("entering BitOutI2C\n\r");
;	genIpush 
	push	ar2
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
	pop	ar2
;	i2c400.c:139: SDA_OUT(bout);              /* Put data out on SDA */
;	genIfx 
	mov	a,r2
;	genIfxJump
; Peephole 110   removed ljmp by inverse jump logic
	jz  00108$
00114$:
;	genOr 
	orl	_P1,#0x01
;	genGoto 
;	genLabel 
; Peephole 132   changed ljmp to sjmp
	sjmp 00109$
00108$:
;	genAnd 
	anl	_P1,#0xFE
;	genLabel 
00109$:
;	i2c400.c:140: I2CDelay(I2CDELAY);
;	genInline 
	 nop nop nop nop nop nop nop nop nop nop nop nop nop nop 
;	i2c400.c:141: SCL_HIGH;                    /* Let SCL go up */
;	genOr 
	orl	_P1,#0x02
;	i2c400.c:142: while(!SCL_IN)            /* Wait until all other devices are ready */
;	genLabel 
00101$:
;	genAnd 
	mov	a,_P1
;	genIfxJump
; Peephole 112   removed ljmp by inverse jump logic
	jb   acc.0,00103$
00115$:
;	i2c400.c:145: printf("/\n\r");
;	genIpush 
	push	ar2
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
	pop	ar2
;	genGoto 
;	genLabel 
; Peephole 132   changed ljmp to sjmp
	sjmp 00101$
00103$:
;	i2c400.c:148: if (SDA_IN != bout)       /* Arbitration lost, release bus and return */
;	genRightShift 
;	genRightShiftLiteral (1), size 1
;	genrshOne
	mov	a,_P1
	clr	c
	rrc	a
;	genAnd 
;	genCmpEq 
;	gencjneshort
; Peephole 139   removed redundant mov
	anl  a,#0x01
	mov  r3,a
	cjne	a,ar2,00116$
; Peephole 132   changed ljmp to sjmp
	sjmp 00105$
00116$:
;	i2c400.c:150: SDA_HIGH;                /* Should be up anyway, but make sure */
;	genOr 
	orl	_P1,#0x01
;	i2c400.c:151: i2cError = I2CERR_LOST;
;	genAssign 
;	genAssign: resultIsFar = TRUE
	mov	dptr,#_i2cError
	mov	a,#0x02
	movx	@dptr,a
;	i2c400.c:152: I2CDumpError(i2cError);
;	genCall 
;	genSend argreg = 1, size = 1 
	mov	dpl,#0x02
	lcall	_I2CDumpError
;	i2c400.c:153: return 1;
;	genRet 
	mov	dpl,#0x01
;	genLabel 
; Peephole 132   changed ljmp to sjmp
	sjmp 00106$
00105$:
;	i2c400.c:155: I2CDelay(I2CDELAY);
;	genInline 
	 nop nop nop nop nop nop nop nop nop nop nop nop nop nop 
;	i2c400.c:156: SCL_LOW;                    /* Pull SCL back down */
;	genAnd 
	anl	_P1,#0xFD
;	i2c400.c:157: I2CDelay(I2CDELAY);                
;	genInline 
	 nop nop nop nop nop nop nop nop nop nop nop nop nop nop 
;	i2c400.c:158: return 0;                   /* OK */
;	genRet 
	mov	dpl,#0x00
;	genLabel 
00106$:
;	genEndFunction 
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'BitInI2C'
;------------------------------------------------------------
;bin                       Allocated to registers r2 
;------------------------------------------------------------
;	i2c400.c:166: char BitInI2C(void)
;	genFunction 
;	-----------------------------------------
;	 function BitInI2C
;	-----------------------------------------
_BitInI2C:
;	i2c400.c:169: printf("entering BitInI2C\n\r");
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
;	i2c400.c:171: SDA_HIGH;
;	genOr 
	orl	_P1,#0x01
;	i2c400.c:173: SCL_HIGH;                    /* Let SCL go up */
;	genOr 
	orl	_P1,#0x02
;	i2c400.c:174: while(!SCL_IN)            /* Wait for other devices */
;	genLabel 
00101$:
;	genAnd 
	mov	a,_P1
;	genIfxJump
; Peephole 112   removed ljmp by inverse jump logic
	jb   acc.0,00103$
00108$:
;	i2c400.c:177: printf("+\n\r");
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
;	genGoto 
;	genLabel 
; Peephole 132   changed ljmp to sjmp
	sjmp 00101$
00103$:
;	i2c400.c:179: bin = SDA_IN;             /* Read in data */
;	genRightShift 
;	genRightShiftLiteral (1), size 1
;	genrshOne
	mov	a,_P1
	clr	c
	rrc	a
	mov	r2,a
;	genAnd 
	anl	ar2,#0x01
;	i2c400.c:180: I2CDelay(I2CDELAY);
;	genInline 
	 nop nop nop nop nop nop nop nop nop nop nop nop nop nop 
;	i2c400.c:181: SCL_LOW;                    /* Pull SCL back up */
;	genAnd 
	anl	_P1,#0xFD
;	i2c400.c:182: I2CDelay(I2CDELAY);
;	genInline 
	 nop nop nop nop nop nop nop nop nop nop nop nop nop nop 
;	i2c400.c:183: return bin;                 /* Return the sampled bit */
;	genRet 
	mov	dpl,r2
;	genLabel 
00104$:
;	genEndFunction 
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'ByteOutI2C'
;------------------------------------------------------------
;dat                       Allocated to registers r2 
;bit_count                 Allocated to registers 
;------------------------------------------------------------
;	i2c400.c:194: char ByteOutI2C(char dat)
;	genFunction 
;	-----------------------------------------
;	 function ByteOutI2C
;	-----------------------------------------
_ByteOutI2C:
;	genReceive 
	mov	r2,dpl
;	i2c400.c:199: printf("entering byteout");
;	genIpush 
	push	ar2
	mov	a,#__str_4
	push	acc
	mov	a,#(__str_4 >> 8)
	push	acc
	mov	a,#(__str_4 >> 16)
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
	pop	ar2
;	i2c400.c:201: while(bit_count) {
;	genAssign 
;	genAssign: resultIsFar = FALSE
	mov	r3,#0x08
;	genLabel 
00108$:
;	genIfx 
	mov	a,r3
;	genIfxJump
	jnz	00121$
	ljmp	00110$
00121$:
;	i2c400.c:202: printf(".");
;	genIpush 
	push	ar2
	push	ar3
	mov	a,#__str_5
	push	acc
	mov	a,#(__str_5 >> 8)
	push	acc
	mov	a,#(__str_5 >> 16)
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
	pop	ar3
	pop	ar2
;	i2c400.c:203: if (dat & 0x80) {
;	genAnd 
	mov	a,r2
;	genIfxJump
; Peephole 111   removed ljmp by inverse jump logic
	jnb  acc.7,00106$
00122$:
;	i2c400.c:204: if (BitOutI2C(1)) {
;	genCall 
	push	ar2
	push	ar3
;	genSend argreg = 1, size = 1 
	mov	dpl,#0x01
	lcall	_BitOutI2C
	pop	ar3
	pop	ar2
;	genIfx 
	mov	a,dpl
;	genIfxJump
; Peephole 110   removed ljmp by inverse jump logic
	jz  00107$
00123$:
;	i2c400.c:205: I2CDumpError(i2cError);
;	genCall 
;	genSend argreg = 1, size = 1 
	mov	dptr,#_i2cError
	movx	a,@dptr
	mov	dpl,a
	lcall	_I2CDumpError
;	i2c400.c:206: return 1;
;	genRet 
	mov	dpl,#0x01
	ljmp	00113$
;	genLabel 
00106$:
;	i2c400.c:209: if (BitOutI2C(0)) {
;	genCall 
	push	ar2
	push	ar3
;	genSend argreg = 1, size = 1 
	mov	dpl,#0x00
	lcall	_BitOutI2C
	pop	ar3
	pop	ar2
;	genIfx 
	mov	a,dpl
;	genIfxJump
; Peephole 110   removed ljmp by inverse jump logic
	jz  00107$
00124$:
;	i2c400.c:210: I2CDumpError(i2cError);
;	genCall 
;	genSend argreg = 1, size = 1 
	mov	dptr,#_i2cError
	movx	a,@dptr
	mov	dpl,a
	lcall	_I2CDumpError
;	i2c400.c:211: return 1;
;	genRet 
	mov	dpl,#0x01
;	genLabel 
; Peephole 132   changed ljmp to sjmp
	sjmp 00113$
00107$:
;	i2c400.c:214: dat <<= 1;
;	genCast 
	mov	ar4,r2
	mov	a,r2
	rlc	a
	subb	a,acc
;	genLeftShift 
;	genLeftShiftLiteral (1), size 2
;	genlshTwo 
; Peephole 105   removed redundant mov
	mov  r5,a
	xch	a,r4
	add	a,acc
	xch	a,r4
	rlc	a
	mov	r5,a
;	genCast 
	mov	ar2,r4
;	i2c400.c:215: bit_count--;
;	genMinus 
	dec	r3
;	genGoto 
	ljmp	00108$
;	genLabel 
00110$:
;	i2c400.c:217: printf("+");
;	genIpush 
	mov	a,#__str_6
	push	acc
	mov	a,#(__str_6 >> 8)
	push	acc
	mov	a,#(__str_6 >> 16)
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
;	i2c400.c:219: if (BitInI2C()) {
;	genCall 
	lcall	_BitInI2C
;	genIfx 
	mov	a,dpl
;	genIfxJump
; Peephole 110   removed ljmp by inverse jump logic
	jz  00112$
00125$:
;	i2c400.c:220: i2cError = I2CERR_NAK;
;	genAssign 
;	genAssign: resultIsFar = TRUE
	mov	dptr,#_i2cError
	mov	a,#0x01
	movx	@dptr,a
;	i2c400.c:221: I2CDumpError(i2cError);
;	genCall 
;	genSend argreg = 1, size = 1 
	mov	dpl,#0x01
	lcall	_I2CDumpError
;	i2c400.c:222: return 1;
;	genRet 
	mov	dpl,#0x01
;	genLabel 
; Peephole 132   changed ljmp to sjmp
;	i2c400.c:224: return 0;
;	genRet 
;	genLabel 
;	genEndFunction 
; Peephole 237a  removed sjmp to ret
	ret
00112$:
	mov     dpl,#0x00
00113$:
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'I2CByteIn'
;------------------------------------------------------------
;ack                       Allocated to registers r2 
;bit_count                 Allocated to registers 
;byte_in                   Allocated to registers r3 
;------------------------------------------------------------
;	i2c400.c:235: char I2CByteIn(char ack)
;	genFunction 
;	-----------------------------------------
;	 function I2CByteIn
;	-----------------------------------------
_I2CByteIn:
;	genReceive 
	mov	r2,dpl
;	i2c400.c:240: byte_in = 0;
;	genAssign 
;	genAssign: resultIsFar = TRUE
	mov	r3,#0x00
;	i2c400.c:242: while(bit_count)
;	genAssign 
;	genAssign: resultIsFar = FALSE
	mov	r4,#0x08
;	genLabel 
00103$:
;	genIfx 
	mov	a,r4
;	genIfxJump
; Peephole 110   removed ljmp by inverse jump logic
	jz  00105$
00111$:
;	i2c400.c:244: byte_in <<= 1;
;	genCast 
	mov	ar5,r3
	mov	a,r3
	rlc	a
	subb	a,acc
;	genLeftShift 
;	genLeftShiftLiteral (1), size 2
;	genlshTwo 
; Peephole 105   removed redundant mov
	mov  r6,a
	xch	a,r5
	add	a,acc
	xch	a,r5
	rlc	a
	mov	r6,a
;	genCast 
	mov	ar3,r5
;	i2c400.c:245: if (BitInI2C()) byte_in |= 0x01;
;	genCall 
	push	ar2
	push	ar3
	push	ar4
	lcall	_BitInI2C
	pop	ar4
	pop	ar3
	pop	ar2
;	genIfx 
	mov	a,dpl
;	genIfxJump
; Peephole 110   removed ljmp by inverse jump logic
	jz  00102$
00112$:
;	genOr 
	orl	ar3,#0x01
;	genLabel 
00102$:
;	i2c400.c:246: bit_count--;
;	genMinus 
	dec	r4
;	genGoto 
;	genLabel 
; Peephole 132   changed ljmp to sjmp
	sjmp 00103$
00105$:
;	i2c400.c:249: BitOutI2C(ack);
;	genCall 
	push	ar3
;	genSend argreg = 1, size = 1 
	mov	dpl,r2
	lcall	_BitOutI2C
	pop	ar3
;	i2c400.c:250: SDA_HIGH;             /* Added 18-Jul-95 - thanks to Ray Bellis */
;	genOr 
	orl	_P1,#0x01
;	i2c400.c:251: return byte_in;
;	genRet 
	mov	dpl,r3
;	genLabel 
00106$:
;	genEndFunction 
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'I2CSendStop'
;------------------------------------------------------------
;count                     Allocated with name '_I2CSendStop_PARM_2'
;send_stop                 Allocated with name '_I2CSendStop_PARM_3'
;addr                      Allocated to registers r2 
;byteptr                   Allocated to registers 
;byte_out                  Allocated to registers r2 
;------------------------------------------------------------
;	i2c400.c:260: char I2CSendStop(char addr, char count, char send_stop)
;	genFunction 
;	-----------------------------------------
;	 function I2CSendStop
;	-----------------------------------------
_I2CSendStop:
;	genReceive 
	mov	r2,dpl
;	i2c400.c:264: if (I2CStart()) return 1;
;	genCall 
	push	ar2
	lcall	_I2CStart
	pop	ar2
;	genIfx 
	mov	a,dpl
;	genIfxJump
; Peephole 110   removed ljmp by inverse jump logic
	jz  00102$
00122$:
;	genRet 
	mov	dpl,#0x01
	ljmp	00113$
;	genLabel 
00102$:
;	i2c400.c:265: i2cError = 0;
;	genAssign 
;	genAssign: resultIsFar = TRUE
	mov	dptr,#_i2cError
; Peephole 180   changed mov to clr
	clr  a
	movx	@dptr,a
;	i2c400.c:267: byte_out = addr & 0xfe;     /* Ensure that it's a write address */
;	genAnd 
	anl	ar2,#0xFE
;	i2c400.c:268: count++;                    /* Include slave address to byte count */
;	genAssign 
	mov	dptr,#_I2CSendStop_PARM_2
;	genAssign: resultIsFar = FALSE
	movx	a,@dptr
	mov	r3,a
;	genPlus 
	mov	dptr,#_I2CSendStop_PARM_2
	mov	a,#0x01
	add	a,r3
	movx	@dptr,a
;	i2c400.c:270: while(count)
;	genAssign 
;	genAssign: resultIsFar = FALSE
	mov	r3,#0x00
;	genAssign 
	mov	dptr,#_I2CSendStop_PARM_2
;	genAssign: resultIsFar = FALSE
	movx	a,@dptr
	mov	r4,a
;	genLabel 
00108$:
;	genIfx 
	mov	a,r4
;	genIfxJump
; Peephole 132   changed ljmp to sjmp
; Peephole 162   removed sjmp by inverse jump logic
	jz   00110$
00123$:
;	i2c400.c:272: if (ByteOutI2C(byte_out))
;	genCall 
	push	ar3
	push	ar4
;	genSend argreg = 1, size = 1 
	mov	dpl,r2
	lcall	_ByteOutI2C
	pop	ar4
	pop	ar3
;	genIfx 
	mov	a,dpl
;	genIfxJump
; Peephole 110   removed ljmp by inverse jump logic
	jz  00107$
00124$:
;	i2c400.c:274: if (i2cError == I2CERR_NAK && send_stop) I2CStop();
;	genCmpEq 
	mov	dptr,#_i2cError
;	gencjneshort
	movx	a,@dptr
; Peephole 132   changed ljmp to sjmp
; Peephole 199   optimized misc jump sequence
	cjne a,#0x01,00104$
;00125$:
; Peephole 200   removed redundant sjmp
00126$:
;	genIfx 
	mov	dptr,#_I2CSendStop_PARM_3
	movx	a,@dptr
;	genIfxJump
; Peephole 110   removed ljmp by inverse jump logic
	jz  00104$
00127$:
;	genCall 
	lcall	_I2CStop
;	genLabel 
00104$:
;	i2c400.c:275: return i2cError;
;	genRet 
	mov     dps, #1
	mov     dptr, #_i2cError
	movx	a,@dptr
	mov	dpl,a
	mov	dps,#0
;	genLabel 
; Peephole 132   changed ljmp to sjmp
	sjmp 00113$
00107$:
;	i2c400.c:277: byte_out = i2cTransmitBuffer[byteptr];
;	genPlus 
	mov	a,r3
	add	a,#_i2cTransmitBuffer
	mov	dpl,a
; Peephole 3.d   changed mov to clr
	clr	a
	addc	a,#(_i2cTransmitBuffer >> 8)
	mov	dph,a
; Peephole 3.d   changed mov to clr
	clr	a
	addc	a,#(_i2cTransmitBuffer >> 16)
	mov	dpx,a
;	genPointerGet 
;	genFarPointerGet
	movx	a,@dptr
	mov	r5,a
;	genAssign 
;	genAssign: resultIsFar = TRUE
	mov	ar2,r5
;	i2c400.c:278: byteptr++;
;	genPlus 
	inc	r3
;	did genPlusIncr
;	i2c400.c:279: count--;
;	genMinus 
	dec	r4
;	genGoto 
;	genLabel 
; Peephole 132   changed ljmp to sjmp
	sjmp 00108$
00110$:
;	i2c400.c:282: if (send_stop) I2CStop();
;	genIfx 
	mov	dptr,#_I2CSendStop_PARM_3
	movx	a,@dptr
;	genIfxJump
; Peephole 110   removed ljmp by inverse jump logic
	jz  00112$
00128$:
;	genCall 
	lcall	_I2CStop
;	genLabel 
00112$:
;	i2c400.c:283: return 0;
;	genRet 
	mov	dpl,#0x00
;	genLabel 
00113$:
;	genEndFunction 
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'i2c_recv'
;------------------------------------------------------------
;count                     Allocated with name '_i2c_recv_PARM_2'
;addr                      Allocated to registers r2 
;byteptr                   Allocated to registers 
;byte_in                   Allocated to registers r2 
;------------------------------------------------------------
;	i2c400.c:292: char i2c_recv(char addr, char count)
;	genFunction 
;	-----------------------------------------
;	 function i2c_recv
;	-----------------------------------------
_i2c_recv:
;	genReceive 
	mov	r2,dpl
;	i2c400.c:296: printf("0\n\r");
;	genIpush 
	push	ar2
	mov	a,#__str_7
	push	acc
	mov	a,#(__str_7 >> 8)
	push	acc
	mov	a,#(__str_7 >> 16)
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
	pop	ar2
;	i2c400.c:297: if (I2CStart()) return 1;
;	genCall 
	push	ar2
	lcall	_I2CStart
	pop	ar2
;	genIfx 
	mov	a,dpl
;	genIfxJump
; Peephole 110   removed ljmp by inverse jump logic
	jz  00102$
00124$:
;	genRet 
	mov	dpl,#0x01
	ljmp	00113$
;	genLabel 
00102$:
;	i2c400.c:298: printf("1\n\r");
;	genIpush 
	push	ar2
	mov	a,#__str_8
	push	acc
	mov	a,#(__str_8 >> 8)
	push	acc
	mov	a,#(__str_8 >> 16)
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
	pop	ar2
;	i2c400.c:299: i2cError = 0;
;	genAssign 
;	genAssign: resultIsFar = TRUE
	mov	dptr,#_i2cError
; Peephole 180   changed mov to clr
	clr  a
	movx	@dptr,a
;	i2c400.c:302: byte_in = addr | 0x01;
;	genOr 
	orl	ar2,#0x01
;	i2c400.c:304: if (ByteOutI2C(byte_in))
;	genCall 
;	genSend argreg = 1, size = 1 
	mov	dpl,r2
	lcall	_ByteOutI2C
;	genIfx 
	mov	a,dpl
;	genIfxJump
	jnz	00125$
	ljmp	00106$
00125$:
;	i2c400.c:306: printf("+1\n\r");
;	genIpush 
	mov	a,#__str_9
	push	acc
	mov	a,#(__str_9 >> 8)
	push	acc
	mov	a,#(__str_9 >> 16)
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
;	i2c400.c:307: if (i2cError == I2CERR_NAK) {printf("--1\n\r");
;	genCmpEq 
	mov	dptr,#_i2cError
;	gencjneshort
	movx	a,@dptr
; Peephole 132   changed ljmp to sjmp
; Peephole 199   optimized misc jump sequence
	cjne a,#0x01,00104$
;00126$:
; Peephole 200   removed redundant sjmp
00127$:
;	genIpush 
	mov	a,#__str_10
	push	acc
	mov	a,#(__str_10 >> 8)
	push	acc
	mov	a,#(__str_10 >> 16)
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
;	i2c400.c:308: I2CStop();printf("apres stop\n\r");
;	genCall 
	lcall	_I2CStop
;	genIpush 
	mov	a,#__str_11
	push	acc
	mov	a,#(__str_11 >> 8)
	push	acc
	mov	a,#(__str_11 >> 16)
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
;	genLabel 
00104$:
;	i2c400.c:310: return i2cError;
;	genRet 
	mov     dps, #1
	mov     dptr, #_i2cError
	movx	a,@dptr
	mov	dpl,a
	mov	dps,#0
	ljmp	00113$
;	genLabel 
00106$:
;	i2c400.c:312: printf("2\n\r");
;	genIpush 
	mov	a,#__str_12
	push	acc
	mov	a,#(__str_12 >> 8)
	push	acc
	mov	a,#(__str_12 >> 16)
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
;	i2c400.c:314: while(count)
;	genAssign 
	mov	dptr,#_i2c_recv_PARM_2
;	genAssign: resultIsFar = FALSE
	movx	a,@dptr
	mov	r3,a
;	genAssign 
;	genAssign: resultIsFar = FALSE
	mov	r4,#0x00
;	genLabel 
00110$:
;	genIfx 
	mov	a,r3
;	genIfxJump
	jnz	00128$
	ljmp	00112$
00128$:
;	i2c400.c:316: count-=1;
;	genMinus 
	dec	r3
;	i2c400.c:317: if (count) {
;	genIfx 
	mov	a,r3
;	genIfxJump
; Peephole 110   removed ljmp by inverse jump logic
	jz  00108$
00129$:
;	i2c400.c:318: byte_in = I2CByteIn(0);
;	genCall 
	push	ar3
	push	ar4
;	genSend argreg = 1, size = 1 
	mov	dpl,#0x00
	lcall	_I2CByteIn
	mov	r5,dpl
	pop	ar4
	pop	ar3
;	genAssign 
;	genAssign: resultIsFar = TRUE
	mov	ar2,r5
;	i2c400.c:319: printf("3\n\r");
;	genIpush 
	push	ar2
	push	ar3
	push	ar4
	mov	a,#__str_13
	push	acc
	mov	a,#(__str_13 >> 8)
	push	acc
	mov	a,#(__str_13 >> 16)
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
	pop	ar4
	pop	ar3
	pop	ar2
;	genGoto 
;	genLabel 
; Peephole 132   changed ljmp to sjmp
	sjmp 00109$
00108$:
;	i2c400.c:322: byte_in = I2CByteIn(1);   /* No ACK during last byte */
;	genCall 
	push	ar3
	push	ar4
;	genSend argreg = 1, size = 1 
	mov	dpl,#0x01
	lcall	_I2CByteIn
	mov	r5,dpl
	pop	ar4
	pop	ar3
;	genAssign 
;	genAssign: resultIsFar = TRUE
	mov	ar2,r5
;	i2c400.c:323: printf("4\n\r");
;	genIpush 
	push	ar2
	push	ar3
	push	ar4
	mov	a,#__str_14
	push	acc
	mov	a,#(__str_14 >> 8)
	push	acc
	mov	a,#(__str_14 >> 16)
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
	pop	ar4
	pop	ar3
	pop	ar2
;	genLabel 
00109$:
;	i2c400.c:325: i2cReceiveBuffer[byteptr] = byte_in;
;	genPlus 
	mov	a,r4
	add	a,#_i2cReceiveBuffer
	mov	dpl,a
; Peephole 3.d   changed mov to clr
	clr	a
	addc	a,#(_i2cReceiveBuffer >> 8)
	mov	dph,a
; Peephole 3.d   changed mov to clr
	clr	a
	addc	a,#(_i2cReceiveBuffer >> 16)
	mov	dpx,a
;	genPointerSet 
	mov	a,r2
	movx	@dptr,a
;	i2c400.c:326: byteptr++;
;	genPlus 
	inc	r4
;	did genPlusIncr
;	i2c400.c:327: printf("5\n\r");
;	genIpush 
	push	ar3
	push	ar4
	mov	a,#__str_15
	push	acc
	mov	a,#(__str_15 >> 8)
	push	acc
	mov	a,#(__str_15 >> 16)
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
	pop	ar4
	pop	ar3
;	genGoto 
	ljmp	00110$
;	genLabel 
00112$:
;	i2c400.c:329: printf("6\n\r");
;	genIpush 
	mov	a,#__str_16
	push	acc
	mov	a,#(__str_16 >> 8)
	push	acc
	mov	a,#(__str_16 >> 16)
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
;	i2c400.c:331: I2CStop();
;	genCall 
	lcall	_I2CStop
;	i2c400.c:333: return (i2cError ? 1 : 0);
;	genIfx 
	mov	dptr,#_i2cError
	movx	a,@dptr
;	genIfxJump
; Peephole 110   removed ljmp by inverse jump logic
	jz  00115$
00130$:
;	genAssign 
;	genAssign: resultIsFar = FALSE
	mov	r2,#0x01
;	genGoto 
;	genLabel 
; Peephole 132   changed ljmp to sjmp
	sjmp 00116$
00115$:
;	genAssign 
;	genAssign: resultIsFar = FALSE
	mov	r2,#0x00
;	genLabel 
00116$:
;	genRet 
	mov	dpl,r2
;	genLabel 
00113$:
;	genEndFunction 
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'I2CSendReceive'
;------------------------------------------------------------
;tx_count                  Allocated with name '_I2CSendReceive_PARM_2'
;rx_count                  Allocated with name '_I2CSendReceive_PARM_3'
;addr                      Allocated to registers r2 
;------------------------------------------------------------
;	i2c400.c:344: char I2CSendReceive(char addr, char tx_count, char rx_count)
;	genFunction 
;	-----------------------------------------
;	 function I2CSendReceive
;	-----------------------------------------
_I2CSendReceive:
;	genReceive 
	mov	r2,dpl
;	i2c400.c:346: if (I2CSendStop(addr, tx_count, 0))
;	genAssign 
	mov	dptr,#_I2CSendReceive_PARM_2
;	genAssign: resultIsFar = TRUE
;	genFarFarAssign (1 byte case)
	movx	a,@dptr
	mov	dptr,#_I2CSendStop_PARM_2
	movx	@dptr,a
;	genAssign 
;	genAssign: resultIsFar = TRUE
	mov	dptr,#_I2CSendStop_PARM_3
; Peephole 180   changed mov to clr
	clr  a
	movx	@dptr,a
;	genCall 
	push	ar2
;	genSend argreg = 1, size = 1 
	mov	dpl,r2
	lcall	_I2CSendStop
	pop	ar2
;	genIfx 
	mov	a,dpl
;	genIfxJump
; Peephole 110   removed ljmp by inverse jump logic
	jz  00104$
00115$:
;	i2c400.c:351: if (i2cError != I2CERR_LOST) I2CStop();
;	genCmpEq 
	mov	dptr,#_i2cError
;	gencjneshort
	movx	a,@dptr
	cjne	a,#0x02,00116$
; Peephole 132   changed ljmp to sjmp
	sjmp 00102$
00116$:
;	genCall 
	lcall	_I2CStop
;	genLabel 
00102$:
;	i2c400.c:352: return 1;
;	genRet 
	mov	dpl,#0x01
;	genLabel 
; Peephole 132   changed ljmp to sjmp
	sjmp 00107$
00104$:
;	i2c400.c:355: SDA_HIGH; /* One of these may be low now, in which case the next */
;	genOr 
	orl	_P1,#0x01
;	i2c400.c:356: SCL_HIGH; /* start condition wouldn't be detected so make */
;	genOr 
	orl	_P1,#0x02
;	i2c400.c:357: I2CDelay(I2CDELAY); /*   sure that they're up and wait for one delay slot */
;	genInline 
	 nop nop nop nop nop nop nop nop nop nop nop nop nop nop 
;	i2c400.c:359: if (i2c_recv((char)(addr|0x01), rx_count)) return 1;
;	genOr 
	orl	ar2,#0x01
;	genAssign 
	mov	dptr,#_I2CSendReceive_PARM_3
;	genAssign: resultIsFar = TRUE
;	genFarFarAssign (1 byte case)
	movx	a,@dptr
	mov	dptr,#_i2c_recv_PARM_2
	movx	@dptr,a
;	genCall 
;	genSend argreg = 1, size = 1 
	mov	dpl,r2
	lcall	_i2c_recv
;	genIfx 
	mov	a,dpl
;	genIfxJump
; Peephole 110   removed ljmp by inverse jump logic
	jz  00106$
00117$:
;	genRet 
	mov	dpl,#0x01
;	genLabel 
; Peephole 132   changed ljmp to sjmp
	sjmp 00107$
00106$:
;	i2c400.c:360: return (i2cError ? 1 : 0);
;	genIfx 
	mov	dptr,#_i2cError
	movx	a,@dptr
;	genIfxJump
; Peephole 110   removed ljmp by inverse jump logic
	jz  00109$
00118$:
;	genAssign 
;	genAssign: resultIsFar = FALSE
	mov	r2,#0x01
;	genGoto 
;	genLabel 
; Peephole 132   changed ljmp to sjmp
	sjmp 00110$
00109$:
;	genAssign 
;	genAssign: resultIsFar = FALSE
	mov	r2,#0x00
;	genLabel 
00110$:
;	genRet 
	mov	dpl,r2
;	genLabel 
00107$:
;	genEndFunction 
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'I2CDumpError'
;------------------------------------------------------------
;error                     Allocated to registers r2 
;------------------------------------------------------------
;	i2c400.c:367: void I2CDumpError(char error)
;	genFunction 
;	-----------------------------------------
;	 function I2CDumpError
;	-----------------------------------------
_I2CDumpError:
;	genReceive 
	mov	r2,dpl
;	i2c400.c:370: switch(error)
;	genCmpLt 
;	genCmp
	mov	a,r2
;	genIfxJump
	jnb	acc.7,00112$
	ljmp	00106$
00112$:
;	genCmpGt 
;	genCmp
	clr	c
; Peephole 159   avoided xrl during execution
	mov  a,#(0x04 ^ 0x80)
	mov	b,r2
	xrl	b,#0x80
	subb	a,b
;	genIfxJump
; Peephole 132   changed ljmp to sjmp
; Peephole 160   removed sjmp by inverse jump logic
	jc   00106$
00113$:
;	genJumpTab 
	mov	a,r2
	add	a,acc
	add	a,acc
	mov	dptr,#00114$
	jmp	@a+dptr
00114$:
	ljmp	00101$
	ljmp	00102$
	ljmp	00103$
	ljmp	00105$
	ljmp	00104$
;	i2c400.c:373: puts("I2C: OK.");
;	genLabel 
00101$:
;	genCall 
;	genSend argreg = 1, size = 4 
; Peephole 182b used 24 bit load of DPTR
	mov dptr,#__str_17
	mov	b,#0x02
	lcall	_puts
;	i2c400.c:374: break;
;	genGoto 
;	i2c400.c:376: puts("I2C: Slave didn't acknowledge");
;	genLabel 
; Peephole 132   changed ljmp to sjmp
	sjmp 00108$
00102$:
;	genCall 
;	genSend argreg = 1, size = 4 
; Peephole 182b used 24 bit load of DPTR
	mov dptr,#__str_18
	mov	b,#0x02
	lcall	_puts
;	i2c400.c:377: break;
;	genGoto 
;	i2c400.c:379: puts("I2C: Lost arbitration with another master");
;	genLabel 
; Peephole 132   changed ljmp to sjmp
	sjmp 00108$
00103$:
;	genCall 
;	genSend argreg = 1, size = 4 
; Peephole 182b used 24 bit load of DPTR
	mov dptr,#__str_19
	mov	b,#0x02
	lcall	_puts
;	i2c400.c:380: break;
;	genGoto 
;	i2c400.c:382: puts("I2C: Timeout on bus");
;	genLabel 
; Peephole 132   changed ljmp to sjmp
	sjmp 00108$
00104$:
;	genCall 
;	genSend argreg = 1, size = 4 
; Peephole 182b used 24 bit load of DPTR
	mov dptr,#__str_20
	mov	b,#0x02
	lcall	_puts
;	i2c400.c:383: break;
;	genGoto 
;	i2c400.c:385: puts("I2C: The bus is stuck");
;	genLabel 
; Peephole 132   changed ljmp to sjmp
	sjmp 00108$
00105$:
;	genCall 
;	genSend argreg = 1, size = 4 
; Peephole 182b used 24 bit load of DPTR
	mov dptr,#__str_21
	mov	b,#0x02
	lcall	_puts
;	i2c400.c:386: break;
;	genGoto 
;	i2c400.c:388: puts("I2C: Unknown error");
;	genLabel 
; Peephole 132   changed ljmp to sjmp
	sjmp 00108$
00106$:
;	genCall 
;	genSend argreg = 1, size = 4 
; Peephole 182b used 24 bit load of DPTR
	mov dptr,#__str_22
	mov	b,#0x02
	lcall	_puts
;	i2c400.c:390: }
;	genLabel 
00108$:
;	genEndFunction 
	ret
	.area CSEG    (CODE)
__str_0:
	.ascii "entering BitOutI2C"
	.db 0x0A
	.db 0x0D
	.db 0x00
__str_1:
	.ascii "/"
	.db 0x0A
	.db 0x0D
	.db 0x00
__str_2:
	.ascii "entering BitInI2C"
	.db 0x0A
	.db 0x0D
	.db 0x00
__str_3:
	.ascii "+"
	.db 0x0A
	.db 0x0D
	.db 0x00
__str_4:
	.ascii "entering byteout"
	.db 0x00
__str_5:
	.ascii "."
	.db 0x00
__str_6:
	.ascii "+"
	.db 0x00
__str_7:
	.ascii "0"
	.db 0x0A
	.db 0x0D
	.db 0x00
__str_8:
	.ascii "1"
	.db 0x0A
	.db 0x0D
	.db 0x00
__str_9:
	.ascii "+1"
	.db 0x0A
	.db 0x0D
	.db 0x00
__str_10:
	.ascii "--1"
	.db 0x0A
	.db 0x0D
	.db 0x00
__str_11:
	.ascii "apres stop"
	.db 0x0A
	.db 0x0D
	.db 0x00
__str_12:
	.ascii "2"
	.db 0x0A
	.db 0x0D
	.db 0x00
__str_13:
	.ascii "3"
	.db 0x0A
	.db 0x0D
	.db 0x00
__str_14:
	.ascii "4"
	.db 0x0A
	.db 0x0D
	.db 0x00
__str_15:
	.ascii "5"
	.db 0x0A
	.db 0x0D
	.db 0x00
__str_16:
	.ascii "6"
	.db 0x0A
	.db 0x0D
	.db 0x00
__str_17:
	.ascii "I2C: OK."
	.db 0x00
__str_18:
	.ascii "I2C: Slave didn't acknowledge"
	.db 0x00
__str_19:
	.ascii "I2C: Lost arbitration with another master"
	.db 0x00
__str_20:
	.ascii "I2C: Timeout on bus"
	.db 0x00
__str_21:
	.ascii "I2C: The bus is stuck"
	.db 0x00
__str_22:
	.ascii "I2C: Unknown error"
	.db 0x00
	.area XINIT   (CODE)
__xinit__i2cError:
	.db #0x00
