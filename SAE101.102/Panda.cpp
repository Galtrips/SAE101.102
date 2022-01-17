#include <SDL.h>
#include <SDL_ttf.h>
#include "config_sdl.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>
#include <SDL_image.h>

using namespace std;

const int LARGEUR = 1000; //largeur fenetre
const int HAUTEUR = 700;  //hauteur fenetre
const int nb_bambous = 8;

struct bambous
{
    int taille[nb_bambous] = { 0 };
    int croissance[nb_bambous] = { 1,9,3,10,5,6,7,8 };
};

bambous tab[nb_bambous];

void init(SDL_Renderer* rendu) {
    for (int i = 1; i < nb_bambous + 1; i++) {
        SDL_Rect rect;
        rect.x = 80 * i;
        rect.w = 10;
        rect.h = (tab[i - 1].croissance[i - 1]*6)+5;
        rect.y = HAUTEUR - rect.h;
        SDL_SetRenderDrawColor(rendu, 106, 164, 30, 255);
        SDL_RenderFillRect(rendu, &rect);
        SDL_SetRenderDrawColor(rendu, 0, 102, 0, 255);
        SDL_RenderDrawRect(rendu, &rect);
        SDL_RenderPresent(rendu);

        SDL_Rect rect2;
        rect2.x = (80 * i) - 4;
        rect2.w = 18;
        rect2.h = 3;
        rect2.y = HAUTEUR - ((tab[i - 1].croissance[i - 1] * 6) + 5);
        SDL_SetRenderDrawColor(rendu, 106, 164, 30, 255);
        SDL_RenderFillRect(rendu, &rect2);
        SDL_SetRenderDrawColor(rendu, 0, 102, 0, 255);
        SDL_RenderDrawRect(rendu, &rect2);
        SDL_RenderPresent(rendu);
    }
}

void init_ligne_max(SDL_Renderer* rendu) {
    SDL_SetRenderDrawColor(rendu, 255, 0, 0, 255);
    SDL_RenderDrawLine(rendu, 0, (65 * 2), LARGEUR, (65 * 2));
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

void texte(SDL_Renderer* rendu, TTF_Font* font) {
    SDL_Color rouge = { 255,0,0 }; //on définit une couleur de texte
    SDL_Rect positionTexte; //rectangle définissant le positionnement du texte, et sa taille

    //on place le texte au point (100,100)
    positionTexte.x = LARGEUR - 50;
    positionTexte.y = (65 * 2) - 30;
    //on crée une texture à partir du texte, de sa couleur, et de la fonte
    SDL_Texture* texture = loadText(rendu, "Max", rouge, font);
    //on maj le rectangle couvrant cette texture
    SDL_QueryTexture(texture, NULL, NULL, &positionTexte.w, &positionTexte.h);
    //si on veut modifier le cadre du texte
    positionTexte.w *= 1;
    positionTexte.h *= 1;
    //on copie la texture dans le rendu
    SDL_RenderCopy(rendu, texture, NULL, &positionTexte);
    //on met à jour le rendu
    SDL_RenderPresent(rendu);
    //on détruit la texture
    SDL_DestroyTexture(texture);
}

void SDL_SetWindowIcon(SDL_Window* window, SDL_Surface* icon);

int main(int argn, char* argv[]) {

    //ouverture de la SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        cout << "Echec à l’ouverture de SDL";
        return 1;
    }


    //on crée la fenêtre
    SDL_Window* win = SDL_CreateWindow("PandaCut",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        LARGEUR,
        HAUTEUR,
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer* rendu = SDL_CreateRenderer(
        win,  //nom de la fenêtre
        -1, //par défaut
        SDL_RENDERER_ACCELERATED); //utilisation du GPU, valeur recommandée

    if (win == NULL)
        cout << "erreur ouverture fenetre";

    SDL_Surface* icon = IMG_Load("logo.png");
    if (!icon)
    {
        printf("Erreur de chargement de l'image : %s", SDL_GetError());
        return -1;
    }
    SDL_SetWindowIcon(win, icon);

    TTF_Init();
    TTF_Font* font = TTF_OpenFont("C:\\Windows\\Fonts\\calibri.ttf", 25);

    SDL_SetRenderDrawColor(rendu, 255, 255, 255, 255);
    SDL_RenderClear(rendu);
    SDL_RenderPresent(rendu);
    init(rendu);
    texte(rendu, font);
    init_ligne_max(rendu);
    SDL_RenderPresent(rendu);

    // on importe une image
    SDL_Surface* image = IMG_Load("panda.png");
    if (!image)
    {
        cout << "Erreur de chargement de l'image ";
        return -1;
    }
    SDL_Texture* pTextureImage = SDL_CreateTextureFromSurface(rendu, image);
    SDL_FreeSurface(image);

    /*SDL_Rect posIng;
    posIng.x = 500;
    posIng.y = 50;*/
    SDL_Rect src{ 0, 0, 0, 0 };
    SDL_Rect dst{ 650, HAUTEUR-50, 50, 50 };

    /*SDL_QueryTexture(pTextureImage, nullptr, nullptr, &posIng.w, &posIng.h);*/
    SDL_QueryTexture(pTextureImage, nullptr, nullptr, &src.w, &src.h);
    //SDL_RenderCopy(rendu, pTextureImage, nullptr, &posIng); // Affiche ma texture sur touts l'écran
    SDL_RenderCopy(rendu, pTextureImage, &src, &dst);

    SDL_RenderPresent(rendu);


    bool continuer = true;
    int fullscreen = 0;
    SDL_Event event;

    while (continuer) {

        SDL_WaitEvent(&event);

        switch (event.type) {

        case SDL_QUIT:
            continuer = false;
            break;
        }
    }

    TTF_CloseFont(font); //on ferme la font
    TTF_Quit(); //on quitte la TTF

    SDL_DestroyRenderer(rendu);
    SDL_DestroyWindow(win);

    //fermeture
    SDL_Quit();

    return 0;
}