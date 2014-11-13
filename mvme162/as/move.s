	.file	"move.c"
	.text
.globl memmove
	.type	memmove, @function
memmove:
	pushl	%ebp
	movl	%esp, %ebp
	nop
.L2:
	movl	8(%ebp), %eax
	cmpl	12(%ebp), %eax
	jb	.L4
	jmp	.L1
.L4:
	movl	16(%ebp), %eax
	movl	%eax, %edx
	movl	8(%ebp), %eax
	incl	8(%ebp)
	movzbl	(%eax), %eax
	movb	%al, (%edx)
	leal	16(%ebp), %eax
	incl	(%eax)
	jmp	.L2
.L1:
	popl	%ebp
	ret
	.size	memmove, .-memmove
.globl main
	.type	main, @function
main:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$24, %esp
	andl	$-16, %esp
	movl	$0, %eax
	subl	%eax, %esp
	movl	$131072, 8(%esp)
	movl	$86096, 4(%esp)
	movl	$86016, (%esp)
	call	memmove
	leave
	ret
	.size	main, .-main
	.section	.note.GNU-stack,"",@progbits
	.ident	"GCC: (GNU) 3.3.6 (Debian 1:3.3.6-7)"
