
CFLAGS = -Wall -Wextra
DEBUGFLAGS = -g

all: clean tabstop
tabstop: app.o mystdio.o
	@gcc $(CFLAGS) -o tabstop app.o mystdio.o
debug: clean
	@gcc $(CFLAGS) $(DEBUGFLAGS) -c mystdio.c
	@gcc $(CFLAGS) $(DEBUGFLAGS) -c app.c
	@gcc $(CFLAGS) $(DEBUGFLAGS) -o tabstop app.o mystdio.o


mystdio.o: mystdio.c mystdio.h
	@gcc $(CFLAGS)  -c mystdio.c
app.o: mystdio.h
	@gcc $(CFLAGS) -c app.c

clean:
	@rm -rf *.o tabstop