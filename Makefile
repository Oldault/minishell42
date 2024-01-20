#!------------------------------------------- DEF -------------------------------------------#
NAME				= minishell
LIBFT				= $(LIBFT_DIR)libft.a

RM					= rm -f
CC					= cc
CFLAGS				= -Wall -Wextra -Werror -g3
CFLAGS				+= -I$(LIBFT_DIR) -I./include
LDFLAGS				= -lreadline

LIBFT_DIR			= ./include/libft/
SRCS_DIR			= ./src/
OBJS_PATH			= ./objs/

SRCS				= $(shell find $(SRCS_DIR) -name '*.c')
OBJS				= $(SRCS:$(SRCS_DIR)%.c=$(OBJS_PATH)%.o)

MAKEFLAGS 			+= --no-print-directory
TOTAL_FILES     	= $(words $(SRCS))
CURRENT_INDEX   	= 0

#*---------------------------------------- MINISHELL -----------------------------------------#

all:				$(LIBFT) $(NAME)

$(LIBFT):
					@make -C $(LIBFT_DIR)

$(NAME):			$(OBJS)
					@echo "\n$(BOLD)┗▷$(GREEN)『./minishell Created』[✅]$(RESET)"
					@$(CC) $(OBJS) -o $(NAME) $(LDFLAGS) -L$(LIBFT_DIR) -lft

$(OBJS_PATH)%.o:	$(SRCS_DIR)%.c
					@mkdir -p $(@D)
					@$(eval CURRENT_INDEX=$(shell echo $$(($(CURRENT_INDEX)+1))))
					@$(eval PERCENT=$(shell echo $$(($(CURRENT_INDEX) * 100 / $(TOTAL_FILES)))))
					@printf "\r$(YELLOW)🔧 $(GREEN)%3d%% $(YELLOW)$(BOLD)Compiling: $(RESET)$(BLUE)$(ITALIC)%-50s $(MAGENTA)[%3d/%3d]$(RESET)" $(PERCENT) "$<" $(CURRENT_INDEX) $(TOTAL_FILES)
					@$(CC) $(CFLAGS) -c $< -o $@

#todo------------------------------------- CLEANING ------------------------------------------#

clean:
					@echo "$(BOLD) [🗑️ ] $(YELLOW)$(REVERSED)Cleaning up object files from $(ITALIC)Minishell...$(RESET)"
					@$(RM) $(OBJS)
					@make -s fclean -C $(LIBFT_DIR)
					@echo "┗▷$(YELLOW)『\".o\" files from $(ITALIC)./Minishell/$(RESET)$(YELLOW) cleaned』$(RESET)"

fclean:				clean
					@$(RM) $(NAME)
					@$(RM) -r $(OBJS_PATH)
					@echo "┗▷$(YELLOW)『executables from $(ITALIC)./Minshell/$(RESET)$(YELLOW) cleaned』$(RESET)"

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