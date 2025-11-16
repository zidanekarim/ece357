CFLAGS = -Wall -Wextra
DEBUGFLAGS = -g

all: clean myshell
runner: runner.o bgrep.o
	@gcc $(CFLAGS) -o bgrep runner.o bgrep.o
debug: clean
	@gcc $(CFLAGS) $(DEBUGFLAGS) -c runner.c
	@gcc $(CFLAGS) $(DEBUGFLAGS) -c bgrep.c
	@gcc $(CFLAGS) $(DEBUGFLAGS) -o bgrep runner.o bgrep.o

runner.o: runner.c 
	@gcc $(CFLAGS)  -c runner.c
bgrep.o: bgrep.c bgrep.h
	@gcc $(CFLAGS) -c bgrep.c
clean:
	@rm -rf *.o bgrep                                                                                                                              
