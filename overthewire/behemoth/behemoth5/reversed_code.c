/* partially reversed main() to basically understand what this program does, which is to send the flag to localhost at port 1337.
The exploit simply listens on that port and prints the first thing it gets, which is the flag. */
void main(void)
{
  long lVar1;
  size_t sVar2;
  int iVar3;
  undefined local_38 [4];
  undefined4 local_34;
  undefined auStack48 [8];
  ssize_t local_28;
  int socket_fd;
  hostent *hostname;
  char *local_1c;
  FILE *fd;
  size_t local_14;
  undefined *puStack12;
  
  puStack12 = &stack0x00000004;
  local_14 = 0;
  fd = fopen("/etc/behemoth_pass/behemoth6","r");
  if (fd == (FILE *)0x0) {
    perror("fopen");
                    /* WARNING: Subroutine does not return */
    exit(1);
  }
  fseek(fd,0,2);
  lVar1 = ftell(fd);
  local_14 = lVar1 + 1;
  rewind(fd);
  local_1c = (char *)malloc(local_14);
  fgets(local_1c,local_14,fd);
  sVar2 = strlen(local_1c);
  local_1c[sVar2] = '\0';
  fclose(fd);
  hostname = gethostbyname("localhost");
  if (hostname == (hostent *)0x0) {
    perror("gethostbyname");
                    /* WARNING: Subroutine does not return */
    exit(1);
  }
                    /* socket(AF_INET, SOCK_DGRAM) */
  socket_fd = socket(2,2,0);
  if (socket_fd == -1) {
    perror("socket");
                    /* WARNING: Subroutine does not return */
    exit(1);
  }
  local_38._0_2_ = 2;
  iVar3 = atoi("1337");
  local_38._2_2_ = htons((uint16_t)iVar3);
  local_34 = *(undefined4 *)*hostname->h_addr_list;
  memset(auStack48,0,8);
  sVar2 = strlen(local_1c);
  local_28 = sendto(socket_fd,local_1c,sVar2,0,(sockaddr *)local_38,0x10);
  if (local_28 == -1) {
    perror("sendto");
                    /* WARNING: Subroutine does not return */
    exit(1);
  }
  close(socket_fd);
                    /* WARNING: Subroutine does not return */
  exit(0);
}
