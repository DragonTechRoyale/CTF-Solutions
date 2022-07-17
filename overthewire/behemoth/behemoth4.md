In short, the program gets its PID and prints the text contens of a file in `/tmp` with the name of its PID.
So, to get the flag I had to run the program in GDB to get its current PID (it was around 9000 at the time), predict a future PID it might have (predicted 9050, which it did have), and created a symlink with the flag to /tmp/<PID> so the program reads the flag.
```sh
behemoth4@behemoth:~$ ln -s /etc/behemoth_pass/behemoth5 /tmp/9050
behemoth4@behemoth:/behemoth$ ./behemoth4
```