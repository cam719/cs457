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

int main(int argc, char *argv[])
{
	struct sockaddr_in server , client, client1; 
	int client_sock, c;
	int sock = socket(AF_INET , SOCK_STREAM , 0);
    int sock1 = socket(AF_INET , SOCK_STREAM , 0);
	char data[200];
    char buf[500];
    char buf1[500];
	struct packet p;
    struct stat stat_buf;
    struct stat stat_buf2;

	memset(&server, 0, sizeof server);
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[1]));

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
        char command2[120];
        char *file_name = strrchr(p.URL, '/' ) + 1;
        sprintf(command, "%s %s", "wget", p.URL);
        system(command);
        printf("PACKET SIZE IS ONE\n");
        FILE *fd;
        fd = fopen(file_name, "r");
        if (fd == -1) {
            fprintf(stderr, "unable to open '%s': %s\n", "file_name", strerror(errno));
            exit(1);
             }
        printf("Relaying file...\n");
        printf("HERE");
        fseek(fd, 0L, SEEK_END);
        int size = ftell(fd);
        fseek(fd, 0L, SEEK_SET);

        char buf_final[size];
        memset(buf_final, 0, sizeof(buf_final));

        int writeSize = 0, length = 0;
        while(!feof(fd)){
            writeSize = fread(buf_final, 1, size, fd);
            length += writeSize; 
            printf("writeSize = %d", writeSize);
            int rv = write(client_sock, buf_final, writeSize);
            if(rv < 0) {
                fprintf(stderr, "Could not write data to socket\n");
                exit(1);
            }
        }
        memset(buf, 0, sizeof(buf));
        memcpy(buf, &length, sizeof(int));
       // send(client_sock , buf , sizeof(buf) , 0);
        printf("LENGTH = %d", length);
        fclose(fd);
        printf("  Goodbye!\n");

    /* get the size of the file to be sent */
    
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

    int length = 0;
    printf("Waiting ...\n");
    send(sock1 , data , sizeof(data) , 0);
    printf("Waiting1 ...\n");
    printf("Waiting3 ...\n");
    memset(buf, 0, sizeof(buf));
    printf("Waiting4 ...\n");
    recv(sock1, buf, sizeof(buf), 0);
    memcpy(&length, buf, sizeof(int));
    printf("Waiting5 ...\n");
   
    printf("LENGTH = %d", length);
    

    
    char buf_final[length];
    
    char *file_name = strrchr(p.URL, '/' ) + 1;
    int read_size = 0;
    FILE *fd;
    printf("Waiting7 ...\n");
    fd = fopen(file_name, "rb+");
    int rv = 0;
    while((rv = read(sock, &buf_final, length)) > 0)
    {
        fwrite(buf_final, 1, rv, fd);
    }
    printf("Waiting8 ...\n");
    rewind(fd);
    int writeSize = 0;
    while(!feof(fd)){
        writeSize = fread(buf_final, 1, length, fd);
        length += writeSize;
        int rv = write(client_sock, buf_final, writeSize);
        if(rv < 0) {
            fprintf(stderr, "Could not write data to socket\n");
            exit(1);
            }
        }
    printf("EXITED LOOP\n");
    fflush(fd);
    close(fd);
    close(sock1);
    close(client_sock);
    //printf("file_name = %s!!!!!!", file_name);
    sprintf(command2, "%s %s", "rm", file_name);
    system(command2);



    }
    free_struct(&p);

    


}
