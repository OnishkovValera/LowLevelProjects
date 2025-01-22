%include "lib.inc"
%include "dict.inc"
%include "words.inc"
%define SIZE 256

section .rodata
	too_long: db "Input string is too long", 0
	not_found: db "Value is not found", 0

section .bss
	input_string: resb SIZE

section .text
global _start

_start:
    mov rdi, input_string
    mov rsi, SIZE - 1
    call .read_string
    test rax, rax
    jz .value_too_long
    mov rdi, input_string
    mov rsi, first_word
    call find_word
    test rax, rax
    jz .value_not_founded
    mov rdi, rax
    add rdi, 8
    push rdi
    call string_length
    pop rdi
    add rdi, rax
    inc rdi
    call print_string
    xor rdi, rdi
    call exit

.value_not_founded:
    mov rdi, not_found
    jmp .print_error
.value_too_long:
    mov rdi, too_long
    
.print_error:
	 push rdi
	 call string_length
	 mov rdx, rax
     pop rsi
   	 mov rax, 1
	 mov rdi, 2
   	 syscall
     mov rdi, 0
	 call exit

.read_string
	push r12
    push r13
    push r14
    xor r14, r14
    mov r12, rsi
    mov r13, rdi
    .skip:
        call read_char
        cmp rax, ' '
        je .skip
        cmp rax, `\t`
        je .skip
        cmp rax, `\n`
        je .skip
        test rax, rax
        jz .end
        cmp r14, r12
        ja .end
        mov byte[r13 + r14], al
        inc r14
    .loop:
        call read_char
        cmp  r14, r12
        ja .end
        cmp rax, `\t`
        je .word_ended
        cmp rax, `\n`
        je .word_ended
        test rax, rax
        jz .word_ended
        mov byte[r13 + r14], al
        inc r14
        jmp .loop
    .word_ended:
        mov rax, r13
        mov rdx, r14
        mov byte[r13 + r14], 0
        pop r14
        pop r13
        pop r12
        ret
    .end:
        pop r14
        pop r13
        pop r12
        xor rdx, rdx
        xor rax, rax
        ret

