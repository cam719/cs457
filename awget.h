#ifndef AWGET_H_
#define AWGET_H_

struct packet {
	int size;
	int urlSize;
	char URL[100]; 
	char **ss_list;
};

#endif

// 0 [127.0.0.1 1337]
// 1 []