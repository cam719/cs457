#include "awget.h"
#include "packet.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int randomInt(int mod)
{
    int seed = time(NULL);
    srand(seed);
    int val = 0;
    int i=0;
    val =rand() % mod;

    return val;
    
}
// Reads in file and initializes/allocates struct
int initialize_struct(struct packet *p, int usize, char *u, char textfile[100])
{
	FILE *file = fopen (textfile, "r" );
	char line [128];
	int len = 0;

	// Get length first, but make sure we opened file correctly
	if(file != NULL)
	{ 
		char *length = fgets(line, sizeof line, file);
		len = atoi(length);
	}
	else
  	{
  		printf("ERROR READING FILE\n");
  		return -1; 
  	}

	p->size = len;
	p->urlSize = usize;
	memset(p->URL, 0, sizeof(p->URL));
	strcpy(p->URL, u);

	p->ss_list = malloc(sizeof(char*) * len);
	
	// Initialize the list on the heap based on the length 
	for (int i = 0; i < len; i++)
		p->ss_list[i] = malloc(20);

	// Fill in data for list 
	for (int i = 0; i < len; i++)
	{
		fgets(line, sizeof line, file);
		strtok(line, "\n"); // Little trick to strip the newline character from input
		strncpy(p->ss_list[i], line, 20); // No chance of overflowing list buffer
	}
	
	fclose(file);
	return 0;
}

// Free structure. Self explanatory
void free_struct(struct packet *p)
{
	for (int i = 0; i < p->size; i++)
		free(p->ss_list[i]);

	free(p->ss_list);
}


// Delete the pos'th position out of the ss_list, then free the last element of the list
void deletePos(struct packet *p, int pos)
{
	for (int i = pos; i < p->size - 1 ; i++)
	{
		strncpy(p->ss_list[i], p->ss_list[i+1], 20);
	}
	p->size--;

	free(p->ss_list[p->size]);
}

// Debug print the packet list
void printPacket(struct packet *p)
{
	printf("====Printing Packet=======\n");
	printf("Size = %d\n",p->size);
	for(int i = 0; i < p->size; i++)
	{
		printf("%s\n", p->ss_list[i]);
	}
}

void printPacket1(struct packet *p)
{
	printf("====Printing Packet=======\n");
	printf("Size = %d\n",p->size);
	printf("UrlSize = %d\n",p->urlSize);
	printf("URL = %s\n",p->URL);
}
