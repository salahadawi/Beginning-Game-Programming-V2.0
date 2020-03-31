
#include "test.h"

void	handle_error(char *message)
{
	ft_printf("%s SDL_ERROR %s\n", message, SDL_GetError());
	exit(0);
}

void	quit_sdl(t_sdl *sdl)
{
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
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		handle_error("SDL could not initialize!");
	sdl->window = SDL_CreateWindow("SDL Tutorial", 700, 200, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (!sdl->window)
		handle_error("Window could not be created!");
	sdl->screen = SDL_GetWindowSurface(sdl->window);
	while (1)
	{
		while (SDL_PollEvent(&sdl->e))
		{
			if (sdl->e.type == SDL_QUIT)
				quit_sdl(sdl);
		}
		SDL_FillRect(sdl->screen, NULL, SDL_MapRGB(sdl->screen->format, 0xFF, 0xFF, 0xFF));
		SDL_UpdateWindowSurface(sdl->window);
	}
	quit_sdl(sdl);
}