
#ifndef SDL_H
# define SDL_H
# define SCREEN_WIDTH 640
# define SCREEN_HEIGHT 480
# define TEXTURES_AMOUNT 1
# define DOTS_AMOUNT 1

# include <SDL2/SDL.h>
# include <SDL2/SDL_image.h>
# include "../libft/includes/libft.h"

enum				e_button_sprite
{
	BUTTON_SPRITE_MOUSE_OUT,
	BUTTON_SPRITE_MOUSE_OVER,
	BUTTON_SPRITE_MOUSE_DOWN,
	BUTTON_SPRITE_MOUSE_UP,
	BUTTON_SPRITE_MOUSE_TOTAL
};

enum				e_textures
{
	PRESS_TEXTURE,
	UP_TEXTURE,
	DOWN_TEXTURE,
	LEFT_TEXTURE,
	RIGHT_TEXTURE
};

typedef	struct		s_texture
{
	SDL_Texture		*texture;
	int				width;
	int				height;
}					t_texture;

typedef struct		s_dot
{
	#define 		DOT_WIDTH 20
	#define			DOT_HEIGHT 20
	#define			DOT_VEL 10
	int				pos_x;
	int				pos_y;
	int				vel_x;
	int				vel_y;
}					t_dot;


typedef struct		s_sdl
{
	SDL_Window		*window;
	SDL_Surface		*screen;
	SDL_Surface		*image;
	SDL_Event		e;
	int				img_flags;
	SDL_Renderer	*renderer;
	t_texture		*textures[TEXTURES_AMOUNT];
	t_dot			*dots[DOTS_AMOUNT];
}					t_sdl;

#endif
