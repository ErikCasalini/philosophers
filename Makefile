# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/28 12:28:37 by ecasalin          #+#    #+#              #
#    Updated: 2025/06/29 16:58:10 by ecasalin         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME = philosophers

# BONUS = checker

CC = cc

FLAGS = -Wall -Wextra -Werror

OBJS_DIR = ./bin

# OBJS_BONUS_DIR = ./OBJECTS_BONUS

DEPS =	philosophers.h \
		Makefile

# DEPS_BONUS = $(DEPS) \
# 			push_swap_bonus.h

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

# SRCS_BONUS =	checker_bonus.c \
# 				utils_bonus.c \
# 				moves_1_bonus.c \
# 				moves_2_bonus.c \
# 				moves_3_bonus.c \
# 				chained_list_utils_1.c \
# 				chained_list_utils_2.c \
# 				ft_atoi_flag.c \
# 				exit_utils_bonus.c \
# 				free_utils.c \
# 				check_input_bonus.c

OBJS = $(addprefix $(OBJS_DIR)/, $(SRCS:.c=.o))

# OBJS_BONUS = $(addprefix $(OBJS_BONUS_DIR)/, $(SRCS_BONUS:.c=.o))

all: makedir $(NAME)

# bonus: makedir_bonus $(BONUS)

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME)

# $(BONUS): $(OBJS_BONUS) $(LIBFT_DIR)/libft.a
# 	$(CC) $(FLAGS) -L$(LIBFT_DIR)/ $(OBJS_BONUS) $(LIBS) -o $(BONUS)

makedir:
	mkdir -p $(OBJS_DIR)

# makedir_bonus:
# 	mkdir -p $(OBJS_BONUS_DIR)

$(OBJS_DIR)/%.o: %.c $(DEPS)
	$(CC) $(FLAGS) -c $< -o $@

# $(OBJS_BONUS_DIR)/%.o: %.c $(DEPS_BONUS)
# 	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS_DIR) $(OBJS_BONUS_DIR)

fclean: clean
	rm -f $(NAME) $(BONUS)

re: fclean all

.PHONY: all clean fclean re bonus makedir makedir_bonus
