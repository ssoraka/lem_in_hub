# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ljalikak <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/04/04 13:04:11 by ljalikak          #+#    #+#              #
#    Updated: 2019/04/04 13:04:14 by ljalikak         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = gcc
CFLAGS = -Wall -Wextra -Werror
NAME = lem-in
LIB = $(LIBSRC)libft.a
HEADER = ./includes/
LIBSRC = ./libft/
SRC = ./sources/
SRC1 = check_comment_and_realnum.c \
check_xy_and_edge.c \
duplicate.c \
ft_create_ants.c \
ft_create_paths.c \
ft_create_rooms.c \
ft_create_tubes.c \
ft_del_functions.c \
ft_dijkstra_algorithm.c \
ft_find_new_paths.c \
ft_main_all_error_flow.c \
ft_move_ants.c \
ft_suurballe_algorithm.c \
ft_suurballe_algorithm2.c \
list_val.c \
splits.c \
validator.c

OBJS = $(SRC1:.c=.o)

.PHONY: clean all fclean re

all: $(NAME)

$(NAME): $(LIB) $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(LIB) $(OBJS)

$(LIB):
	make lib_refresh

%.o: $(SRC)%.c
	$(CC) $(CFLAGS) -c $< -I $(HEADER) -I $(LIBSRC)

lib_refresh:
	make -C $(LIBSRC)

norm:
	norminette -R CheckForbiddenSourceHeader

clean:
	rm -rf $(OBJS)
	make -C $(LIBSRC) clean

fclean: clean
	rm -rf $(NAME)
	make -C $(LIBSRC) fclean

re: fclean all
