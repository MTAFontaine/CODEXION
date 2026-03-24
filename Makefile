NAME = codexion
CC = cc -fsanitize=address
CFLAGS = -Wall -Wextra -Werror -pthread

SRCS = main.c main_utils.c fifo.c coder_utils.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
