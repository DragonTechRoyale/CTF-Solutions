from pwn import *

p = process("./vortex0")
sleep(1)
tmp = p.recv().split()
username = tmp[1]
password = tmp[-1].replace('\n', '')
print("Username: " + username + " Password: " + password)

shell = ssh(host="vortex.labs.overthewire.org", user=username, password=password, port=2228)
shell.interactive()
p = process("/vortex/vortex1")