Ive gdb'd the program and si'd and ni'd some instructions, observing what's going on. I've looked that after the strcpy (that was in assembly) there's a bunch of text in memory, so ive x'd the registers and on one of them ive found a message about upx and a link to here: https://upx.github.io/. I try unpacking the binary with upx and it works. after that i gdb it again now unpacked and breakpoint to 0x401344 which is just right before exit (and after the strcpy). I run and viola, the flag is pointed to by rdi. 
```sh
Starting program: /root/flag.2
I will malloc() and strcpy the flag there. take it.
[ Legend: Modified register | Code | Heap | Stack | String ]
───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── registers ────
$rax   : 0x0
$rbx   : 0x00000000401ae0  →  <__libc_csu_fini+0> push rbx
$rcx   : 0x00000000496650  →  0x00000000000029 (")"?)
$rdx   : 0x000000006c96d8  →  0x00000000000029 (")"?)
$rsp   : 0x007fffffffe040  →  0x0000000000000000
$rbp   : 0x0
$rsi   : 0x8
$rdi   : 0x000000006c96b0  →  "UPX...? sounds like a delivery service :)"
$rip   : 0x00000000401344  →  <__libc_start_main+404> mov edi, eax
$r8    : 0x1
$r9    : 0x3a20656369767265 ("ervice :"?)
$r10   : 0x22
$r11   : 0x0
$r12   : 0x00000000401a50  →  <__libc_csu_init+0> push r14
$r13   : 0x0
$r14   : 0x0
$r15   : 0x0
$eflags: [zero carry parity adjust sign trap INTERRUPT direction overflow resume virtualx86 identification]
$cs: 0x33 $ss: 0x2b $ds: 0x00 $es: 0x00 $fs: 0x00 $gs: 0x00
───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── stack ────
0x007fffffffe040│+0x0000: 0x0000000000000000	 ← $rsp
0x007fffffffe048│+0x0008: 0x0000000100000000
0x007fffffffe050│+0x0010: 0x007fffffffe128  →  0x007fffffffe42d  →  "/root/flag.2"
0x007fffffffe058│+0x0018: 0x00000000401164  →  <main+0> push rbp
0x007fffffffe060│+0x0020: 0x00000000401ae0  →  <__libc_csu_fini+0> push rbx
0x007fffffffe068│+0x0028: 0x0000000000000000
0x007fffffffe070│+0x0030: 0x00000000401a50  →  <__libc_csu_init+0> push r14
0x007fffffffe078│+0x0038: 0x0000000000000000
─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── code:x86:64 ────
     0x401337 <__libc_start_main+391> mov    rsi, QWORD PTR [rsp+0x10]
     0x40133c <__libc_start_main+396> mov    edi, DWORD PTR [rsp+0xc]
     0x401340 <__libc_start_main+400> call   QWORD PTR [rsp+0x18]
●→   0x401344 <__libc_start_main+404> mov    edi, eax
     0x401346 <__libc_start_main+406> call   0x401c30 <exit>
     0x40134b <__libc_start_main+411> mov    edi, 0x49668c
     0x401350 <__libc_start_main+416> call   0x4026a0 <__libc_fatal>
     0x401355 <__libc_start_main+421> xor    eax, eax
     0x401357 <__libc_start_main+423> jmp    0x4011ef <__libc_start_main+63>
─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── threads ────
[#0] Id 1, Name: "flag.2", stopped 0x401344 in __libc_start_main (), reason: BREAKPOINT
───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── trace ────
[#0] 0x401344 → __libc_start_main()
[#1] 0x401081 → _start()
────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────

Breakpoint 5, 0x0000000000401344 in __libc_start_main ()
gef➤
```
I dont really understand why the flag is here, excpet that as far as I know the strcpy here copied all the strings in the program or everything in the program and the flag was at the bottom, and rdi pointed to the bottom. Anyways this CTF was pretty easy
