#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <stdio.h>
#include "timeline.h"

#define RESX 800
#define RESY 600
#define BPP  32

void drawScreen(int frame)
{
    printf("%d\n", frame);
}

int main(int argc, char* argv[])
{
    SDL_Surface *screen;
    SDL_Event event;
    int quit = 0;
    int frame = 0;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) return 1;
    if (!(screen = SDL_SetVideoMode(RESX, RESY, BPP, SDL_HWSURFACE))) {
        SDL_Quit();
        return 1;
    }
    while (!quit) {
        drawScreen(frame++);
        while(SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = 1; break;
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym) {
                        case SDLK_ESCAPE: quit = 1; break;
                        default: break;
                    }
                    break;
            }
        }
    }
    SDL_Quit();
    return 0;
}
