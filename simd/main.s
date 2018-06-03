	.file	"main.cpp"
	.text
	.p2align 4,,15
	.def	__tcf_0;	.scl	3;	.type	32;	.endef
	.seh_proc	__tcf_0
__tcf_0:
.LFB7219:
	.seh_endprologue
	leaq	_ZStL8__ioinit(%rip), %rcx
	jmp	_ZNSt8ios_base4InitD1Ev
	.seh_endproc
	.def	__main;	.scl	2;	.type	32;	.endef
	.section .rdata,"dr"
.LC3:
	.ascii " | \0"
.LC4:
	.ascii " \0"
.LC5:
	.ascii "\12\0"
	.section	.text.startup,"x"
	.p2align 4,,15
	.globl	main
	.def	main;	.scl	2;	.type	32;	.endef
	.seh_proc	main
main:
.LFB6697:
	pushq	%r14
	.seh_pushreg	%r14
	pushq	%r13
	.seh_pushreg	%r13
	pushq	%r12
	.seh_pushreg	%r12
	pushq	%rbp
	.seh_pushreg	%rbp
	pushq	%rdi
	.seh_pushreg	%rdi
	pushq	%rsi
	.seh_pushreg	%rsi
	pushq	%rbx
	.seh_pushreg	%rbx
	addq	$-128, %rsp
	.seh_stackalloc	128
	vmovaps	%xmm6, 48(%rsp)
	.seh_savexmm	%xmm6, 48
	vmovaps	%xmm7, 64(%rsp)
	.seh_savexmm	%xmm7, 64
	vmovaps	%xmm8, 80(%rsp)
	.seh_savexmm	%xmm8, 80
	vmovaps	%xmm9, 96(%rsp)
	.seh_savexmm	%xmm9, 96
	vmovaps	%xmm10, 112(%rsp)
	.seh_savexmm	%xmm10, 112
	.seh_endprologue
	call	__main
	xorl	%ecx, %ecx
.LEHB0:
	call	*__imp__time64(%rip)
.LEHE0:
	movl	%eax, %ecx
	call	srand
	movl	$1600000000, %ecx
.LEHB1:
	call	_Znwy
.LEHE1:
	vmovss	.LC0(%rip), %xmm10
	leaq	1600000000(%rax), %rdi
	movq	%rax, %rbx
	movq	%rax, %rbp
	movabsq	$1152921504606846975, %r12
	movl	$100000000, %esi
	vmovaps	%xmm10, %xmm9
	jmp	.L20
	.p2align 4,,10
.L56:
	testq	%rbx, %rbx
	je	.L5
	vmovss	%xmm6, (%rbx)
	vmovss	%xmm8, 4(%rbx)
	vmovss	%xmm7, 8(%rbx)
	vmovss	%xmm9, 12(%rbx)
.L5:
	addq	$16, %rbx
	subl	$1, %esi
	je	.L55
.L20:
	call	rand
	vxorps	%xmm7, %xmm7, %xmm7
	vcvtsi2ss	%eax, %xmm7, %xmm7
	call	rand
	vxorps	%xmm8, %xmm8, %xmm8
	vcvtsi2ss	%eax, %xmm8, %xmm8
	call	rand
	cmpq	%rbx, %rdi
	vxorps	%xmm6, %xmm6, %xmm6
	leal	1(%rax), %edx
	vcvtsi2ss	%edx, %xmm6, %xmm6
	jne	.L56
	movq	%rbx, %r13
	subq	%rbp, %r13
	movq	%r13, %rax
	sarq	$4, %rax
	testq	%rax, %rax
	je	.L25
	addq	%rax, %rax
	jc	.L48
	cmpq	%r12, %rax
	jbe	.L53
.L48:
	movq	$-16, %rdi
	.p2align 4,,10
.L7:
	movq	%rdi, %rcx
.LEHB2:
	call	_Znwy
.LEHE2:
	movq	%rax, %r14
.L11:
	addq	%r14, %r13
	je	.L15
	vmovss	%xmm6, 0(%r13)
	vmovss	%xmm8, 4(%r13)
	vmovss	%xmm7, 8(%r13)
	vmovss	%xmm10, 12(%r13)
.L15:
	cmpq	%rbp, %rbx
	je	.L27
	movq	%r14, %rcx
	movq	%rbp, %rdx
	.p2align 4,,10
.L18:
	testq	%rcx, %rcx
	je	.L17
	movq	(%rdx), %r9
	movq	8(%rdx), %r10
	movq	%r9, (%rcx)
	movq	%r10, 8(%rcx)
.L17:
	addq	$16, %rdx
	addq	$16, %rcx
	cmpq	%rdx, %rbx
	jne	.L18
	leaq	16(%rbp), %rax
	subq	%rax, %rbx
	andq	$-16, %rbx
	leaq	16(%r14,%rbx), %rbx
.L16:
	addq	$16, %rbx
	testq	%rbp, %rbp
	je	.L19
	movq	%rbp, %rcx
	call	_ZdlPv
.L19:
	addq	%r14, %rdi
	subl	$1, %esi
	movq	%r14, %rbp
	jne	.L20
.L55:
	movq	.refptr._ZSt4cout(%rip), %rdi
	movl	$10, %ebx
	vxorps	%xmm9, %xmm9, %xmm9
	vmovsd	.LC2(%rip), %xmm10
	leaq	.LC3(%rip), %r12
	leaq	.LC4(%rip), %rsi
	.p2align 4,,10
.L22:
	call	clock
	vmovss	0(%rbp), %xmm8
	vmovss	4(%rbp), %xmm7
	movl	%eax, %r13d
	vmulss	%xmm8, %xmm8, %xmm0
	vmovss	8(%rbp), %xmm6
	vmulss	%xmm7, %xmm7, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vmulss	%xmm6, %xmm6, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vucomiss	%xmm0, %xmm9
	jbe	.L21
	call	sqrtf
.L21:
	call	clock
	vxorpd	%xmm1, %xmm1, %xmm1
	movq	%rdi, %rcx
	subl	%r13d, %eax
	vcvtsi2sd	%eax, %xmm1, %xmm1
	vdivsd	%xmm10, %xmm1, %xmm1
.LEHB3:
	call	_ZNSo9_M_insertIdEERSoT_
	movl	$3, %r8d
	movq	%r12, %rdx
	movq	%rax, %rcx
	movq	%rax, %r13
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_x
	movq	%r13, %rcx
	vxorpd	%xmm1, %xmm1, %xmm1
	vcvtss2sd	%xmm8, %xmm1, %xmm1
	call	_ZNSo9_M_insertIdEERSoT_
	movl	$1, %r8d
	movq	%rsi, %rdx
	movq	%rax, %rcx
	movq	%rax, %r13
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_x
	movq	%r13, %rcx
	vxorpd	%xmm1, %xmm1, %xmm1
	vcvtss2sd	%xmm7, %xmm1, %xmm1
	call	_ZNSo9_M_insertIdEERSoT_
	movl	$1, %r8d
	movq	%rsi, %rdx
	movq	%rax, %rcx
	movq	%rax, %r13
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_x
	movq	%r13, %rcx
	vxorpd	%xmm1, %xmm1, %xmm1
	vcvtss2sd	%xmm6, %xmm1, %xmm1
	call	_ZNSo9_M_insertIdEERSoT_
	movl	$1, %r8d
	movq	%rax, %rcx
	leaq	.LC5(%rip), %rdx
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_x
.LEHE3:
	subl	$1, %ebx
	jne	.L22
	movq	%rbp, %rcx
	call	_ZdlPv
	nop
	vmovaps	48(%rsp), %xmm6
	xorl	%eax, %eax
	vmovaps	64(%rsp), %xmm7
	vmovaps	80(%rsp), %xmm8
	vmovaps	96(%rsp), %xmm9
	vmovaps	112(%rsp), %xmm10
	subq	$-128, %rsp
	popq	%rbx
	popq	%rsi
	popq	%rdi
	popq	%rbp
	popq	%r12
	popq	%r13
	popq	%r14
	ret
	.p2align 4,,10
.L25:
	movl	$16, %edi
	jmp	.L7
.L27:
	movq	%r14, %rbx
	jmp	.L16
.L30:
.L51:
	vzeroupper
.L14:
	movq	%rax, %rcx
.LEHB4:
	call	_Unwind_Resume
.LEHE4:
.L53:
	xorl	%edi, %edi
	xorl	%r14d, %r14d
	testq	%rax, %rax
	je	.L11
	salq	$4, %rax
	movq	%rax, %rdi
	jmp	.L7
.L29:
.L13:
	movq	%rbp, %rcx
	movq	%rax, 40(%rsp)
	vzeroupper
	call	_ZdlPv
	movq	40(%rsp), %rax
	jmp	.L14
.L31:
	testq	%rbp, %rbp
	jne	.L13
	jmp	.L51
	.def	__gxx_personality_seh0;	.scl	2;	.type	32;	.endef
	.seh_handler	__gxx_personality_seh0, @unwind, @except
	.seh_handlerdata
.LLSDA6697:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE6697-.LLSDACSB6697
.LLSDACSB6697:
	.uleb128 .LEHB0-.LFB6697
	.uleb128 .LEHE0-.LEHB0
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB1-.LFB6697
	.uleb128 .LEHE1-.LEHB1
	.uleb128 .L30-.LFB6697
	.uleb128 0
	.uleb128 .LEHB2-.LFB6697
	.uleb128 .LEHE2-.LEHB2
	.uleb128 .L31-.LFB6697
	.uleb128 0
	.uleb128 .LEHB3-.LFB6697
	.uleb128 .LEHE3-.LEHB3
	.uleb128 .L29-.LFB6697
	.uleb128 0
	.uleb128 .LEHB4-.LFB6697
	.uleb128 .LEHE4-.LEHB4
	.uleb128 0
	.uleb128 0
.LLSDACSE6697:
	.section	.text.startup,"x"
	.seh_endproc
	.p2align 4,,15
	.def	_GLOBAL__sub_I_main;	.scl	3;	.type	32;	.endef
	.seh_proc	_GLOBAL__sub_I_main
_GLOBAL__sub_I_main:
.LFB7220:
	subq	$40, %rsp
	.seh_stackalloc	40
	.seh_endprologue
	leaq	_ZStL8__ioinit(%rip), %rcx
	call	_ZNSt8ios_base4InitC1Ev
	leaq	__tcf_0(%rip), %rcx
	addq	$40, %rsp
	jmp	atexit
	.seh_endproc
	.section	.ctors,"w"
	.align 8
	.quad	_GLOBAL__sub_I_main
.lcomm _ZStL8__ioinit,1,1
	.section .rdata,"dr"
	.align 4
.LC0:
	.long	1065353216
	.align 8
.LC2:
	.long	0
	.long	1083129856
	.ident	"GCC: (x86_64-posix-seh, Built by MinGW-W64 project) 6.1.0"
	.def	_ZNSt8ios_base4InitD1Ev;	.scl	2;	.type	32;	.endef
	.def	srand;	.scl	2;	.type	32;	.endef
	.def	_Znwy;	.scl	2;	.type	32;	.endef
	.def	rand;	.scl	2;	.type	32;	.endef
	.def	_ZdlPv;	.scl	2;	.type	32;	.endef
	.def	clock;	.scl	2;	.type	32;	.endef
	.def	sqrtf;	.scl	2;	.type	32;	.endef
	.def	_ZNSo9_M_insertIdEERSoT_;	.scl	2;	.type	32;	.endef
	.def	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_x;	.scl	2;	.type	32;	.endef
	.def	_Unwind_Resume;	.scl	2;	.type	32;	.endef
	.def	_ZNSt8ios_base4InitC1Ev;	.scl	2;	.type	32;	.endef
	.def	atexit;	.scl	2;	.type	32;	.endef
	.section	.rdata$.refptr._ZSt4cout, "dr"
	.globl	.refptr._ZSt4cout
	.linkonce	discard
.refptr._ZSt4cout:
	.quad	_ZSt4cout
