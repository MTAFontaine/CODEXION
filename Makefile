NAME = codexion
CC = cc 
CFLAGS = -Wall -Wextra -Werror -pthread 
BIN_DIR = bin

SRCS = main.c\
parsing.c\
main_utils.c\
queue.c\
coder.c\
queue_utils.c\
dongles.c\
monitor.c\
cleanup.c
OBJS = $(addprefix $(BIN_DIR)/,$(SRCS:.c=.o))

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(BIN_DIR)/%.o: %.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -rf $(BIN_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
