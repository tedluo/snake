CC := gcc
SDLCONFIG := sdl-config
CFLAGS := --cflags
LFLAGS := --libs

SDL := `$(SDLCONFIG) $(CFLAGS) $(LFLAGS)`

all:snake.c 
	$(CC) $(SDL) snake.c -o snake

clean:
	rm -rf snake snake.o *~ .*.*.swp .*.*.swo
