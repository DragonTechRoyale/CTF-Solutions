# Exploit
```sh
python -c 'print("A"*96 + "\x04\xa0\x04\x08\n134514135")' | /home/passcode/passcode
```

# Why it work

Ok so you have to do some debugging (and reading the code obviously) but because of the stack is aligned, there's some overlap in the stack between functions, let me explain. At first, welcome() reads 100 charcaters to the stack with:
```c
char name[100];
...
scanf("%100s", name);
...
```
So now if you supply a 100 characters (or less) to the program, they will obviously be in the stack.
If you supply more then scanf() will simply ignore them because the 100 in "%100s" tells scanf() to not take more than a 100 characters. You can try to input more and examine the stack and see that the rest of the charcaters after the 100 will simply not be there. 
After welcome(), login() is called. Here something intresting happens:
```c
void login(){
	int passcode1;
	int passcode2;
    ...
```
Right the the start of login() we see two ints being declared, but not initialized with any value. So what will be their value? To that we have to look over the stack. 
We can see on the start of wellcome that 0x88 (136 in decimal) bytes are being given to the stack, by esp being substracted with 0x88.
```asm
(gdb) disas welcome
Dump of assembler code for function welcome:
   0x08048609 <+0>:	push   ebp
   0x0804860a <+1>:	mov    ebp,esp
   0x0804860c <+3>:	sub    esp,0x88
```
After welcome(), in login 0x28 (40 in decimal) bytes are being given to the stack. And because the stack is the same stack because the program is the same program, we've got a big of an alignment between the two stack frames of the two functions. Calculating the difference between the two stack frames, we get 96, because 0x88 - 0x28 = 0x60, or 136 - 40 = 96. So we have 4 bytes left that aren't in the difference between the two stack frames, which means that they will carry on with us to login(), from welcome(). And because a 100 bytes are being written to the stack on welcome(), the last 4 of those 100 bytes will carry with us to login(), and because ints in c are really just parts of the stack, because passcode1 is uninitiallized the 4 leftover bytes will go into it - maybe if it was initialzed the last 4 bytes would've been simply overwritten. (Now I wonder what are those 36 bytes in every start of a function, because in welcome() they aren't the buffer name[100] and are anything in login() afaik because the last 4 of the 40 of login are int passcode1). What's up with passcode2 then? Well, only 4 bytes were carried on from welcome(), so it won't contain something user controllable (or even useful tbh), and after some debugging I've found it really just contains junk. So now we can control passcode1! But what can we do with this? Well, for that let's look at the rest of login(). 
```c
	printf("enter passcode1 : ");
	scanf("%d", passcode1);
	fflush(stdin);
```
After the decleration of the two ints above we see calls to printf(), scanf() anf fflush(). The printf() call is really unintresting, because it's simply printing stuff. However, the scanf() call right after it is where the fun begins. Here, scanf() scans user input, and writes it to the memroy location being pointed to by passcode1. Why is that? Well that's because scanf(), gets two arguments: a format string and (in the case here of scanning for an integer) a *pointer* to where to write the scanned inetegr. If we wanted to write to passcode1 itself than scanf() should've recevived a pointer to passcode1, and be writte like this:
```c
scanf("%d", &passcode1);
```
The & sign tells the compiler to pass the pointer to passcode1 as an argument to scanf() instead of what actually happens in this program, that the contens of passcode1 gets passed. Why is this important? Because it allows us arbitrary memory write to the programs memory! This way we could write somewhere in the program to make something happen so we get the flag. About that, what about the fflush() call right afterwards? Maybe can we write somewhere to make fflush() give us the flag? Well, not exactly, but we can use this call to get it. To this let's examine fflush():
```gdb
(gdb) disas fflush
Dump of assembler code for function fflush@plt:
   0x08048430 <+0>:	jmp    DWORD PTR ds:0x804a004
   0x08048436 <+6>:	push   0x8
   0x0804843b <+11>:	jmp    0x8048410
End of assembler dump.
```
Disassembling fflush() we can see that its code isn't in the program, but there's a simple jump to somewhere. Let's examine that memory of this somewhere:
```gdb
(gdb) x 0x804a004
   0x804a004 <fflush@got.plt>:	test   BYTE PTR ss:[eax+ecx*1],al
```
It's being pointed to the GOT - the Global Offset Table. The GOT is part of (apprently in this program at least, writeable) memory that is used for dynamic linking of functions. In this program, various glibc functions like fflush() are being called to but aren't in the program itself, because they're dynamically linked during runtime to the function in other parts of the OS. This is useful because it saves time in compilation, the compiler doesn't have to compile the same functions all the time, and because thanks to this the programs compiled can be smaller because the regularily used glibc functions don't need to be compiled in every new program that uses them, a simple link between the call in the program to the function is formed, using pointers. As we ca see here, the pointer 0x804a004 is obviously now pointing to junk, as ```test   BYTE PTR ss:[eax+ecx*1],al``` isn't appears to be a valid asm code of fflush(), that is probably because this memry examination is before running the program (inside gdb), so the dynamic linker has yet did the link. So in execution of this program, what's being pointed to by 0x804a004 will run, and in normal execution this will be fflush(), but, we don't want normal execution, we want 1337 h4x and the flag. So to get the flag we need 0x804a004 to point where we want so our program jumps there and get's us the flag. And how handy is that, there is part of the program that prints out the flag! It's this in C:
```c
                printf("Login OK!\n");
                system("/bin/cat flag");
```
And this in asm:
```asm
   0x080485d7 <+115>:	mov    DWORD PTR [esp],0x80487a5
   0x080485de <+122>:	call   0x8048450 <puts@plt>
   0x080485e3 <+127>:	mov    DWORD PTR [esp],0x80487af
   0x080485ea <+134>:	call   0x8048460 <system@plt>
```
In normal execution this part never gets executed because of the ```if(passcode1==338150 && passcode2==13371337){``` statement, which is never true because you can't really write to passcode1 and passcode2 (unless you've found a cl3v3r way to do it). Jumping to this part should give us our flag and it does, by making 0x804a004 point to 0x080485d7. To do this we need use the first scanf() i login() to write to 0x804a004 the value 0x080485d7, and to do this we need to create this attack string:
```
96 bytes for padding + 4 byte memory pointer for scanf() + \n + a number that will be our memory location of the flag printing instructions
```
The initial 100 bytes will be scanned by the scanf() in welcome(), then the \n will tell it to stop scanning and continue execution, then the first scanf() in login() will get a number which it will write to passcode1, and because now passocode1 = 0x804a004, this number will be written to 0x804a004. This number needs to equal 0x080485d7 so we can resume execution there when fflush() is supposed to be called, so we give it the number 134514135 which is 0x080485d7 in decimal. That's why the string ends like this:
```
"A"*96 + "\x04\xa0\x04\x08\n134514135"
```
After 134514135 is written to 0x804a004 the program jumps to 0x080485d7 and prints the flag!