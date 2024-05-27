; File: read_rbp.asm

.code
PUBLIC read_rbp

read_rbp PROC
    mov rax, rbp    ; Move the value of rbp into rax
    mov [rcx], rax  ; Store the value of rbp at the address in rcx (first argument in x64 calling convention)
    ret
read_rbp ENDP

PUBLIC read_rsp

read_rsp PROC
    mov rax, rsp    ; Move the value of rsp into rax
    mov [rcx], rax  ; Store the value of rsp at the address in rcx (first argument in x64 calling convention)
    ret
read_rsp ENDP

END
