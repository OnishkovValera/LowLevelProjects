%include "lib.inc"
%define byte 8
section .text
global find_word

find_word:
	.loop:
		add rsi, byte
        push rdi
		call string_equals
        pop rdi
		test rax, rax
		jnz .success

		sub rsi, byte
		test rsi, rsi
		jz .dict_end
		jmp .loop
	.success:
		mov rax, rsi
		ret
	.dict_end:
		xor rax, rax
		ret
