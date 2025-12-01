CFLAGS = -Wall -Wextra
DEBUGFLAGS = -g

all: clean testspin
testspin: test_spin_lock.o spinlock.o tas.o
	@gcc $(CFLAGS) -o test_spin_lock test_spin_lock.o spinlock.o tas64.o
test_spin_lock.o: test_spin_lock.c spinlock.h tas.h
	@gcc $(CFLAGS) -c test_spin_lock.c

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
