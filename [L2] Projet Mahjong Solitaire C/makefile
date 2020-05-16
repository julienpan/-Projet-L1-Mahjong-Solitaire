CC=gcc
SRC=mahjong.c main.c
OBJ=$(SRC:.c=.o)
FLAGS=-Wall

fil:    $(OBJ) m.h
				$(CC) -o $@ $(OBJ)
%.o:	%.c
				$(CC) -c $< $(CFLAGS)

clean:
	rm *.o *~ core
