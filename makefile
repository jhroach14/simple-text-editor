CC = g++	
DEBUG = -g	
CFLAGS = -Wall -std=c++0x -c $(DEBUG) -O0 -pedantic-errors	
LFLAGS = -Wall $(DEBUG)	

all: chmod cp cat mkdir ln ls pwd penv mv stat kill rm
rm: rm.o library.o
	$(CC) $(LFLAGS) -o rm rm.o library.o
rm.o: rm.cpp
	$(CC) $(CFLAGS) rm.cpp
kill: kill.o library.o
	$(CC) $(LFLAGS) -o kill kill.o library.o
kill.o: kill.cpp
	$(CC) $(CFLAGS) kill.cpp
stat: stat.o library.o
	$(CC) $(LFLAGS) -o stat stat.o library.o
stat.o: stat.cpp
	$(CC) $(CFLAGS) stat.cpp
ls: ls.o library.o
	$(CC) $(LFLAGS) -o ls ls.o library.o
ls.o: ls.cpp
	$(CC) $(CFLAGS) ls.cpp
ln: ln.o library.o
	$(CC) $(LFLAGS) -o ln ln.o library.o
ln.o: ln.cpp
	$(CC) $(CFLAGS) ln.cpp
pwd: pwd.o library.o
	$(CC) $(LFLAGS) -o pwd pwd.o library.o
pwd.o: pwd.cpp
	$(CC) $(CFLAGS) pwd.cpp
penv: penv.o library.o
	$(CC) $(LFLAGS) -o penv penv.o library.o
penv.o: penv.cpp
	$(CC) $(CFLAGS) penv.cpp
mv: mv.o library.o
	$(CC) $(LFLAGS) -o mv mv.o library.o
mv.o: mv.cpp
	$(CC) $(CFLAGS) mv.cpp
mkdir: mkdir.o library.o
	$(CC) $(LFLAGS) -o mkdir mkdir.o library.o
mkdir.o: mkdir.cpp
	$(CC) $(CFLAGS) mkdir.cpp
chmod: chmod.o library.o
	$(CC) $(LFLAGS) -o chmod chmod.o library.o
chmod.o: chmod.cpp
	$(CC) $(CFLAGS) chmod.cpp
cp: cp.o library.o
	$(CC) $(LFLAGS) -o cp cp.o library.o
cp.o: cp.cpp
	$(CC) $(CFLAGS) cp.cpp
cat: cat.o library.o
	$(CC) $(LFLAGS) -o cat cat.o library.o
cat.o: cat.cpp
	$(CC) $(CFLAGS) cat.cpp
library.o: library.cpp
	$(CC) $(CFLAGS) library.cpp

clean:	
	rm -rf chmod
	rm -rf cp
	rm -rf cat
	rm -rf mkdir
	rm -rf pwd
	rm -rf penv
	rm -rf mv
	rm -rf ls
	rm -rf ln
	rm -rf kill
	rm -rf stat
	rm -rf rm
	rm -rf *.o
