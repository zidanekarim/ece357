# read is 0, write is 1, exit is 603 
.section .bss
.lcomm buf, 4096 # looked this up wtvr was in instructions did not work

.section .text
.global _start

_start:
loop:
    mov $0, %rax
    mov $0, %rdi
    mov $buf, %rsi
    mov $4096, %rdx
    syscall

    cmp $0, %rax
    jle done

    mov %rax, %rdx
    mov $1, %rax
    mov $1, %rdi
    mov $buf, %rsi
    syscall
    jmp loop

done:
    mov %rax, %rdi
    mov $60, %rax
    syscall
