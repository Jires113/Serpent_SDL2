# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
#include <time.h>
# include <SDL2/SDL.h>
#include <assert.h>
# include  <unistd.h>

# include "fonction_nana.h"
# define vmax_na 100
# define Hauteur_na 600
# define Hauteur_na_na 600
# define pixel_na 32
# define taille_na 20


int score_na = -1;
SDL_Color couleur_texte = {255, 255, 255, 255};
Direction_na direction_courante_na = DROITE;
void pause_na(){
    SDL_Event event_na;
    bool quit_na = false;
    while (!quit_na) {
        while (SDL_PollEvent(&event_na)) {
            if (event_na.type == SDL_QUIT) {
                quit_na = true;
            }
        }
    }
    
}

void dessiner_score_na(SDL_Renderer* renderer_na) {
    int temp_score = score_na;
    int position_na = 0;
    int digits[10];
    int count_na = 0;

    do {
        digits[count_na++] = temp_score % 10;
        temp_score /= 10;
    } while(temp_score > 0);
    
    for(int i = count_na-1; i >= 0; i--) {
        dessiner_chiffre_na(renderer_na, 10 + position_na * 30, 10, digits[i]);
        position_na++;
    }
}

Digit_na digits_na[10] = {
    {{1,1,1,1,1,1,0}}, 
    {{0,1,1,0,0,0,0}},  
    {{1,1,0,1,1,0,1}}, 
    {{1,1,1,1,0,0,1}}, 
    {{0,1,1,0,0,1,1}}, 
    {{1,0,1,1,0,1,1}},  
    {{1,0,1,1,1,1,1}}, 
    {{1,1,1,0,0,0,0}},  
    {{1,1,1,1,1,1,1}},  
    {{1,1,1,1,0,1,1}}   
};

void dessiner_segment_na(SDL_Renderer* renderer_na, int x, int y, int type) {
    SDL_Rect segment_na;
    
    switch(type) {
        case 0: // Horizontal
            segment_na = (SDL_Rect){x, y, 20, 4};
            break;
        case 1: // Vertical
            segment_na = (SDL_Rect){x, y, 4, 20};
            break;
    }
    
    SDL_SetRenderDrawColor(renderer_na, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer_na, &segment_na);
}

void dessiner_chiffre_na(SDL_Renderer* renderer_na, int x, int y, int nombre) {
    Digit_na digit_na = digits_na[nombre];

    if(digit_na.segments_na[0]) dessiner_segment_na(renderer_na, x+4, y, 0);     
    if(digit_na.segments_na[1]) dessiner_segment_na(renderer_na, x+20, y+4, 1);  
    if(digit_na.segments_na[2]) dessiner_segment_na(renderer_na, x+20, y+28, 1);  
    if(digit_na.segments_na[3]) dessiner_segment_na(renderer_na, x+4, y+44, 0);   
    if(digit_na.segments_na[4]) dessiner_segment_na(renderer_na, x, y+28, 1);     
    if(digit_na.segments_na[5]) dessiner_segment_na(renderer_na, x, y+4, 1);      
    if(digit_na.segments_na[6]) dessiner_segment_na(renderer_na, x+4, y+22, 0);   
}



void init_pause_na(PauseSystem_na* pause) {
    pause->isPaused = false;
}


void handle_pause_na(SDL_Event* event, PauseSystem_na* pause) {
    if (event->type == SDL_KEYDOWN) {
        if (event->key.keysym.sym == SDLK_ESCAPE || event->key.keysym.sym == SDLK_0) {
            pause->isPaused = !pause->isPaused;
        }
    }
}
Bonus_traversee_na bonus_mur_na = {
    .x_na = -1,
    .y_na = -1,
    .actif_na = false,
    .temps_debut_na = 0,
    .duree_na = 10000  
};

void dessiner_bonus_na(SDL_Renderer* renderer_na, Bonus_traversee_na bonus_na) {
    if (bonus_na.x_na >= 0 && bonus_na.y_na >= 0) {
        SDL_SetRenderDrawColor(renderer_na, 0, 255, 255, 255); // Couleur cyan
        SDL_Rect bonus_rect = {
            .x = bonus_na.x_na * pixel_na,
            .y = bonus_na.y_na * pixel_na,
            .w = pixel_na,
            .h = pixel_na
        };
        SDL_RenderFillRect(renderer_na, &bonus_rect);
    }
}

// Fonction pour dessiner l'indicateur de bonus actif
void dessiner_indicateur_bonus_na(SDL_Renderer* renderer_na, Bonus_traversee_na bonus_na) {
    if (bonus_na.actif_na) {
        Uint32 temps_ecoule = SDL_GetTicks() - bonus_na.temps_debut_na;
        float pourcentage = 1.0f - ((float)temps_ecoule / bonus_na.duree_na);
        
        if (pourcentage > 0) {
            SDL_SetRenderDrawColor(renderer_na, 0, 0, 255, 255); // Couleur bleue
            SDL_Rect indicateur = {
                .x = Hauteur_na - 20,
                .y = 10,
                .w = 10,
                .h = (int)(50 * pourcentage)
            };
            SDL_RenderFillRect(renderer_na, &indicateur);
        }
    }
}


void gerer_bonus_mur_na(Cellule_na *Serpent_na, Bonus_traversee_na *bonus_na) {
    // Si aucun bonus n'est présent sur le terrain, en placer un nouveau
    if (bonus_na->x_na == -1 && !bonus_na->actif_na) {
        *bonus_na = placer_nouveau_bonus_na(Serpent_na);
    }
    
    // Si le serpent attrape le bonus
    if (!bonus_na->actif_na && Serpent_na->x_na == bonus_na->x_na && Serpent_na->y_na == bonus_na->y_na) {
        
        bonus_na->actif_na = true;
        bonus_na->temps_debut_na = SDL_GetTicks();
        bonus_na->x_na = -1;  // Cache le bonus actuel
        bonus_na->y_na = -1;
        
        // Place immédiatement un nouveau bonus
        *bonus_na = placer_nouveau_bonus_na(Serpent_na);
    }
    
    // Vérification de la durée du bonus
    if (bonus_na->actif_na) {
        Uint32 temps_actuel = SDL_GetTicks();
        if (temps_actuel - bonus_na->temps_debut_na >= bonus_na->duree_na) {
            bonus_na->actif_na = false;
        }
    }
}
Bonus_traversee_na placer_nouveau_bonus_na(Cellule_na *Serpent_na) {
    Bonus_traversee_na bonus_na;
    bool position_valide = false;
    
    while (!position_valide) {
        bonus_na.x_na = rand() % (taille_na - 2) + 1;
        bonus_na.y_na = rand() % (taille_na - 2) + 1;
        
        // Vérification collision avec serpent
        position_valide = true;
        Cellule_na *courant = Serpent_na;
        while (courant != NULL) {
            if (courant->x_na == bonus_na.x_na && courant->y_na == bonus_na.y_na) {
                position_valide = false;
                break;
            }
            courant = courant->suivant;
        }
    }
    
    bonus_na.actif_na = false;
    bonus_na.temps_debut_na = 0;
    bonus_na.duree_na = 10000;  
    return bonus_na;
}


void initSDL_na(SDL_Window *window_na, SDL_Renderer *renderer_na) {
    Cellule_na *Serpent_na = NULL;
    int statut_na = EXIT_FAILURE;
    Uint32 Temp_final_na = SDL_GetTicks();
    bool running = true;
    int score = 0;
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Erreur SDL_Init() : %s", SDL_GetError());
        goto Quit;
    }
    window_na = SDL_CreateWindow("Serpent", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Hauteur_na_na, Hauteur_na, SDL_WINDOW_SHOWN);
    if (window_na == NULL) {
        fprintf(stderr, "Erreur SDL_CreateWindow() : %s", SDL_GetError());
        goto Quit;
    }
    renderer_na = SDL_CreateRenderer(window_na, -1, SDL_RENDERER_ACCELERATED);
    if (renderer_na == NULL) {
        fprintf(stderr, "Erreur SDL_CreateRenderer() : %s", SDL_GetError());
        goto Quit;
    }
    Serpent_na = (Cellule_na*)malloc(sizeof(Cellule_na));
    if(Serpent_na == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire");
        goto Quit;
    }
    PauseSystem_na pause_system;
    init_pause_na(&pause_system);
    Serpent_na->x_na = 5;
    Serpent_na->y_na = 5;
    Serpent_na->suivant = NULL;
    Nourriture_na nourriture_na = ajout_Nourriture_na(nourriture_na, Serpent_na);

        
        while (running) {
           if (!bonus_mur_na.actif_na && bonus_mur_na.x_na == -1 && (rand() % 500 == 0)) {  // Encore plus rare (1/500)
        bonus_mur_na = placer_nouveau_bonus_na(Serpent_na);
    }
    
    // Si le serpent attrape le bonus
    if (!bonus_mur_na.actif_na && Serpent_na->x_na == bonus_mur_na.x_na && Serpent_na->y_na == bonus_mur_na.y_na) {
        
        bonus_mur_na.actif_na = true;
        bonus_mur_na.temps_debut_na = SDL_GetTicks();
        bonus_mur_na.x_na = -1;  // Cache le bonus
        bonus_mur_na.y_na = -1;
    }
    
    // Vérification de la durée du bonus
    if (bonus_mur_na.actif_na) {
        Uint32 temps_actuel = SDL_GetTicks();
        if (temps_actuel - bonus_mur_na.temps_debut_na >= bonus_mur_na.duree_na) {
            bonus_mur_na.actif_na = false;
        }
    }
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            handle_pause_na(&event, &pause_system);
            if (!pause_system.isPaused) {
                // Vérifier la collision avec le bonus
    if (Serpent_na->x_na == bonus_mur_na.x_na && Serpent_na->y_na == bonus_mur_na.y_na) {
        bonus_mur_na.actif_na = true;
        bonus_mur_na.temps_debut_na = SDL_GetTicks();
        bonus_mur_na.x_na = -1;
        bonus_mur_na.y_na = -1;
    }

    // Gérer l'état du bonus
    gerer_bonus_mur_na(Serpent_na, &bonus_mur_na);

    // Créer un nouveau bonus périodiquement
    if (!bonus_mur_na.actif_na && bonus_mur_na.x_na == -1 && (rand() % 300 == 0)) {  
        bonus_mur_na = placer_nouveau_bonus_na(Serpent_na);
    }
                if(event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_UP)
                {
                    if (direction_courante_na == BAS)
                    {
                        continue;
                    }
                    else{
                        direction_courante_na = HAUT;
                    }
                }
                else if(event.key.keysym.sym == SDLK_DOWN){
                    if (direction_courante_na == HAUT)
                    {
                        continue;
                    }
                    else{
                        direction_courante_na = BAS;
                    }
                    
                }
                else if (event.key.keysym.sym == SDLK_LEFT)
                {
                    if (direction_courante_na == DROITE)
                    {
                        continue;
                    }
                    else {
                        direction_courante_na = GAUCHE;
                    }
                    
                }
                else if (event.key.keysym.sym == SDLK_RIGHT)
                {
                    if (direction_courante_na == GAUCHE)
                    {
                        continue;
                    }
                    else {
                        direction_courante_na = DROITE;
                    }
                }
                
                
                }
            }
        }
        if (!pause_system.isPaused) {
            
        }
        Uint32 Temp_present_na = SDL_GetTicks();
        if (Temp_present_na - Temp_final_na >= vmax_na) {
            switch(direction_courante_na) {
                case HAUT:
                
        
                /*  if((Serpent_na)->y_na == 0){
                        printf("Game Over! Le serpent s'est mordu!\n");
                        printf(" Score: %d\n", score_na);
                        running = false;
                        continue;
                    }
*/
                        //ajouter_cellule_na((Serpent_na)->x_na, taille_na - 2, &Serpent_na);
                   // else
                        ajouter_cellule_na((Serpent_na)->x_na, (Serpent_na)->y_na - 1, &Serpent_na);
                    break;
                case BAS:
                
                   /*if((Serpent_na)->y_na == taille_na - 2)
                        ajouter_cellule_na((Serpent_na)->x_na, 0, &Serpent_na);
                        */
                    //else
                        ajouter_cellule_na((Serpent_na)->x_na, (Serpent_na)->y_na + 1, &Serpent_na);
                    break;
                case GAUCHE:
                   /* if((Serpent_na)->x_na == 0)
                        ajouter_cellule_na(taille_na - 2, (Serpent_na)->y_na, &Serpent_na);
                       */ 
                   // else
                        ajouter_cellule_na((Serpent_na)->x_na - 1, (Serpent_na)->y_na, &Serpent_na);
                        
                    break;
                case DROITE:
           
                   /* if((Serpent_na)->x_na == taille_na - 2)
                       ajouter_cellule_na(0, (Serpent_na)->y_na, &Serpent_na);
                       */
                    //else
                        ajouter_cellule_na((Serpent_na)->x_na + 1, (Serpent_na)->y_na, &Serpent_na);
                    break;
            }
            
            if(Serpent_na->x_na == nourriture_na.x_na && Serpent_na->y_na == nourriture_na.y_na) {

                nourriture_na = ajout_Nourriture_na(nourriture_na, Serpent_na);
              
                
            }

            else if(mourrir_serpent_na(Serpent_na,renderer_na, &bonus_mur_na)) {
                printf("Game Over! Le serpent s'est mordu!\n");
                printf(" Score: %d\n", score_na);
                running = false;
                continue;
            }
            else {
                suppression_queue_na(&Serpent_na);
            }
            
            
            Temp_final_na = Temp_present_na;
        }
            SDL_SetRenderDrawColor(renderer_na, 0, 0, 0, 255);
            SDL_RenderClear(renderer_na);
            dessiner_grille_na(renderer_na);
            dessin_nourriture_na(renderer_na, nourriture_na);
            dessin_Serpent_na(renderer_na, Serpent_na);
            dessiner_score_na(renderer_na);
            //dessiner_bonus_na(renderer_na, bonus_mur_na);
            //dessiner_indicateur_bonus_na(renderer_na, bonus_mur_na);
            dessiner_bonus_na(renderer_na, bonus_mur_na);
            dessiner_indicateur_bonus_na(renderer_na, bonus_mur_na);
            SDL_RenderPresent(renderer_na);
        if (pause_system.isPaused) {

        SDL_SetRenderDrawColor(renderer_na, 255, 255, 255, 255);
        SDL_Rect pauseRect1 = {Hauteur_na_na/2 - 20, Hauteur_na/2 - 30, 15, 60};
        SDL_Rect pauseRect2 = {Hauteur_na_na/2 + 20, Hauteur_na/2 - 30, 15, 60};
        SDL_RenderFillRect(renderer_na, &pauseRect1);
        SDL_RenderFillRect(renderer_na, &pauseRect2);
    }

        SDL_Delay(16);
    }
    statut_na = EXIT_SUCCESS;

Quit:
    if(Serpent_na != NULL) {
        while(Serpent_na != NULL) {
            Cellule_na* temp = Serpent_na;
            Serpent_na = Serpent_na->suivant;
            free(temp);
        }
    }
    if(renderer_na != NULL)
        SDL_DestroyRenderer(renderer_na);
    if(window_na != NULL)
        SDL_DestroyWindow(window_na);
    SDL_Quit();
}

void dessiner_grille_na(SDL_Renderer *renderer_na) {
    if (renderer_na == NULL) {
        return;
    }
    SDL_SetRenderDrawColor(renderer_na, 40, 40, 40, 255);
    SDL_Rect fond = {0, 0, Hauteur_na_na, Hauteur_na};
    SDL_RenderFillRect(renderer_na, &fond);
    SDL_Rect rect;
    for (int i = 0; i < Hauteur_na/pixel_na; ++i) {
        for (int j = 0; j < Hauteur_na_na/pixel_na; ++j) {
            rect.x = j * pixel_na;
            rect.y = i * pixel_na;
            rect.w = pixel_na;
            rect.h = pixel_na;
            if ((i + j) % 2 == 0) {
                SDL_SetRenderDrawColor(renderer_na, 45, 45, 45, 255);
                SDL_RenderFillRect(renderer_na, &rect);
            }
        }
    }

    SDL_SetRenderDrawColor(renderer_na, 70, 70, 70, 255);
    
    for (int i = 0; i <= Hauteur_na/pixel_na; i++) {
        SDL_RenderDrawLine(renderer_na, 0, i*pixel_na, Hauteur_na_na, i*pixel_na);
    }
    
    for (int i = 0; i <= Hauteur_na_na/pixel_na; i++) {
        SDL_RenderDrawLine(renderer_na, i*pixel_na, 0, i*pixel_na, Hauteur_na);
    }


    SDL_SetRenderDrawColor(renderer_na, 100, 100, 100, 255);
    SDL_Rect border = {0, 0, Hauteur_na_na - 1, Hauteur_na - 1};
    SDL_RenderDrawRect(renderer_na, &border);
}



void ajouter_cellule_na(int x1_na, int y1_na, Cellule_na **Serpent){
    Cellule_na *nouveau = (Cellule_na *)malloc(sizeof(Cellule_na));
    assert(nouveau != NULL);
    nouveau -> x_na = x1_na;
    nouveau -> y_na = y1_na;
    nouveau -> suivant = *Serpent;
    *Serpent = nouveau;
}


void suppression_queue_na(Cellule_na **Serpent_na) {
    if (*Serpent_na == NULL) {
        printf("Liste vide ");
        return;
    }
    if ((*Serpent_na)->suivant == NULL) {
        free(*Serpent_na);
        *Serpent_na = NULL;
        return;
    }
    Cellule_na *tempo = *Serpent_na;
    Cellule_na *tempoPre = NULL;
    while (tempo->suivant != NULL) {
        tempoPre = tempo;
        tempo = tempo->suivant;
    }
    free(tempo);
    tempoPre->suivant = NULL;
}


void dessin_Serpent_na(SDL_Renderer *renderer_na, const Cellule_na *Serpent_na) {
    if (renderer_na == NULL || Serpent_na == NULL) {
        return;
    }
    const Cellule_na *present_na = Serpent_na;
    SDL_Rect rect;
    int isHead = 1; 
    while (present_na != NULL) {
        rect.x = present_na->x_na * pixel_na;
        rect.y = present_na->y_na * pixel_na;
        rect.w = pixel_na;
        rect.h = pixel_na;
        if (rect.x >= 0 && rect.y >= 0) {
            if (isHead) {
                SDL_SetRenderDrawColor(renderer_na, 255, 0, 0, 255); 
                isHead = 0; 
            } else {
                SDL_SetRenderDrawColor(renderer_na, 0, 255, 0, 255); 
            }
            SDL_RenderFillRect(renderer_na, &rect);
            
            SDL_SetRenderDrawColor(renderer_na, 0, 200, 0, 255);
            SDL_RenderDrawRect(renderer_na, &rect);
        }
        present_na = present_na->suivant;
    }
}
void direction_na(Cellule_na **Serpent_na, char touche) {
    if (Serpent_na == NULL || *Serpent_na == NULL) {
        return;
    }

    int nouvelle_x = (*Serpent_na)->x_na;
    int nouvelle_y = (*Serpent_na)->y_na;

    switch(touche) {
        case 'z': 
            nouvelle_y = nouvelle_y - 1;
            if (nouvelle_y < 0) {
                nouvelle_y = taille_na - 1;
            }
            break;
        case 's': 
            nouvelle_y = nouvelle_y + 1;
            if (nouvelle_y >= taille_na) {
                nouvelle_y = 0;
            }
            break;
        case 'q': 
            nouvelle_x = nouvelle_x - 1;
            if (nouvelle_x < 0) {
                nouvelle_x = taille_na - 1;
            }
            break;
        case 'd': 
            nouvelle_x = nouvelle_x + 1;
            if (nouvelle_x >=taille_na) {
                nouvelle_x = 0;
            }
            break;
        default:
            return;
    }

    ajouter_cellule_na(nouvelle_x, nouvelle_y, Serpent_na);
    suppression_queue_na(Serpent_na);
}



Nourriture_na ajout_Nourriture_na(Nourriture_na food_na, Cellule_na *Serpent_na) {
    srand(time(NULL));
    score_na += 1;
    food_na.x_na = rand() % (taille_na - 2) + 1;
    food_na.y_na = rand() % (taille_na - 2) + 1;
    Cellule_na *courant_na = Serpent_na;
    while (courant_na != NULL) {
        if (courant_na->x_na == food_na.x_na && courant_na->y_na == food_na.y_na) {
            food_na.x_na = rand() % (taille_na - 2) + 1;
            food_na.y_na = rand() % (taille_na - 2) + 1;
        }
        courant_na = courant_na->suivant;
    }
    return food_na;
}

void dessin_nourriture_na(SDL_Renderer *renderer_na, Nourriture_na food_na) {
    SDL_Rect rect;
    rect.x = food_na.x_na * pixel_na;
    rect.y = food_na.y_na * pixel_na;
    rect.w = pixel_na;
    rect.h = pixel_na;
    SDL_SetRenderDrawColor(renderer_na, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer_na, &rect);
}




bool mourrir_serpent_na(Cellule_na *Serpent_na, SDL_Renderer *renderer_na, Bonus_traversee_na *bonus_na) {
    if (Serpent_na == NULL || Serpent_na->suivant == NULL) {
        return false;
    }
    
    Cellule_na *Tete_na = Serpent_na;
    Cellule_na *corps_na = Serpent_na->suivant;
    
    // Si le bonus est actif, gérer la traversée des murs
    if (bonus_na->actif_na) {
        if (Tete_na->x_na < 0) Tete_na->x_na = taille_na - 1;
        if (Tete_na->x_na >= taille_na) Tete_na->x_na = 0;
        if (Tete_na->y_na < 0) Tete_na->y_na = taille_na - 1;
        if (Tete_na->y_na >= taille_na) Tete_na->y_na = 0;
    }
    
    while (corps_na != NULL) {
        if (Tete_na->x_na == corps_na->x_na && Tete_na->y_na == corps_na->y_na || 
            (!bonus_na->actif_na && ((Serpent_na)->y_na == 0 || 
                             (Serpent_na)->y_na == taille_na-1 || 
                             (Serpent_na)->x_na == 0 || 
                             (Serpent_na)->x_na == taille_na-1))) {
            return true;
        }
        corps_na = corps_na->suivant;
    }
    return false;
}

void dessiner_texte_na(SDL_Renderer* renderer_na, const char* texte, int x, int y) {
    int len = strlen(texte);
    for(int i = 0; i < len; i++) {
        if(texte[i] >= '0' && texte[i] <= '9') {
            dessiner_chiffre_na(renderer_na, x + (i * 30), y, texte[i] - '0');
        }
    }
}


