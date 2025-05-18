# include <stdio.h>
# include <stdlib.h>
# include <SDL2/SDL.h>
# include "fonction_nana.h"
#include <stdbool.h>

int main(){
	SDL_Window * window_na  =  NULL;
	SDL_Renderer *renderer_na = NULL;

	initSDL_na(window_na, renderer_na);	
	return 0;
}
