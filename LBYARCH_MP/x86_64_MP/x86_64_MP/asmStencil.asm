section .text
bits 64
default rel

global asmStencil

asmStencil:
    sub rsp, 8*11

    mov rsi, rcx    
    mov rdx, rdx    
    mov rdi, r8     

    ; Index initialization
    xor rax, rax    

LOOP:
    cmp rax, rsi
    jge end         

    cmp rax, 3
    jl COMPUTE_STENCIL
    cmp rax, rsi
    jge COMPUTE_STENCIL

    jmp INCREMENT_AND_LOOP

COMPUTE_STENCIL:
    ; Stencil computation
    xorpd xmm0, xmm0           
    movsd xmm1, [rdx+rax*8-24] 
    addsd xmm0, xmm1
    movsd xmm1, [rdx+rax*8-16] 
    addsd xmm0, xmm1
    movsd xmm1, [rdx+rax*8-8]  
    addsd xmm0, xmm1
    movsd xmm1, [rdx+rax*8]    
    addsd xmm0, xmm1
    movsd xmm1, [rdx+rax*8+8]  
    addsd xmm0, xmm1
    movsd xmm1, [rdx+rax*8+16] 
    addsd xmm0, xmm1
    movsd xmm1, [rdx+rax*8+24] 
    addsd xmm0, xmm1

    movsd [rdi+rax*8], xmm0

INCREMENT_AND_LOOP:
    inc rax        
    jmp SHORT LOOP 

end:
    add rsp, 8*11
    ret