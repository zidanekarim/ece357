CFLAGS = -Wall -Wextra
DEBUGFLAGS = -g

all: clean wordgen wordsearch # pager
wordgen: wordgen.o
	@gcc $(CFLAGS) -o wordgen wordgen.o
wordgen.o: wordgen.c wordgen.h
	@gcc $(CFLAGS) -c wordgen.c
wordsearch: wordsearch.o
	@gcc $(CFLAGS) -o wordsearch wordsearch.o
wordsearch.o: wordsearch.c wordgen.h
	@gcc $(CFLAGS) -c wordsearch.c
debug: clean
	@gcc $(CFLAGS) $(DEBUGFLAGS) -c myshell.c
	@gcc $(CFLAGS) $(DEBUGFLAGS) -c app.c
	@gcc $(CFLAGS) $(DEBUGFLAGS) -o myshell app.o myshell.o

clean:
	@rm -rf *.o wordgen                                                                                                                              
