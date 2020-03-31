
#ifndef SDL_H
# define SDL_H
# define SCREEN_WIDTH 640
# define SCREEN_HEIGHT 480

#include <SDL2/SDL.h>
#include "../libft/includes/libft.h"

typedef struct		s_sdl
{
	SDL_Window		*window;
	SDL_Surface		*screen;
	SDL_Surface		*image;
	SDL_Event		e;
}					t_sdl;

#endif