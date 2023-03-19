NAME = minishell

CC = gcc
CFLAGS = -Wall -Werror -Wextra

SRCS = $(addprefix ./src/, testing.c)
SRCS_UTILS = $(addprefix ./src/utils/, free_fts.c command_spliter.c inputs_utils.c pipes_and_redirs.c)
SRCS_LIST = $(addprefix ./src/list/, list_filler.c list_moves.c list_utils.c)

OBJS = $(addsuffix .o, $(notdir $(basename $(SRCS))))
OBJS_UTILS = $(addsuffix .o, $(notdir $(basename $(SRCS_UTILS))))
OBJS_LIST = $(addsuffix .o, $(notdir $(basename $(SRCS_LIST))))

LIBFT = ./includes/libft_plus/libft.a
LIBFT_LINK = -L./includes/libft_plus -lft
FT_PRINTF_LINK = -L./includes/libft_plus/ft_printf -lftprintf

all:		$(NAME)

$(OBJS_LIST):	$(SRCS_LIST)
			@$(CC) $(CFLAGS) -c $(SRCS_LIST)

$(OBJS_UTILS):	$(SRCS_UTILS)
			@$(CC) $(CFLAGS) -c $(SRCS_UTILS)

$(OBJS):	$(SRCS)
			@$(CC) $(CFLAGS) -c $(SRCS)

$(NAME):	$(OBJS) $(OBJS_UTILS) $(OBJS_LIST) $(LIBFT)
			@$(CC) $(CFLAGS) $(OBJS) $(OBJS_UTILS) $(OBJS_LIST) $(LIBFT_LINK) $(FT_PRINTF_LINK) -lreadline -o $(NAME)
			@echo "Minishell compiled!"

$(LIBFT):
			@make -C ./includes/libft_plus

clean:
			@rm -f $(OBJS) $(OBJS_UTILS) $(OBJS_LIST)
			@make -C ./includes/libft_plus clean

fclean:		clean
			@rm -f $(NAME)
			@make -C ./includes/libft_plus fclean
			@echo "Everything cleaned!"

re:			fclean all

.PHONY: all, clean, fclean, re
