;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.3.7 Thu Feb  5 13:39:25 2004

;--------------------------------------------------------
	.module tini_i2c
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
	.globl _i2c_waitforscl
	.globl _i2c_writereadblock_PARM_5
	.globl _i2c_writereadblock_PARM_4
	.globl _i2c_writereadblock_PARM_3
	.globl _i2c_writereadblock_PARM_2
	.globl _i2c_readblock_PARM_3
	.globl _i2c_readblock_PARM_2
	.globl _i2c_writeblock_PARM_3
	.globl _i2c_writeblock_PARM_2
	.globl _i2c_version
	.globl _i2c_start
	.globl _i2c_bit
	.globl _i2c_readbit
	.globl _i2c_stop
	.globl _i2c_readbyte
	.globl _i2c_writebyte
	.globl _i2c_select
	.globl _i2c_writeblock
	.globl _i2c_readblock
	.globl _i2c_writereadblock
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
_i2c_writeblock_PARM_2::
	.ds 4
_i2c_writeblock_PARM_3::
	.ds 2
_i2c_readblock_PARM_2::
	.ds 4
_i2c_readblock_PARM_3::
	.ds 2
_i2c_readblock_sloc0_1_0::
	.ds 4
_i2c_writereadblock_PARM_2::
	.ds 4
_i2c_writereadblock_PARM_3::
	.ds 2
_i2c_writereadblock_PARM_4::
	.ds 4
_i2c_writereadblock_PARM_5::
	.ds 2
_i2c_writereadblock_sloc0_1_0::
	.ds 4
;--------------------------------------------------------
; external initialized ram data
;--------------------------------------------------------
	.area XISEG   (XDATA)
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
;Allocation info for local variables in function 'i2c_version'
;------------------------------------------------------------
;------------------------------------------------------------
;	tini_i2c.c:47: int i2c_version(void)
;	genFunction 
;	-----------------------------------------
;	 function i2c_version
;	-----------------------------------------
_i2c_version:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
;	tini_i2c.c:49: return 1;
;	genRet 
; Peephole 182c   used 16 bit load of dptr
	mov  dptr,#0x0001
;	genLabel 
00101$:
;	genEndFunction 
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'i2c_waitforscl'
;------------------------------------------------------------
;waitcount                 Allocated to registers 
;------------------------------------------------------------
;	tini_i2c.c:86: void i2c_waitforscl()
;	genFunction 
;	-----------------------------------------
;	 function i2c_waitforscl
;	-----------------------------------------
_i2c_waitforscl:
;	tini_i2c.c:92: for (waitcount = 0;waitcount < I2C_MAXIMUM_SCL_WAITCOUNT;waitcount++)
;	genAssign 
;	genAssign: resultIsFar = FALSE
	mov	r2,#0x00
	mov	r3,#0x00
;	genLabel 
00103$:
;	genCast 
	mov	ar4,r2
	mov	ar5,r3
	mov	a,r3
	rlc	a
	subb	a,acc
	mov	r6,a
	mov	r7,a
;	genCmpLt 
;	genCmp
	clr	c
	mov	a,r4
	subb	a,#0x20
	mov	a,r5
	subb	a,#0xA1
	mov	a,r6
	subb	a,#0x07
	mov	a,r7
	xrl	a,#0x80
	subb	a,#0x80
;	genIfxJump
; Peephole 108   removed ljmp by inverse jump logic
	jnc  00107$
00112$:
;	tini_i2c.c:94: if (SCL_IN) return;
;	genRightShift 
;	genRightShiftLiteral (1), size 1
;	genrshOne
	mov	a,_P1
	clr	c
	rrc	a
;	genAnd 
; Peephole 105   removed redundant mov
	mov  r4,a
;	genIfxJump
; Peephole 111   removed ljmp by inverse jump logic
	jnb  acc.0,00105$
00113$:
;	genRet 
;	genLabel 
; Peephole 132   changed ljmp to sjmp
	sjmp 00107$
00105$:
;	tini_i2c.c:92: for (waitcount = 0;waitcount < I2C_MAXIMUM_SCL_WAITCOUNT;waitcount++)
;	genPlus 
	inc	r2
	cjne	r2,#0,00114$
	inc	r3
00114$:
;	did genPlusIncr
;	genGoto 
;	genLabel 
; Peephole 132   changed ljmp to sjmp
	sjmp 00103$
00107$:
;	genEndFunction 
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'i2c_start'
;------------------------------------------------------------
;------------------------------------------------------------
;	tini_i2c.c:103: void i2c_start(void)
;	genFunction 
;	-----------------------------------------
;	 function i2c_start
;	-----------------------------------------
_i2c_start:
;	tini_i2c.c:105: SDA_LOW;
;	genAnd 
	anl	_P1,#0xFE
;	tini_i2c.c:106: i2c_delay();
;	genInline 
	 nop nop nop nop nop nop nop 
;	tini_i2c.c:107: SCL_LOW;
;	genAnd 
	anl	_P1,#0xFD
;	tini_i2c.c:108: i2c_delay();
;	genInline 
	 nop nop nop nop nop nop nop 
;	genLabel 
00101$:
;	genEndFunction 
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'i2c_bit'
;------------------------------------------------------------
;singlebit                 Allocated to registers r2 
;------------------------------------------------------------
;	tini_i2c.c:117: void i2c_bit(unsigned char singlebit)
;	genFunction 
;	-----------------------------------------
;	 function i2c_bit
;	-----------------------------------------
_i2c_bit:
;	genReceive 
	mov	r2,dpl
;	tini_i2c.c:119: SDA_OUT(singlebit);
;	genIfx 
	mov	a,r2
;	genIfxJump
; Peephole 110   removed ljmp by inverse jump logic
	jz  00103$
00106$:
;	genOr 
	orl	_P1,#0x01
;	genGoto 
;	genLabel 
; Peephole 132   changed ljmp to sjmp
	sjmp 00104$
00103$:
;	genAnd 
	anl	_P1,#0xFE
;	genLabel 
00104$:
;	tini_i2c.c:120: i2c_delay();
;	genInline 
	 nop nop nop nop nop nop nop 
;	tini_i2c.c:121: SCL_HIGH;
;	genOr 
	orl	_P1,#0x02
;	tini_i2c.c:123: i2c_waitforscl();
;	genCall 
	lcall	_i2c_waitforscl
;	tini_i2c.c:125: i2c_delay();
;	genInline 
	 nop nop nop nop nop nop nop 
;	tini_i2c.c:126: SCL_LOW;
;	genAnd 
	anl	_P1,#0xFD
;	tini_i2c.c:127: i2c_delay();
;	genInline 
	 nop nop nop nop nop nop nop 
;	genLabel 
00101$:
;	genEndFunction 
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'i2c_readbit'
;------------------------------------------------------------
;retVal                    Allocated to registers r2 
;------------------------------------------------------------
;	tini_i2c.c:136: unsigned char i2c_readbit(void)
;	genFunction 
;	-----------------------------------------
;	 function i2c_readbit
;	-----------------------------------------
_i2c_readbit:
;	tini_i2c.c:140: SDA_HIGH;
;	genOr 
	orl	_P1,#0x01
;	tini_i2c.c:141: i2c_delay();
;	genInline 
	 nop nop nop nop nop nop nop 
;	tini_i2c.c:142: SCL_HIGH;
;	genOr 
	orl	_P1,#0x02
;	tini_i2c.c:144: i2c_waitforscl();
;	genCall 
	lcall	_i2c_waitforscl
;	tini_i2c.c:146: i2c_delay();
;	genInline 
	 nop nop nop nop nop nop nop 
;	tini_i2c.c:147: retVal = SDA_IN;
;	genAnd 
;	better literal AND.
	mov	a,_P1
	anl	a, #0x01
	mov	r2,a
;	tini_i2c.c:148: i2c_delay();
;	genInline 
	 nop nop nop nop nop nop nop 
;	tini_i2c.c:149: SCL_LOW;
;	genAnd 
	anl	_P1,#0xFD
;	tini_i2c.c:150: i2c_delay();
;	genInline 
	 nop nop nop nop nop nop nop 
;	tini_i2c.c:152: return retVal;
;	genRet 
	mov	dpl,r2
;	genLabel 
00101$:
;	genEndFunction 
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'i2c_stop'
;------------------------------------------------------------
;------------------------------------------------------------
;	tini_i2c.c:159: void i2c_stop(void)
;	genFunction 
;	-----------------------------------------
;	 function i2c_stop
;	-----------------------------------------
_i2c_stop:
;	tini_i2c.c:161: SDA_LOW;
;	genAnd 
	anl	_P1,#0xFE
;	tini_i2c.c:162: i2c_delay();
;	genInline 
	 nop nop nop nop nop nop nop 
;	tini_i2c.c:163: SCL_HIGH;
;	genOr 
	orl	_P1,#0x02
;	tini_i2c.c:164: i2c_delay();
;	genInline 
	 nop nop nop nop nop nop nop 
;	tini_i2c.c:165: SDA_HIGH;
;	genOr 
	orl	_P1,#0x01
;	tini_i2c.c:166: i2c_delay();
;	genInline 
	 nop nop nop nop nop nop nop 
;	genLabel 
00101$:
;	genEndFunction 
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'i2c_readbyte'
;------------------------------------------------------------
;doACK                     Allocated to registers r2 
;i                         Allocated to registers 
;result                    Allocated to registers r3 
;------------------------------------------------------------
;	tini_i2c.c:178: unsigned char i2c_readbyte(unsigned char doACK)
;	genFunction 
;	-----------------------------------------
;	 function i2c_readbyte
;	-----------------------------------------
_i2c_readbyte:
;	genReceive 
	mov	r2,dpl
;	tini_i2c.c:181: unsigned char result = 0;
;	genAssign 
;	genAssign: resultIsFar = TRUE
	mov	r3,#0x00
;	tini_i2c.c:183: for (i = 0;i < 8;i++)
;	genAssign 
;	genAssign: resultIsFar = FALSE
	mov	r4,#0x08
;	genLabel 
00106$:
;	tini_i2c.c:185: result <<= 1;
;	genCast 
	mov	ar5,r3
	mov	r6,#0
;	genLeftShift 
;	genLeftShiftLiteral (1), size 2
;	genlshTwo 
	mov	a,r6
	xch	a,r5
	add	a,acc
	xch	a,r5
	rlc	a
	mov	r6,a
;	genCast 
	mov	ar3,r5
;	tini_i2c.c:186: result |= (i2c_readbit() & 0x01);
;	genCall 
	push	ar2
	push	ar3
	push	ar4
	lcall	_i2c_readbit
	pop	ar4
	pop	ar3
	pop	ar2
;	genAnd 
;	better literal AND.
	mov	a,dpl
	anl	a, #0x01
;	genOr 
; Peephole 105   removed redundant mov
	mov  r5,a
	orl	ar3,a
;	genDjnz
; Peephole 132   changed ljmp to sjmp
; Peephole 205   optimized misc jump sequence
; Peephole 236e
	djnz  r4,00106$
00112$:
00113$:
;	tini_i2c.c:183: for (i = 0;i < 8;i++)
;	tini_i2c.c:189: if (doACK == 0)
;	genCmpEq 
;	gencjneshort
	mov	a,r2
; Peephole 132   changed ljmp to sjmp
; Peephole 199   optimized misc jump sequence
	cjne a,#0x00,00102$
;00114$:
; Peephole 200   removed redundant sjmp
00115$:
;	tini_i2c.c:190: i2c_bit(1);  // No ACK
;	genCall 
	push	ar3
;	genSend argreg = 1, size = 1 
	mov	dpl,#0x01
	lcall	_i2c_bit
	pop	ar3
;	genGoto 
;	genLabel 
; Peephole 132   changed ljmp to sjmp
	sjmp 00103$
00102$:
;	tini_i2c.c:192: i2c_bit(0);  // Do the ACK
;	genCall 
	push	ar3
;	genSend argreg = 1, size = 1 
	mov	dpl,#0x00
	lcall	_i2c_bit
	pop	ar3
;	genLabel 
00103$:
;	tini_i2c.c:194: return result;
;	genRet 
	mov	dpl,r3
;	genLabel 
00107$:
;	genEndFunction 
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'i2c_writebyte'
;------------------------------------------------------------
;singlebyte                Allocated to registers r2 
;i                         Allocated to registers 
;------------------------------------------------------------
;	tini_i2c.c:205: unsigned char i2c_writebyte(unsigned char singlebyte)
;	genFunction 
;	-----------------------------------------
;	 function i2c_writebyte
;	-----------------------------------------
_i2c_writebyte:
;	genReceive 
	mov	r2,dpl
;	tini_i2c.c:209: for (i = 0;i < 8;i++)
;	genAssign 
;	genAssign: resultIsFar = FALSE
	mov	r3,#0x00
;	genLabel 
00104$:
;	genCmpLt 
;	genCmp
	cjne	r3,#0x08,00113$
00113$:
;	genIfxJump
; Peephole 108   removed ljmp by inverse jump logic
	jnc  00107$
00114$:
;	tini_i2c.c:211: if (singlebyte & 0x80)
;	genAnd 
	mov	a,r2
;	genIfxJump
; Peephole 111   removed ljmp by inverse jump logic
	jnb  acc.7,00102$
00115$:
;	tini_i2c.c:212: i2c_bit(1);
;	genCall 
	push	ar2
	push	ar3
;	genSend argreg = 1, size = 1 
	mov	dpl,#0x01
	lcall	_i2c_bit
	pop	ar3
	pop	ar2
;	genGoto 
;	genLabel 
; Peephole 132   changed ljmp to sjmp
	sjmp 00103$
00102$:
;	tini_i2c.c:214: i2c_bit(0);
;	genCall 
	push	ar2
	push	ar3
;	genSend argreg = 1, size = 1 
	mov	dpl,#0x00
	lcall	_i2c_bit
	pop	ar3
	pop	ar2
;	genLabel 
00103$:
;	tini_i2c.c:215: singlebyte <<= 1;
;	genCast 
	mov	ar4,r2
	mov	r5,#0
;	genLeftShift 
;	genLeftShiftLiteral (1), size 2
;	genlshTwo 
	mov	a,r5
	xch	a,r4
	add	a,acc
	xch	a,r4
	rlc	a
	mov	r5,a
;	genCast 
	mov	ar2,r4
;	tini_i2c.c:209: for (i = 0;i < 8;i++)
;	genPlus 
	inc	r3
;	did genPlusIncr
;	genGoto 
;	genLabel 
; Peephole 132   changed ljmp to sjmp
	sjmp 00104$
00107$:
;	tini_i2c.c:218: return i2c_readbit();
;	genCall 
	lcall	_i2c_readbit
;	genRet 
;	genLabel 
00108$:
;	genEndFunction 
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'i2c_select'
;------------------------------------------------------------
;address                   Allocated to registers r2 
;------------------------------------------------------------
;	tini_i2c.c:230: unsigned char i2c_select(unsigned char address)
;	genFunction 
;	-----------------------------------------
;	 function i2c_select
;	-----------------------------------------
_i2c_select:
;	genReceive 
	mov	r2,dpl
;	tini_i2c.c:232: i2c_start();
;	genCall 
	push	ar2
	lcall	_i2c_start
	pop	ar2
;	tini_i2c.c:233: return i2c_writebyte(address);
;	genCall 
;	genSend argreg = 1, size = 1 
	mov	dpl,r2
	lcall	_i2c_writebyte
;	genRet 
;	genLabel 
00101$:
;	genEndFunction 
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'i2c_writeblock'
;------------------------------------------------------------
;barr                      Allocated with name '_i2c_writeblock_PARM_2'
;length                    Allocated with name '_i2c_writeblock_PARM_3'
;address                   Allocated to registers r2 
;i                         Allocated to registers 
;------------------------------------------------------------
;	tini_i2c.c:250: unsigned char i2c_writeblock(unsigned char address,unsigned char *barr,int length)
;	genFunction 
;	-----------------------------------------
;	 function i2c_writeblock
;	-----------------------------------------
_i2c_writeblock:
;	genReceive 
	mov	r2,dpl
;	tini_i2c.c:254: if (i2c_select(address & 0xFE) != 0)
;	genAnd 
	anl	ar2,#0xFE
;	genCall 
;	genSend argreg = 1, size = 1 
	mov	dpl,r2
	lcall	_i2c_select
;	genCmpEq 
;	gencjneshort
	mov	a,dpl
	cjne	a,#0x00,00115$
; Peephole 132   changed ljmp to sjmp
	sjmp 00114$
00115$:
;	tini_i2c.c:255: return 1;
;	genRet 
	mov	dpl,#0x01
	ljmp	00109$
;	tini_i2c.c:257: for (i = 0;i < length;i++)
;	genLabel 
00114$:
;	genAssign 
	mov	dptr,#_i2c_writeblock_PARM_2
;	genAssign: resultIsFar = FALSE
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	inc	dptr
	movx	a,@dptr
	mov	r4,a
	inc	dptr
	movx	a,@dptr
	mov	r5,a
;	genAssign 
;	genAssign: resultIsFar = FALSE
	mov	r6,#0x00
	mov	r7,#0x00
;	genLabel 
00105$:
;	genCmpLt 
	mov	dptr,#_i2c_writeblock_PARM_3
;	genCmp
	clr	c
	mov	a,r6
	xch	a, b
	movx	a,@dptr
	xch	a, b
	subb	a,b
	mov	a,r7
	xrl	a,#0x80
	xch	a, b
	inc	dptr
	movx	a,@dptr
	xch	a, b
	xrl	b,#0x80
	subb	a,b
;	genIfxJump
; Peephole 108   removed ljmp by inverse jump logic
	jnc  00108$
00116$:
;	tini_i2c.c:258: if (i2c_writebyte(barr[i]) != 0) return 1;
;	genPlus 
	mov	a,r6
	add	a,r2
	mov	dpl,a
	mov	a,r7
	addc	a,r3
	mov	dph,a
; Peephole 3.d   changed mov to clr
	clr	a
	addc	a,r4
	mov	dpx,a
	mov	b,r5
;	genPointerGet 
;	genGenPointerGet 
	lcall	__gptrget
	mov	r0,a
;	genCall 
	push	ar2
	push	ar3
	push	ar4
	push	ar5
	push	ar6
	push	ar7
;	genSend argreg = 1, size = 1 
	mov	dpl,r0
	lcall	_i2c_writebyte
	pop	ar7
	pop	ar6
	pop	ar5
	pop	ar4
	pop	ar3
	pop	ar2
;	genCmpEq 
;	gencjneshort
	mov	a,dpl
	cjne	a,#0x00,00117$
; Peephole 132   changed ljmp to sjmp
	sjmp 00107$
00117$:
;	genRet 
	mov	dpl,#0x01
;	genLabel 
; Peephole 132   changed ljmp to sjmp
	sjmp 00109$
00107$:
;	tini_i2c.c:257: for (i = 0;i < length;i++)
;	genPlus 
	inc	r6
	cjne	r6,#0,00118$
	inc	r7
00118$:
;	did genPlusIncr
;	genGoto 
	ljmp	00105$
;	genLabel 
00108$:
;	tini_i2c.c:260: i2c_stop();
;	genCall 
	lcall	_i2c_stop
;	tini_i2c.c:262: return 0;
;	genRet 
	mov	dpl,#0x00
;	genLabel 
00109$:
;	genEndFunction 
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'i2c_readblock'
;------------------------------------------------------------
;barr                      Allocated with name '_i2c_readblock_PARM_2'
;length                    Allocated with name '_i2c_readblock_PARM_3'
;address                   Allocated to registers r2 
;i                         Allocated to registers 
;sloc0                     Allocated with name '_i2c_readblock_sloc0_1_0'
;------------------------------------------------------------
;	tini_i2c.c:279: unsigned char i2c_readblock(unsigned char address,unsigned char *barr,int length)
;	genFunction 
;	-----------------------------------------
;	 function i2c_readblock
;	-----------------------------------------
_i2c_readblock:
;	genReceive 
	mov	r2,dpl
;	tini_i2c.c:283: if (i2c_select(address | 0x01) != 0)
;	genOr 
	orl	ar2,#0x01
;	genCall 
;	genSend argreg = 1, size = 1 
	mov	dpl,r2
	lcall	_i2c_select
;	genCmpEq 
;	gencjneshort
	mov	a,dpl
	cjne	a,#0x00,00112$
; Peephole 132   changed ljmp to sjmp
	sjmp 00102$
00112$:
;	tini_i2c.c:284: return 1;
;	genRet 
	mov	dpl,#0x01
	ljmp	00107$
;	genLabel 
00102$:
;	tini_i2c.c:287: length--;
;	genAssign 
	mov	dptr,#_i2c_readblock_PARM_3
;	genAssign: resultIsFar = FALSE
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
;	genMinus 
	dec	r2
	cjne	r2,#0xFF,00113$
	dec	r3
00113$:
;	genAssign 
;	genAssign: resultIsFar = TRUE
	mov	dptr,#_i2c_readblock_PARM_3
	mov	a,r2
	movx	@dptr,a
	inc	dptr
	mov	a,r3
	movx	@dptr,a
;	tini_i2c.c:290: for (i = 0;i < length;i++)
;	genAssign 
	mov	dptr,#_i2c_readblock_PARM_2
;	genAssign: resultIsFar = TRUE
;	genFarFarAssign (390 auto-toggle fun)
	mov	dps,#0x21
	mov	dptr,#_i2c_readblock_sloc0_1_0
	movx	a,@dptr
	movx	@dptr,a
	inc	dptr
	inc	dptr
	movx	a,@dptr
	movx	@dptr,a
	inc	dptr
	inc	dptr
	movx	a,@dptr
	movx	@dptr,a
	inc	dptr
	inc	dptr
	movx	a,@dptr
	movx	@dptr,a
	mov	dps,#0
;	genAssign 
;	genAssign: resultIsFar = FALSE
	mov	r6,#0x00
	mov	r7,#0x00
;	genLabel 
00103$:
;	genCmpLt 
	mov	dptr,#_i2c_readblock_PARM_3
;	genCmp
	clr	c
	mov	a,r6
	xch	a, b
	movx	a,@dptr
	xch	a, b
	subb	a,b
	mov	a,r7
	xrl	a,#0x80
	xch	a, b
	inc	dptr
	movx	a,@dptr
	xch	a, b
	xrl	b,#0x80
	subb	a,b
;	genIfxJump
; Peephole 108   removed ljmp by inverse jump logic
	jnc  00106$
00114$:
;	tini_i2c.c:291: barr[i] = i2c_readbyte(1);
;	genPlus 
	mov	dptr,#_i2c_readblock_sloc0_1_0
;	Swapped plus args.
	movx	a,@dptr
	add	a,r6
	mov	r0,a
	inc	dptr
	movx	a,@dptr
	addc	a,r7
	mov	r1,a
	inc	dptr
	movx	a,@dptr
	addc	a,#0
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
;	genCall 
	push	ar2
	push	ar3
	push	ar6
	push	ar7
	push	ar0
	push	ar1
;	genSend argreg = 1, size = 1 
	mov	dpl,#0x01
	lcall	_i2c_readbyte
	mov	r4,dpl
	pop	ar1
	pop	ar0
	pop	ar7
	pop	ar6
	pop	ar3
	pop	ar2
;	genPointerSet 
	mov	dpl,r0
	mov	dph,r1
	mov	dpx,r2
	mov	b,r3
	mov	a,r4
	lcall	__gptrput
;	tini_i2c.c:290: for (i = 0;i < length;i++)
;	genPlus 
	inc	r6
	cjne	r6,#0,00115$
	inc	r7
00115$:
;	did genPlusIncr
;	genGoto 
	ljmp	00103$
;	genLabel 
00106$:
;	tini_i2c.c:294: barr[i] = i2c_readbyte(0);
;	genPlus 
	mov	dptr,#_i2c_readblock_sloc0_1_0
;	Swapped plus args.
	movx	a,@dptr
	add	a,r6
	mov	r6,a
	inc	dptr
	movx	a,@dptr
	addc	a,r7
	mov	r7,a
	inc	dptr
	movx	a,@dptr
	addc	a,#0
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
;	genCall 
	push	ar2
	push	ar3
	push	ar6
	push	ar7
;	genSend argreg = 1, size = 1 
	mov	dpl,#0x00
	lcall	_i2c_readbyte
	mov	r4,dpl
	pop	ar7
	pop	ar6
	pop	ar3
	pop	ar2
;	genPointerSet 
	mov	dpl,r6
	mov	dph,r7
	mov	dpx,r2
	mov	b,r3
	mov	a,r4
	lcall	__gptrput
;	tini_i2c.c:296: i2c_stop();
;	genCall 
	lcall	_i2c_stop
;	tini_i2c.c:298: return 0;
;	genRet 
	mov	dpl,#0x00
;	genLabel 
00107$:
;	genEndFunction 
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'i2c_writereadblock'
;------------------------------------------------------------
;barr1                     Allocated with name '_i2c_writereadblock_PARM_2'
;length1                   Allocated with name '_i2c_writereadblock_PARM_3'
;barr2                     Allocated with name '_i2c_writereadblock_PARM_4'
;length2                   Allocated with name '_i2c_writereadblock_PARM_5'
;address                   Allocated to registers r2 
;i                         Allocated to registers 
;sloc0                     Allocated with name '_i2c_writereadblock_sloc0_1_0'
;------------------------------------------------------------
;	tini_i2c.c:318: unsigned char i2c_writereadblock(unsigned char address,unsigned char *barr1,int length1,unsigned char *barr2,int length2)
;	genFunction 
;	-----------------------------------------
;	 function i2c_writereadblock
;	-----------------------------------------
_i2c_writereadblock:
;	genReceive 
	mov	r2,dpl
;	tini_i2c.c:322: if (i2c_select(address & 0xFE) != 0)
;	genAnd 
;	better literal AND.
	mov	a,r2
	anl	a, #0xFE
	mov	r3,a
;	genCall 
	push	ar2
;	genSend argreg = 1, size = 1 
	mov	dpl,r3
	lcall	_i2c_select
	pop	ar2
;	genCmpEq 
;	gencjneshort
	mov	a,dpl
	cjne	a,#0x00,00121$
; Peephole 132   changed ljmp to sjmp
	sjmp 00117$
00121$:
;	tini_i2c.c:323: return 1;
;	genRet 
	mov	dpl,#0x01
	ljmp	00113$
;	tini_i2c.c:325: for (i = 0;i < length1;i++)
;	genLabel 
00117$:
;	genAssign 
	mov	dptr,#_i2c_writereadblock_PARM_2
;	genAssign: resultIsFar = FALSE
	movx	a,@dptr
	mov	r3,a
	inc	dptr
	movx	a,@dptr
	mov	r4,a
	inc	dptr
	movx	a,@dptr
	mov	r5,a
	inc	dptr
	movx	a,@dptr
	mov	r6,a
;	genAssign 
;	genAssign: resultIsFar = FALSE
	mov	r7,#0x00
	mov	r0,#0x00
;	genLabel 
00105$:
;	genCmpLt 
	mov	dptr,#_i2c_writereadblock_PARM_3
;	genCmp
	clr	c
	mov	a,r7
	xch	a, b
	movx	a,@dptr
	xch	a, b
	subb	a,b
	mov	a,r0
	xrl	a,#0x80
	xch	a, b
	inc	dptr
	movx	a,@dptr
	xch	a, b
	xrl	b,#0x80
	subb	a,b
;	genIfxJump
; Peephole 108   removed ljmp by inverse jump logic
	jnc  00108$
00122$:
;	tini_i2c.c:326: i2c_writebyte(barr1[i]);
;	genPlus 
	mov	a,r7
	add	a,r3
	mov	dpl,a
	mov	a,r0
	addc	a,r4
	mov	dph,a
; Peephole 3.d   changed mov to clr
	clr	a
	addc	a,r5
	mov	dpx,a
	mov	b,r6
;	genPointerGet 
;	genGenPointerGet 
	lcall	__gptrget
	mov	r1,a
;	genCall 
	push	ar2
	push	ar3
	push	ar4
	push	ar5
	push	ar6
	push	ar7
	push	ar0
;	genSend argreg = 1, size = 1 
	mov	dpl,r1
	lcall	_i2c_writebyte
	pop	ar0
	pop	ar7
	pop	ar6
	pop	ar5
	pop	ar4
	pop	ar3
	pop	ar2
;	tini_i2c.c:325: for (i = 0;i < length1;i++)
;	genPlus 
	inc	r7
	cjne	r7,#0,00123$
	inc	r0
00123$:
;	did genPlusIncr
;	genGoto 
	ljmp	00105$
;	genLabel 
00108$:
;	tini_i2c.c:328: SDA_HIGH;
;	genOr 
	orl	_P1,#0x01
;	tini_i2c.c:329: i2c_delay();
;	genInline 
	 nop nop nop nop nop nop nop 
;	tini_i2c.c:330: SCL_HIGH;
;	genOr 
	orl	_P1,#0x02
;	tini_i2c.c:331: i2c_delay();
;	genInline 
	 nop nop nop nop nop nop nop 
;	tini_i2c.c:333: if (i2c_select(address | 0x01) != 0)
;	genOr 
	orl	ar2,#0x01
;	genCall 
;	genSend argreg = 1, size = 1 
	mov	dpl,r2
	lcall	_i2c_select
;	genCmpEq 
;	gencjneshort
	mov	a,dpl
	cjne	a,#0x00,00124$
; Peephole 132   changed ljmp to sjmp
	sjmp 00104$
00124$:
;	tini_i2c.c:334: return 1;
;	genRet 
	mov	dpl,#0x01
	ljmp	00113$
;	genLabel 
00104$:
;	tini_i2c.c:337: length2--;
;	genAssign 
	mov	dptr,#_i2c_writereadblock_PARM_5
;	genAssign: resultIsFar = FALSE
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
;	genMinus 
	dec	r2
	cjne	r2,#0xFF,00125$
	dec	r3
00125$:
;	genAssign 
;	genAssign: resultIsFar = TRUE
	mov	dptr,#_i2c_writereadblock_PARM_5
	mov	a,r2
	movx	@dptr,a
	inc	dptr
	mov	a,r3
	movx	@dptr,a
;	tini_i2c.c:340: for (i = 0;i < length2;i++)
;	genAssign 
	mov	dptr,#_i2c_writereadblock_PARM_4
;	genAssign: resultIsFar = TRUE
;	genFarFarAssign (390 auto-toggle fun)
	mov	dps,#0x21
	mov	dptr,#_i2c_writereadblock_sloc0_1_0
	movx	a,@dptr
	movx	@dptr,a
	inc	dptr
	inc	dptr
	movx	a,@dptr
	movx	@dptr,a
	inc	dptr
	inc	dptr
	movx	a,@dptr
	movx	@dptr,a
	inc	dptr
	inc	dptr
	movx	a,@dptr
	movx	@dptr,a
	mov	dps,#0
;	genAssign 
;	genAssign: resultIsFar = FALSE
	mov	r6,#0x00
	mov	r7,#0x00
;	genLabel 
00109$:
;	genCmpLt 
	mov	dptr,#_i2c_writereadblock_PARM_5
;	genCmp
	clr	c
	mov	a,r6
	xch	a, b
	movx	a,@dptr
	xch	a, b
	subb	a,b
	mov	a,r7
	xrl	a,#0x80
	xch	a, b
	inc	dptr
	movx	a,@dptr
	xch	a, b
	xrl	b,#0x80
	subb	a,b
;	genIfxJump
; Peephole 108   removed ljmp by inverse jump logic
	jnc  00112$
00126$:
;	tini_i2c.c:341: barr2[i] = i2c_readbyte(1);
;	genPlus 
	mov	dptr,#_i2c_writereadblock_sloc0_1_0
;	Swapped plus args.
	movx	a,@dptr
	add	a,r6
	mov	r0,a
	inc	dptr
	movx	a,@dptr
	addc	a,r7
	mov	r1,a
	inc	dptr
	movx	a,@dptr
	addc	a,#0
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
;	genCall 
	push	ar2
	push	ar3
	push	ar6
	push	ar7
	push	ar0
	push	ar1
;	genSend argreg = 1, size = 1 
	mov	dpl,#0x01
	lcall	_i2c_readbyte
	mov	r4,dpl
	pop	ar1
	pop	ar0
	pop	ar7
	pop	ar6
	pop	ar3
	pop	ar2
;	genPointerSet 
	mov	dpl,r0
	mov	dph,r1
	mov	dpx,r2
	mov	b,r3
	mov	a,r4
	lcall	__gptrput
;	tini_i2c.c:340: for (i = 0;i < length2;i++)
;	genPlus 
	inc	r6
	cjne	r6,#0,00127$
	inc	r7
00127$:
;	did genPlusIncr
;	genGoto 
	ljmp	00109$
;	genLabel 
00112$:
;	tini_i2c.c:344: barr2[i] = i2c_readbyte(0);
;	genPlus 
	mov	dptr,#_i2c_writereadblock_sloc0_1_0
;	Swapped plus args.
	movx	a,@dptr
	add	a,r6
	mov	r6,a
	inc	dptr
	movx	a,@dptr
	addc	a,r7
	mov	r7,a
	inc	dptr
	movx	a,@dptr
	addc	a,#0
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
;	genCall 
	push	ar2
	push	ar3
	push	ar6
	push	ar7
;	genSend argreg = 1, size = 1 
	mov	dpl,#0x00
	lcall	_i2c_readbyte
	mov	r4,dpl
	pop	ar7
	pop	ar6
	pop	ar3
	pop	ar2
;	genPointerSet 
	mov	dpl,r6
	mov	dph,r7
	mov	dpx,r2
	mov	b,r3
	mov	a,r4
	lcall	__gptrput
;	tini_i2c.c:346: i2c_stop();
;	genCall 
	lcall	_i2c_stop
;	tini_i2c.c:348: return 0;
;	genRet 
	mov	dpl,#0x00
;	genLabel 
00113$:
;	genEndFunction 
	ret
	.area CSEG    (CODE)
	.area XINIT   (CODE)
