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

    if (win == NULL)
        cout << "erreur ouverture fenetre";




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

    SDL_DestroyWindow(win);

    //fermeture
    SDL_Quit();

    return 0;
}