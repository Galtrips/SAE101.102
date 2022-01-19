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


const int LARGEUR = 1000; //largeur fenetre
const int LARGEUR_TOTALE = 1250;//largeur fenetre
const int HAUTEUR = 700;  //hauteur fenetre
int nb_bambous = 8;

bool Menu = true;
bool Pause = false;
bool Config = false;
bool Apli = false;

bool batterieOff = false;

int jours = 0;
int nbCoupe = 0;
int maxi = 0;
int maxiBambou = 0;
int choixUser = 1;
int choix = 1;
bool boucle = false;
int xpanda;
int batterie = 100;

struct bambous
{
    int taille = 0;
    int croissance = 1;
    int cpt = 0;
};


bambous tab[8];
int coPanda[9];

void logo(SDL_Window* win) {

    SDL_Surface* icon = IMG_Load("logo.png");
    if (!icon)
    {
        printf("Erreur de chargement de l'image : %s", SDL_GetError());

    }
    SDL_SetWindowIcon(win, icon);
}

void menu(SDL_Renderer* rendu) {
    SDL_Surface* menus = IMG_Load("menu.png");

    SDL_Texture* pTextureImagemenu = SDL_CreateTextureFromSurface(rendu, menus);
    SDL_FreeSurface(menus);

    SDL_Rect src1{ 0, 0, 0, 0 };
    SDL_Rect dst1{ 0,0, LARGEUR_TOTALE, HAUTEUR };

    SDL_QueryTexture(pTextureImagemenu, nullptr, nullptr, &src1.w, &src1.h);
    SDL_RenderCopy(rendu, pTextureImagemenu, &src1, &dst1);
    SDL_RenderPresent(rendu);
}

void config(SDL_Renderer* rendu) {
    SDL_Surface* config = IMG_Load("config.png");

    SDL_Texture* pTextureImageconfig = SDL_CreateTextureFromSurface(rendu, config);
    SDL_FreeSurface(config);

    SDL_Rect src1{ 0, 0, 0, 0 };
    SDL_Rect dst1{ 0,0, LARGEUR_TOTALE, HAUTEUR };

    /*SDL_QueryTexture(pTextureImage, nullptr, nullptr, &posIng.w, &posIng.h);*/
    SDL_QueryTexture(pTextureImageconfig, nullptr, nullptr, &src1.w, &src1.h);
    //SDL_RenderCopy(rendu, pTextureImage, nullptr, &posIng); // Affiche ma texture sur touts l'écran
    SDL_RenderCopy(rendu, pTextureImageconfig, &src1, &dst1);
    SDL_RenderPresent(rendu);
}

void pause(SDL_Renderer* rendu) {
    SDL_Surface* pause = IMG_Load("pause.png");

    SDL_Texture* pTextureImagepause = SDL_CreateTextureFromSurface(rendu, pause);
    SDL_FreeSurface(pause);

    SDL_Rect src1{ 0, 0, 0, 0 };
    SDL_Rect dst1{ 0,0, LARGEUR_TOTALE, HAUTEUR };

    /*SDL_QueryTexture(pTextureImage, nullptr, nullptr, &posIng.w, &posIng.h);*/
    SDL_QueryTexture(pTextureImagepause, nullptr, nullptr, &src1.w, &src1.h);
    //SDL_RenderCopy(rendu, pTextureImage, nullptr, &posIng); // Affiche ma texture sur touts l'écran
    SDL_RenderCopy(rendu, pTextureImagepause, &src1, &dst1);
    SDL_RenderPresent(rendu);
}


void coordonéesPanda() {
    for (int i = 1; i < nb_bambous + 2; i++) {
        if (i < 9) {
            coPanda[i - 1] = 80 * i + 10;
        }
        else {
            coPanda[i - 1] = LARGEUR - 95;
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

        for (int j = 1; j < (tab[i - 1].cpt) + 1; j++) {

            bam2.x = (80 * i) - 4;
            bam2.w = 18;
            bam2.h = 3;
            bam2.y = HAUTEUR - ((((tab[i - 1].croissance * 6) + 5) * j) + 11);

            SDL_SetRenderDrawColor(rendu, 106, 164, 30, 255);
            SDL_RenderFillRect(rendu, &bam2);
            SDL_SetRenderDrawColor(rendu, 0, 102, 0, 255);
            SDL_RenderDrawRect(rendu, &bam2);

        }
    }
    if (maxi != 0) {
        init_ligne_max(rendu, maxi + 11, font);
    }
}

void batteriePleine(SDL_Renderer* rendu) {

    SDL_Rect rect1;
    rect1.x = 874;
    rect1.w = 10;
    rect1.h = 20;
    rect1.y = 80;
    SDL_SetRenderDrawColor(rendu, 58, 157, 35, 0);
    SDL_RenderFillRect(rendu, &rect1);


    SDL_Rect rect2;
    rect2.x = 886;
    rect2.w = 10;
    rect2.h = 20;
    rect2.y = 80;
    SDL_SetRenderDrawColor(rendu, 58, 157, 35, 50);
    SDL_RenderFillRect(rendu, &rect2);

    SDL_Rect rect3;
    rect3.x = 898;
    rect3.w = 10;
    rect3.h = 20;
    rect3.y = 80;
    SDL_SetRenderDrawColor(rendu, 58, 157, 35, 50);
    SDL_RenderFillRect(rendu, &rect3);

    SDL_Rect rect4;
    rect4.x = 910;
    rect4.w = 10;
    rect4.h = 20;
    rect4.y = 80;
    SDL_SetRenderDrawColor(rendu, 58, 157, 35, 50);
    SDL_RenderFillRect(rendu, &rect4);

    SDL_RenderPresent(rendu);
}

void batterie3barres(SDL_Renderer* rendu) {

    SDL_Rect rect1;
    rect1.x = 874;
    rect1.w = 10;
    rect1.h = 20;
    rect1.y = 80;
    SDL_SetRenderDrawColor(rendu, 255, 255, 0, 0);
    SDL_RenderFillRect(rendu, &rect1);


    SDL_Rect rect2;
    rect2.x = 886;
    rect2.w = 10;
    rect2.h = 20;
    rect2.y = 80;
    SDL_SetRenderDrawColor(rendu, 255, 255, 0, 0);
    SDL_RenderFillRect(rendu, &rect2);

    SDL_Rect rect3;
    rect3.x = 898;
    rect3.w = 10;
    rect3.h = 20;
    rect3.y = 80;
    SDL_SetRenderDrawColor(rendu, 255, 255, 0, 0);
    SDL_RenderFillRect(rendu, &rect3);


    SDL_RenderPresent(rendu);
}

void batterie2barres(SDL_Renderer* rendu) {

    SDL_Rect rect1;
    rect1.x = 874;
    rect1.w = 10;
    rect1.h = 20;
    rect1.y = 80;
    SDL_SetRenderDrawColor(rendu, 255, 127, 0, 0);
    SDL_RenderFillRect(rendu, &rect1);


    SDL_Rect rect2;
    rect2.x = 886;
    rect2.w = 10;
    rect2.h = 20;
    rect2.y = 80;
    SDL_SetRenderDrawColor(rendu, 255, 127, 0, 0);
    SDL_RenderFillRect(rendu, &rect2);


    SDL_RenderPresent(rendu);
}

void batterie1barre(SDL_Renderer* rendu) {

    SDL_Rect rect1;
    rect1.x = 874;
    rect1.w = 10;
    rect1.h = 20;
    rect1.y = 80;
    SDL_SetRenderDrawColor(rendu, 255, 0, 0, 0);
    SDL_RenderFillRect(rendu, &rect1);

    SDL_RenderPresent(rendu);
}

void batterieAuto(SDL_Renderer* rendu) {

    if (batterie > 75) {
        batteriePleine(rendu);
    }
    if (batterie <= 75 && batterie > 50) {
        batterie3barres(rendu);
    }
    if (batterie <= 50 && batterie > 25) {
        batterie2barres(rendu);
    }
    if (batterie <= 25 && batterie >= 1) {
        batterie1barre(rendu);
    }
}

void maximum2(SDL_Point tab[], int taille) {

    int i;
    int max = 0;
    int ind = 0;
    for (i = 0; i < taille; i = i + 1) {
        if (max > tab[i].x) {
            max = tab[i].x;
            max = tab[i].y;
            ind = i;
        }
        tab[i].x = i * 30 + 1000;
        tab[i].y = rand() % 100 + 400;
    }
}

void minimum(SDL_Point tab[], int taille) {

    int i;
    int min = 0;
    int ind = 0;
    for (i = 0; i < taille; i = i + 1) {
        if (min > tab[i].x) {
            min = tab[i].x;
            min = tab[i].y;
            ind = i;
        }
        tab[i].x = i * 30 + 1000;
        tab[i].y = rand() % 100 + 600;
    }
}

void moyenne(SDL_Point tab[], int taille) {

    int i;
    int som = 0;
    int moy = 0;

    for (i = 0; i < taille; i++) {

        som = som + tab[i].x;
        moy = som / taille;
        tab[i].x = i * 3 + 1000;
        tab[i].y = rand() % 100 + 500;
    }
}

void Graphique(SDL_Renderer* rendu) {

    SDL_Point points[100];
    maximum2(points, 100);
    SDL_SetRenderDrawColor(rendu, 58, 157, 35, 0);
    SDL_RenderDrawPoints(rendu, points, 100);
    SDL_SetRenderDrawColor(rendu, 58, 157, 35, 0);
    SDL_RenderDrawLines(rendu, points, 100);
    SDL_RenderPresent(rendu);
    moyenne(points, 100);
    SDL_SetRenderDrawColor(rendu, 127, 0, 255, 0);
    SDL_RenderDrawPoints(rendu, points, 100);
    SDL_SetRenderDrawColor(rendu, 127, 0, 255, 0);
    SDL_RenderDrawLines(rendu, points, 100);
    SDL_RenderPresent(rendu);
    minimum(points, 100);
    SDL_SetRenderDrawColor(rendu, 255, 0, 0, 0);
    SDL_RenderDrawPoints(rendu, points, 100);
    SDL_SetRenderDrawColor(rendu, 255, 0, 0, 0);
    SDL_RenderDrawLines(rendu, points, 100);
    SDL_RenderPresent(rendu);

}


void affichage(SDL_Renderer* rendu, TTF_Font* font) {

    SDL_SetRenderDrawColor(rendu, 255, 255, 255, 255);
    SDL_RenderClear(rendu);

    // on importe une image de maison
    SDL_Surface* fond = IMG_Load("fond.png");
    SDL_Texture* pTextureImagefond = SDL_CreateTextureFromSurface(rendu, fond);
    SDL_FreeSurface(fond);

    SDL_Rect src2{ 0, 0, 0, 0 };
    SDL_Rect dst2{ 0,0,LARGEUR_TOTALE,HAUTEUR };

    /*SDL_QueryTexture(pTextureImage, nullptr, nullptr, &posIng.w, &posIng.h);*/
    SDL_QueryTexture(pTextureImagefond, nullptr, nullptr, &src2.w, &src2.h);
    //SDL_RenderCopy(rendu, pTextureImage, nullptr, &posIng); // Affiche ma texture sur touts l'écran
    SDL_RenderCopy(rendu, pTextureImagefond, &src2, &dst2);



    texteLégende(rendu, font);
    texteHautMax(rendu, font);
    texteHautMoy(rendu, font);
    texteHautMin(rendu, font);
    texteNBcoupe(rendu, font);

    Graphique(rendu);

    bambou(rendu, font);


    if (jours == 0) {
        affichage_panda(rendu, 790);
        xpanda = 8;
    }
    batterieAuto(rendu);
    SDL_RenderPresent(rendu);

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
        nbCoupe++;
    }
}

void choix0() {

    tab[xpanda].taille = 0;
    tab[xpanda].cpt = 0;
    nbCoupe++;

}


void croissance(SDL_Renderer* rendu, TTF_Font* font) {

    for (int i = 1; i <= nb_bambous; i++) {
        if (tab[i - 1].taille + ((tab[i - 1].croissance * 6) + 5) < HAUTEUR - (65 * 2)) {
            tab[i - 1].taille = (tab[i - 1].taille) + ((tab[i - 1].croissance * 6) + 5);
            tab[i - 1].cpt++;
        }
    }

    jours++;



    if (choix == 1) {
        if (batterie < 6) {
            affichage(rendu, font);
            affichage_panda(rendu, coPanda[8]);
            batterie = batterie - 5;
            while (batterie < 100) {
                SDL_Delay(150);
                batterie = batterie + 10;
                batterieAuto(rendu);
            }

        }
        else {
            choix1();
            affichage(rendu, font);
            affichage_panda(rendu, coPanda[maxiBambou]);
            xpanda = maxiBambou;
        }

    }
    else if (choix == 0) {
        if (batterie == 0) {
            batterieOff = true;
        }
        else {
            batterie = batterie - 5;
            choix0();
            affichage(rendu, font);
            affichage_panda(rendu, coPanda[xpanda]);
        }
    }

}

int main(int argn, char* argv[]) {

    //ouverture de la SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        cout << "Echec à l?ouverture de SDL";
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

    menu(rendu);


    int cpt = 0;
    bool continuer = true;


    SDL_Event event;
    SDL_Event events;

    while (continuer) {

        SDL_WaitEvent(&event);

        switch (event.type) {

        case SDL_QUIT:
            continuer = false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_RETURN && batterieOff == false) {
                if (Apli == true) {
                    choix = choixUser;
                    croissance(rendu, font);
                }
            }
            if (event.key.keysym.sym == SDLK_r) {
                if (Apli == true) {
                    choix = choixUser;

                    for (int i = 0; i < nb_bambous; i++) {
                        tab[i].taille = 0;
                        jours = 0;
                        nbCoupe = 0;
                        tab[i].cpt = 0;
                        batterie = 100;
                        batterieOff = false;
                    }
                    affichage(rendu, font);
                }
            }
            if (event.key.keysym.sym == SDLK_RSHIFT && batterieOff == false) {
                if (Apli == true) {
                    choix = choixUser;
                    boucle = true;
                    while (boucle == true) {
                        SDL_PollEvent(&events);
                        SDL_Delay(600);
                        if (events.key.keysym.sym == SDLK_s) {
                            boucle = false;
                        }
                        else {
                            croissance(rendu, font);
                        }

                    }
                }


            }
            if (event.key.keysym.sym == SDLK_LEFT) {
                if (batterie == 0) {
                    batterieOff = true;
                }
                if (Apli == true && batterieOff == false) {
                    choix = 0;
                    if (jours == 0) {
                        jours = 1;
                    }
                    affichage(rendu, font);
                    if (xpanda == 0) {

                        affichage_panda(rendu, coPanda[7]);
                        xpanda = 7;

                    }
                    else {
                        affichage_panda(rendu, coPanda[xpanda - 1]);
                        xpanda = xpanda - 1;
                    }

                    SDL_Delay(300);
                }
            }
            if (event.key.keysym.sym == SDLK_RIGHT) {
                if (batterie == 0) {
                    batterieOff = true;
                }
                if (Apli == true && batterieOff == false) {
                    choix = 0;
                    if (jours == 0) {
                        jours = 1;
                    }
                    affichage(rendu, font);
                    if (xpanda == 7 || xpanda == 8) {

                        affichage_panda(rendu, coPanda[0]);
                        xpanda = 0;
                    }
                    else {
                        affichage_panda(rendu, coPanda[xpanda + 1]);
                        xpanda = xpanda + 1;
                    }
                    SDL_Delay(300);
                }



            }

            if (event.key.keysym.sym == SDLK_c) {
                if (batterie == 0) {
                    batterieOff = true;
                }
                if (Apli == true && batterieOff == false) {
                    choix = 0;
                    croissance(rendu, font);
                }
            }

            if (event.key.keysym.sym == SDLK_ESCAPE) {
                if (Apli == true) {
                    pause(rendu);
                    Pause = true;
                    Apli = false;
                }
            }

            break;

        case SDL_MOUSEBUTTONUP://appui souris
            if (event.button.button == SDL_BUTTON_LEFT) {//si on clique bouton gauche
                //Bouton Play menu
                if (event.button.x > 497 && event.button.x < 497 + 258 && event.button.y>380 && event.button.y < 380 + 82) {
                    if (Menu == true) {
                        Menu = false;
                        Apli = true;
                        affichage(rendu, font);
                        SDL_RenderPresent(rendu);

                    }
                }
                //Bouton exit menu
                if (event.button.x > 497 && event.button.x < 497 + 258 && event.button.y>537 && event.button.y < 537 + 82) {
                    if (Menu == true) {
                        Menu = false;
                        continuer = false;
                    }
                }
                //Bouton config menu
                if (event.button.x > 1054 && event.button.x < 1054 + 136 && event.button.y>167 && event.button.y < 167 + 48) {
                    if (Menu == true) {
                        config(rendu);
                        Config = true;
                        Menu = false;
                    }
                }
                if (event.button.x > 550 && event.button.x < 550 + 154 && event.button.y>48 && event.button.y < 93) {
                    if (Config == true) {
                        Menu = true;
                        Config = false;
                        menu(rendu);
                    }
                }

                //Config de bambou

                if (event.button.x > 550 && event.button.x < 570 && event.button.y>181 && event.button.y < 215) {
                    if (Config == true) {
                        config(rendu);
                        SDL_Rect underline;
                        underline.x = 550;
                        underline.w = 19;
                        underline.h = 5;
                        underline.y = 217;
                        SDL_SetRenderDrawColor(rendu, 255, 100, 100, 255);
                        SDL_RenderFillRect(rendu, &underline);
                        SDL_RenderPresent(rendu);
                        nb_bambous = 1;
                    }
                }
                if (event.button.x > 620 && event.button.x < 650 && event.button.y>181 && event.button.y < 215) {
                    if (Config == true) {
                        config(rendu);
                        SDL_Rect underline1;
                        underline1.x = 625;
                        underline1.w = 19;
                        underline1.h = 5;
                        underline1.y = 217;
                        SDL_SetRenderDrawColor(rendu, 255, 100, 100, 255);
                        SDL_RenderFillRect(rendu, &underline1);
                        SDL_RenderPresent(rendu);
                        nb_bambous = 2;
                    }
                }
                if (event.button.x > 700 && event.button.x < 725 && event.button.y>181 && event.button.y < 215) {
                    if (Config == true) {
                        config(rendu);
                        SDL_Rect underline;
                        underline.x = 704;
                        underline.w = 19;
                        underline.h = 5;
                        underline.y = 217;
                        SDL_SetRenderDrawColor(rendu, 255, 100, 100, 255);
                        SDL_RenderFillRect(rendu, &underline);
                        SDL_RenderPresent(rendu);
                        nb_bambous = 3;
                    }
                }
                if (event.button.x > 780 && event.button.x < 803 && event.button.y>181 && event.button.y < 215) {
                    if (Config == true) {
                        config(rendu);
                        SDL_Rect underline;
                        underline.x = 783;
                        underline.w = 19;
                        underline.h = 5;
                        underline.y = 217;
                        SDL_SetRenderDrawColor(rendu, 255, 100, 100, 255);
                        SDL_RenderFillRect(rendu, &underline);
                        SDL_RenderPresent(rendu);
                        nb_bambous = 4;
                    }
                }
                if (event.button.x > 860 && event.button.x < 883 && event.button.y>181 && event.button.y < 215) {
                    if (Config == true) {
                        config(rendu);
                        SDL_Rect underline;
                        underline.x = 862;
                        underline.w = 19;
                        underline.h = 5;
                        underline.y = 217;
                        SDL_SetRenderDrawColor(rendu, 255, 100, 100, 255);
                        SDL_RenderFillRect(rendu, &underline);
                        SDL_RenderPresent(rendu);
                        nb_bambous = 5;
                    }
                }
                if (event.button.x > 940 && event.button.x < 963 && event.button.y>181 && event.button.y < 215) {
                    if (Config == true) {
                        config(rendu);
                        SDL_Rect underline;
                        underline.x = 943;
                        underline.w = 19;
                        underline.h = 5;
                        underline.y = 217;
                        SDL_SetRenderDrawColor(rendu, 255, 100, 100, 255);
                        SDL_RenderFillRect(rendu, &underline);
                        SDL_RenderPresent(rendu);
                        nb_bambous = 6;
                    }
                }
                if (event.button.x > 1020 && event.button.x < 1043 && event.button.y>181 && event.button.y < 215) {
                    if (Config == true) {
                        config(rendu);
                        SDL_Rect underline;
                        underline.x = 1021;
                        underline.w = 19;
                        underline.h = 5;
                        underline.y = 217;
                        SDL_SetRenderDrawColor(rendu, 255, 100, 100, 255);
                        SDL_RenderFillRect(rendu, &underline);
                        SDL_RenderPresent(rendu);
                        nb_bambous = 7;
                    }
                }
                if (event.button.x > 1100 && event.button.x < 1123 && event.button.y>181 && event.button.y < 215) {
                    if (Config == true) {
                        config(rendu);
                        SDL_Rect underline;
                        underline.x = 1101;
                        underline.w = 19;
                        underline.h = 5;
                        underline.y = 217;
                        SDL_SetRenderDrawColor(rendu, 255, 100, 100, 255);
                        SDL_RenderFillRect(rendu, &underline);
                        SDL_RenderPresent(rendu);
                        nb_bambous = 8;
                    }
                }

                //Menu Pause

                if (event.button.x > 539 && event.button.x < 539 + 175 && event.button.y>232 && event.button.y < 232 + 50) {
                    if (Pause == true) {
                        affichage(rendu, font);
                        Pause = false;
                        Apli = true;
                        if (jours == 0) {
                            affichage_panda(rendu, 790);
                        }
                        else {
                            affichage_panda(rendu, coPanda[xpanda]);
                        }
                    }
                }
                if (event.button.x > 539 && event.button.x < 539 + 175 && event.button.y>323 && event.button.y < 323 + 50) {
                    if (Pause == true) {
                        Pause = false;
                        Config = true;
                        config(rendu);
                    }
                }
                if (event.button.x > 539 && event.button.x < 539 + 175 && event.button.y>414 && event.button.y < 414 + 50) {
                    if (Pause == true) {
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