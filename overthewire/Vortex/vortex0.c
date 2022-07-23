//
//  main.c
//  vortex0
//
//  Created by DTR4K on 23/07/2022.
//

// Client side C/C++ program to demonstrate Socket
// programming
#include <fcntl.h> // for open
#include <unistd.h> // for close

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#define MAX 80
#define PORT 5842
#define SA struct sockaddr
#define host "vortex.labs.overthewire.org"
void func(int sockfd)
{
    unsigned int buff[MAX];
    unsigned int result = 0;
    int i = 0;
    char stuff[MAX];
    
    bzero(buff, sizeof(buff));
    bzero(stuff, sizeof(stuff));

    sleep(1);
    read(sockfd, buff, sizeof(buff));
    for (i=0; i<4; i++)
    {
        result += buff[i];
    }

    write(sockfd, &result, sizeof(result));
    bzero(buff, sizeof(buff));
    
    read(sockfd, stuff, sizeof(buff));
    printf("%s\n", &stuff);
}

int main()
{
    int sockfd;
    struct sockaddr_in servaddr;
    struct hostent *hp;
    
    if((hp = gethostbyname(host)) == NULL){
        herror("gethostbyname");
        exit(1);
    }
    
    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }

    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    bcopy(hp->h_addr, &servaddr.sin_addr, hp->h_length);
    servaddr.sin_port = htons(PORT);

    // connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }


    // function for chat
    func(sockfd);

    // close the socket
    close(sockfd);
}
