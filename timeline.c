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

SDL_Surface *screen, *map, *hackerspaces, *text, *point3, *point7, *grid, *ending;
TTF_Font *font;
int quit = 0;
int frame = -1000;
int idx = -1;
int cnt = 0;
SDL_Color cWhite = {255, 255, 255};
SDL_Color cBlack = {0, 0, 0};
SDL_Rect rDate = {8, 368, 0, 0};
SDL_Rect rName = {300, 368, 0, 0};
SDL_Rect rCity = {300, 388, 0, 0};
SDL_Rect rCountry = {300, 408, 0, 0};
SDL_Rect rTotal = {300, 454, 0, 0};
SDL_Rect rChart = {8, 392, 280, 80};

void drawScreen()
{
    int x, y;
    time_t date = frame*86400/2 + 24*365*86400;
    char datestr[32], totalstr[32];
    strftime(datestr, sizeof(datestr), " %d %b %Y", gmtime(&date));

    if (frame == 0) {
        SDL_FillRect(screen, NULL, 0);
        SDL_BlitSurface(map, 0, screen, 0);
    }

    SDL_Rect rect = {300, 368, 420, 120};
    SDL_FillRect(screen, &rect, 0);

    TTF_SizeText(font, datestr, &x, NULL);
    rDate.x = 160 - x;
    text = TTF_RenderText_Shaded(font, datestr, cWhite, cBlack);
    SDL_BlitSurface(text, NULL, screen, &rDate);
    SDL_FreeSurface(text);

    snprintf(totalstr, sizeof(totalstr), "Total hackerspaces: %d", cnt);
    text = TTF_RenderText_Shaded(font, totalstr, cWhite, cBlack);
    SDL_BlitSurface(text, NULL, screen, &rTotal);
    SDL_FreeSurface(text);

    if (frame == 0) {
        SDL_BlitSurface(grid, NULL, screen, &rChart);
    } else
    if (frame % 25 == 1) {
        SDL_Rect rect = {8+frame/50-1, 392+80-1-cnt/5, 3, 3};
        SDL_BlitSurface(point3, NULL, screen, &rect);
    }

    struct hsdata *hs = NULL;
    cnt = 0;
    while (timeline[cnt].date && timeline[cnt].date < date) {
        ++cnt;
    }
    if (timeline[cnt].name) {
        hs = timeline+cnt;
    } else {
        hs = NULL;
        frame = 1000000;
    }

    if (hs) {

        if (hs->lon != 0.0 && hs->lat != 0.0) {
            x = 360 + hs->lon*2;
            y = 180 - hs->lat*2;
            SDL_Rect rect = {x-1, y-1, 3, 3};
            SDL_BlitSurface(point3, NULL, screen, &rect);
        }

        text = TTF_RenderText_Shaded(font, hs->name, cWhite, cBlack);
        SDL_BlitSurface(text, NULL, screen, &rName);
        SDL_FreeSurface(text);

        text = TTF_RenderText_Shaded(font, hs->city, cWhite, cBlack);
        SDL_BlitSurface(text, NULL, screen, &rCity);
        SDL_FreeSurface(text);

        text = TTF_RenderText_Shaded(font, hs->country, cWhite, cBlack);
        SDL_BlitSurface(text, NULL, screen, &rCountry);
        SDL_FreeSurface(text);
    }

    SDL_Flip(screen);
    ++frame;
}

void drawIntro()
{
    SDL_BlitSurface(hackerspaces, 0, screen, 0);
    SDL_Flip(screen);
    ++frame;
}

void drawOutro()
{
    SDL_BlitSurface(ending, 0, screen, 0);
    SDL_Flip(screen);
    if (++frame > 1001000) {
        quit = 1;
    }
}


int main(int argc, char* argv[])
{
    SDL_Event event;

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
    point3 = IMG_Load("point3.png");
    point7 = IMG_Load("point7.png");
    grid = IMG_Load("grid.png");
    ending = IMG_Load("ending.png");
    if (!map || !hackerspaces || !point3 || !point7 || !grid || !ending) {
        SDL_Quit();
        return 5;
    }
    while (!quit) {
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
        if (frame < 0) {
            drawIntro();
        } else if (frame < 1000000) {
            drawScreen();
        } else {
            drawOutro();
        }
        SDL_Delay(1);
    }
    SDL_FreeSurface(map);
    SDL_FreeSurface(hackerspaces);
    SDL_FreeSurface(point3);
    SDL_FreeSurface(point7);
    SDL_FreeSurface(grid);
    SDL_FreeSurface(ending);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
