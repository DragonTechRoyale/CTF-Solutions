	.section .text
	.global _start

	_start:
		.code 32

		# Thumb-Mode on
		add 	r6, pc, #1
		bx	r6
		.code 	16

		# _write()
		mov 	r2, #16
		mov	r1, pc
		add	r1, #14		
		mov 	r0, $0x1	      
		mov 	r7, $0x4
		svc 	1

		# _exit()
		sub	r4, r4, r4
		mov	r0, r4
		mov 	r7, $0x1
		svc	1
	.ascii "shell-storm.org\n"