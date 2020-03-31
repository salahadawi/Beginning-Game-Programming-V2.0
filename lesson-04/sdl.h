
#ifndef SDL_H
# define SDL_H
# define SCREEN_WIDTH 640
# define SCREEN_HEIGHT 480

# include <SDL2/SDL.h>
# include "../libft/includes/libft.h"

enum				e_key_surfaces
{
	KEY_SURFACE_DEFAULT,
	KEY_SURFACE_UP,
	KEY_SURFACE_DOWN,
	KEY_SURFACE_LEFT,
	KEY_SURFACE_RIGHT,
	KEY_SURFACE_TOTAL
};

typedef struct		s_sdl
{
	SDL_Window		*window;
	SDL_Surface		*screen;
	SDL_Surface		*image;
	SDL_Event		e;
	SDL_Surface		*key_press_surfaces[KEY_SURFACE_TOTAL];
}					t_sdl;

#endif
