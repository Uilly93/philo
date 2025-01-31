NAME = philo
CC = cc
SRC_DIR = src
OBJ_DIR = obj
SRC_NAME = init_philos.c check_data.c parsing_check_errors.c utils.c routine.c main.c
OBJS = $(addprefix $(OBJ_DIR)/,$(SRC_NAME:%.c=%.o))
SRC = $(addprefix $(SRC_DIR)/,$(SRC_NAME))
CFLAGS = -Wall -Wextra -Werror -g3 -D_REENTRANT
RM = rm -rf

all: $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

clean:
	$(RM) $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re