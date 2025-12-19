# read is 63, write is 64, exit is 93 (this is on wsl2.0 using ubuntu)

section .data

section .comm 
    buf,4096,4

.section .text 
    global _start

_start:
    loop

loop:
    mov rax, 63          
    mov rdi, 0            
    mov rsi, buf         
    mov rdx, 4096        
    syscall

    cmpq rax, 0
    jle done               
 # write
    mov rax, 64         
    mov rdi, 1           
    mov rsi, buf        
    mov rdx, rax         
    syscall

    jmp loop             

done: # exit
    mov rdi, rax # since the read return value is in rax
    mov rax, 93     # set rax to 93 now to exit
    syscall