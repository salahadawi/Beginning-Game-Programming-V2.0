
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
	sdl->img_flags = IMG_INIT_PNG;
	if (!(IMG_Init(sdl->img_flags) & sdl->img_flags))
		handle_error("SDL image could not initialize!");
	if (!(sdl->renderer = SDL_CreateRenderer(sdl->window, -1, SDL_RENDERER_ACCELERATED)))
		handle_error("Renderer could not be created!");
	SDL_SetRenderDrawColor(sdl->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
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
	sdl->key_press_surfaces[KEY_SURFACE_DEFAULT] = load_surface("stretch.bmp", sdl);
	sdl->key_press_surfaces[KEY_SURFACE_UP] = load_surface("up.bmp", sdl);
	sdl->key_press_surfaces[KEY_SURFACE_DOWN] = load_surface("down.bmp", sdl);
	sdl->key_press_surfaces[KEY_SURFACE_LEFT] = load_surface("left.bmp", sdl);
	sdl->key_press_surfaces[KEY_SURFACE_RIGHT] = load_surface("right.bmp", sdl);
	if (!(sdl->texture = load_texture("texture.png", sdl)))
		handle_error("Failed to load texture image!");
}

int	main(int argc, char **argv)
{
	t_sdl *sdl;
	SDL_Event e;

	(void)argc;
	(void)argv;
	sdl = (t_sdl*)ft_memalloc(sizeof(t_sdl));
	init(sdl);
	load_media(sdl);
	sdl->image = sdl->key_press_surfaces[KEY_SURFACE_DEFAULT];
	while (1)
	{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
				exit(0);
			else if(e.type == SDL_KEYDOWN)
			{
				if (e.key.keysym.sym == SDLK_UP)
					sdl->image = sdl->key_press_surfaces[KEY_SURFACE_UP];
				else if (e.key.keysym.sym == SDLK_DOWN)
					sdl->image = sdl->key_press_surfaces[KEY_SURFACE_DOWN];
				else if (e.key.keysym.sym == SDLK_LEFT)
					sdl->image = sdl->key_press_surfaces[KEY_SURFACE_LEFT];
				else if (e.key.keysym.sym == SDLK_RIGHT)
					sdl->image = sdl->key_press_surfaces[KEY_SURFACE_RIGHT];
				else
					sdl->image = sdl->key_press_surfaces[KEY_SURFACE_DEFAULT];
			}
		}
		/*if (sdl->image == sdl->key_press_surfaces[KEY_SURFACE_DEFAULT])
		{
			SDL_Rect stretch_rect;
			stretch_rect.x = 0;
			stretch_rect.y = 0;
			stretch_rect.w = SCREEN_WIDTH;
			stretch_rect.h = SCREEN_HEIGHT;
			SDL_BlitScaled(sdl->image, NULL, sdl->screen, &stretch_rect);
		}
		else
			SDL_BlitSurface(sdl->image, NULL, sdl->screen, NULL);
		SDL_UpdateWindowSurface(sdl->window);*/
		SDL_RenderClear(sdl->renderer);
		SDL_RenderCopy(sdl->renderer, sdl->texture, NULL, NULL);
		SDL_RenderPresent(sdl->renderer);
	}
	close_sdl(sdl);
	return (0);
}