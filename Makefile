NAME = Wolf
CC = gcc
CFLAGS =  -I include -I libft
LDPC = -L lib -L libft -lmingw32 -lSDL2main -lSDL2  
LD = -L /Users/vboissel/.brew/Cellar/sdl2/2.0.9/lib -l SDL2-2.0.0



FLAGSSLD = -lmingw32 -lSDL2main -lSDL2  -mwindows -L lib

FLAGSMLIB = -lmlx -framework OpenGL -framework AppKit
LIBFT_DIR = ./libft/
LIBFT = ./libft/libft.a
SRC = $(wildcard ./src/*.c)
HDR = $(wildcard ./headers/*.h)
INC = ./include/
INC_MLX = /usr/local/include

all : $(NAME)

$(NAME): $(SRC) $(HDR)
	@$(MAKE) -C $(LIBFT_DIR)
	@$(CC) $(CFLAGS) $(SRC) $(LIBFT) $(LD)

clean :
	@(cd $(LIBFT_DIR) && $(MAKE) $@)

fclean : clean
	@(cd $(LIBFT_DIR) && $(MAKE) $@)
	@rm -f $(NAME)

re : fclean all

.phony : $(NAME)