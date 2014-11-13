	.file	"print.c"
	.version	"01.01"
gcc2_compiled.:
.section	.rodata
.LC0:
	.string	"2.71828"
.globl memset
.LC1:
	.string	"hello %d %f\n\r"
.LC2:
	.string	"%f"
.LC3:
	.string	"x lu = %f"
.text
	.align 	2
.globl main
	.type	 main,@function
main:
	link.w %a6,#-48
	move.l %d2,-(%sp)
	move.l .LC0,-30(%a6)
	move.l .LC0+4,-26(%a6)
	moveq.l #-22,%d0
	add.l %a6,%d0
	pea 22.w
	clr.l -(%sp)
	move.l %d0,-(%sp)
	jbsr memset
	lea (12,%sp),%sp
	move.l #0x402df84d,-38(%a6)
	move.l #1074340347,-46(%a6)
	move.l #1413754136,-42(%a6)
	move.l -42(%a6),-(%sp)
	move.l -46(%a6),-(%sp)
	jbsr sin
	addq.l #8,%sp
	fmove.d %fp0,-46(%a6)
	clr.l -34(%a6)
	.align 	2
.L3:
	moveq.l #29,%d0
	cmp.l -34(%a6),%d0
	jbge .L6
	jbra .L4
	.align 	2
.L6:
	fmove.l -34(%a6),%fp0
	fmove.x %fp0,%fp1
	fmul.d #0r6.28318530717958623200e0,%fp1
	fmove.x %fp1,%fp0
	fdiv.d #0r3.00000000000000000000e1,%fp0
	fmove.d %fp0,-(%sp)
	jbsr sin
	addq.l #8,%sp
	fmove.d %fp0,-(%sp)
	move.l -34(%a6),-(%sp)
	pea .LC1
	moveq.l #-30,%d0
	add.l %a6,%d0
	move.l %d0,-(%sp)
	jbsr sprintf
	lea (20,%sp),%sp
	moveq.l #-30,%d2
	add.l %a6,%d2
	moveq.l #-30,%d0
	add.l %a6,%d0
	move.l %d0,-(%sp)
	jbsr strlen
	addq.l #4,%sp
	move.l %d2,%d1
	add.l %d0,%d1
	move.l %d1,-(%sp)
	moveq.l #-30,%d0
	add.l %a6,%d0
	move.l %d0,-(%sp)
	jbsr outstr
	addq.l #8,%sp
.L5:
	addq.l #1,-34(%a6)
	jbra .L3
	.align 	2
.L4:
	moveq.l #-38,%d0
	add.l %a6,%d0
	move.l %d0,-(%sp)
	pea .LC2
	moveq.l #-30,%d0
	add.l %a6,%d0
	move.l %d0,-(%sp)
	jbsr sscanf
	lea (12,%sp),%sp
	fmove.s -38(%a6),%fp0
	fmove.d %fp0,-(%sp)
	pea .LC3
	moveq.l #-30,%d0
	add.l %a6,%d0
	move.l %d0,-(%sp)
	jbsr sprintf
	lea (16,%sp),%sp
	moveq.l #-30,%d2
	add.l %a6,%d2
	moveq.l #-30,%d0
	add.l %a6,%d0
	move.l %d0,-(%sp)
	jbsr strlen
	addq.l #4,%sp
	move.l %d2,%d1
	add.l %d0,%d1
	move.l %d1,-(%sp)
	moveq.l #-30,%d0
	add.l %a6,%d0
	move.l %d0,-(%sp)
	jbsr outstr
	addq.l #8,%sp
.L2:
	move.l -52(%a6),%d2
	unlk %a6
	rts
.Lfe1:
	.size	 main,.Lfe1-main
	.ident	"GCC: (GNU) 2.95.4  (Debian prerelease)"
