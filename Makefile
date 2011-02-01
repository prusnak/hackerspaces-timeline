CFLAGS=$(shell sdl-config --cflags) -Wall
LDFLAGS=$(shell sdl-config --libs) -lSDL_image -lSDL_mixer -lSDL_ttf

all: timeline

timeline: timeline.c
	gcc timeline.c -o timeline $(CFLAGS) $(LDFLAGS)

clean:
	rm -f timeline
