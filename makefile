# Nom du programme
NAME = philo

# Compilateur et flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread

# Chemins
LIBFT_PATH = libft
LIBFT = $(LIBFT_PATH)/libft.a

# Sources
SRCS = main.c \
       init.c \
       philosopher_action.c \
       philosopher_routine.c \
       simulation_monitor.c

# Objets
OBJS = $(SRCS:.c=.o)

# Headers
HEADERS = philo.h
INCLUDES = -I. -I$(LIBFT_PATH)

# Règles
all: $(LIBFT) $(NAME)

# Compilation de la libft
$(LIBFT):
	make -C $(LIBFT_PATH)

# Compilation du programme principal
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -L$(LIBFT_PATH) -lft -o $(NAME)

# Compilation des objets
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	make -C $(LIBFT_PATH) clean
	rm -f $(OBJS)

fclean: clean
	make -C $(LIBFT_PATH) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
