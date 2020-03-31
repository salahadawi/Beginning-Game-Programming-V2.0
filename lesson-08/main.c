
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
	if (!(sdl->renderer = SDL_CreateRenderer(sdl->window, -1, SDL_RENDERER_ACCELERATED)))
		handle_error("Renderer could not be created!");
	SDL_SetRenderDrawColor(sdl->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	sdl->img_flags = IMG_INIT_PNG;
	if (!(IMG_Init(sdl->img_flags) & sdl->img_flags))
		handle_error("SDL image could not initialize!");
	sdl->screen = SDL_GetWindowSurface(sdl->window);
}

void	close_sdl(t_sdl *sdl)
{
		SDL_DestroyTexture(sdl->texture);
	sdl->texture = NULL;
	SDL_DestroyRenderer(sdl->renderer);
	sdl->renderer = NULL;
	SDL_FreeSurface(sdl->image);
	sdl->image = NULL;
	SDL_DestroyWindow(sdl->window);
	sdl->window = NULL;
	free(sdl);
	IMG_Quit();
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
	while (1)
	{
		while (SDL_PollEvent(&sdl->e))
		{
			if (sdl->e.type == SDL_QUIT)
				close_sdl(sdl);
		}
		SDL_SetRenderDrawColor(sdl->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(sdl->renderer);
		SDL_Rect fill_rect = {SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
		SDL_SetRenderDrawColor(sdl->renderer, 0xFF, 0x00, 0x00, 0xFF);
		SDL_RenderFillRect(sdl->renderer, &fill_rect);
		SDL_Rect outline_rect = {SCREEN_WIDTH / 6, SCREEN_HEIGHT / 6, SCREEN_WIDTH * 2 / 3, SCREEN_HEIGHT * 2 / 3};
		SDL_SetRenderDrawColor(sdl->renderer, 0x00, 0xFF, 0x00, 0xFF);
		SDL_RenderDrawRect(sdl->renderer, &outline_rect);
		SDL_SetRenderDrawColor(sdl->renderer, 0x00, 0x00, 0xFF, 0xFF);
		SDL_RenderDrawLine(sdl->renderer, 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2);
		SDL_SetRenderDrawColor(sdl->renderer, 0xFF, 0xFF, 0x00, 0xFF);
		for (int i = 0; i < SCREEN_HEIGHT; i += 4)
			SDL_RenderDrawPoint(sdl->renderer, SCREEN_WIDTH /2, i);
		SDL_RenderPresent(sdl->renderer);
	}
	close_sdl(sdl);
}
