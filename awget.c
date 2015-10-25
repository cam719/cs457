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
#include <getopt.h>



int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        printf("ERROR: NO URL GIVEN\n");
        return 1;
    }
    char textfile[100];
    char URL[100]; 
    int c;
   while (optind < argc) {
  if ((c = getopt(argc, argv, "c:")) != -1) 
  {
    // Option argument
    switch (c) {
        case 'c': {
            strcpy(textfile, optarg);
        }
        default:
            break;
    }
	}
    else if (argc != 2)
    {
        // Regular argument
        strcpy(URL, optarg);
        optind++;  // Skip to the next argument
    }
    	optind++;
    }

    if(strlen(textfile) == 0 || argc == 2)
    {
    	strcpy(textfile, "chaingang.txt\0");
    }
	struct sockaddr_in server;
	
	strcpy(URL, argv[1]);
	printf("URL  = %s",URL);
	struct packet p;
	struct packet p2;
	char data[500];
    char buf[500];
    int size = 0;
	int sock = socket(AF_INET , SOCK_STREAM , 0);
	int ret = initialize_struct(&p, strlen(URL), URL, textfile);
	if(ret == -1)
	{
		return -1;
	}

    printPacket(&p);
   
    int pos = randomInt(p.size);
    char current[100];
    strcpy(current, p.ss_list[pos]);
    char *pch;
    char port[20], ip[20];
    deletePos(&p, pos);
    serialize(&p, data);
    deserialize(data,&p2);

   
    //Split line up into ip and port variables
    pch = strtok (current," ");
    strcpy(ip, pch);
    pch = strtok (NULL," ");
    strcpy(port, pch);

	server.sin_family = AF_INET;
   	server.sin_addr.s_addr = inet_addr(ip);
    server.sin_port = htons(atoi(port));

    printf("Connecting to server...\n");
	if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
    }

    send(sock , data , sizeof(data) , 0);


    //char *URL = "http://www.host.com/r/web/www/pic01.jpg";
    char *file_name;
    file_name = strrchr( URL, '/' ) + 1;
    printf("URL = %s\n", URL);
    printf("FILE NAME = %s\n", file_name);
    memset(buf, 0, sizeof(buf));
    recv(sock, buf, sizeof(buf), 0);
    memcpy(&size, buf, sizeof(int));
    printf("SIZE = %d\n", size);
    int read_size = 0;
    FILE *fd = fopen(file_name, "wb");
    int length = 0;
    char buffer[size];
    char recv_buf[500];
    int rv = 0;
    while((rv = read(sock, buffer, size)) > 0){
        fwrite(buffer, 1, rv, fd);
    }

    fwrite(buffer,1,length,fd);

    send(sock, buf , sizeof(buf) , 0);
    close(sock);
    fclose(fd);
    free_struct(&p);
    free_struct(&p2);
}
