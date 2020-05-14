CC=gcc
SRC=tetris.c main.c
OBJ=$(SRC:.c=.o)
FLAGS=-Wall

fil:    $(OBJ) tetris.h
	$(CC) -o tetris $^
%.o:	%.c
	$(CC) -c $< $(CFLAGS)

clean:
	rm *.o *~ core
