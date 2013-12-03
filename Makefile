CC = gcc
CFLAG = -W -Wall -Werror

all:

ant_ai.o:	ant_ai.c ant_ai.h
	$(CC) $(CFLAG) -o ant_ai.o ant_ai.c

ant.o:	ant.c ant.h
	$(CC) $(CFLAG) -o ant.o ant.c

colony.o:	colony.c colony.h
	$(CC) $(CFLAG) -o colony.o colony.c

display.o:	display.c display.h
	$(CC) $(CFLAG) -o display.o display.c

events.o:
