NAME = philo

CC = cc

FLAGS = -Wall -Wextra -Werror -g

OBJS_DIR = ./bin

DEPS =	philosophers.h \
		Makefile

vpath %.c src
vpath %.o bin
vpath %.h src

SRCS =	main.c \
		allocate_heap.c \
		args_errors_utils.c \
		cleaning_utils.c \
		data_init_tools_1.c \
		data_init_tools_2.c \
		forks_utils.c \
		free_utils.c \
		ft_atoi_flag.c \
		routine.c \
		utils_1.c \
		utils_2.c

OBJS = $(addprefix $(OBJS_DIR)/, $(SRCS:.c=.o))

all: makedir $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME)

makedir:
	mkdir -p $(OBJS_DIR)

$(OBJS_DIR)/%.o: %.c $(DEPS)
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re makedir
