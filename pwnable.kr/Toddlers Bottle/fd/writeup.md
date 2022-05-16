## To get the flag:

```
./fd 4661
LETMEWIN
good job :)
<FLAG>
```

## Why it works:

The argument 4661:

First, the program checks if there is an argument by checking the length of argc, and returns without one.
```
if(argc<2){
    printf("pass argv[1] a number\n");
    return 0;
}
```
```
./fd
pass argv[1] a number
```


Afterwards, an integer named "fd" is created.
```
int fd = atoi( argv[1] ) - 0x1234;
```
This integer is the file descriptor.


Two lines afterwards, the program executes read(), and reads to buf (a 32 charcater long char array), according to the file descriptor.
```
int len = 0;
len = read(fd, buf, 32);
```
If fd equels 0 then read() will read from standart input, if it equels 1 than from standart output, and if 2 than from satndart error.
As we see in the code fd isn't initially assigned with 0, 1 or 2 but with ```atoi( argv[1] ) - 0x1234```. ```atoi( argv[1] )``` Takes the first argument given to the program, (in the "To get the flag" above it's 4661) and converts it to a number, because it's initially a string. ``` - 0x1234``` reduces 0x1234 (4660 in decimal) from the number ```atoi( argv[1] )``` returned, leaving us with ```fd = 1``` in the "To get the flag" above.


So if we try 4600 or 4662 it should still work, because 0 and 2 are file descriptors too.
```
./fd 4660
LETMEWIN
good job :)
```
```
./fd 4662
LETMEWIN
good job :)
```


But, if we try different number it won't work, because only 0, 1 and 2 are file descriptors.
```
./fd 4663
learn about Linux file IO
```


The read() function reads 32 bytes to buf, so we can enter text. the text we need to enter is ```LETMEWIN``` because after the read() function there's an if statement that checks if we entered ```LETMEWIN```. If we did it prints "good job :)", gives us the flag, and exists.
```
if(!strcmp("LETMEWIN\n", buf)){
    printf("good job :)\n");
    system("/bin/cat flag");
    exit(0);
}
```


If we entered text that isn't ```LETMEWIN``` it will not enter the if statement and print "learn about Linux file IO".
```
./fd 4660
D0ntL3tM3W1n
learn about Linux file IO
```
