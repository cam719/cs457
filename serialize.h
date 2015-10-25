#ifndef SERIAL_H_
#define SERIAL_H_

#include "awget.h"

void serialize(struct packet *packt, char *data);
void deserialize(char *data, struct packet *packt);
#endif 
