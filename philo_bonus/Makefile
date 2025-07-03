NAME = philo_bonus

CC = cc

FLAGS = -Wall -Wextra -Werror

OBJS_DIR = ./bin

DEPS =	philosophers_bonus.h \
		Makefile

vpath %.c src_bonus
vpath %.o bin
vpath %.h src_bonus

SRCS =	children_utils_bonus.c \
		cleaning_utils_bonus.c \
		data_init_tools_1_bonus.c \
		errors_utils_bonus.c \
		ft_atoi_flag_bonus.c \
		main_bonus.c \
		threads_utils_bonus.c \
		utils_1_bonus.c \
		utils_2_bonus.c

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
