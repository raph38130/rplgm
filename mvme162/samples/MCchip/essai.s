	.file	"essai.c"
	.version	"01.01"
gcc2_compiled.:
.text
	.align 	2
.globl f
	.type	 f,@function
f:
	link.w %a6,#0
#APP
		movem.l %d0-%d1/%a0-%a1,-(%sp)
#NO_APP
	move.l #-778173,pb
	move.l pb,%a0
	move.b #31,(%a0)
	moveq.l #5,%d0
	move.l %d0,test
#APP
		movem.l (%sp)+,%d0-%d1/%a0-%a1
		rte
#NO_APP
.L1:
	unlk %a6
	rts
.Lfe1:
	.size	 f,.Lfe1-f
	.align 	2
.globl g
	.type	 g,@function
g:
	link.w %a6,#0
#APP
		movem.l %d0-%d1/%a0-%a1,-(%sp)
#NO_APP
	move.l #-778213,pb
	move.l pb,%a0
	move.b #31,(%a0)
	moveq.l #7,%d0
	move.l %d0,test
#APP
		movem.l (%sp)+,%d0-%d1/%a0-%a1
		rte
#NO_APP
.L2:
	unlk %a6
	rts
.Lfe2:
	.size	 g,.Lfe2-g
.section	.rodata
.LC0:
	.string	"ABORT"
.LC1:
	.string	"tick"
.text
	.align 	2
.globl main
	.type	 main,@function
main:
	link.w %a6,#0
	move.l %d2,-(%sp)
	move.l #-778238,pb
	move.l pb,%a0
	move.b #2,(%a0)
	move.l #-778237,pb
	move.l pb,%a0
	move.b #80,(%a0)
	move.l pb,%a0
	move.b (%a0),%d0
	extb.l %d0
	move.l %d0,%a1
	lea (14,%a1),%a0
	move.l %a0,%d0
	move.l %d0,%d1
	lsl.l #2,%d1
	move.l %d1,plw
	move.l plw,%a0
	move.l #f+4,(%a0)
	move.l pb,%a0
	move.b (%a0),%d0
	extb.l %d0
	move.l %d0,%a1
	lea (9,%a1),%a0
	move.l %a0,%d0
	move.l %d0,%d1
	lsl.l #2,%d1
	move.l %d1,plw
	move.l plw,%a0
	move.l #g+4,(%a0)
	move.l #-778173,pb
	move.l pb,%a0
	move.b #23,(%a0)
	move.l #-778236,plw
	move.l plw,%a0
	move.l #1000000,(%a0)
	move.l #-778213,pb
	move.l pb,%a0
	move.b #23,(%a0)
	move.l #-778217,pb
	move.l pb,%a0
	move.b #3,(%a0)
#APP
	move.w #0x2000,%SR
#NO_APP
	clr.l test
.L4:
	jbra .L6
	.align 	2
	jbra .L5
	.align 	2
.L6:
	move.l test,%d0
	moveq.l #5,%d2
	cmp.l %d0,%d2
	jbeq .L9
	moveq.l #7,%d2
	cmp.l %d0,%d2
	jbeq .L8
	jbra .L7
	.align 	2
.L8:
	pea .LC0
	jbsr strlen
	addq.l #4,%sp
	move.l %d0,%d1
	add.l #.LC0,%d1
	move.l %d1,-(%sp)
	pea .LC0
	jbsr outstr
	addq.l #8,%sp
	clr.l test
	jbra .L7
	.align 	2
.L9:
	pea .LC1
	jbsr strlen
	addq.l #4,%sp
	move.l %d0,%d1
	add.l #.LC1,%d1
	move.l %d1,-(%sp)
	pea .LC1
	jbsr outstr
	addq.l #8,%sp
	clr.l test
	jbra .L7
	.align 	2
.L11:
.L7:
	jbra .L4
	.align 	2
.L5:
.L3:
	move.l -4(%a6),%d2
	unlk %a6
	rts
.Lfe3:
	.size	 main,.Lfe3-main
	.comm	test,4,2
	.comm	pb,4,2
	.comm	plw,4,2
	.ident	"GCC: (GNU) 2.8.1"
