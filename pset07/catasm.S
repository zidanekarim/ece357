# read is 63, write is 64, exit is 93 (this is on wsl2.0 using ubuntu)
.section .bss
.lcomm buf, 4096

.section .text
.global _start

_start:
loop:
    mov $63, %rax
    mov $0, %rdi
    mov $buf, %rsi
    mov $4096, %rdx
    syscall

    cmp $0, %rax
    jle done

    mov $64, %rax
    mov $1, %rdi
    mov $buf, %rsi
    mov %rax, %rdx
    syscall
    jmp loop

done:
    mov %rax, %rdi
    mov $93, %rax
    syscall
