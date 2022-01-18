#include <SDL.h>
#include <SDL_ttf.h>
#include "config_sdl.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>
#include <SDL_image.h>

using namespace std;

SDL_Rect bam1;
SDL_Rect bam2;
SDL_Rect title;
SDL_Surface* image;
SDL_Texture* pTextureImage;
SDL_Surface* sable;
SDL_Texture* pTextureImagesable;
SDL_Surface* ciel;
SDL_Texture* pTextureImageciel;
SDL_Surface* soleil;
SDL_Texture* pTextureImageSoleil;
SDL_Surface* maison;
SDL_Texture* pTextureImagemaison;

const int LARGEUR = 1000; //largeur fenetre
const int LARGEUR_TOTALE = 1250;//largeur fenetre
const int HAUTEUR = 700;  //hauteur fenetre
const int nb_bambous = 8;
bool Menu = false;
int jours = 0;
int maxi = 0;
int maxiBambou = 0;
int choix = 1;

struct bambous
{
    int taille = 0;
    int croissance = 1;
    int cpt = 0;
};


bambous tab[nb_bambous];
int coPanda[nb_bambous + 1];

void logo(SDL_Window* win) {

    SDL_Surface* icon = IMG_Load("logo.png");
    if (!icon)
    {
        printf("Erreur de chargement de l'image : %s", SDL_GetError());

    }
    SDL_SetWindowIcon(win, icon);
}

void menu(SDL_Renderer* rendu, TTF_Font* font) {
    SDL_Surface* menus = IMG_Load("menu.png");
    if (!menus)
    {
        cout << "Erreur de chargement de l'image ";
    }
    SDL_Texture* pTextureImagemenu = SDL_CreateTextureFromSurface(rendu, menus);
    SDL_FreeSurface(menus);

    SDL_Rect src1{ 0, 0, 0, 0 };
    SDL_Rect dst1{ 0,0, LARGEUR_TOTALE, HAUTEUR };

    /*SDL_QueryTexture(pTextureImage, nullptr, nullptr, &posIng.w, &posIng.h);*/
    SDL_QueryTexture(pTextureImagemenu, nullptr, nullptr, &src1.w, &src1.h);
    //SDL_RenderCopy(rendu, pTextureImage, nullptr, &posIng); // Affiche ma texture sur touts l'écran
    SDL_RenderCopy(rendu, pTextureImagemenu, &src1, &dst1);
    SDL_RenderPresent(rendu);
}

void coordonéesPanda() {
    for (int i = 1; i < nb_bambous + 2; i++) {
        if (i < 9) {
            coPanda[i - 1] = 80 * i + 10;
        }
        else {
            coPanda[i - 1] = LARGEUR - 70;
        }
    }
}

void texteLégende(SDL_Renderer* rendu, TTF_Font* font) {

    SDL_Color blanche = { 255,255,255 }; //on définit une couleur de texte

    SDL_Rect positionTexte; //rectangle définissant le positionnement du texte, et sa taille

    //on place le texte au point (100,100)
    positionTexte.x = LARGEUR_TOTALE - 185;
    positionTexte.y = 50;
    //on crée une texture à partir du texte, de sa couleur, et de la fonte
    SDL_Texture* texture = loadText(rendu, "Légende :", blanche, font);
    //on maj le rectangle couvrant cette texture
    SDL_QueryTexture(texture, NULL, NULL, &positionTexte.w, &positionTexte.h);
    //si on veut modifier le cadre du texte
    positionTexte.w *= 1.5;
    positionTexte.h *= 1.5;
    //on copie la texture dans le rendu
    SDL_RenderCopy(rendu, texture, NULL, &positionTexte);
    //on met à jour le rendu
    SDL_RenderPresent(rendu);
    //on détruit la texture
    SDL_DestroyTexture(texture);
}

void texteHautMax(SDL_Renderer* rendu, TTF_Font* font) {
    SDL_Color vert = { 85, 255, 51 }; //on définit une couleur de texte
    SDL_Rect positionTexte; //rectangle définissant le positionnement du texte, et sa taille

    //on place le texte au point (100,100)
    positionTexte.x = LARGEUR_TOTALE - 160;
    positionTexte.y = 150;
    //on crée une texture à partir du texte, de sa couleur, et de la fonte
    SDL_Texture* texture = loadText(rendu, "Haut max", vert, font);
    //on maj le rectangle couvrant cette texture
    SDL_QueryTexture(texture, NULL, NULL, &positionTexte.w, &positionTexte.h);
    //si on veut modifier le cadre du texte
    positionTexte.w *= 0.8;
    positionTexte.h *= 0.8;
    //on copie la texture dans le rendu
    SDL_RenderCopy(rendu, texture, NULL, &positionTexte);
    //on met à jour le rendu
    SDL_RenderPresent(rendu);
    //on détruit la texture
    SDL_DestroyTexture(texture);
}

void texteHautMoy(SDL_Renderer* rendu, TTF_Font* font) {
    SDL_Color violet = { 127,0,255 }; //on définit une couleur de texte
    SDL_Rect positionTexte; //rectangle définissant le positionnement du texte, et sa taille

    //on place le texte au point (100,100)
    positionTexte.x = LARGEUR_TOTALE - 160;
    positionTexte.y = 200;
    //on crée une texture à partir du texte, de sa couleur, et de la fonte
    SDL_Texture* texture = loadText(rendu, "Haut moy", violet, font);
    //on maj le rectangle couvrant cette texture
    SDL_QueryTexture(texture, NULL, NULL, &positionTexte.w, &positionTexte.h);
    //si on veut modifier le cadre du texte
    positionTexte.w *= 0.8;
    positionTexte.h *= 0.8;
    //on copie la texture dans le rendu
    SDL_RenderCopy(rendu, texture, NULL, &positionTexte);
    //on met à jour le rendu
    SDL_RenderPresent(rendu);
    //on détruit la texture
    SDL_DestroyTexture(texture);
}

void texteHautMin(SDL_Renderer* rendu, TTF_Font* font) {
    SDL_Color rouge = { 255,0,0 }; //on définit une couleur de texte
    SDL_Rect positionTexte; //rectangle définissant le positionnement du texte, et sa taille

    //on place le texte au point (100,100)
    positionTexte.x = LARGEUR_TOTALE - 160;
    positionTexte.y = 250;
    //on crée une texture à partir du texte, de sa couleur, et de la fonte
    SDL_Texture* texture = loadText(rendu, "Haut min", rouge, font);
    //on maj le rectangle couvrant cette texture
    SDL_QueryTexture(texture, NULL, NULL, &positionTexte.w, &positionTexte.h);
    //si on veut modifier le cadre du texte
    positionTexte.w *= 0.8;
    positionTexte.h *= 0.8;
    //on copie la texture dans le rendu
    SDL_RenderCopy(rendu, texture, NULL, &positionTexte);
    //on met à jour le rendu
    SDL_RenderPresent(rendu);
    //on détruit la texture
    SDL_DestroyTexture(texture);
}



void texteNBcoupe(SDL_Renderer* rendu, TTF_Font* font) {
    SDL_Color bleu = { 0,0,255 }; //on définit une couleur de texte
    SDL_Rect positionTexte; //rectangle définissant le positionnement du texte, et sa taille

    //on place le texte au point (100,100)
    positionTexte.x = LARGEUR_TOTALE - 185;
    positionTexte.y = 300;
    //on crée une texture à partir du texte, de sa couleur, et de la fonte
    SDL_Texture* texture = loadText(rendu, "nombre de coupe", bleu, font);
    //on maj le rectangle couvrant cette texture
    SDL_QueryTexture(texture, NULL, NULL, &positionTexte.w, &positionTexte.h);
    //si on veut modifier le cadre du texte
    positionTexte.w *= 0.8;
    positionTexte.h *= 0.8;
    //on copie la texture dans le rendu
    SDL_RenderCopy(rendu, texture, NULL, &positionTexte);
    //on met à jour le rendu
    SDL_RenderPresent(rendu);
    //on détruit la texture
    SDL_DestroyTexture(texture);
}

void affichage_panda(SDL_Renderer* rendu, int x) {
    // on importe une image
    image = IMG_Load("panda.png");
    pTextureImage = SDL_CreateTextureFromSurface(rendu, image);
    SDL_FreeSurface(image);

    SDL_Rect src{ 0, 0, 0, 0 };
    SDL_Rect dst{ x, HAUTEUR - 50, 50, 50 };


    SDL_QueryTexture(pTextureImage, nullptr, nullptr, &src.w, &src.h);
    SDL_RenderCopy(rendu, pTextureImage, &src, &dst);
    SDL_DestroyTexture(pTextureImage);
    SDL_RenderPresent(rendu);
}

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

void init_ligne_max(SDL_Renderer* rendu, int taille, TTF_Font* font) {

    SDL_SetRenderDrawColor(rendu, 255, 0, 0, 255);
    SDL_RenderDrawLine(rendu, 0, HAUTEUR - taille, LARGEUR, HAUTEUR - taille);

    SDL_Color rouge = { 255,0,0 }; //on définit une couleur de texte
    SDL_Rect positionTexte; //rectangle définissant le positionnement du texte, et sa taille

    //on place le texte au point (100,100)
    positionTexte.x = LARGEUR - 28;
    positionTexte.y = HAUTEUR - taille - 15;
   
    SDL_Texture* texture = loadText(rendu, "Max", rouge, font);
   
    SDL_QueryTexture(texture, NULL, NULL, &positionTexte.w, &positionTexte.h);
  
    positionTexte.w *= 0.6;
    positionTexte.h *= 0.6;

    SDL_RenderCopy(rendu, texture, NULL, &positionTexte);

 
    SDL_DestroyTexture(texture);

}

void bambou(SDL_Renderer* rendu, TTF_Font* font) {

    for (int i = 1; i < nb_bambous + 1; i++) {
     
        bam1.x = 80 * i;
        bam1.w = 10;
        bam1.h = -(tab[i - 1].taille);
        bam1.y = HAUTEUR - 10;

        if (tab[i - 1].taille > maxi) {
            maxi = tab[i - 1].taille;
        }

        SDL_SetRenderDrawColor(rendu, 106, 164, 30, 255);
        SDL_RenderFillRect(rendu, &bam1);
        SDL_SetRenderDrawColor(rendu, 0, 102, 0, 255);
        SDL_RenderDrawRect(rendu, &bam1);
        SDL_RenderPresent(rendu);

        for (int j = 1; j < (tab[i-1].cpt) + 1; j++) {

            bam2.x = (80 * i) - 4;
            bam2.w = 18;
            bam2.h = 3;
            bam2.y = HAUTEUR - ((((tab[i-1].croissance * 6) +5) * j) + 11);

            SDL_SetRenderDrawColor(rendu, 106, 164, 30, 255);
            SDL_RenderFillRect(rendu, &bam2);
            SDL_SetRenderDrawColor(rendu, 0, 102, 0, 255);
            SDL_RenderDrawRect(rendu, &bam2);

        }
    }
    if (maxi != 0) {
        init_ligne_max(rendu, maxi + 11, font);
    }


    SDL_RenderPresent(rendu);
}

void affichage(SDL_Renderer* rendu, TTF_Font* font) {

    SDL_SetRenderDrawColor(rendu, 255, 255, 255, 255);
    SDL_RenderClear(rendu);
    // on importe une image de sable
    sable = IMG_Load("sable.png");
    pTextureImagesable = SDL_CreateTextureFromSurface(rendu, sable);
    SDL_FreeSurface(sable);


    SDL_Rect src3{ 0, 0, 0, 0 };
    SDL_Rect dst3{ 0, HAUTEUR - 170,1000 , 270 };
    SDL_QueryTexture(pTextureImagesable, nullptr, nullptr, &src3.w, &src3.h);
    SDL_RenderCopy(rendu, pTextureImagesable, &src3, &dst3);
    SDL_DestroyTexture(pTextureImagesable);
    // on importe une image de ciel
    ciel = IMG_Load("ciel1.png");
    pTextureImageciel = SDL_CreateTextureFromSurface(rendu, ciel);
    SDL_FreeSurface(ciel);


    SDL_Rect src1{ 0, 0, 0, 0 };
    SDL_Rect dst1{ 0,0, 1000, 400 };

    SDL_QueryTexture(pTextureImageciel, nullptr, nullptr, &src1.w, &src1.h);
    SDL_RenderCopy(rendu, pTextureImageciel, &src1, &dst1);
    SDL_DestroyTexture(pTextureImageciel);


    soleil = IMG_Load("soleil.png");
    pTextureImageSoleil = SDL_CreateTextureFromSurface(rendu, soleil);
    SDL_FreeSurface(soleil);
    SDL_Rect src4{ 0, 0, 0, 0 };
    SDL_Rect dst4{ 25, HAUTEUR - (HAUTEUR - 25), 110, 110 };
    SDL_QueryTexture(pTextureImageSoleil, nullptr, nullptr, &src4.w, &src4.h);
    SDL_RenderCopy(rendu, pTextureImageSoleil, &src4, &dst4);
    SDL_DestroyTexture(pTextureImageSoleil);



    // on importe une image de maison
    maison = IMG_Load("maison.png");
    pTextureImagemaison = SDL_CreateTextureFromSurface(rendu, maison);
    SDL_FreeSurface(maison);
    SDL_Rect src2{ 0, 0, 0, 0 };
    SDL_Rect dst2{ 730,HAUTEUR - 186,300,200 };
    SDL_QueryTexture(pTextureImagemaison, nullptr, nullptr, &src2.w, &src2.h);
    SDL_RenderCopy(rendu, pTextureImagemaison, &src2, &dst2);
    SDL_DestroyTexture(pTextureImagemaison);

    SDL_RenderPresent(rendu);

    SDL_Rect rect;
    rect.x = LARGEUR - 70;
    rect.w = 50;
    rect.h = 8;
    rect.y = HAUTEUR - 8;
    SDL_SetRenderDrawColor(rendu, 0, 30, 164, 255);
    SDL_RenderFillRect(rendu, &rect);

    // créé le rectangle noir
    SDL_Rect rectt;
    rectt.x = LARGEUR;
    rectt.y = 0;
    rectt.h = HAUTEUR;
    rectt.w = 300;
    // Dessin du rectangle
    SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
    SDL_RenderFillRect(rendu, &rectt);
    texteLégende(rendu, font);
    texteHautMax(rendu, font);
    texteHautMoy(rendu, font);
    texteHautMin(rendu, font);
    texteNBcoupe(rendu, font);
  

    bambou(rendu, font);
    SDL_RenderPresent(rendu);

    if (jours == 0 ) {
        affichage_panda(rendu, 810);
    }
}

void choix1() {
    maxiBambou = 0;
    if (jours > 0) {

        for (int i = 0; i < nb_bambous; i++) {
            if (tab[i].taille > tab[maxiBambou].taille) {
                maxiBambou = i;
            }
        }

        tab[maxiBambou].taille = 0;
        tab[maxiBambou].cpt = 0;
    }
}

void croissance(SDL_Renderer* rendu, TTF_Font* font) {

    for (int i = 1; i <= nb_bambous; i++) {
        if (tab[i - 1].taille + ((tab[i - 1].croissance * 6) + 5) < HAUTEUR - (65 * 2)) {
            tab[i - 1].taille = (tab[i - 1].taille) + ((tab[i - 1].croissance * 6) + 5);
            tab[i - 1].cpt++;
        }
    }

    jours++;
    affichage(rendu, font);
    

    if (choix == 1) {
        affichage_panda(rendu, coPanda[maxiBambou]);
        choix1();
    }
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
        LARGEUR_TOTALE,
        HAUTEUR,
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer* rendu = SDL_CreateRenderer(
        win,  //nom de la fenêtre
        -1, //par défaut
        SDL_RENDERER_ACCELERATED); //utilisation du GPU, valeur recommandée

    if (win == NULL)
        cout << "erreur ouverture fenetre";

    TTF_Init();
    TTF_Font* font = TTF_OpenFont("C:\\Windows\\Fonts\\calibri.ttf", 25);

    logo(win);
    init_croissance();
    coordonéesPanda();

    menu(rendu, font);
    

    int cpt = 0;
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

                croissance(rendu, font);
            }
       

        case SDL_MOUSEBUTTONUP://appui souris
            if (event.button.button == SDL_BUTTON_LEFT) {//si on clique bouton gauche
                if (event.button.x > 497 && event.button.x < 497 + 258 && event.button.y>380 && event.button.y < 380 + 82) {
                    if (Menu == false) {

                        Menu = true;
                        affichage(rendu, font);
                        SDL_RenderPresent(rendu);

                    }
                }
                else if (event.button.x > 497 && event.button.x < 497 + 258 && event.button.y>537 && event.button.y < 537 + 82) {
                    if (Menu == false) {
                        continuer = false;
                    }
                }
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