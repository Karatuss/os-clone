.text
    .code 32

    .global vector_start
    .global vector_end

    vector_start:
        mov r0, r1
    vector_end:
        .space 1024, 0
.end