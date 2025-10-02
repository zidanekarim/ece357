CFLAGS = -Wall -Wextra
DEBUGFLAGS = -g

all: clean simplefind
simplefind: app.o mystdio.o
	@gcc $(CFLAGS) -o simplefind app.o simplefind.o
debug: clean
	@gcc $(CFLAGS) $(DEBUGFLAGS) -c simplefind.c
	@gcc $(CFLAGS) $(DEBUGFLAGS) -c app.c
	@gcc $(CFLAGS) $(DEBUGFLAGS) -o simplefind app.o simplefind.o


simplefind.o: simplefind.c simplefind.h
	@gcc $(CFLAGS)  -c simplefind.c
app.o: mystdio.h
	@gcc $(CFLAGS) -c app.c
clean:
	@rm -rf *.o simplefind                                                                                                                              
