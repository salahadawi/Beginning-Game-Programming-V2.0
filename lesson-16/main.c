
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
	if (TTF_Init() == -1)
		handle_error("SDL_ttf could not initialize!");
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

void	load_from_rendered_text(t_sdl *sdl, t_texture *texture, char *text, SDL_Color text_color)
{
	SDL_Surface *text_surface;
	
	free(texture);
	if (!(text_surface = TTF_RenderText_Solid(sdl->font, text, text_color)))
		handle_error("Unable to render text surface!");
	if (!(texture->texture = SDL_CreateTextureFromSurface(sdl->renderer, text_surface)))
		handle_error("Unable to create texture from rendered text!");
	texture->width = text_surface->w;
	texture->height = text_surface->h;
	SDL_FreeSurface(text_surface);
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

void	render_texture(t_sdl *sdl, t_texture *texture, int x, int y, SDL_Rect *clip,
double angle, SDL_Point *center, SDL_RendererFlip flip)
{
	SDL_Rect render_quad = {x, y, texture->width, texture->height};
	if (clip)
	{
		render_quad.w = clip->w;
		render_quad.h = clip->h;
	}
	SDL_RenderCopyEx(sdl->renderer, texture->texture, clip, &render_quad, angle, center, flip);
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
	if (!(sdl->font = TTF_OpenFont("lazy.ttf", 28)))
		handle_error("Failed to load lazy font!");
	SDL_Color text_color = {0, 0, 0, 0};
	load_from_rendered_text(sdl, sdl->textures[0],
	"The quick brown fox jumps over the lazy dog" ,text_color);
}

void	close_sdl(t_sdl *sdl)
{
	for (int i = 0; i < TEXTURES_AMOUNT; i++)
		free_texture(sdl->textures[i]);
	TTF_CloseFont(sdl->font);
	SDL_DestroyRenderer(sdl->renderer);
	sdl->renderer = NULL;
	SDL_FreeSurface(sdl->image);
	sdl->image = NULL;
	SDL_DestroyWindow(sdl->window);
	sdl->window = NULL;
	free(sdl);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	exit(0);
}

int	main(int argc, char **argv)
{
	t_sdl *sdl;
	double degrees = 0;
	SDL_RendererFlip flip_type = SDL_FLIP_NONE;

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
			else if (sdl->e.type == SDL_KEYDOWN)
			{
				switch (sdl->e.key.keysym.sym)
				{
					case SDLK_a:
					degrees -= 60;
					break;
					case SDLK_d:
					degrees += 60;
					break;
					case SDLK_q:
					flip_type = SDL_FLIP_HORIZONTAL;
					break;
					case SDLK_w:
					flip_type = SDL_FLIP_NONE;
					break;
					case SDLK_e:
					flip_type = SDL_FLIP_VERTICAL;
					break;
				}
			}
		}
		SDL_SetRenderDrawColor(sdl->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(sdl->renderer);
		render_texture(sdl, sdl->textures[0], (SCREEN_WIDTH - sdl->textures[0]->width) / 2,
		(SCREEN_HEIGHT - sdl->textures[0]->height) / 2, NULL, degrees, NULL, flip_type);
		SDL_RenderPresent(sdl->renderer);
	}
	close_sdl(sdl);
}
