%define READ 0 
%define WRITE 1
%define EXIT 60
%define IN 0
%define OUT 1
%define TAB 9
%define SPACE 32 
%define NEWLINE 10
%define NULL 0


section .text

global exit
global string_length
global print_string
global print_char
global print_newline
global print_uint
global print_int
global string_equals
global read_char
global read_word
global read_string
global parse_uint
global parse_int
global string_copy

; Принимает код возврата и завершает текущий процесс
exit: 
    mov rax, EXIT
    syscall


; Принимает указатель на нуль-терминированную строку, возвращает её длину
string_length:
    xor rax, rax
    .loop: 
        cmp byte[rdi + rax], 0
        je .escape
        inc rax
        jmp .loop
    .escape:
        ret

; Принимает указатель на нуль-терминированную строку, выводит её в stdout
print_string:
    push rdi
    call string_length
    pop rsi
    mov rdx, rax
    mov rax, WRITE
    mov rdi, OUT
    syscall
    ret

; Принимает код символа и выводит его в stdout
print_char:

    push rdi
    mov rsi, rsp
    mov rdx, 1
    mov rdi, OUT
    mov rax, WRITE
    syscall
    pop rax
    ret

; Переводит строку (выводит символ с кодом 0xA)
print_newline:
    xor rax, rdi
    mov rdi, `/n`
    jmp print_char

; Выводит беззнаковое 8-байтовое число в десятичном формате 
; Совет: выделите место в стеке и храните там результаты деления
; Не забудьте перевести цифры в их ASCII коды.
print_uint:
	mov rax, rdi
	mov rcx, rsp
	mov rdi, NEWLINE
	push 0
	.loop:
		xor rdx, rdx
		div rdi	
		add rdx, '0'
		dec rsp
		mov byte[rsp], dl
		test rax, rax
		jnz .loop
	
	mov rdi, rsp
	push rcx
	call print_string
	pop rcx
	mov rsp, rcx 
    ret


; Выводит знаковое 8-байтовое число в десятичном формате e
print_int:
    test rdi, rdi 
    jns print_uint
    push rdi 
    mov rdi, "-"
    call print_char
    pop rdi
    neg rdi
    jmp print_uint


; Принимает два указателя на нуль-терминированные строки, возвращает 1 если они равны, 0 иначе
string_equals:
    xor rcx, rcx
    .loop:
        mov dl, [rdi + rcx]
        cmp dl, [rsi + rcx]
        jne .not_equal
        inc rcx
        test dl, dl
        jnz .loop
        mov rax, 1
        jmp .equals
    .not_equal:
        xor rax, rax

    .equals:
        ret

; Читает один символ из stdin и возвращает его. Возвращает 0 если достигнут конец потока
read_char:
    mov rax, READ
    mov rdi, IN
    mov rdx, 1
    dec rsp
	mov rsi, rsp
	syscall
	test rax, rax
	jz .end
	mov rax, [rsp]
	
	.end:
		inc rsp
		ret 


; Принимает: адрес начала буфера, размер буфера
; Читает в буфер слово из stdin, пропуская пробельные символы в начале, .
; Пробельные символы это пробел 0x20, табуляция 0x9 и перевод строки 0xA.
; Останавливается и возвращает 0 если слово слишком большое для буфера
; При успехе возвращает адрес буфера в rax, длину слова в rdx.
; При неудаче возвращает 0 в rax
; Эта функция должна дописывать к слову нуль-терминатор

read_word:
    push r12
    push r13 
    push r14
    xor r12, r12 
    mov r13, rdi
    mov r14, rsi

    .null_cheker: 
        call read_char 
        cmp al, SPACE
        jz .null_cheker
        cmp al, NEWLINE
        jz .null_cheker
        cmp al, TAB
        jz .null_cheker

    .loop:
        cmp r12, r14
        jz .overflow
        cmp al, NULL
        jz .add_zero
        cmp al, SPACE
        jz .add_zero
        cmp al, NEWLINE
        jz .add_zero
        cmp al, TAB
        jz .add_zero
        mov [r13 + r12], al 
        inc r12
        call read_char
        jmp .loop

    .overflow: 
        xor rax, rax
        jmp .end

    .add_zero: 
        mov [r13 + r12], al
        mov rax, r13
        mov rdx, r12
        
    .end:
        pop r14
        pop r13 
        pop r12
        ret



; Принимает указатель на строку, пытается
; прочитать из её начала беззнаковое число.
; Возвращает в rax: число, rdx : его длину в символах
; rdx = 0 если число прочитать не удалось
parse_uint:
   xor rax, rax
   xor rcx, rcx
   xor rdx, rdx
   
   .loop:
        mov dl, byte[rdi + rcx]
	sub dl, '0'
        js .quit
	cmp dl, '9' - '0' + 1
	jns .quit
	inc rcx
	imul rax, 10
	add al , dl
	jmp .loop
	
    .quit:
	mov rdx, rcx
	ret



; Принимает указатель на строку, пытается
; прочитать из её начала знаковое число.
; Если есть знак, пробелы между ним и числом не разрешены.
; Возвращает в rax: число, rdx : его длину в символах (включая знак, если он был) 
; rdx = 0 если число прочитать не удалось
parse_int:
        xor rax, rax
        cmp byte[rdi], '-'
        jz .negative_number
	    cmp byte[rdi], '+'
	    jnz parse_uint
	    inc rdi
        call parse_uint
	    inc rdx
	    ret 

    .negative_number:
	    inc rdi
	    call parse_uint
	    neg rax
	    inc rdx
	    ret 

; Принимает указатель на строку, указатель на буфер и длину буфера
; Копирует строку в буфер
; Возвращает длину строки если она умещается в буфер, иначе 0
string_copy:
    xor rcx, rcx
	.loop:
		cmp rcx, rdx
		jz .overflow
		mov rax, [rdi + rcx]
		mov [rsi + rcx], rax
		inc rcx
		test rax, rax
		jz .end
		jmp .loop
		
	.overflow:
		xor rax, rax
		
    .end:
		ret
