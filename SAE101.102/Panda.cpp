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
int coPanda[nb_bambous];

struct bambous
{
    int taille;
    int croissance;
};

bambous tab[nb_bambous];

void init_croissance() {

    tab[0].croissance = 1;
    tab[1].croissance = 9;
    tab[2].croissance = 3;
    tab[3].croissance = 10;
    tab[4].croissance = 5;
    tab[5].croissance = 6;
    tab[6].croissance = 7;
    tab[7].croissance = 8;

}

void init(SDL_Renderer* rendu) {
    for (int i = 1; i < nb_bambous + 1; i++) {
        SDL_Rect rect;
        rect.x = 80 * i;
        rect.w = 10;
        rect.h = (tab[i - 1].croissance*6)+5;
        rect.y = HAUTEUR - rect.h;
        SDL_SetRenderDrawColor(rendu, 106, 164, 30, 255);
        SDL_RenderFillRect(rendu, &rect);
        SDL_SetRenderDrawColor(rendu, 0, 102, 0, 255);
        SDL_RenderDrawRect(rendu, &rect);
        SDL_RenderPresent(rendu);

        tab[i-1].taille = rect.h;

        SDL_Rect rect2;
        rect2.x = (80 * i) - 4;
        rect2.w = 18;
        rect2.h = 3;
        rect2.y = HAUTEUR - ((tab[i - 1].croissance * 6) + 5);
        SDL_SetRenderDrawColor(rendu, 106, 164, 30, 255);
        SDL_RenderFillRect(rendu, &rect2);
        SDL_SetRenderDrawColor(rendu, 0, 102, 0, 255);
        SDL_RenderDrawRect(rendu, &rect2);
        SDL_RenderPresent(rendu);
    }
}

void ajout (SDL_Renderer* rendu) {
    for (int i = 1; i < nb_bambous + 1; i++) {
        if (tab[i - 1].taille + ((tab[i - 1].croissance * 6) + 5) < HAUTEUR - (65 * 2)) {
            SDL_Rect rect;
            rect.x = 80 * i;
            rect.w = 10;
            rect.h = (tab[i - 1].croissance * 6) + 5;
            rect.y = HAUTEUR - tab[i - 1].taille - ((tab[i - 1].croissance * 6) + 5);
            SDL_SetRenderDrawColor(rendu, 106, 164, 30, 255);
            SDL_RenderFillRect(rendu, &rect);
            SDL_SetRenderDrawColor(rendu, 0, 102, 0, 255);
            SDL_RenderDrawRect(rendu, &rect);
            SDL_RenderPresent(rendu);

            SDL_Rect rect2;
            rect2.x = (80 * i) - 4;
            rect2.w = 18;
            rect2.h = 3;
            rect2.y = HAUTEUR - ((tab[i - 1].croissance * 6) + 5 + tab[i - 1].taille);
            SDL_SetRenderDrawColor(rendu, 106, 164, 30, 255);
            SDL_RenderFillRect(rendu, &rect2);
            SDL_SetRenderDrawColor(rendu, 0, 102, 0, 255);
            SDL_RenderDrawRect(rendu, &rect2);
            SDL_RenderPresent(rendu);

            tab[i - 1].taille += rect.h;
        }
    }
}

void init_ligne_max(SDL_Renderer* rendu) {
    SDL_SetRenderDrawColor(rendu, 255, 0, 0, 255);
    SDL_RenderDrawLine(rendu, 0, (65 * 2), LARGEUR, (65 * 2));
}

void texte(SDL_Renderer* rendu, TTF_Font* font) {
    SDL_Color rouge = { 255,0,0 }; //on définit une couleur de texte
    SDL_Rect positionTexte; //rectangle définissant le positionnement du texte, et sa taille

    //on place le texte au point (100,100)
    positionTexte.x = LARGEUR - 28;
    positionTexte.y = (65 * 2) - 15;
    //on crée une texture à partir du texte, de sa couleur, et de la fonte
    SDL_Texture* texture = loadText(rendu, "Max", rouge, font);
    //on maj le rectangle couvrant cette texture
    SDL_QueryTexture(texture, NULL, NULL, &positionTexte.w, &positionTexte.h);
    //si on veut modifier le cadre du texte
    positionTexte.w *= 0.6;
    positionTexte.h *= 0.6;
    //on copie la texture dans le rendu
    SDL_RenderCopy(rendu, texture, NULL, &positionTexte);
    //on met à jour le rendu
    SDL_RenderPresent(rendu);
    //on détruit la texture
    SDL_DestroyTexture(texture);
}

void coordonéesPanda() {
    for (int i = 1; i < nb_bambous; i++) {
        coPanda[i-1] = 80 * i;
    }
}   

void deplacement(SDL_Renderer* rendu, SDL_Rect dst) {
    dst.x = coPanda[0]; 
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
    init_croissance();
    init(rendu);
    texte(rendu, font);
    init_ligne_max(rendu);
    SDL_RenderPresent(rendu);

    // on importe une image de sable
    SDL_Surface* sable = IMG_Load("sable.png");
    if (!sable)
    {
        cout << "Erreur de chargement de l'image ";
        return -1;
    }
    SDL_Texture* pTextureImagesable = SDL_CreateTextureFromSurface(rendu, sable);
    SDL_FreeSurface(sable);

    SDL_Rect src3{ 0, 0, 0, 0 };
    SDL_Rect dst3{ 0, HAUTEUR - 170,1000 , 270 };

    SDL_QueryTexture(pTextureImagesable, nullptr, nullptr, &src3.w, &src3.h);
    SDL_RenderCopy(rendu, pTextureImagesable, &src3, &dst3);
    SDL_RenderPresent(rendu);


    init(rendu);
    texte(rendu, font);
    init_ligne_max(rendu);
    SDL_RenderPresent(rendu);



    
    
    // on importe une image de ciel
    SDL_Surface* ciel = IMG_Load("ciel1.png");
    if (!ciel)
    {
        cout << "Erreur de chargement de l'image ";
        return -1;
    }
    SDL_Texture* pTextureImageciel = SDL_CreateTextureFromSurface(rendu, ciel);
    SDL_FreeSurface(ciel);

    /*SDL_Rect posIng;
    posIng.x = 500;
    posIng.y = 50;*/
    SDL_Rect src1{ 0, 0, 0, 0 };
    SDL_Rect dst1{ 0,0, 1000, 400 };

    /*SDL_QueryTexture(pTextureImage, nullptr, nullptr, &posIng.w, &posIng.h);*/
    SDL_QueryTexture(pTextureImageciel, nullptr, nullptr, &src1.w, &src1.h);
    //SDL_RenderCopy(rendu, pTextureImage, nullptr, &posIng); // Affiche ma texture sur touts l'écran
    SDL_RenderCopy(rendu, pTextureImageciel, &src1, &dst1);
    SDL_RenderPresent(rendu);

    SDL_Surface* soleil = IMG_Load("soleil.png");
    if (!soleil)
    {
        cout << "Erreur de chargement de l'image ";
        return -1;
    }
    SDL_Texture* pTextureImageSoleil = SDL_CreateTextureFromSurface(rendu, soleil);
    SDL_FreeSurface(soleil);

    /*SDL_Rect posIng;
    posIng.x = 500;
    posIng.y = 50;*/
    SDL_Rect src4{ 0, 0, 0, 0 };
    SDL_Rect dst4{ 25, HAUTEUR - (HAUTEUR - 25), 110, 110 };

    /*SDL_QueryTexture(pTextureImage, nullptr, nullptr, &posIng.w, &posIng.h);*/
    SDL_QueryTexture(pTextureImageSoleil, nullptr, nullptr, &src4.w, &src4.h);
    //SDL_RenderCopy(rendu, pTextureImage, nullptr, &posIng); // Affiche ma texture sur touts l'écran
    SDL_RenderCopy(rendu, pTextureImageSoleil, &src4, &dst4);
    SDL_RenderPresent(rendu);

   
    // on importe une image de maison
    SDL_Surface* maison = IMG_Load("maison.png");
    if (!maison)
    {
        cout << "Erreur de chargement de l'image ";
        return -1;
    }
    SDL_Texture* pTextureImagemaison = SDL_CreateTextureFromSurface(rendu, maison);
    SDL_FreeSurface(maison);

    /*SDL_Rect posIng;
    posIng.x = 500;
    posIng.y = 50;*/
    SDL_Rect src2{ 0, 0, 0, 0 };
    SDL_Rect dst2{ 730,HAUTEUR - 186,300,200 };

    /*SDL_QueryTexture(pTextureImage, nullptr, nullptr, &posIng.w, &posIng.h);*/
    SDL_QueryTexture(pTextureImagemaison, nullptr, nullptr, &src2.w, &src2.h);
    //SDL_RenderCopy(rendu, pTextureImage, nullptr, &posIng); // Affiche ma texture sur touts l'écran
    SDL_RenderCopy(rendu, pTextureImagemaison, &src2, &dst2);
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

    SDL_Rect src{ 0, 0, 0, 0 };
    SDL_Rect dst{ 810, HAUTEUR - 50, 50, 50 };

    /*SDL_QueryTexture(pTextureImage, nullptr, nullptr, &posIng.w, &posIng.h);*/
    SDL_QueryTexture(pTextureImage, nullptr, nullptr, &src.w, &src.h);
    //SDL_RenderCopy(rendu, pTextureImage, nullptr, &posIng); // Affiche ma texture sur touts l'écran
    SDL_RenderCopy(rendu, pTextureImage, &src, &dst);
    SDL_RenderPresent(rendu);

    coordonéesPanda();

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
            if (event.key.keysym.sym == SDLK_RETURN) {
                ajout(rendu);
                SDL_RenderPresent(rendu);
            }
            if (event.key.keysym.sym == SDLK_m) {
                //SDL_DestroyTexture();
            }
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