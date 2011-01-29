#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <stdio.h>
#include "timeline.h"

#define RESX 720
#define RESY 480
#define BPP  32

SDL_Surface *screen, *map;
int frame = 0;

void drawScreen()
{
    int x, y;
    int idx = frame/100;

    SDL_BlitSurface(map,0,screen,0);

    struct hsdata hs = timeline[idx];

    x = 360 + hs.lon*2;
    y = 180 - hs.lat*2;
    SDL_Rect rect = {x,y,3,3};
    SDL_FillRect(screen, &rect, 0xFF0000);


    printf("%d %s, %s, %s\n", idx, hs.name, hs.city, hs.country);
    SDL_Flip(screen);
    ++frame;
}

int main(int argc, char* argv[])
{
    SDL_Event event;
    int quit = 0;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) return 1;
    if (!(screen = SDL_SetVideoMode(RESX, RESY, BPP, SDL_HWSURFACE))) {
        SDL_Quit();
        return 1;
    }
    map = IMG_Load("map.jpg");
    if (!map) {
        SDL_Quit();
        return 2;
    }
    while (!quit) {
        drawScreen();
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
    SDL_FreeSurface(map);
    SDL_Quit();
    return 0;
}
