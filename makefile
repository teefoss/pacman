CC=clang
EXEC=pacman
LIBS=-L/usr/local/lib
INCLUDE=-I/usr/local/include
LINK=-lSDL2

SRCS=pacman.c

all:
	$(CC) $(SRCS) -o $(EXEC) $(LIBS) $(INCLUDE) $(LINK)

clean:
	rm -f *.obj $(EXEC)