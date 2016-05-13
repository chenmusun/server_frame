CC=g++
CFLAGS= -std=c++11 -w
objects = libevent_server.o main.o worker_thread.o HttpImp.o
all:$(objects) nedmalloc.o
	g++   -o libevent_server $(objects) nedmalloc.o  -std=c++11 -levent -pthread
$(objects): %.o: %.cpp 
	$(CC) -c -g $(CFLAGS) $< -o $@
nedmalloc.o:
	g++ -c -g -std=c++11 -w nedmalloc.c -o nedmalloc.o
clean:
	rm -f *.o libevent_server

dist: libeventserver-1.0.tar.gz
libeventserver-1.0.tar.gz: libevent_server 
	rm -rf libeventserver-1.0
	mkdir libeventserver-1.0
	cp *.cpp *.h *.conf *.xml makefile libeventserver-1.0
	tar zcvf $@ libeventserver-1.0
