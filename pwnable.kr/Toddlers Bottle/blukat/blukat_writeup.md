# "if this challenge is hard, you are a skilled player."

Man it was too easy it was hard, I tried to run `passwords = [''.join(x) for x in itertools.product(''.join(chr(i) for i in range(128)), repeat=23)]` in python to bruteforce all the 23 length (the length of the key) strings that exist... Even then you didn't even need GDB according to the top solution in the writeups on pwnable...

# To get the flag (exploit):
```
$ gdb blukat
(gdb) b *0x000000000040086f
Breakpoint 2 at 0x40086f
(gdb) r
Continuing.
guess the password!
aaaaaaaaaaaaaaaaaaaaaaa

Breakpoint 2, 0x000000000040086f in main ()
(gdb) set $eflags |= (1 << 6)
(gdb) c
Continuing.
<FLAG>
[Inferior 1 (process 327062) exited normally]
(gdb) q
```

# Why it works (writeup)

0x000000000040086f is the adress of `<main+117>: jne 0x4008a0 <main+166>` which jumps to the else{} at the end of main(), so i made it skip that jne (by setting the Z flag) to jump to calc_flag() which apperently just prints the flag (initially i though it won't work in gdb).

