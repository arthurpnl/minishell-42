# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: arpenel <arpenel@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/23 15:56:54 by arpenel           #+#    #+#              #
#    Updated: 2025/10/23 15:56:54 by arpenel          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= minishell

CC			= cc
CFLAGS		= -Wall -Wextra -Werror -g
INCLUDES	= -I includes -I Libft/includes
LIBS		= -lreadline -L Libft -lft

SRC_DIR		= src
PARSING_DIR	= $(SRC_DIR)/parsing
EXEC_DIR	= $(SRC_DIR)/exec
BUILTIN_DIR	= $(EXEC_DIR)/builtin
OBJ_DIR		= obj
LIBFT_DIR	= Libft

LIBFT		= $(LIBFT_DIR)/libft.a

PARSING_SRCS	= $(PARSING_DIR)/check_syntax_2.c \
				  $(PARSING_DIR)/check_syntax.c \
				  $(PARSING_DIR)/clean_space_2.c \
				  $(PARSING_DIR)/clean_space.c \
				  $(PARSING_DIR)/expand_2.c \
				  $(PARSING_DIR)/expand.c \
				  $(PARSING_DIR)/free.c \
				  $(PARSING_DIR)/ft_split_token.c \
				  $(PARSING_DIR)/ft_split_word.c \
				  $(PARSING_DIR)/main.c \
				  $(PARSING_DIR)/quote_and_env.c \
				  $(PARSING_DIR)/token_2.c \
				  $(PARSING_DIR)/token_to_command_2.c \
				  $(PARSING_DIR)/token_to_command.c \
				  $(PARSING_DIR)/token_to_command3.c \
				  $(PARSING_DIR)/token_word.c \
				  $(PARSING_DIR)/token.c \
				  $(PARSING_DIR)/utils.c

EXEC_SRCS		= $(EXEC_DIR)/builtin_exec.c \
				  $(EXEC_DIR)/cleanup.c \
				  $(EXEC_DIR)/cmd_type.c \
				  $(EXEC_DIR)/exec_child.c \
				  $(EXEC_DIR)/exec_cmd.c \
				  $(EXEC_DIR)/exec_signal.c \
				  $(EXEC_DIR)/exec_utils.c \
				  $(EXEC_DIR)/exec.c \
				  $(EXEC_DIR)/free.c \
				  $(EXEC_DIR)/here_doc.c \
				  $(EXEC_DIR)/path.c \
				  $(EXEC_DIR)/pipeline.c \
				  $(EXEC_DIR)/redirect_pipes.c \
				  $(EXEC_DIR)/redirect.c \
				  $(EXEC_DIR)/utils.c \
				  $(EXEC_DIR)/utils2.c
				  
BUILTIN_SRCS	= $(BUILTIN_DIR)/cd_utils.c \
				  $(BUILTIN_DIR)/cd.c \
				  $(BUILTIN_DIR)/echo.c \
				  $(BUILTIN_DIR)/env.c \
				  $(BUILTIN_DIR)/exit.c \
				  $(BUILTIN_DIR)/export_utils.c \
				  $(BUILTIN_DIR)/export_utils2.c \
				  $(BUILTIN_DIR)/export.c \
				  $(BUILTIN_DIR)/pwd.c \
				  $(BUILTIN_DIR)/unset.c

SRCS			= $(PARSING_SRCS) $(EXEC_SRCS) $(BUILTIN_SRCS)

OBJS			= $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)
	@echo "\033[0;32m✓ $(NAME) compiled successfully\033[0m"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
	@echo "\033[0;32m✓\033[0m Compiled: $<"

$(LIBFT):
	@echo "\033[0;32mCompiling libft...\033[0m"
	@$(MAKE) -C $(LIBFT_DIR) --no-print-directory
	@echo "\033[0;32m✓ libft compiled\033[0m"

clean:
	@rm -rf $(OBJ_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean --no-print-directory
	@echo "\033[0;31m✗ Object files removed\033[0m"

fclean: clean
	@$(MAKE) -C $(LIBFT_DIR) fclean --no-print-directory
	@rm -f $(NAME)
	@echo "\033[0;31m✗ $(NAME) removed\033[0m"

re: fclean all

.PHONY: all clean fclean re