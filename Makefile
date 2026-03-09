# Makefile for Codexion

NAME = codexion
CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread
SRCS_DIR = coders
SRCS = $(wildcard $(SRCS_DIR)/*.c)
OBJS = $(SRCS:.c=.o)

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
