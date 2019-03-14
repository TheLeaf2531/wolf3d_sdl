NAME = Wolf
CC = gcc
CFLAGS = -Wall -Wextra -Werror -O3 -I include -I libft
LD = -L lib -L libft -lmingw32 -lSDL2main -lSDL2  




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