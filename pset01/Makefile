all: tabstop
tabstop: app.o mystdio.o
	@gcc -o tabstop app.o mystdio.o

mystdio.o: mystdio.c mystdio.h
	@gcc -c mystdio.c
app.o: mystdio.h
	@gcc -c app.c

clean:
	@rm -rf *.o tabstop