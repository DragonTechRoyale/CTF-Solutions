from pwn import *

context.log_level = 'debug'

shell = ssh(host="pwnable.kr", port=2222, user="syscall", password="guest", level='debug')
print(repr(shell.sendline("cd tmp")))
shell("vi c.c")
shell("a")
f = open("./clean.c", 'r')
shell(f.read())
f.close()
shell("\x1b") # esc
shell(":wq")
shell("gcc c.c")
shell("./a.out")
shell.close()