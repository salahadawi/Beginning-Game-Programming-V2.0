
SRCS = main.c
LIBS = -lSDL2 ../libft/libft.a
NAME = SDL2
FLAGS = -Wall -Werror -Wextra
RUN_LIB= make -C ../libft/ fclean && make -C ../libft/

all:
	@$(RUN_LIB)
	gcc $(SRCS) $(FLAGS) $(LIBS) -o $(NAME)

noflags:
	@$(RUN_LIB)
	gcc $(SRCS) $(LIBS) -o $(NAME)