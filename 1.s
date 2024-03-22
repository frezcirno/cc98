    .globl start
    .type   start, @function
start:
    pushq   %rbp
    movq    %rsp, %rbp
    subq    $16, %rsp
    movq    -0x10(%rbp), -0x20(%rbp)
    leave
    ret
    .size   start, .-start
    .ident  "GCC: (GNU) 4.8.5 20150623 (Red Hat 4.8.5-4)"
    .section    .note.GNU-stack,"",@progbits
