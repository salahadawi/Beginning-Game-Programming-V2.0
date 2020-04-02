
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

void	init_button(t_button **button, int i)
{
	*button = (t_button*)ft_memalloc(sizeof(t_button));
	if (!i)
	{
		(*button)->position.x = 0;
		(*button)->position.y = 0;
	}
	else if (i == 1)
	{
		(*button)->position.x = 320;
		(*button)->position.y = 0;
	}
	else if (i == 2)
	{
		(*button)->position.x = 0;
		(*button)->position.y = 240;
	}
	else if (i == 3)
	{
		(*button)->position.x = 320;
		(*button)->position.y = 240;
	}
	(*button)->current_sprite = BUTTON_SPRITE_MOUSE_OUT;
}

void	set_position(t_button *button, int x, int y)
{
	button->position.x = x;
	button->position.y = y;
}

void	handle_event(t_button *button, SDL_Event *e)
{
	int x;
	int y;

	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN ||
		e->type == SDL_MOUSEBUTTONUP)
	{
		SDL_GetMouseState(&x, &y);
		if (x < button->position.x || x > button->position.x + BUTTON_WIDTH ||
			y < button->position.y || y > button->position.y + BUTTON_HEIGHT)
			button->current_sprite = BUTTON_SPRITE_MOUSE_OUT;
		else
		{
			switch(e->type)
			{
				case SDL_MOUSEMOTION:
				button->current_sprite = BUTTON_SPRITE_MOUSE_OVER;
				break;
				case SDL_MOUSEBUTTONDOWN:
				button->current_sprite = BUTTON_SPRITE_MOUSE_DOWN;
				break;
				case SDL_MOUSEBUTTONUP:
				button->current_sprite = BUTTON_SPRITE_MOUSE_UP;
			}
		}
	}
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
	for (int i = 0; i < TOTAL_BUTTONS; i++)
		init_button(&sdl->buttons[i], i);
	#if defined(_SDL_TTF_H)
	if (TTF_Init() == -1)
		handle_error("SDL_ttf could not initialize!");
	#endif
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
	
	free_texture(texture);
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
	if (!(sdl->font = TTF_OpenFont("lazy.ttf", 28)))
		handle_error("Failed to load lazy font!");
	SDL_Color text_color = {0, 0, 0, 255};
	load_from_rendered_text(sdl, sdl->textures[0], "Press Enter to Reset Start Time.",text_color);
}

void	close_sdl(t_sdl *sdl)
{
	for (int i = 0; i < TEXTURES_AMOUNT; i++)
		free_texture(sdl->textures[i]);
	#if defined(_SDL_TTF_H)
	TTF_CloseFont(sdl->font);
	TTF_Quit();
	#endif
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
	SDL_Color text_color = {0, 0, 0, 255};
	Uint32	start_time = 0;
	char *time_text = malloc(100);

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
			else if (sdl->e.type == SDL_KEYDOWN && sdl->e.key.keysym.sym == SDLK_RETURN)
				start_time = SDL_GetTicks();
		}
		time_text = ft_sprintf("Milliseconds since start time %d", SDL_GetTicks() - start_time);
		load_from_rendered_text(sdl, sdl->textures[1], time_text, text_color);
		SDL_SetRenderDrawColor(sdl->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(sdl->renderer);
		render_texture(sdl, sdl->textures[0], (SCREEN_WIDTH - sdl->textures[0]->width) / 2,
		0, NULL);
		render_texture(sdl, sdl->textures[1], (SCREEN_WIDTH - sdl->textures[0]->width) / 2,
		(SCREEN_HEIGHT - sdl->textures[1]->height) / 2, NULL);
		SDL_RenderPresent(sdl->renderer);
	}
	close_sdl(sdl);
}
