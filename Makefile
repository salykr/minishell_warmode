# Standard
NAME                = minishell
LIBFT               = ./Libft/libft.a
GNL               = ./getnextline/getnextline.a
INC                 = ./
SRC_DIR             = ./
BUILTINS_DIR        = $(SRC_DIR)builtins/
CD_DIR              = $(BUILTINS_DIR)cd/
EXIT_DIR              = $(BUILTINS_DIR)exit/
EXPORT_DIR              = $(BUILTINS_DIR)export/
PWD_DIR              = $(BUILTINS_DIR)pwd/
ECHO_DIR              = $(BUILTINS_DIR)echo/
UNSET_DIR              = $(BUILTINS_DIR)unset/
ENV_DIR              = $(BUILTINS_DIR)env/
EXEC_DIR        = $(SRC_DIR)execution/
SIGNALS_DIR        = $(SRC_DIR)signals/
TOKENS_DIR        = $(SRC_DIR)tokens/
PARSE_DIR        = $(SRC_DIR)parsing/
GNL_DIR        = $(SRC_DIR)getnextline/
OBJ_DIR             = obj/

# Compiler and Flags
CC                  = gcc
CFLAGS              = -Wall -Werror -Wextra -I$(INC) -g
EXTRAFLAGS          = -lreadline -L/usr/local/opt/readline/lib -I/usr/local/opt/readline/include
RM                  = rm -f

# Source and Object Files
SRCS                = $(PARSE_DIR)parsing_creation.c \
                      $(PARSE_DIR)parsing_handling.c \
                      $(PARSE_DIR)parsing_freeing.c \
                      $(PARSE_DIR)parsing_identifier.c \
                      $(PARSE_DIR)parsing_handling_helper.c \
                      $(PARSE_DIR)parsing_main.c \
                      $(PARSE_DIR)parsing_path.c \
                      $(PARSE_DIR)parsing_UTILS.c \
                      $(PARSE_DIR)parsing_UTILS2.c \
                      $(TOKENS_DIR)token_creation.c \
                      $(TOKENS_DIR)token_handle_main.c \
                      $(TOKENS_DIR)token_handling_1.c \
                      $(TOKENS_DIR)token_handling_2.c \
                      $(TOKENS_DIR)token_utils.c \
                      $(ECHO_DIR)builtin_echo.c \
                      $(ECHO_DIR)builtin_echo_helper.c \
                      $(PWD_DIR)builtin_pwd.c \
                      $(EXIT_DIR)builtin_exit.c \
                      $(EXIT_DIR)builtin_exit_helper.c \
                      $(ENV_DIR)builtin_env.c \
                      $(ENV_DIR)builtin_env_helper1.c \
                      $(ENV_DIR)builtin_env_helper2.c \
                      $(UNSET_DIR)builtin_unset.c \
                      $(UNSET_DIR)builtin_unset_helper_0.c \
                      $(UNSET_DIR)builtin_unset_helper_1.c \
                      $(EXPORT_DIR)builtin_export.c \
                      $(CD_DIR)builtin_cd.c \
                      $(CD_DIR)builtin_cd_helper.c \
                      $(EXEC_DIR)execute.c \
                      $(EXEC_DIR)execute_helpers_0.c \
                      $(EXEC_DIR)execute_helpers_1.c \
                      $(EXEC_DIR)execute_helpers_2.c \
                      $(EXEC_DIR)execute_helpers_3.c \
                      $(EXEC_DIR)execute_helpers_4.c \
                      $(EXEC_DIR)execute_helpers_5.c \
                      $(SIGNALS_DIR)signals_0.c\
                      $(SIGNALS_DIR)signals_1.c\
                      $(SRC_DIR)main_fts.c \
                      $(SRC_DIR)main.c \
                      $(EXPORT_DIR)builtin_export_helper_0.c \
                      $(EXPORT_DIR)builtin_export_helper_1.c \
                      $(EXPORT_DIR)builtin_export_helper_2.c \
                      $(EXPORT_DIR)builtin_export_helper_3.c \
                      $(EXPORT_DIR)builtin_export_helper_4.c \
                      $(EXPORT_DIR)builtin_export_helper_5_0.c \
                      $(EXPORT_DIR)builtin_export_helper_5_1.c \
                      $(EXPORT_DIR)builtin_export_helper_5_2.c \
                      $(EXPORT_DIR)builtin_export_helper_5_3.c \
                      $(EXPORT_DIR)builtin_export_helper_6.c \
                      $(EXPORT_DIR)builtin_export_helper_7.c \
                      $(EXPORT_DIR)builtin_export_helper_utils.c\
                      $(GNL_DIR)get_next_line_utils.c\
                      $(GNL_DIR)get_next_line.c\
                      $(TOKENS_DIR)printing.c
                 
OBJS                = $(patsubst $(SRC_DIR)%.c,$(OBJ_DIR)%.o,$(SRCS))


$(NAME):            $(OBJS) $(LIBFT) $(GNL)
	@$(CC) $(OBJS) $(LIBFT) $(GNL)  $(CFLAGS) $(EXTRAFLAGS) -o $(NAME)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	@make -C ./Libft
$(GNL):
	@make -C ./getnextline
# Targets
all:                $(NAME)

clean:
	@$(RM) -r $(OBJ_DIR)
	@make clean -C ./Libft
	@make clean -C ./getnextline

fclean:             clean
	@$(RM) $(NAME)
	@make fclean -C ./Libft
	@make fclean -C ./getnextline

re:                 fclean all

.PHONY:             all clean fclean re