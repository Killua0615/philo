# **************************************************************************** #
#                                   Makefile                                   #
# **************************************************************************** #

NAME        = philo
CC          = cc
CFLAGS      = -Wall -Wextra -Werror -pthread

SRCS        = main.c \
              philo.c \
              utils.c \
              init.c \
              threads.c \
              monitor.c

OBJS        = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS) philo.h
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
