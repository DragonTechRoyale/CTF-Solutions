# To Get The Flag:
```sh
./col `printf "\xc8\xce\xc5\x06\xc8\xce\xc5\x06\xc8\xce\xc5\x06\xc8\xce\xc5\x06\xcc\xce\xc5\x06"`
```

# Why It works
First, when you run ```col``` it asks for a passcode.
```sh
./col
usage : ./col [passcode]
```
That's because the program checks if there is an argument by checking the length of argc, and returns without one.
```c
if(argc<2){
    printf("usage : %s [passcode]\n", argv[0]);
    return 0;
}
```


Afterwards, the program checks if the length of the argument is 20 bytes long or not.
```sh
./col 11
passcode length should be 20 bytes
```
```c
if(strlen(argv[1]) != 20){
    printf("passcode length should be 20 bytes\n");
    return 0;
}
```

So let's give it a 20 byte (by using print in python) long argument by using printf and `` in the argument:
```python
python3
Python 3.10.2 (v3.10.2:a58ebcc701, Jan 13 2022, 14:50:16) [Clang 13.0.0 (clang-1300.0.29.30)] on darwin
Type "help", "copyright", "credits" or "license" for more information.
>>> print("a"*20)
aaaaaaaaaaaaaaaaaaaa
```
```sh
./col `printf "aaaaaaaaaaaaaaaaaaaa"`
wrong passcode.
```
It tells us that the passcode is wrong, because the hash of 20 a's isn't matching the hash in the beggining of the program.
The hash:
```c
unsigned long hashcode = 0x21DD09EC;
```
The if statement comparing the hash of the argument to the intended hash of the passcode:
```c
if(hashcode == check_password( argv[1] )){
    system("/bin/cat flag");
    return 0;
}
else
    printf("wrong passcode.\n");
return 0;
```

As we see, we will get the flag if the passcode will be correct, or even if it's different? :)
For this we need to examine check_password().
```c
unsigned long check_password(const char* p){
	int* ip = (int*)p;
	int i;
	int res=0;
	for(i=0; i<5; i++){
		res += ip[i];
	}
	return res;
}
```
As we can see, this functions returns an unsigned long integer, and gets a pointer to a char array named "p".
In the first line 
```c
int* ip = (int*)p;
```
an int type pointer is being declared, with it's value being "p"'s value, but caster as an int pointer instead of a char pointer.


```c
int i;
int res=0;
for(i=0; i<5; i++){
    res += ip[i];
}
return res;
```
Then, an i integer is being declared for the for loop, and a "res" integer that will be returned at the end. This one is the hash of the argument supplied to the program.
The initial char pointer that our program received as an argument was 20 bytes long, and because an integer is 4 bytes long, then "ip" points to an array of 5 integers, that's why the for loop will loop 5 times. The loop just summs all the 5 inetegers in the array to res. From this we understand that the hash is a sum of the array, so to get a hash that is equal to "hashcode" we need a 20 byte array who's sum equals to "hashcode", or 0x21DD09EC.

To do this, we need to divide 0x21DD09EC by 5, because of the 5 integers the 20 byte array equats to. For this we will use python again.
```python
>>> hash = 0x21DD09EC
>>> hash / 5
113626824.8
```
As we can see, after dividing it by 5 we get a number that isn't a whole number. This isn't a problem because check_password() sums the 5 integers in the array "ip", so one of the members of the array can be larger. So, we need 4 members equal to 113626824 and one that equals to 113626828, so the total will be 568134124, which is 0x21DD09EC in hex. In order to pass the array to the function we need to convert each of the members to hex, than to little endian, and then create a 20 byte string that houses the 5 members in their little endian hex form. 
```python
>>> hex(113626824)
'0x6c5cec8'
>>> from pwn import *
>>> p32(0x6c5cec8)
b'\xc8\xce\xc5\x06'
>>> hash
568134124
>>> 568134124 - 113626824 * 4
113626828
>>> hex(113626828)
'0x6c5cecc'
>>> p32(0x6c5cecc)
b'\xcc\xce\xc5\x06'
```
So we need four times '\xc8\xce\xc5\x06' and one time '\xcc\xce\xc5\x06, to create a 20 byte string that it's sum is equal to 0x21DD09EC.
The string is this:
```sh
\xc8\xce\xc5\x06\xc8\xce\xc5\x06\xc8\xce\xc5\x06\xc8\xce\xc5\x06\xcc\xce\xc5\x06
```
Now let's pass it to the program and get the flag!
```sh
./col `printf "\xc8\xce\xc5\x06\xc8\xce\xc5\x06\xc8\xce\xc5\x06\xc8\xce\xc5\x06\xcc\xce\xc5\x06"`
```