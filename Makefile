CC = gcc
CFLAGS = -g -Wall

all: vs_send vs_recv rudp_sender rudp_recver

vs_send: vs_send.o rudp.o event.o
	$(CC) $(CFLAGS) $^ -o $@

vs_recv: vs_recv.o rudp.o event.o
	$(CC) $(CFLAGS) $^ -o $@

rudp_sender: rudp_sender.o rudp.o event.o
	$(CC) $(CFLAGS) $^ -o $@

rudp_recver: rudp_recver.o rudp.o event.o
	$(CC) $(CFLAGS) $^ -o $@

vs_send.o vs_recv.o rudp.o rudp_sender.o rudp_recver.o: rudp.h rudp_api.h event.h

event.c: event.h

rudp.tar: vs_send.c vs_recv.c vsftp.h Makefile rudp_api.h rudp.h event.h \
	event.c rudp.c
	tar cf rudp.tar $^

clean:
	/bin/rm -f vs_send vs_recv *.o rudp.tar
