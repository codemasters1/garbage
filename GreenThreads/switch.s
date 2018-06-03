.intel_syntax noprefix
.globl switchContext_impl

switchContext_impl:
mov [rdi + 0x00], rsp
mov [rdi + 0x08], r15
mov [rdi + 0x10], r14
mov [rdi + 0x18], r13
mov [rdi + 0x20], r12
mov [rdi + 0x28], rbx
mov [rdi + 0x30], rbp
mov rsp, [rsi + 0x00]
mov r15, [rsi + 0x08]
mov r14, [rsi + 0x10]
mov r13, [rsi + 0x18]
mov r12, [rsi + 0x20]
mov rbx, [rsi + 0x28]
mov rbp, [rsi + 0x30]
ret