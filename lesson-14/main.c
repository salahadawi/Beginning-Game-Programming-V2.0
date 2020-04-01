
#include "sdl.h"

void	handle_error(char *message)
{
	ft_printf("%s SDL_ERROR %s\n", message, SDL_GetError());
	exit(0);
}

void init_texture(t_texture **texture)
{
	*texture = (t_texture*)ft_memalloc(sizeof(t_texture));
	(*texture)->texture = NULL;
	(*texture)->width = 0;
	(*texture)->height = 0;
}

void	init(t_sdl *sdl)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		handle_error("SDL could not initialize!");
	sdl->window = SDL_CreateWindow("SDL Tutorial", 700, 200, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (!sdl->window)
		handle_error("Window could not be created!");
	if (!(sdl->renderer = SDL_CreateRenderer(sdl->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)))
		handle_error("Renderer could not be created!");
	SDL_SetRenderDrawColor(sdl->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	sdl->img_flags = IMG_INIT_PNG;
	if (!(IMG_Init(sdl->img_flags) & sdl->img_flags))
		handle_error("SDL image could not initialize!");
	for (int i = 0; i < TEXTURES_AMOUNT; i++)
		init_texture(&sdl->textures[i]);
	sdl->screen = SDL_GetWindowSurface(sdl->window);
}

void	free_texture(t_texture *texture)
{	
	if (texture->texture)
	{
		SDL_DestroyTexture(texture->texture);
		texture->texture = NULL;
		texture->width = 0;
		texture->height = 0;
	}
}

void	set_color(t_texture *texture, uint8_t red, uint8_t green, uint8_t blue)
{
	SDL_SetTextureColorMod(texture->texture, red, green, blue);
}

void	set_blend_mode(t_texture *texture, SDL_BlendMode blending)
{
	SDL_SetTextureBlendMode(texture->texture, blending);
}

void	set_alpha(t_texture *texture, Uint8 alpha)
{
	SDL_SetTextureAlphaMod(texture->texture, alpha);
}

void	texture_load_from_file(t_sdl *sdl, t_texture *texture, char *path)
{
	SDL_Surface *loaded_surface;
	SDL_Texture	*new_texture;

	free_texture(texture);
	if (!(loaded_surface = IMG_Load(path)))
		handle_error("Unable to load image!");
	SDL_SetColorKey(loaded_surface, SDL_TRUE, SDL_MapRGB(loaded_surface->format, 0, 0xFF, 0xFF));
	if (!(new_texture = SDL_CreateTextureFromSurface(sdl->renderer, loaded_surface)))
		handle_error("Unable to create texture from surface!");
	texture->texture = new_texture;
	texture->width = loaded_surface->w;
	texture->height = loaded_surface->h;
	SDL_FreeSurface(loaded_surface);
}

void	render_texture(t_sdl *sdl, t_texture *texture, int x, int y, SDL_Rect *clip)
{
	SDL_Rect render_quad = {x, y, texture->width, texture->height};
	if (clip)
	{
		render_quad.w = clip->w;
		render_quad.h = clip->h;
	}
	SDL_RenderCopy(sdl->renderer, texture->texture, clip, &render_quad);
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
	texture_load_from_file(sdl, sdl->textures[0], "foo.png");
	sdl->sprite_clips[0].x = 0;
	sdl->sprite_clips[0].y = 0;
	sdl->sprite_clips[0].w = 64;
	sdl->sprite_clips[0].h = 205;

	sdl->sprite_clips[1].x = 64;
	sdl->sprite_clips[1].y = 0;
	sdl->sprite_clips[1].w = 64;
	sdl->sprite_clips[1].h = 205;

	sdl->sprite_clips[2].x = 128;
	sdl->sprite_clips[2].y = 0;
	sdl->sprite_clips[2].w = 64;
	sdl->sprite_clips[2].h = 205;

	sdl->sprite_clips[3].x = 196;
	sdl->sprite_clips[3].y = 0;
	sdl->sprite_clips[3].w = 64;
	sdl->sprite_clips[3].h = 205;
}

void	close_sdl(t_sdl *sdl)
{
	for (int i = 0; i < TEXTURES_AMOUNT; i++)
		free_texture(sdl->textures[i]);
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
	int	frame = 0;

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
		SDL_SetRenderDrawColor(sdl->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(sdl->renderer);
		SDL_Rect *current_clip = &sdl->sprite_clips[frame / 4];
		render_texture(sdl, sdl->textures[0], (SCREEN_WIDTH - current_clip->w) / 2,
		(SCREEN_HEIGHT - current_clip->h) / 2, current_clip);
		SDL_RenderPresent(sdl->renderer);
		if (++frame / 4 >= ANIMATION_FRAMES)
			frame = 0;
	}
	close_sdl(sdl);
}
