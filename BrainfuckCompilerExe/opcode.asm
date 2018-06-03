[bits 32]


inc esi
dec esi
inc byte [esi]
dec byte [esi]

; putchar
xor eax, eax ; zerowanie
mov al, [esi]
push eax
call dword [0x4090A0]
add esp, 4
; ---------

; getchar
call dword [0x4090A4]
mov [esi], al

ret ; C3
; -----------

mov esi, 0x401000

; [
cmp byte [esi], 0
jnz L1
mov eax, 0x12345678
jmp eax
L1:

; ]
mov eax, 0x12345678
jmp eax



