from pwn import *

p = remote("vortex.labs.overthewire.org", 5842)
data_list = []
data = p.recv() + p.recv()
data_len = len(data)
for i in range(data_len):
    tmp = data[:4]
    data_list.append(struct.unpack('!i', tmp))
    data.replace(tmp, '')

print(data_list)