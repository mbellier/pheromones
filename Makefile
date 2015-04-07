CC = gcc
CFLAG = -W -Wall -Werror 
# -I/usr/local/include/SDL
LFLAG= -I/usr/local/include/SDL 

main: ant_ai.o ant.o colony.o display.o events.o fifo.o map.o maths.o matrix.o pheromone.o point2d.o random.o rgb_color.o
	$(CC) $(CFLAG) -o main main.c *.o $(LFLAG) -L/usr/local/lib -lSDL

clean:
	@rm -f *.o main; echo Cleaned



ant_ai.o:	ant_ai.c ant_ai.h
	$(CC) $(CFLAG) -o ant_ai.o -c ant_ai.c $(LFLAG)

ant.o:	ant.c ant.h
	$(CC) $(CFLAG) -o ant.o -c ant.c  $(LFLAG)

colony.o:	colony.c colony.h
	$(CC) $(CFLAG) -o colony.o -c colony.c $(LFLAG)

display.o:	display.c display.h
	$(CC) $(CFLAG) -o display.o -c display.c $(LFLAG)

events.o: 	events.c events.h
	$(CC) $(CFLAGS) -o events.o -c events.c $(LFLAG)

fifo.o: 	fifo.c fifo.h
	$(CC) $(CFLAGS) -o fifo.o -c fifo.c $(LFLAG)

map.o: 		map.c map.h
	$(CC) $(CFLAGS) -o map.o -c map.c $(LFLAG)

maths.o: 	maths.c maths.h
	$(CC) $(CFLAGS) -o maths.o -c maths.c $(LFLAG)

matrix.o:	 matrix.c matrix.h
	$(CC) $(CFLAGS) -o matrix.o -c matrix.c $(LFLAG)

misc.o:	 misc.c misc.h
	$(CC) $(CFLAGS) -o misc.o -c misc.c $(LFLAG)

pheromone.o: 	pheromone.c pheromone.h
	$(CC) $(CFLAGS) -o pheromone.o -c pheromone.c $(LFLAG)

point2d.o: 	point2d.c point2d.h
	$(CC) $(CFLAGS) -o point2d.o -c point2d.c $(LFLAG)

random.o: random.c random.h
	$(CC) $(CFLAGS) -o random.o -c random.c $(LFLAG)

rgb_color.o: rgb_color.c rgb_color.h
	$(CC) $(CFLAGS) -o rgb_color.o -c rgb_color.c $(LFLAG)

test.o: test.c test.h
	$(CC) $(CFLAGS) -o test.o -c test.c $(LFLAG)
