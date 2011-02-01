#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include <stdio.h>
#include <time.h>
#include "timeline.h"

#define RESX 720
#define RESY 480
#define BPP  32

SDL_Surface *screen, *map, *hackerspaces, *text;
TTF_Font *font;
int frame = 0;
int idx = -1;
SDL_Color cWhite = {255, 255, 255};
SDL_Color cBlack = {0, 0, 0};
SDL_Rect rDate = {8, 368, 0, 0};
SDL_Rect rName = {300, 368, 0, 0};
SDL_Rect rCity = {300, 388, 0, 0};
SDL_Rect rCountry = {300, 408, 0, 0};

void drawScreen()
{
    int x, y;
    time_t date = frame*86400*2;
    char datestr[32];
    strftime(datestr, sizeof(datestr), "%d %b %Y", gmtime(&date));

    SDL_BlitSurface(map, 0, screen, 0);

    struct hsdata hs = timeline[0];

    x = 360 + hs.lon*2;
    y = 180 - hs.lat*2;
    SDL_Rect rect = {x, y, 3, 3};
    SDL_FillRect(screen, &rect, 0xFF0000);


    text = TTF_RenderText_Shaded(font, datestr, cWhite, cBlack);
    SDL_BlitSurface(text, NULL, screen, &rDate);
    SDL_FreeSurface(text);

    text = TTF_RenderText_Shaded(font, hs.name, cWhite, cBlack);
    SDL_BlitSurface(text, NULL, screen, &rName);
    SDL_FreeSurface(text);

    text = TTF_RenderText_Shaded(font, hs.city, cWhite, cBlack);
    SDL_BlitSurface(text, NULL, screen, &rCity);
    SDL_FreeSurface(text);

    text = TTF_RenderText_Shaded(font, hs.country, cWhite, cBlack);
    SDL_BlitSurface(text, NULL, screen, &rCountry);
    SDL_FreeSurface(text);

    printf("%d %s \n", frame, datestr);
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
        return 2;
    }
    if (TTF_Init() == -1) {
        return 3;
    }
    font = TTF_OpenFont("visitor.ttf", 24);
    if (!font) {
        return 4;
    }
    map = IMG_Load("map.png");
    hackerspaces = IMG_Load("hackerspaces.png");
    if (!map || !hackerspaces) {
        SDL_Quit();
        return 5;
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
    SDL_FreeSurface(hackerspaces);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
