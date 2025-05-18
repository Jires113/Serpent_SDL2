# ifndef FONCTION_NANA_H
#define FONCTION_NANA_H
#include <stdbool.h>

typedef struct Cellule_na {
    int x_na;
    int y_na;
    struct Cellule_na* suivant;
} Cellule_na;

typedef enum {
    HAUT,
    BAS,
    GAUCHE,
    DROITE
} Direction_na;

typedef struct {
    int x_na;
    int y_na;
    bool actif_na;
    Uint32 temps_debut_na;
    Uint32 duree_na;  
} Bonus_traversee_na;


typedef struct Nourriture_na {
    int x_na;
    int y_na;
} Nourriture_na;

typedef struct{
    int x_na;
    int y_na;
}Bonus_na;

typedef struct {
    int segments_na[7];  
} Digit_na;

typedef struct {
    bool isPaused;
} PauseSystem_na;





void pause_na();
void initSDL_na(SDL_Window *window_na, SDL_Renderer *renderer_na);
void dessiner_grille_na(SDL_Renderer *renderer_na);
void ajouter_cellule_na(int x1_na, int y1_na, Cellule_na **Serpent);
void suppression_queue_na(Cellule_na **Serpent_na);
void dessin_Serpent_na(SDL_Renderer *renderer_na, const Cellule_na *Serpent_na);
void direction_na(Cellule_na **Serpent_na, char touche) ;
Nourriture_na ajout_Nourriture_na(Nourriture_na food_na, Cellule_na *Serpent_na);
void dessin_nourriture_na(SDL_Renderer *renderer_na, Nourriture_na food_na);
bool mourrir_serpent_na(Cellule_na *Serpent_na, SDL_Renderer *renderer_na, Bonus_traversee_na *bonus_na) ;
void dessiner_score_na(SDL_Renderer *renderer_na) ;
void dessiner_segment_na(SDL_Renderer* renderer_na, int x, int y, int type) ;
void dessiner_chiffre_na(SDL_Renderer* renderer_na, int x, int y, int nombre);
void init_pause_na(PauseSystem_na* pause);
void handle_pause_na(SDL_Event* event, PauseSystem_na* pause) ;
//Bonus_na ajout_bonus(Bonus_na bonus, Cellule_na *Serpent_na);
void dessiner_texte_na(SDL_Renderer* renderer_na, const char* texte, int x, int y);
//Bonus_na ajout_bonus(Bonus_na bonus, Cellule_na *Serpent_na);
void gerer_bonus_mur_na(Cellule_na *Serpent_na, Bonus_traversee_na *bonus_na);
Bonus_traversee_na placer_nouveau_bonus_na(Cellule_na *Serpent_na);
void dessiner_bonus_na(SDL_Renderer* renderer_na, Bonus_traversee_na bonus_na) ;

#endif
