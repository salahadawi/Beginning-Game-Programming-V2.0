
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
	sdl->img_flags = IMG_INIT_PNG;
	if (!(IMG_Init(sdl->img_flags) & sdl->img_flags))
		handle_error("SDL image could not initialize!");
	sdl->screen = SDL_GetWindowSurface(sdl->window);
}
SDL_Surface	*load_surface(char *path, t_sdl *sdl)
{
	SDL_Surface *loaded_surface;
	SDL_Surface	*optimized_surface;

	if (!(loaded_surface = IMG_Load(path)))
		handle_error("Unable to load image!");
	if (!(optimized_surface = SDL_ConvertSurface(loaded_surface, sdl->screen->format, 0)))
		handle_error("Unable to optimize image!");
	SDL_FreeSurface(loaded_surface);
	return optimized_surface;
}

void	load_media(t_sdl *sdl)
{
	if (!(sdl->image = load_surface("loaded.png", sdl)))
		handle_error("Failed to load default image!");
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
	SDL_Rect stretch_rect;

	stretch_rect.x = 0;
	stretch_rect.y = 0;
	stretch_rect.w = SCREEN_WIDTH;
	stretch_rect.h = SCREEN_HEIGHT;
	(void)argc;
	(void)argv;
		sdl = (t_sdl*)ft_memalloc(sizeof(t_sdl));
	init(sdl);
	load_media(sdl);
	while (1)
	{
		while (SDL_PollEvent(&sdl->e))
		{
			if (sdl->e.type == SDL_QUIT)
				close_sdl(sdl);
		}
		SDL_BlitScaled(sdl->image, NULL, sdl->screen, &stretch_rect);
		SDL_UpdateWindowSurface(sdl->window);
	}
	close_sdl(sdl);
}
