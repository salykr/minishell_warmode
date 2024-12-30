/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_shell.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skreik <skreik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 11:05:26 by skreik            #+#    #+#             */
/*   Updated: 2024/12/30 17:12:07 by skreik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_SHELL_H
# define MINI_SHELL_H

# ifndef INITIAL_SIZE
#  define INITIAL_SIZE 1024
# endif

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 4096
# endif

# include "Libft/libft.h"
# include <ctype.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>
#include <termios.h>
# include <sys/stat.h>
# include "get_next_line.h"
#include <signal.h>

# define CMD_NOT_EXECUTABLE 126
# ifndef GLOBALS_H
#  define GLOBALS_H

extern int global_var; // Declare the global variable

# endif

typedef enum
{
	T_BRACES,
	T_APPEND,
	T_HEREDOC,
	T_OUTPUT,
	T_INPUT,
	T_NUMBER,
	T_OPERATOR,
	T_PARENTHESIS,
	T_IDENTIFIER,
	T_WHITESPACE,
	T_UNKNOWN,
	T_ARGUMENT,
	T_PIPE,
	T_REDIRECTION,
	T_ENV,
	T_PATH,
	T_TILDE,
	T_QUOTED_STRING,
	T_SEMICOLON,
	T_AMPERSAND,
	T_LOGICAL_AND,
	T_LOGICAL_OR,
	T_SUBSHELL,
	T_COMMAND_SEPARATOR,
	T_QUOTE,
	T_PERIODS,
}							t_type;

typedef struct s_env_signals
{
	t_type					type;
	char					**env;
	struct s_env_signals	*next;
	struct s_env_signals	*prev;
}							t_env_signals;

typedef struct s_input
{
	t_type					type;
	char *value;
	struct s_input *next;
	struct s_input *prev;
}							t_input;

typedef struct
{
	t_input *head;
	t_input *tail;
}							t_tokenlist;

typedef struct s_fd
{
	int						fd_1;
	int						fd_2;
}							t_fd;

typedef struct s_ij
{
	int						i;
	int						j;
}		t_ij;

typedef struct s_quoted
{
	char					*single;
	char					*doubles;
}							t_quoted;




typedef struct s_parser
{
	char					*command;
	char					**input;
	char					**operations;
	int						*redirection;
	char					**delimeter;
	char					**args;
	char					**infile;
	char					**outfile;
	char					**heredoc;
	struct s_parser			*prev;
	struct s_parser			*next;
}							t_parser;

typedef struct s_env_var
{
	const char				*name;
	int						length;
}							t_env_var;

typedef struct s_env
{
	char					**env;
	int						exit_code;
}							t_env;

typedef struct s_name_value {
    char *new_name;
    char *new_value;
} t_name_value;
typedef struct s_context{
    size_t total_size;  // Start with 1 for null terminator
    char *start;
    char *dollar;
    char *new_str;
    char *var_name;
    char *end_of_var;
    char temp_char;
    char first_char;
    char *input;
    t_env env;
    char *env_value;
}t_context;
void process_dollar_strings(char **strs, t_env *env);
void set_signal_handler(void (*handler)(int));
void ctrl_c_press_heredoc(int sig);
void manage_pipe(t_parser *parser,t_fd *f, int fd[2]);
int get_last_input_redirection(int *redirection);
int manage_redirection_output(t_parser *parser, int *fd);
void setup_signal_handlers(void);
void handle_child_signals(void(*handler)(int));
void configure_child_signals();
void restore_signals();
void ignore_signals();
void setup_signals();
void setup_signals_heredoc(void);
void set_signal_handler_heredoc();
//execution
t_parser* check_exit(t_parser *parser);
int	is_builtin(t_parser *parser);
int	manage_redirection_input(t_parser *parser, int *fd, t_env *env);
int	handle_input_output(t_parser *parser, t_fd *f, int fd[2], t_env *env);
void restore_original_fds(int original_stdin, int original_stdout);
int check_heredoc_existence(int *redirection);
int save_original_fds(int *original_stdin, int *original_stdout);
int check_permissions(const char *filepath, int flag);
void manage_input_output( t_fd *f, int fd[2], t_parser *parser);
void initialize_heredoc(int *heredoc_fd, t_parser *parser);
void initialize_execution(t_parser *parser,t_env *env,char **cmd_path);
void handle_child_exit(t_fd *f);
int	ft_handle_redirections(t_parser *parser);
void buitlin(t_parser *parser, t_env *env);
void write_in_heredoc(t_parser *node);
void free_heredoc(t_parser *node);

// void                        ctrl_c_press_here(int signal);
void update_pwd_m(t_env *myenv, int use_manual_path);
void						ctrl_c_press(int signal_nb);
int							ft_isprintable(char c);
void						ft_redirection(t_parser *node);
int builtin_pwd(t_parser *parser, t_env *env);
int							builtin_unset(t_parser *parser, t_env *myenv);
int						builtin_echo(t_parser *list, t_env *env);
void builtin_echo_helper(char **input, char quote, t_env env); 
void	print_expanded_input(char **input, bool inside_single_quotes, t_env env);
int		is_special_char(char c);
int						add_or_update_to_env(char *name, char *value,
								t_env *env);
int						builtin_export(t_parser *list, t_env *env);
int handle_shlvl_case(char *name, char *value, t_name_value *new_nv);
char *resize_string(char *str, size_t new_size);
size_t pv_count_backslashes(t_context *ctx);
void pv_resize_concat(char **resized_str, size_t len_resize, char *concat_str, size_t len_concat);
int pv_initialise_vars(t_context *ctx);
int pv_backslashes_cases(t_context *ctx);
int pv_question_mark(t_context *ctx);
int pv_update_pointers(t_context *ctx);
int pv_env_variable(t_context *ctx, char *env_value);
void pv_fill_values(t_context *ctx);
void pv_initialize_context(t_context *ctx, char *input, t_env *env);
int pv_handle_backslashes(t_context *ctx);
char						**add_string_to_2d_array(char **array,char *new_string);
int							ft_checkft(t_parser *parser);
int							ft_handle_redirections(t_parser *parser);
void						ft_redirection_delimiter(t_parser *node);
void	cmds_exec(t_parser *parser, t_env *env);
void						update_env_level(t_env *myenv);
bool						check_balanced_quotes(const char *input);
char						*remove_quotes_new(const char *str);
int	handle_heredoc(char **heredoc_content, t_env *env);
char						*get_path_pwd(t_env env, char *cmd);
void						print_expanded_input(char **input,
								bool inside_single_quotes, t_env env);
void	free_heredoc(t_parser *node);
int check_args_nb(t_parser *list);
void replace_with_str(char ***array, char *new_str);
void print_env_sorted(t_env *env);
int check_value(char *str);
int check_input(char *str);
void check_semicolon(char *name, char **value);
int ft_haschar(char *str, char c);
char *ft_escape_char(char *str);
void free_2d_array(char **array);
char *process_variable(char *input, t_env *env);
char *ft_trim_string(char *str);
int ft_doublecharlen(t_env *env);
void free_2d_array(char **input_array);
void handle_memory_errors(char *new_name, char *new_value);
void free_env(t_env *env);
char *concatenate_value(char *current_value, char *new_value);
void replace_or_append_value(char **env_entry, char *new_name, char *new_value);
int find_and_update_env(int check_input_status, char *new_name, char *new_value, t_env *env);
void parse_export_input(char *input, char **name, char **value);
char *itoa(int num);
char *get_env_value(t_env *env, const char *var);
int check_input_end(char *str);
char *remove_quotes_new_new(const char *str);
char *remove_paired_quotes(char *str);
void memory_free(char *str1, char *str2);
char *remove_closing_quote_after_equals(char *str);
int calculate_len_for_backslash(const char *str);
void manage_input(t_parser *parser, int *fd, int re, int *place);
void manage_output(t_parser *parser, int *fd, int re, int *place);


//---------------------------tokens

t_input						*create_token(t_type type, const char *value);
t_tokenlist					*create_token_list(void);
void						add_token(t_tokenlist *list, t_input *token);
void						free_token(t_input *token);
void						free_token_list(t_tokenlist *list);
void						handle_braces(const char **p,
								t_tokenlist *token_list);
void						handle_quote_identifier(const char **p);
void						handle_identifier(const char **p, t_tokenlist *token_list);
void						tokenize_special_input(const char **p, t_tokenlist *token_list);
t_tokenlist					*tokenize_input(const char *input);
void						handle_pipe(const char **p, t_tokenlist *token_list);
void						handle_redirection(const char **p, t_tokenlist *token_list);
void						handle_number(const char **p, t_tokenlist *token_list);
void						handle_argument(const char **p, t_tokenlist *token_list);
void						handle_path(const char **p, t_tokenlist *token_list);
void						handle_tilde(const char **p, t_tokenlist *token_list);
void						handle_env_variable(const char **p, t_tokenlist *token_list);
void						handle_parenthesis(const char **p, t_tokenlist *token_list);
char						*ft_find_str( const char **str, char c);
void						handle_quote(const char **p, t_tokenlist *token_list);






//---------------------------parsing
t_parser	*create_parser(void);
void		add_parser_node(t_parser **head, t_parser *new_node);
int			parse_tokens(t_parser **parser, t_tokenlist *list, t_env env);
int			parse_tokens_helper(t_input **tokens, t_parser *curr,t_env env);
int			handle_parsing_identifier(t_input *tokens, t_parser *curr, t_env env);
int			handle_parsing_identifier_helper(t_input *tokens, t_parser *curr,char *value);
int			handle_parsing_identifier_helper_errors(t_input *tokens, t_parser *curr);
int			handle_parsing_identifier_helper_errors_helper(t_input *tokens, t_parser *curr);
int			handle_parsing_path(t_input *tokens, t_parser *curr, t_env env);
int			handle_parsing_path_helper_1(t_input *tokens, t_parser *curr,t_env env);
int			handle_parsing_path_helper_2(t_input *tokens, t_parser *curr);
char		*remove_quotes(const char *str);
bool		count_dash(char *str);
int			handle_parsing_argument(t_input *tokens, t_parser *curr);
int			handle_parsing_quotes_helper_2(t_input *tokens,char * value);
int			handle_parsing_quotes(t_input *tokens, t_parser *curr,t_env env);
int			handle_parsing_redirection(t_input *tokens, t_parser *curr);
int			is_all_spaces(const char *str);
int			isempty(const char *str);
char		*expand_variable(const char *input ,t_env env);
int			parse_tokens(t_parser **parser, t_tokenlist *list, t_env env);
int			is_executable(char *cmd);
int			is_executable_PWD(t_env env, char *cmd);
char	*get_path(t_env env, char *cmd);
char	**ft_create_args(t_parser *parser, t_tokenlist *token);
int			handle_parsing_path_helper_1(t_input *tokens, t_parser *curr,t_env env);
int			handle_parsing_path_helper_2(t_input *tokens, t_parser *curr);
int			handle_parsing_path(t_input *tokens, t_parser *curr, t_env env);
bool		ft_check_n_operation(char *str);
int			count_rows(char **array);
char		**add_string_to_2d_array(char **array, char *new_string);
void		add_to_array_helper(t_parser *parser, int new_redirection);
void		add_to_array(t_parser *parser, int new_redirection);
void		free_ptr(void *ptr);
char		*join_strs(char *str, char *add);
void		errmsg_cmd(char *command, char *detail, char *error_message);
bool		cmd_is_dir(char *cmd);


//--------------------------------------printing
void	print_tokens(const t_tokenlist *list);
void	print_token_list(t_tokenlist *list);
void	print_parser(t_parser *parser);
void	print_2d_array(char **array);


//-----------------------free
void						ft_free_env(t_env **myenv);
void						free_parser(t_parser *parser);


//----------------------build-ins


//_______exit
void						builtin_exit(t_parser *parser, t_env *myenv);
int							handle_exit_arguments(char *input);
int							is_numeric(const char *str);

//_______env
int							builtin_env(t_parser *list, t_env *myenv);
char						*ft_getenv(t_env *myenv, char *str);
t_env						*init_env(char **env);
char						*check_env_input(t_parser *list);
int							check_ls_pwd_in_env(t_parser *list, t_env *myenv);
void						print_env_vars(t_env *myenv, char **input_list);
void						print_all_env_vars(t_env *myenv);
void						update_shlvl(t_env *myenv);

//______cd
int							builtin_cd(t_parser *parser, t_env *myenv);
int							change_directory_and_update(t_parser *list, t_env *myenv);

void	update_pwd(t_parser *list, t_env *myenv);
void						replace_with_str(char ***array, char *new_str);
int							is_oldpwd_input(const char *input);
char *remove_quotes_with_free(char *str);
#endif