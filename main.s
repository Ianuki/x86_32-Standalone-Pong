	.file	"main.c"
	.text
	.align 16
	.globl	delay
	.type	delay, @function
delay:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$16, %esp
	imull	$100000, 8(%ebp), %edx
	movl	$0, -4(%ebp)
	movl	-4(%ebp), %eax
	cmpl	%eax, %edx
	jbe	.L1
	.align 16
.L3:
	movl	-4(%ebp), %eax
	addl	$1, %eax
	movl	%eax, -4(%ebp)
	movl	-4(%ebp), %eax
	cmpl	%edx, %eax
	jb	.L3
.L1:
	leave
	ret
	.size	delay, .-delay
	.align 16
	.globl	putsat
	.type	putsat, @function
putsat:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	movl	8(%ebp), %edx
	movl	16(%ebp), %eax
	movzbl	20(%ebp), %ebx
	movzbl	(%edx), %ecx
	testb	%cl, %cl
	je	.L7
	leal	(%eax,%eax,4), %eax
	addl	$1, %edx
	sall	$4, %eax
	addl	12(%ebp), %eax
	leal	753664(%eax,%eax), %eax
	.align 16
.L9:
	movb	%cl, (%eax)
	addl	$1, %edx
	addl	$2, %eax
	movb	%bl, -1(%eax)
	movzbl	-1(%edx), %ecx
	testb	%cl, %cl
	jne	.L9
.L7:
	movl	-4(%ebp), %ebx
	leave
	ret
	.size	putsat, .-putsat
	.align 16
	.globl	clear_buffer
	.type	clear_buffer, @function
clear_buffer:
	movl	$753824, %eax
	.align 16
.L16:
	movb	$46, (%eax)
	addl	$2, %eax
	movb	$9, -1(%eax)
	cmpl	$757664, %eax
	jne	.L16
	ret
	.size	clear_buffer, .-clear_buffer
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	" PLAYER A: 0 "
.LC1:
	.string	" PLAYER B: 0 "
	.text
	.align 16
	.globl	show_data
	.type	show_data, @function
show_data:
	movl	$753664, %eax
	.align 16
.L19:
	movb	$-37, (%eax)
	addl	$2, %eax
	movb	$15, -1(%eax)
	cmpl	$753824, %eax
	jne	.L19
	movl	$.LC0+1, %ecx
	movl	$753666, %eax
	movl	$32, %edx
	.align 16
.L20:
	movb	%dl, (%eax)
	movzbl	(%ecx), %edx
	addl	$2, %eax
	addl	$1, %ecx
	movb	$-126, -1(%eax)
	testb	%dl, %dl
	jne	.L20
	movl	$.LC1+1, %ecx
	movl	$753796, %eax
	movl	$32, %edx
	.align 16
.L21:
	movb	%dl, (%eax)
	movzbl	(%ecx), %edx
	addl	$2, %eax
	addl	$1, %ecx
	movb	$-127, -1(%eax)
	testb	%dl, %dl
	jne	.L21
	ret
	.size	show_data, .-show_data
	.align 16
	.globl	_start
	.type	_start, @function
_start:
	pushl	%ebp
	movl	$2, %ecx
	movl	%esp, %ebp
	pushl	%edi
	movl	$1, %edi
	pushl	%esi
	movl	$2, %esi
	pushl	%ebx
	movl	$1, %ebx
	subl	$44, %esp
	movl	$2, -44(%ebp)
	.align 16
.L26:
	movl	%edi, %eax
	leal	-1(%ebx), %edx
	negl	%eax
	cmpl	$21, %edx
	cmova	%eax, %edi
	movl	$753824, %eax
	.align 16
.L28:
	movb	$46, (%eax)
	addl	$2, %eax
	movb	$9, -1(%eax)
	cmpl	$757664, %eax
	jne	.L28
	movl	%ecx, -48(%ebp)
	call	show_data
	movl	-44(%ebp), %ecx
	leal	5(%ebx,%ebx,4), %eax
	sall	$4, %eax
	leal	(%ecx,%eax), %edx
	leal	1(%ecx,%eax), %eax
	movl	-48(%ebp), %ecx
	addl	%eax, %eax
	addl	%edx, %edx
	movb	$-37, 753664(%edx)
	movb	$5, 753665(%edx)
	movb	$-37, 753664(%eax)
	movb	$5, 753665(%eax)
	movl	$0, -28(%ebp)
	movl	-28(%ebp), %eax
	cmpl	$39999999, %eax
	ja	.L32
	.align 16
.L29:
	movl	-28(%ebp), %eax
	addl	$1, %eax
	movl	%eax, -28(%ebp)
	movl	-28(%ebp), %eax
	cmpl	$39999999, %eax
	jbe	.L29
.L32:
	addl	%esi, %ecx
	movl	%esi, %eax
	addl	%edi, %ebx
	leal	-1(%ecx), %edx
	negl	%eax
	movl	%ecx, -44(%ebp)
	cmpl	$76, %edx
	cmova	%eax, %esi
	jmp	.L26
	.size	_start, .-_start
	.align 16
	.globl	draw_square
	.type	draw_square, @function
draw_square:
	pushl	%ebp
	movl	%esp, %ebp
	movl	12(%ebp), %eax
	movl	8(%ebp), %ecx
	leal	5(%eax,%eax,4), %eax
	sall	$4, %eax
	leal	(%ecx,%eax), %edx
	leal	1(%ecx,%eax), %eax
	addl	%edx, %edx
	addl	%eax, %eax
	movb	$-37, 753664(%edx)
	movb	$5, 753665(%edx)
	movb	$-37, 753664(%eax)
	movb	$5, 753665(%eax)
	popl	%ebp
	ret
	.size	draw_square, .-draw_square
	.ident	"GCC: (GNU) 11.2.0"
