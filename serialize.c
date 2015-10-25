#include "serialize.h"

void serialize(struct packet *packt, char *data)
{
	int *q = (int*)data;	
	*q = packt->size;		
	q++;	
	*q = packt->urlSize;	
	q++;
	char *p = (char *)q;
	int i = 0, j = 0;
	while(i < packt->urlSize)
	{
		*p = packt->URL[i];
		p++;
		i++;
	}

	i = 0;
	while(i < (packt->size))
	{
		while(j < 20)
		{
			*p = packt->ss_list[i][j];
			p++;
			j++;
		}
		i++;
		j = 0;
	}



}

void deserialize(char *data, struct packet *packt)
{
	int *q = (int*)data;	
	packt->size = *q;	
	q++;	
	packt->urlSize = *q;	
	q++;
	char *p = (char*)q;
	int i = 0, j = 0;
	memset(packt->URL, 0, sizeof(packt->URL));
	while(i < (packt->urlSize))
	{
		packt->URL[i] = *p;
		p++;
		i++;
	}

	packt->ss_list = malloc(sizeof(char*) * packt->size);
	for (int i = 0; i < packt->size; i++)
		packt->ss_list[i] = malloc(20);

	i = 0;
	j = 0;
	while(i < (packt->size))
	{
		while(j < 20)
		{
			packt->ss_list[i][j] = *p;
			p++;
			j++;
		}
		i++;
		j = 0;
	}
}

