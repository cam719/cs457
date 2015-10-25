#ifndef PACKET_H_
#define PACKET_H_

int randomInt(int mod);
int initialize_struct(struct packet *p, int usize, char *u, char textfile[100]);
void free_struct(struct packet *p);
void deletePos(struct packet *p, int pos);
void printPacket(struct packet *p);
void printPacket1(struct packet *p);

#endif

