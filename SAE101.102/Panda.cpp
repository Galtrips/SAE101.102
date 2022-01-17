#include <SDL.h>
#include <SDL_ttf.h>
#include "config_sdl.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>

using namespace std;

const int LARGEUR = 1000; //largeur fenetre
const int HAUTEUR = 500;  //hauteur fenetre
const int nb_bambous = 6;

struct bambous
{
    int taille[nb_bambous] = { 0 };
    int croissance[nb_bambous] = { 10,3,4,2,6,1 };
};

bambous tab[nb_bambous];

void init(SDL_Renderer* rendu) {
    for (int i = 1; i < nb_bambous + 1; i++) {
        SDL_Rect rect;
        rect.x = 80 * i;
        rect.w = 10;
        rect.h = 30;
        rect.y = HAUTEUR - rect.h;
        SDL_SetRenderDrawColor(rendu, 106, 164, 30, 255);
        SDL_RenderFillRect(rendu, &rect);
        SDL_SetRenderDrawColor(rendu, 0, 102, 0, 255);
        SDL_RenderDrawRect(rendu, &rect);
        SDL_RenderPresent(rendu);
    }
}

void init_croissances(bambous tab[]) {
    for (int i = 0; i < nb_bambous; i++) {
        *tab[i].croissance = *tab[i].croissance * 1000;
    }
}

void croissance(SDL_Renderer* rendu, bambous tab[]) {
    SDL_Delay(*tab[1].croissance);
    SDL_Rect rect;
    rect.x = 80;
    rect.w = 10;
    rect.h = 30;
    rect.y = HAUTEUR - (rect.h * 2);
    SDL_SetRenderDrawColor(rendu, 106, 164, 30, 255);
    SDL_RenderFillRect(rendu, &rect);
    SDL_SetRenderDrawColor(rendu, 0, 102, 0, 255);
    SDL_RenderDrawRect(rendu, &rect);
    SDL_RenderPresent(rendu);
}

int main(int argn, char* argv[]) {

    //ouverture de la SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        cout << "Echec à l’ouverture de SDL";
        return 1;
    }


    //on crée la fenêtre
    SDL_Window* win = SDL_CreateWindow("Panda",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        LARGEUR,
        HAUTEUR,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );

    SDL_Renderer* rendu = SDL_CreateRenderer(
        win,  //nom de la fenêtre
        -1, //par défaut
        SDL_RENDERER_ACCELERATED); //utilisation du GPU, valeur recommandée

    if (win == NULL)
        cout << "erreur ouverture fenetre";

    SDL_SetRenderDrawColor(rendu, 255, 255, 255, 255);
    SDL_RenderClear(rendu);
    SDL_RenderPresent(rendu);
    init(rendu);
    SDL_RenderPresent(rendu);
    init_croissances(tab);

    bool continuer = true;
    int fullscreen = 0;
    SDL_Event event;

    while (continuer) {

        SDL_WaitEvent(&event);

        switch (event.type) {

        case SDL_QUIT:
            continuer = false;
            break;

        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_F11) {

                if (fullscreen == 0)
                {
                    fullscreen = 1;
                    SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN);
                }
                else if (fullscreen == 1)
                {
                    fullscreen = 0;
                    SDL_SetWindowFullscreen(win, 0);
                }

            }
            break;
        }
    }

    SDL_DestroyRenderer(rendu);
    SDL_DestroyWindow(win);

    //fermeture
    SDL_Quit();

    return 0;
}