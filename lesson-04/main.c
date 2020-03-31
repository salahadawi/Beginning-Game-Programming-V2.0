
#include "sdl.h"

void	handle_error(char *message)
{
	ft_printf("%s SDL_ERROR %s\n", message, SDL_GetError());
	exit(0);
}

void	init(t_sdl *sdl)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		handle_error("SDL could not initialize!");
	sdl->window = SDL_CreateWindow("SDL Tutorial", 700, 200, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (!sdl->window)
		handle_error("Window could not be created!");
	sdl->screen = SDL_GetWindowSurface(sdl->window);
}

SDL_Surface	*load_surface(char *path)
{
	SDL_Surface *loaded_surface;

	if (!(loaded_surface = SDL_LoadBMP(path)))
		handle_error("Unable to load image!");
	return loaded_surface;
}

void	load_media(t_sdl *sdl)
{
	sdl->key_press_surfaces[KEY_SURFACE_DEFAULT] = load_surface("press.bmp");
	if (!sdl->key_press_surfaces[KEY_SURFACE_DEFAULT])
		handle_error("Failed to load default image!");
	sdl->key_press_surfaces[KEY_SURFACE_UP] = load_surface("up.bmp");
	if (!sdl->key_press_surfaces[KEY_SURFACE_UP])
		handle_error("Failed to load up image!");
	sdl->key_press_surfaces[KEY_SURFACE_DOWN] = load_surface("down.bmp");
	if (!sdl->key_press_surfaces[KEY_SURFACE_DOWN])
		handle_error("Failed to load down image!");
	sdl->key_press_surfaces[KEY_SURFACE_LEFT] = load_surface("left.bmp");
	if (!sdl->key_press_surfaces[KEY_SURFACE_LEFT])
		handle_error("Failed to load left image!");
	sdl->key_press_surfaces[KEY_SURFACE_RIGHT] = load_surface("right.bmp");
	if (!sdl->key_press_surfaces[KEY_SURFACE_RIGHT])
		handle_error("Failed to load right image!");
}

void	close_sdl(t_sdl *sdl)
{
	SDL_FreeSurface(sdl->image);
	sdl->image = NULL;
	SDL_DestroyWindow(sdl->window);
	SDL_Quit();
	exit(0);
}

int	main(int argc, char **argv)
{
	t_sdl *sdl;

	(void)argc;
	(void)argv;
		sdl = (t_sdl*)ft_memalloc(sizeof(t_sdl));
	init(sdl);
	load_media(sdl);
	sdl->image = sdl->key_press_surfaces[KEY_SURFACE_DEFAULT];
	while (1)
	{
		while (SDL_PollEvent(&sdl->e))
		{
			if (sdl->e.type == SDL_QUIT)
				close_sdl(sdl);
			else if (sdl->e.type == SDL_KEYDOWN)
			{
				if (sdl->e.key.keysym.sym == SDLK_UP)
					sdl->image = sdl->key_press_surfaces[KEY_SURFACE_UP];
				else if (sdl->e.key.keysym.sym == SDLK_DOWN)
					sdl->image = sdl->key_press_surfaces[KEY_SURFACE_DOWN];
				else if (sdl->e.key.keysym.sym == SDLK_LEFT)
					sdl->image = sdl->key_press_surfaces[KEY_SURFACE_LEFT];
				else if (sdl->e.key.keysym.sym == SDLK_RIGHT)
					sdl->image = sdl->key_press_surfaces[KEY_SURFACE_RIGHT];
				else
					sdl->image = sdl->key_press_surfaces[KEY_SURFACE_DEFAULT];
			}
		}
		SDL_BlitSurface(sdl->image, NULL, sdl->screen, NULL);
		SDL_UpdateWindowSurface(sdl->window);
	}
	close_sdl(sdl);
}
