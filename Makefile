#!------------------------------------------- DEF -------------------------------------------#
LIBFT				= $(LIBFT_DIR)libft.a

RM					= rm -f
CC					= cc
CFLAGS				= -Wall -Wextra -Werror -lreadline -g3
CFLAGS				+= -I$(LIBFT_DIR) -I./include

LIBFT_DIR			= ./include/libft/
SRCS_DIR			= ./src/
OBJS_PATH			= ./objs/

SRCS				= $(shell find $(SRCS_DIR) -name '*.c')
OBJS				= $(SRCS:$(SRCS_DIR)%.c=$(OBJS_PATH)%.o)

MAKEFLAGS 			+= --no-print-directory
TOTAL_FILES     	= $(words $(SRCS))
CURRENT_INDEX   	= 0

#*---------------------------------------- MINISHELL -----------------------------------------#

all:				$(LIBFT)

$(LIBFT):
					@make -C $(LIBFT_DIR)

#todo------------------------------------- CLEANING ------------------------------------------#

clean:
					@make -s fclean -C $(LIBFT_DIR)

fclean:				clean

re:					fclean all

.PHONY:				all clean fclean re

#?----------------------------------------- DESIGN -------------------------------------------#
# Colors
RED := \033[0;31m
GREEN := \033[0;32m
YELLOW := \033[0;33m
BLUE := \033[0;34m
MAGENTA := \033[0;35m
CYAN := \033[0;36m
WHITE := \033[0;37m
RESET := \033[0m

# Text Styles
BOLD := \033[1m
UNDERLINE := \033[4m
REVERSED := \033[7m
ITALIC := \033[3m