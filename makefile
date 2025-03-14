# Nom du programme
NAME = philo

# Compilateur et flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread

# Sources
SRCS = main.c \
       init.c \
       philosopher_action.c \
	   philosopher_routine2.c \
	   philosopher_action2.c \
       philosopher_routine.c \
       simulation_monitor.c \
	   gettime.c \
	   philosopher_action3.c


# Objets
OBJS = $(SRCS:.c=.o)

# Headers
HEADERS = philo.h
INCLUDES = -I.

# Règles
all: $(NAME)

# Compilation du programme principal
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

# Compilation des objets
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
