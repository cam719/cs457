#include "awget.h"
#include "packet.h"
#include "serialize.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <getopt.h>

int main(int argc, char *argv[])
{
    struct sockaddr_in server , client, client1; 
    int client_sock, c;
    int sock = socket(AF_INET , SOCK_STREAM , 0);
    int sock1 = socket(AF_INET , SOCK_STREAM , 0);
    char data[200];
    char buf[500];
    char buf1[500];
    int port = 1337;
    struct packet p;
    struct stat stat_buf;
    struct stat stat_buf2;
    int c1;
    while ((c1 = getopt (argc, argv, "p:")) != -1)
    switch (c1)
      {
      case 'p':
      port =  atoi(optarg);
        break;
      default:
        printf("ERROR: UNKNOWN ARGUMENT\n");
      }

    if(port == NULL)
    {
        port == 1337;
    }  

    printf("PORT = %d\n", port);

    memset(&server, 0, sizeof server);
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    if(bind(sock,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }

    printf("Listening!\n");
    listen(sock, 3);

    client_sock = accept(sock, (struct sockaddr *)&client, (socklen_t*)&c);
    recv(client_sock , data, sizeof(data), 0);
    deserialize(data, &p);
    printPacket(&p);

    if(p.size == 0)
    {
        char command[100];
	memset(command, 0, sizeof(command));
        char command2[120];
        char *file_name = strrchr(p.URL, '/' ) + 1;
	printf("URL = %s\n", p.URL);
        sprintf(command, "%s %s", "wget", p.URL);
	printf("COMMAND = %s\n", command);
        system(command);
        printf("PACKET SIZE IS ONE\n");
        int fd = open(file_name, O_RDONLY);
        if (fd == -1) {
            fprintf(stderr, "unable to open '%s': %s\n", "file_name", strerror(errno));
            exit(1);
             }

    /* get the size of the file to be sent */
    fstat(fd, &stat_buf);
    printf("SIZE OF TRANSFER = %d", stat_buf.st_size);
    int size = (int) stat_buf.st_size;
    memcpy(buf, &size, sizeof(int));
    send(client_sock, buf , sizeof(buf) , 0);
    off_t offset = 0; 
    int rc = 0;
    rc = sendfile(client_sock, fd, &offset, stat_buf.st_size);
    if (rc == -1) {
      fprintf(stderr, "error from sendfile: %s\n", strerror(errno));
      exit(1);
    }
    if (rc != stat_buf.st_size) {
      fprintf(stderr, "incomplete transfer from sendfile: %d of %d bytes\n",
              rc,
              (int)stat_buf.st_size);
      exit(1);
    }
    memset(buf1, 0, sizeof(buf1));
    rc = recv(client_sock, buf1, sizeof(buf1), 0);
    //printf("file_name = %s!!!!!!", file_name);
    sprintf(command2, "%s %s", "rm", file_name);
    system(command2);
    }
    else 
    {
        int pos = randomInt(p.size);
        char current[100];
        char command2[120];
        strcpy(current, p.ss_list[pos]);
        deletePos(&p, pos);
        memset(data, 0, sizeof(data));
        serialize(&p, data);
        char *pch;
        char port[20], ip[20];
        pch = strtok (current," ");
        strcpy(ip, pch);
        pch = strtok (NULL," ");
        strcpy(port, pch);

        client1.sin_family = AF_INET;
        client1.sin_addr.s_addr = inet_addr(ip);
        client1.sin_port = htons(atoi(port));

    printf("Connecting to server...\n");
    if (connect(sock1 , (struct sockaddr *)&client1 , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
    }

    send(sock1 , data , sizeof(data) , 0);
    int size = 0;
    memset(buf, 0, sizeof(buf));
    recv(sock1, buf, sizeof(buf), 0);
    memcpy(&size, buf, sizeof(int));
    printf("SIZE = %d", size);

    char *file_name = strrchr(p.URL, '/' ) + 1;
    int read_size = 0;
    FILE *file;
    file = fopen(file_name, "wb");
    int length = 0;
    while(length < size)
    {
        memset(buf, 0, sizeof(buf));
        read_size = recv(sock1 , buf, sizeof(buf), 0);
        length += read_size;
        
        fwrite(&buf,1,read_size,file);
       // printf("LENGTH: %d\n", length);
    read_size = 0;
    }  
    printf("EXITED LOOP\n");
    fflush(file);

    FILE *file2;
    file2 = fopen(file_name, "rb");
    if (file2 == -1) {
	fprintf(stderr, "unable to open '%s': %s\n", file_name, strerror(errno));
        exit(1);
    }

    /* get the size of the file to be sent */
 


   
    printf("SIZE OF TRANSFER = %d", size);
    memset(buf1, 0, sizeof(buf1));
    int size1 = (int) stat_buf.st_size;
    memcpy(buf, &size, sizeof(int));
    send(client_sock, buf , sizeof(buf) , 0);
    

    off_t offset = 0; 
    int rc = 0;
    int fd2 = fileno(file2);
    fstat(fd2, &stat_buf);
    rc = sendfile(client_sock, fd2, &offset, stat_buf.st_size);
    if (rc == -1) {
      fprintf(stderr, "error from sendfile: %s\n", strerror(errno));
      exit(1);
    }
    if (rc != stat_buf.st_size) {
      fprintf(stderr, "incomplete transfer from sendfile: %d of %d bytes\n",
              rc,
              (int)stat_buf.st_size);
      exit(1);
    }

    printf("SENT %d BYTES\n", stat_buf.st_size);
    memset(buf1, 0, sizeof(buf1));

    //printf("file_name = %s!!!!!!", file_name);
    sprintf(command2, "%s %s", "rm", file_name);
    system(command2);


    }
//    fclose(file);
//    close(file2);
//    close(sock1);
//    close(client_sock);
    free_struct(&p);

    


}
