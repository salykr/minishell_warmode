# Standard
NAME                = minishell
LIBFT               = ./Libft/libft.a
INC                 = ./
SRC_DIR             = ./
OBJ_DIR             = obj/

# Compiler and Flags
CC                  = gcc
CFLAGS              = -Wall -Werror -Wextra -I$(INC)
EXTRAFLAGS          = -lreadline -L/usr/local/opt/readline/lib -I/usr/local/opt/readline/include
RM                  = rm -f
# Source and Object Files
SRCS                = $(SRC_DIR)parsing_creation.c \
                      $(SRC_DIR)parsing_handling.c \
                      $(SRC_DIR)parsing_freeing.c \
                      $(SRC_DIR)parsing_identifier.c \
                      $(SRC_DIR)parsing_main.c \
                      $(SRC_DIR)parsing_path.c \
                      $(SRC_DIR)parsing_UTILS.c \
                      $(SRC_DIR)parsing_UTILS2.c \
                      $(SRC_DIR)token_creation.c \
                      $(SRC_DIR)token_handle_main.c \
                      $(SRC_DIR)token_handling_1.c \
                      $(SRC_DIR)token_handling_2.c \
                      $(SRC_DIR)token_utils.c \
                      $(SRC_DIR)builtin_echo.c \
                      $(SRC_DIR)builtin_pwd.c \
                      $(SRC_DIR)builtin_exit.c \
                      $(SRC_DIR)builtin_exit_helper.c \
                      $(SRC_DIR)builtin_env.c \
                      $(SRC_DIR)builtin_env_helper1.c \
                      $(SRC_DIR)builtin_env_helper2.c \
                      $(SRC_DIR)builtin_unset.c \
                      $(SRC_DIR)builtin_export.c \
                      $(SRC_DIR)builtin_cd.c \
                      $(SRC_DIR)builtin_cd_helper.c \
                      $(SRC_DIR)execute.c \
                      $(SRC_DIR)signals.c\
                      $(SRC_DIR)main.c \
                      $(SRC_DIR)builtin_export_helper_0.c \
                      $(SRC_DIR)builtin_export_helper_1.c \
                      $(SRC_DIR)builtin_export_helper_2.c \
                      $(SRC_DIR)builtin_export_helper_3.c \
                      $(SRC_DIR)builtin_export_helper_4.c \
                      $(SRC_DIR)builtin_export_helper_5.c \
                      $(SRC_DIR)builtin_export_helper_utils.c\
                      $(SRC_DIR)printing.c
                 
OBJS                = $(patsubst $(SRC_DIR)%.c,$(OBJ_DIR)%.o,$(SRCS))

# Targets
all:                $(NAME)

$(NAME):            $(OBJS) $(LIBFT)
	@$(CC) $(OBJS) $(LIBFT) $(CFLAGS) $(EXTRAFLAGS) -o $(NAME)

$(OBJ_DIR)%.o:      $(SRC_DIR)%.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	@make -C ./Libft

clean:
	@$(RM) -r $(OBJ_DIR)
	@make clean -C ./Libft

fclean:             clean
	@$(RM) $(NAME)
	@make fclean -C ./Libft

re:                 fclean all

.PHONY:             all clean fclean re
