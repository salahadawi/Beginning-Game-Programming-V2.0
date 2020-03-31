
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

SDL_Texture	*load_texture(char *path, t_sdl *sdl)
{
	SDL_Texture *new_texture;
	SDL_Surface	*loaded_surface;

	if (!(loaded_surface = IMG_Load(path)))
		handle_error("Unable to load image!");
	if (!(new_texture = SDL_CreateTextureFromSurface(sdl->renderer, loaded_surface)))
		handle_error("Unable to create texture!");
	SDL_FreeSurface(loaded_surface);
	return (new_texture);
}

void	load_media(t_sdl *sdl)
{
	if (!(sdl->texture = load_texture("viewport.png", sdl)))
		handle_error("Failed to load texture image!");
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
	load_media(sdl);
	while (1)
	{
		while (SDL_PollEvent(&sdl->e))
		{
			if (sdl->e.type == SDL_QUIT)
				close_sdl(sdl);
		}
		SDL_RenderClear(sdl->renderer);
		SDL_Rect top_left_viewport;
		top_left_viewport.x = 0;
		top_left_viewport.y = 0;
		top_left_viewport.w = SCREEN_WIDTH / 2;
		top_left_viewport.h = SCREEN_HEIGHT / 2;
		SDL_RenderSetViewport(sdl->renderer, &top_left_viewport);
		SDL_RenderCopy(sdl->renderer, sdl->texture, NULL, NULL);
		SDL_Rect top_right_viewport;
		top_right_viewport.x = SCREEN_WIDTH / 2;
		top_right_viewport.y = 0;
		top_right_viewport.w = SCREEN_WIDTH / 2;
		top_right_viewport.h = SCREEN_HEIGHT / 2;
		SDL_RenderSetViewport(sdl->renderer, &top_right_viewport);
		SDL_RenderCopy(sdl->renderer, sdl->texture, NULL, NULL);
		SDL_Rect bottom_viewport;
		bottom_viewport.x = 0;
		bottom_viewport.y = SCREEN_HEIGHT / 2;
		bottom_viewport.w = SCREEN_WIDTH;
		bottom_viewport.h = SCREEN_HEIGHT / 2;
		SDL_RenderSetViewport(sdl->renderer, &bottom_viewport);
		SDL_RenderCopy(sdl->renderer, sdl->texture, NULL, NULL);
		SDL_RenderPresent(sdl->renderer);
	}
	close_sdl(sdl);
}
