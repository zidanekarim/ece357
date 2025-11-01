CFLAGS = -Wall -Wextra
DEBUGFLAGS = -g

all: clean myshell wordgen wordsearch pager
myshell: app.o myshell.o
	@gcc $(CFLAGS) -o myshell app.o myshell.o
debug: clean
	@gcc $(CFLAGS) $(DEBUGFLAGS) -c myshell.c
	@gcc $(CFLAGS) $(DEBUGFLAGS) -c app.c
	@gcc $(CFLAGS) $(DEBUGFLAGS) -o myshell app.o myshell.o


myshell.o: myshell.c myshell.h
	@gcc $(CFLAGS)  -c myshell.c
app.o: myshell.h
	@gcc $(CFLAGS) -c app.c
clean:
	@rm -rf *.o myshell                                                                                                                              
