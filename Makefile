# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/19 09:35:53 by nlouis            #+#    #+#              #
#    Updated: 2025/03/25 22:19:52 by nlouis           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# **************************************************************************** #
#                          MINITALK MAKEFILE 💬                                #
# **************************************************************************** #

# Compiler
CC		:= cc
CFLAGS	:= -Wall -Wextra -Werror -g -I libft/include -I include

# Directories
OBJDIR	:= objs
LIBDIR	:= libft/lib

# Executables
NAME_CL	:= client
NAME_SV	:= server

# Sources
SRC_CL	:= srcs/client.c srcs/utils.c
SRC_SV	:= srcs/server.c srcs/utils.c

# Objects
OBJ_CL	:= $(addprefix $(OBJDIR)/, $(SRC_CL:.c=.o))
OBJ_SV	:= $(addprefix $(OBJDIR)/, $(SRC_SV:.c=.o))

# Lib
LIBFT	:= $(LIBDIR)/libft.a

# Colors
GREEN	:= \033[0;32m
CYAN	:= \033[0;36m
YELLOW	:= \033[1;33m
RESET	:= \033[0m

# Default rule
.DEFAULT_GOAL := all

# Build rules
all: $(NAME_CL) $(NAME_SV)

$(NAME_CL): $(OBJ_CL) $(LIBFT)
	@$(CC) $(CFLAGS) -o $@ $^
	@echo "$(CYAN)🚀 Built:$@$(RESET)"

$(NAME_SV): $(OBJ_SV) $(LIBFT)
	@$(CC) $(CFLAGS) -o $@ $^
	@echo "$(CYAN)🚀 Built:$@$(RESET)"

$(OBJDIR)/%.o: %.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(GREEN)🛠️  Compiled:$(RESET) $<"

$(LIBFT):
	@make -C libft

clean:
	@rm -rf $(OBJDIR)
	@make -C libft clean
	@echo "$(YELLOW)🧹 Cleaned object files.$(RESET)"

fclean: clean
	@rm -f $(NAME_CL) $(NAME_SV)
	@make -C libft fclean
	@echo "$(YELLOW)🗑️  Removed binaries.$(RESET)"

re: fclean all

.PHONY: all clean fclean re

# **************************************************************************** #
#                                💡 USAGE GUIDE                            	  #
# **************************************************************************** #
# make            → Compile all source files and create libft.a 📦
# make clean      → Remove all object files 🧹
# make fclean     → Remove object files, libft.a, and the lib/ folder 🗑️
# make re         → Fully clean and recompile everything 🔁
# **************************************************************************** #
