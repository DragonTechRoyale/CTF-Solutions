
.global _start
.section .text

_start:
        mov r7, #0x4
        mov r0, #1
        ldr r1, sp
        mov r2, #13
        swi 0

        mov r7, #0x1
        mov r0, #65
        swi 0
