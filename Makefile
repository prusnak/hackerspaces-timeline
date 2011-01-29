CFLAGS=$(shell sdl-config --cflags) -Wall
LDFLAGS=$(shell sdl-config --libs)

all: timeline

timeline: timeline.c
	gcc timeline.c -o timeline $(CFLAGS) $(LDFLAGS)

clean:
	rm -f timeline
