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
        int fd = fopen(file_name, "w");
        int length = 0;
        while(length < size)
        {
            memset(buf, 0, sizeof(buf));
            read_size = recv(sock1 , buf, sizeof(buf), 0);
            length += read_size;
            read_size = 0;
            fwrite(&buf,1,sizeof(buf),fd);
            printf("LENGTH: %d\n", length);
            
        }  
        fflush(fd);
        close(fd);
        fd = 0;
        fd = open(file_name, "r");
            if (fd == -1) {
                fprintf(stderr, "unable to open '%s': %s\n", "file_name", strerror(errno));
                exit(1);
                 }

        /* get the size of the file to be sent */
        memset(&stat_buf, 0, sizeof(stat_buf));
        fstat(fd, &stat_buf);


        send(sock1, buf , sizeof(buf) , 0);

        
        stat_buf.st_size = length;    
        printf("SIZE OF TRANSFER = %d", stat_buf.st_size);
        memset(buf1, 0, sizeof(buf1));
        int size1 = (int) stat_buf.st_size;
        memcpy(buf, &size1, sizeof(int));
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
        rc = recv(sock1, buf1, sizeof(buf1), 0);
        close(fd);
        close(sock1);
        close(client_sock);
        //printf("file_name = %s!!!!!!", file_name);
        sprintf(command2, "%s %s", "rm", file_name);
        system(command2);



    }