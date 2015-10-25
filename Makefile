all: awget ss
awget: awget.o packet.o serialize.o serialize.c packet.c ss.c
	gcc -o awget awget.o packet.o serialize.o
ss: ss.o packet.o serialize.o serialize.c packet.c 
	gcc -pthread -o ss ss.o packet.o serialize.o
packet.o: awget.h awget.c packet.c packet.h
	gcc -c -std=c99 packet.c
serialize.o: awget.h awget.c packet.c packet.h
	gcc -c -std=c99 serialize.c
awget.o: awget.h awget.c packet.c packet.h serialize.c packet.c ss.c
	gcc -c -std=c99 awget.c
ss.o: awget.h awget.c packet.c packet.h
	gcc -c -pthread -std=c99 ss.c
clean:
	rm -rf *o exec8
tar:  
	tar -cvf CurrentSubmission.tar *.c *.h Makefile chaingang.txt
