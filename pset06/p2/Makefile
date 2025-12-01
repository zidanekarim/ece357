CFLAGS = -Wall -Wextra
DEBUGFLAGS = -g

all: clean runner
runner: runner.o bgrep.o
	@gcc $(CFLAGS) -o bgrep runner.o bgrep.o
debug: clean
	@gcc $(CFLAGS) $(DEBUGFLAGS) -c runner.c
	@gcc $(CFLAGS) $(DEBUGFLAGS) -c bgrep.c
	@gcc $(CFLAGS) $(DEBUGFLAGS) -o bgrep runner.o bgrep.o

spinlock.o: spinlock.h spinlock.c tas.o
	@gcc $(CFLAGS) -c spinlock.c

tas.o: tas.h tas64.S
	@gcc $(CFLAGS) -c tas64.S
clean:
	@rm -rf *.o bgrep                                                                                                                              
