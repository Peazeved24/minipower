CC = cc
CFLAGS = -Wall -Wextra -Werror

SERVER_BONUS_SRC = serverbonus.c src.c srcserver.c
CLIENT_BONUS_SRC = clientbonus.c src.c srcclient.c

OBJS_SBONUS = $(SERVER_BONUS_SRC:.c=.o)
OBJS_CBONUS = $(CLIENT_BONUS_SRC:.c=.o)


NAME_SBONUS = serve
NAME_CBONUS = client

all: $(NAME_SBONUS) $(NAME_CBONUS)

$(NAME_SBONUS): $(OBJS_SBONUS)
	$(CC) $(CFLAGS) $^ -o $@

$(NAME_CBONUS): $(OBJS_CBONUS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS_SBONUS) $(OBJS_CBONUS)

fclean: clean
	rm -f $(NAME_SBONUS) $(NAME_CBONUS)

re: fclean all

.PHONY: all clean fclean re
