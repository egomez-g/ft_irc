NAME = ircserv

CC = c++

CFLAGS = -Wall -Werror -Wextra -std=c++98 -g3

SRC =	main.cpp 	\
		classes/Client.cpp	\
		classes/Server.cpp	\
		classes/ServerUtils.cpp	\
		classes/Parse.cpp	\
		classes/Cmds.cpp	\
		classes/Channel.cpp	\


OBJ = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re