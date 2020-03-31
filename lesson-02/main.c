
#include "test.h"

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

void	load_media(t_sdl *sdl)
{
	if (!(sdl->image = SDL_LoadBMP("hello_world.bmp")))
		handle_error("Unable to load image!");
}

void	close_sdl(t_sdl *sdl)
{
	SDL_FreeSurface(sdl->image);
	sdl->image = NULL;
	SDL_DestroyWindow(sdl->window);
	SDL_Quit();
}

int	main(int argc, char **argv)
{
	t_sdl *sdl;

	(void)argc;
	(void)argv;
	sdl = (t_sdl*)ft_memalloc(sizeof(t_sdl));
	init(sdl);
	load_media(sdl);
	SDL_BlitSurface(sdl->image, NULL, sdl->screen, NULL);
	SDL_UpdateWindowSurface(sdl->window);
	SDL_Delay(2000);
	close_sdl(sdl);
	return (0);
}