# Exploit 
```sh
printf "8\n16\n32\n72\n136\n264\n520\n1032\n2056\n4096\n" | nc 0 9022
```

# Why it works
Before continueing to read, if you don't understand yet memory alignment, watch this video by Prof. Harry Porter: https://youtu.be/OKjOZBaKlOc
I run the program with the low values (8, 16, 32...) and get a segfault during the fast_memcpy() of 128. Let's create a new directory in /tmp/m3mcpy and compile the program there so we can debug and disassembly it.
```sh
mkdir /tmp/m3mcpy
cd /tmp/m3mcpy
gcc -o memcpy ~/memcpy.c -m32 -lm
```
Disassemblying the func fast_memcpy() we see this:
```assembly
(gdb) disas fast_memcpy
Dump of assembler code for function fast_memcpy:
   0x08048798 <+0>:	push   ebp
   0x08048799 <+1>:	mov    ebp,esp
   0x0804879b <+3>:	sub    esp,0x10
   0x0804879e <+6>:	cmp    DWORD PTR [ebp+0x10],0x3f
   0x080487a2 <+10>:	jbe    0x80487f0 <fast_memcpy+88>
   0x080487a4 <+12>:	mov    eax,DWORD PTR [ebp+0x10]
   0x080487a7 <+15>:	shr    eax,0x6
   0x080487aa <+18>:	mov    DWORD PTR [ebp-0x4],eax
   0x080487ad <+21>:	and    DWORD PTR [ebp+0x10],0x3f
   0x080487b1 <+25>:	jmp    0x80487e3 <fast_memcpy+75>
   0x080487b3 <+27>:	mov    eax,DWORD PTR [ebp+0xc]
   0x080487b6 <+30>:	mov    edx,DWORD PTR [ebp+0x8]
   0x080487b9 <+33>:	movdqa xmm0,XMMWORD PTR [eax]
   0x080487bd <+37>:	movdqa xmm1,XMMWORD PTR [eax+0x10]
   0x080487c2 <+42>:	movdqa xmm2,XMMWORD PTR [eax+0x20]
   0x080487c7 <+47>:	movdqa xmm3,XMMWORD PTR [eax+0x30]
   0x080487cc <+52>:	movntps XMMWORD PTR [edx],xmm0
   0x080487cf <+55>:	movntps XMMWORD PTR [edx+0x10],xmm1
   0x080487d3 <+59>:	movntps XMMWORD PTR [edx+0x20],xmm2
   0x080487d7 <+63>:	movntps XMMWORD PTR [edx+0x30],xmm3
   0x080487db <+67>:	add    DWORD PTR [ebp+0x8],0x40
   0x080487df <+71>:	add    DWORD PTR [ebp+0xc],0x40
   0x080487e3 <+75>:	mov    eax,DWORD PTR [ebp-0x4]
   0x080487e6 <+78>:	lea    edx,[eax-0x1]
   0x080487e9 <+81>:	mov    DWORD PTR [ebp-0x4],edx
   0x080487ec <+84>:	test   eax,eax
   0x080487ee <+86>:	jne    0x80487b3 <fast_memcpy+27>
   0x080487f0 <+88>:	cmp    DWORD PTR [ebp+0x10],0x0
   0x080487f4 <+92>:	je     0x8048807 <fast_memcpy+111>
   0x080487f6 <+94>:	push   DWORD PTR [ebp+0x10]
   0x080487f9 <+97>:	push   DWORD PTR [ebp+0xc]
   0x080487fc <+100>:	push   DWORD PTR [ebp+0x8]
   0x080487ff <+103>:	call   0x8048763 <slow_memcpy>
   0x08048804 <+108>:	add    esp,0xc
   0x08048807 <+111>:	mov    eax,DWORD PTR [ebp+0x8]
   0x0804880a <+114>:	leave
   0x0804880b <+115>:	ret
End of assembler dump.
```
At the address 0x080487b9 eax and edx are pointers to the source and destination of the memory block to be copied. They are being moved to from ebp+0xc and ebp+0x8, so let's set a breakpoint at 0x080487b1 and run with the low values. 
```sh
printf "8\n16\n32\n64\n128\n256\n512\n1024\n2048\n4096\n" > payload
```
```
(gdb) r < payload
The program being debugged has been started already.
Start it from the beginning? (y or n) y
Starting program: /tmp/m3mcpy/memcpy < payload
Hey, I have a boring assignment for CS class.. :(
The assignment is simple.
-----------------------------------------------------
- What is the best implementation of memcpy?        -
- 1. implement your own slow/fast version of memcpy -
- 2. compare them with various size of data         -
- 3. conclude your experiment and submit report     -
-----------------------------------------------------
This time, just help me out with my experiment and get flag
No fancy hacking, I promise :D
specify the memcpy amount between 8 ~ 16 : specify the memcpy amount between 16 ~ 32 : specify the memcpy amount between 32 ~ 64 : specify the memcpy amount between 64 ~ 128 : specify the memcpy amount between 128 ~ 256 : specify the memcpy amount between 256 ~ 512 : specify the memcpy amount between 512 ~ 1024 : specify the memcpy amount between 1024 ~ 2048 : specify the memcpy amount between 2048 ~ 4096 : specify the memcpy amount between 4096 ~ 8192 : ok, lets run the experiment with your configuration
experiment 1 : memcpy with buffer size 8
ellapsed CPU cycles for slow_memcpy : 2162
ellapsed CPU cycles for fast_memcpy : 222

experiment 2 : memcpy with buffer size 16
ellapsed CPU cycles for slow_memcpy : 246
ellapsed CPU cycles for fast_memcpy : 244

experiment 3 : memcpy with buffer size 32
ellapsed CPU cycles for slow_memcpy : 290
ellapsed CPU cycles for fast_memcpy : 388

experiment 4 : memcpy with buffer size 64
ellapsed CPU cycles for slow_memcpy : 480

Breakpoint 2, 0x080487b1 in fast_memcpy ()
```
Let's see our destination and source adresses:
```
(gdb) x $ebp+0xc
0xffffdb74:	0xf7fca000
(gdb) x $ebp+0x8
0xffffdb70:	0x0804d060
```
They both end in 0 so we should be good (that means that so far the memory is aligned). Let's continue execution and see that there isn't a segfault.
```
(gdb) c
Continuing.
ellapsed CPU cycles for fast_memcpy : 797854075176

experiment 5 : memcpy with buffer size 128
ellapsed CPU cycles for slow_memcpy : 1118

Breakpoint 2, 0x080487b1 in fast_memcpy ()
(gdb) x $ebp+0xc
0xffffdb74:	0xf7fca000
(gdb) x $ebp+0x8
0xffffdb70:	0x0804d0a8
```
Now we see that the destination address ends with 8, that means that it isn't aligned and the memory copy won't work. Let's continue to validate:
```
(gdb) c
Continuing.

Program received signal SIGSEGV, Segmentation fault.
0x080487cc in fast_memcpy ()
```
Yes, segfault. That happned because a none aligned memory copy was attempted, which causes a segfault. 
```
   0x080487b9 <+33>:	movdqa xmm0,XMMWORD PTR [eax]
   0x080487bd <+37>:	movdqa xmm1,XMMWORD PTR [eax+0x10]
   0x080487c2 <+42>:	movdqa xmm2,XMMWORD PTR [eax+0x20]
   0x080487c7 <+47>:	movdqa xmm3,XMMWORD PTR [eax+0x30]
=> 0x080487cc <+52>:	movntps XMMWORD PTR [edx],xmm0
   0x080487cf <+55>:	movntps XMMWORD PTR [edx+0x10],xmm1
   0x080487d3 <+59>:	movntps XMMWORD PTR [edx+0x20],xmm2
   0x080487d7 <+63>:	movntps XMMWORD PTR [edx+0x30],xmm3
```
```
(gdb) i r $edx
edx            0x804d0a8	1345332881
```
According to https://c9x.me/x86/html/file_module_x86_id_197.html, movntps will throw an execption when one of the memory operands isn't aligned, in this case edx containing our destination address. To make it aligned we need to make the last 4 bits be 0, and for that we need to change the size of memory we copy. Because the last 4 bits are 8, we need to either add 8 or remove 8 from the size to make the destination address be bigger or smaller by 8, and because we already give the lowest possible value we need to add by 8 so we can have an aligned adress. For this we need to replace the size 64 with 72 because 64 + 8 = 72, which will cause our destination address to be 0x804D0B0. 
```sh
printf "8\n16\n32\n72\n128\n256\n512\n1024\n2048\n4096\n" > payload
```
```
(gdb) r < payload
The program being debugged has been started already.
Start it from the beginning? (y or n) y
Starting program: /tmp/m3mcpy/memcpy < payload
Hey, I have a boring assignment for CS class.. :(
The assignment is simple.
-----------------------------------------------------
- What is the best implementation of memcpy?        -
- 1. implement your own slow/fast version of memcpy -
- 2. compare them with various size of data         -
- 3. conclude your experiment and submit report     -
-----------------------------------------------------
This time, just help me out with my experiment and get flag
No fancy hacking, I promise :D
specify the memcpy amount between 8 ~ 16 : specify the memcpy amount between 16 ~ 32 : specify the memcpy amount between 32 ~ 64 : specify the memcpy amount between 64 ~ 128 : specify the memcpy amount between 128 ~ 256 : specify the memcpy amount between 256 ~ 512 : specify the memcpy amount between 512 ~ 1024 : specify the memcpy amount between 1024 ~ 2048 : specify the memcpy amount between 2048 ~ 4096 : specify the memcpy amount between 4096 ~ 8192 : ok, lets run the experiment with your configuration
experiment 1 : memcpy with buffer size 8
ellapsed CPU cycles for slow_memcpy : 2090
ellapsed CPU cycles for fast_memcpy : 228

experiment 2 : memcpy with buffer size 16
ellapsed CPU cycles for slow_memcpy : 258
ellapsed CPU cycles for fast_memcpy : 244

experiment 3 : memcpy with buffer size 32
ellapsed CPU cycles for slow_memcpy : 324
ellapsed CPU cycles for fast_memcpy : 344

experiment 4 : memcpy with buffer size 72
ellapsed CPU cycles for slow_memcpy : 656

Breakpoint 2, 0x080487b1 in fast_memcpy ()
(gdb) c
Continuing.

Breakpoint 1, 0x080487b9 in fast_memcpy ()
(gdb) c
Continuing.
ellapsed CPU cycles for fast_memcpy : 33972732044

experiment 5 : memcpy with buffer size 128
ellapsed CPU cycles for slow_memcpy : 1032

Breakpoint 2, 0x080487b1 in fast_memcpy ()
(gdb) c
Continuing.

Breakpoint 1, 0x080487b9 in fast_memcpy ()
(gdb) i r $eax $edx
eax            0xf7fca000	-134438912
edx            0x804d0b0	134533296
```
Viola it is indeed aligned now, so it shouldn't segfault. 
```
(gdb) c
Continuing.
ellapsed CPU cycles for fast_memcpy : 81249483344

experiment 6 : memcpy with buffer size 256
ellapsed CPU cycles for slow_memcpy : 1936

Breakpoint 2, 0x080487b1 in fast_memcpy ()
```
Indeed it doesn't now, but will again because the adress got unaligned again.
```
Breakpoint 1, 0x080487b9 in fast_memcpy ()
(gdb) c
Continuing.
ellapsed CPU cycles for fast_memcpy : 81249483344

experiment 6 : memcpy with buffer size 256
ellapsed CPU cycles for slow_memcpy : 1936

Breakpoint 2, 0x080487b1 in fast_memcpy ()
(gdb) c
Continuing.

Breakpoint 1, 0x080487b9 in fast_memcpy ()
(gdb) i r $eax $edx
eax            0xf7fca000	-134438912
edx            0x804d138	134533432
(gdb) c
Continuing.

Program received signal SIGSEGV, Segmentation fault.
0x080487cc in fast_memcpy ()
```
Now the address in $edx is unaligned because the last 4 bits are 8, so let's add 8 to the size again. 128 + 8 = 136. And let's continue adding 8's to the rest of the sizes so we get aligned addresses, except the last size because we don't care after the end of the last experiment if the memory is aligned or not because it doesn't matter. 
```sh
memcpy@pwnable:/tmp/m3mcpy$ printf "8\n16\n32\n72\n136\n264\n520\n1032\n2056\n4096\n" | ./memcpy
Hey, I have a boring assignment for CS class.. :(
The assignment is simple.
-----------------------------------------------------
- What is the best implementation of memcpy?        -
- 1. implement your own slow/fast version of memcpy -
- 2. compare them with various size of data         -
- 3. conclude your experiment and submit report     -
-----------------------------------------------------
This time, just help me out with my experiment and get flag
No fancy hacking, I promise :D
specify the memcpy amount between 8 ~ 16 : specify the memcpy amount between 16 ~ 32 : specify the memcpy amount between 32 ~ 64 : specify the memcpy amount between 64 ~ 128 : specify the memcpy amount between 128 ~ 256 : specify the memcpy amount between 256 ~ 512 : specify the memcpy amount between 512 ~ 1024 : specify the memcpy amount between 1024 ~ 2048 : specify the memcpy amount between 2048 ~ 4096 : specify the memcpy amount between 4096 ~ 8192 : ok, lets run the experiment with your configuration
experiment 1 : memcpy with buffer size 8
ellapsed CPU cycles for slow_memcpy : 2020
ellapsed CPU cycles for fast_memcpy : 322

experiment 2 : memcpy with buffer size 16
ellapsed CPU cycles for slow_memcpy : 182
ellapsed CPU cycles for fast_memcpy : 330

experiment 3 : memcpy with buffer size 32
ellapsed CPU cycles for slow_memcpy : 332
ellapsed CPU cycles for fast_memcpy : 424

experiment 4 : memcpy with buffer size 72
ellapsed CPU cycles for slow_memcpy : 612
ellapsed CPU cycles for fast_memcpy : 268

experiment 5 : memcpy with buffer size 136
ellapsed CPU cycles for slow_memcpy : 1072
ellapsed CPU cycles for fast_memcpy : 278

experiment 6 : memcpy with buffer size 264
ellapsed CPU cycles for slow_memcpy : 1984
ellapsed CPU cycles for fast_memcpy : 326

experiment 7 : memcpy with buffer size 520
ellapsed CPU cycles for slow_memcpy : 3792
ellapsed CPU cycles for fast_memcpy : 446

experiment 8 : memcpy with buffer size 1032
ellapsed CPU cycles for slow_memcpy : 7324
ellapsed CPU cycles for fast_memcpy : 624

experiment 9 : memcpy with buffer size 2056
ellapsed CPU cycles for slow_memcpy : 13974
ellapsed CPU cycles for fast_memcpy : 924

experiment 10 : memcpy with buffer size 4096
ellapsed CPU cycles for slow_memcpy : 28732
ellapsed CPU cycles for fast_memcpy : 1370

thanks for helping my experiment!
flag : ----- erased in this source code -----
```
And it works! 
