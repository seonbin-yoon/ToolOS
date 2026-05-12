;
; SPDX-License-Identifier: GPL-2.0-only
;
; Copyright (c) 2026 Seonbin Yoon
; Project: ToolOS
; File: init.S
; Description:
; 
;

[BITS 64]
default rel

section .text
	global _start
	extern kernel_main

_start:
	cli
	; init all registers to 0; exclude rdi
	xor rax, rax
	mov rbx, rax
	mov rcx, rax
	mov rdx, rax
	mov rsi, rax
	mov r8, rax
	mov r9, rax
	mov r10, rax
	mov r11, rax
	mov r12, rax
	mov r13, rax
	mov r14, rax
	mov r15, rax
	
	; there registers unused on 64bit environment
	; so, clear to 0
	mov ds, ax
	mov ss, ax
	mov es, ax

	; set stack_pointer
	mov rsp, stack_memory + 16384
	mov rbp, rsp

	jmp _main

_main:
	call _check_signature
	call _set_gdt_table
	call _set_paging
	call kernel_main

.halt
	hlt
	jmp .halt

_check_signature:
	mov rax, [rdi]
	mov rbx, [sig_part1]
	cmp rax, rbx
	jne _not_equal

	mov rax, [rdi + 8]
	mov rbx, [sig_part2]
	cmp rax, rbx
	jne _not_equal

	cmp byte [rdi + 119], 1
	jne _not_equal
	ret

_not_equal:
	mov rax, 0x12345678
	hlt

_set_gdt_table:
	; TODO: add logic
	ret

_set_paging:
	; TODO: add logic
	ret

section .data
	sig_part1 dq 0x4220534F4C4F4F54 ; "TOOLOS B"
	sig_part2 dq 0x004F464E49544F4F ; "OOTINFO\0"

section .bss
align 16
stack_memory:
	resb 16384

