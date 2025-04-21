/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_shell.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isalayan <isalayan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 11:05:26 by isalayan          #+#    #+#             */
/*   Updated: 2025/04/19 12:17:37 by isalayan         ###   ########.fr       */
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
# include "getnextline/get_next_line.h"
# include <ctype.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>
# define CMD_NOT_EXECUTABLE 126
# ifndef GLOBALS_H
#  define GLOBALS_H

extern int					g_v;

# endif

typedef enum s_type
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
	T_ARGUMENT,
	T_PIPE,
	T_REDIRECTION,
	T_ENV,
	T_PATH,
	T_TILDE,
	T_QUOTED_STRING,
	T_SEMICOLON,
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
	char					*value;
	struct s_input			*next;
	struct s_input			*prev;
}							t_input;

typedef struct s_tokenlist
{
	t_input					*head;
	t_input					*tail;
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
}							t_ij;

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
	int						permission;
	struct s_parser			*prev;
	struct s_parser			*next;
}							t_parser;

typedef struct s_env
{
	char					**env;
	int						exit_code;
}							t_env;

typedef struct s_name_value
{
	char					*new_name;
	char					*new_value;
}							t_name_value;

typedef struct s_strings
{
	char					*prefix;
	char					*expanded;
	int						end;
}							t_strings;

typedef struct s_quoted
{
	int						inside_quote;
	int						d_start;
	int						d_end;
	int						s_start;
	int						s_end;
}							t_quoted;
char						*retreive_path(t_env env);
void						close_file(t_parser *parser, int fd[2], t_fd *f);
int							is_valid_var_name(const char *var);
int							get_len_before_dollar(const char *str);
char						*get_before_dollar(const char *name);
int							find_env_var(t_env *myenv, const char *name);
void						remove_env_var(t_env *myenv, int index);
int							includes_exlamation_mark(const char *str);
int							handle_unset_options(char **operations,
								bool *save_val);
int							handle_operations_dash(t_parser *parser,
								t_env *env);
int							handle_input_dash(t_parser *parser,
								bool *save_val, t_env *env);
void						process_dollar_strings(char **strs, t_env *env);
void						ctrl_c_press_heredoc(int sig);
int							get_last_input_redirection(int *redirection);
int							manage_redirection_output(t_parser *parser,
								int *fd, t_env *env);
void						configure_child_signals(void);
void						restore_signals(void);
void						ignore_signals(void);
void						set_signal_handler_heredoc(void);
int							is_builtin_command(const char *cmd);
// execution
void						process_parser_files(t_parser *parser, t_env *env);
int							get_variable(t_parser *parser, t_env *env);
t_parser					*check_exit(t_parser *parser);
int							is_builtin(t_parser *parser);
int							manage_redirection_input(t_parser *parser, int *fd,
								t_env *env);
int							handle_input_output(t_parser *parser, t_fd *f,
								int fd[2], t_env *env);
void						restore_original_fds(int original_stdin,
								int original_stdout);
int							check_heredoc_existence(int *redirection);
int							save_original_fds(int *original_stdin,
								int *original_stdout);
int							check_permissions(const char *filepath, int flag,
								t_parser *parser, t_env *env);
void						handle_wait_status(t_parser *parser, t_env *env);
void						manage_input_output(t_fd *f, int fd[2],
								t_parser *parser);
// void						initialize_heredoc(int *heredoc_fd,
// 								t_parser *parser, t_env *env);
t_parser					*find_last_exit(t_parser *parser);
void						initialize_execution(t_parser *parser, t_env *env,
								char **cmd_path);
void						handle_child_exit(t_fd *f);
void						buitlin(t_parser *parser, t_env *env);
int							write_in_heredoc(t_parser *node, t_env *env);
void						free_heredoc(t_parser *node);
t_env						*initialize_environment(char **envp);
int							ft_manage_empty_input(char *line);
int							handle_tokenization_error(t_tokenlist *token_list,
								char *line);
int							handle_parser_error(t_parser *parser,
								t_tokenlist *token_list, char *line);
void						cleanup_resources(char *line,
								t_tokenlist *token_list, t_parser *parser);
// void                        ctrl_c_press_here(int signal);
void						process_dollar_strings(char **strs, t_env *env);
char						*retreive_path(t_env env);
void						ctrl_c_press(int signal_nb);
int							builtin_pwd(t_parser *parser, t_env *env);
int							builtin_unset(t_parser *parser, t_env *myenv);
int							builtin_echo(t_parser *list, t_env *env);
void						builtin_echo_helper(char **input, char quote,
								t_env env);
void						print_expanded_input(char **input,
								bool inside_single_quotes, t_env env);
int							is_special_char(char c, int i);
int							add_or_update_to_env(char *name, char *value,
								t_env *env);
int							builtin_export(t_parser *list, t_env *env);
int							handle_shlvl_case(char *name, char *value,
								t_name_value *new_nv);
int							find_dollar_pos1(char *str);
int							find_end_variable(char *str, int j);
void						check_quotes_status_and_update(t_quoted *q, char c);
int							is_num_or_char(char c);
void						check_quotes_till_end(char *str, t_quoted *q,
								int start, int end);
char						*check_char_after_dollar(char *str,
								int inside_quote, t_env *envp);
char						*create_array_till_dollar(char *str, int index);
void						process_variable_helper_elseif(int *i, t_strings *s,
								char **str, t_quoted *q);
void						process_variable_helper(int *i, char **str,
								t_quoted *q, t_env *env);
void						expand_and_replace(char **str, char *prefix,
								int end);
char						*add_path(char *str);
char						*check_tilde(char *str);

char						**add_string_to_2d_array(char **array,
								char *new_string);
void						cmds_exec(t_parser *parser, t_env *env);
bool						check_balanced_quotes(const char *input);
int							handle_heredoc(char **heredoc_content, t_env *env,
								int same);
char						*get_path_pwd(t_env *env, char *cmd);
void						print_expanded_input(char **input,
								bool inside_single_quotes, t_env env);
void						free_heredoc(t_parser *node);
void						replace_with_str(char ***array, char *new_str);
void						print_env_sorted(t_env *env);
int							check_value(char *str);
int							check_input(char *str);
void						check_semicolon(char *name, char **value);
int							ft_haschar(char *str, char c);
char						*ft_escape_char(char *str);
void						free_2d_array(char **array);
char						*process_variable(char *x_str, t_env *env);
char						*add_path(char *str);
char						*ft_trim_string(char *str);
int							ft_doublecharlen(t_env *env);
void						free_2d_array(char **input_array);
void						handle_memory_errors(char *new_name,
								char *new_value);
void						free_env(t_env *env);
char						*concatenate_value(char *current_value,
								char *new_value);
void						replace_or_append_value(char **env_entry,
								char *new_name, char *new_value);
int							find_and_update_env(int check_input_status,
								char *new_name, char *new_value, t_env *env);
void						parse_export_input(char *input, char **name,
								char **value);
char						*get_env_value(t_env *env, const char *var);
int							check_input_end(char *str);
char						*remove_quotes_new_new(const char *str);
char						*remove_paired_quotes(char *str);
char						*remove_closing_quote_after_equals(char *str);

//---------------------------tokens

t_input						*create_token(t_type type, const char *value);
t_tokenlist					*create_token_list(void);
void						add_token(t_tokenlist *list, t_input *token);
void						free_token(t_input *token);
void						free_token_list(t_tokenlist *list);
void						handle_braces(const char **p,
								t_tokenlist *token_list);
void						handle_quote_identifier(const char **p);
void						handle_identifier(const char **p,
								t_tokenlist *token_list);
void						tokenize_special_input(const char **p,
								t_tokenlist *token_list);
t_tokenlist					*tokenize_input(const char *input);
void						handle_pipe(const char **p,
								t_tokenlist *token_list);
void						handle_redirection(const char **p,
								t_tokenlist *token_list);
void						handle_number(const char **p,
								t_tokenlist *token_list);
void						handle_argument(const char **p,
								t_tokenlist *token_list);
void						handle_path(const char **p,
								t_tokenlist *token_list);
void						handle_tilde(const char **p,
								t_tokenlist *token_list);
void						handle_env_variable(const char **p,
								t_tokenlist *token_list);
void						handle_parenthesis(const char **p,
								t_tokenlist *token_list);
char						*ft_find_str(const char **str, char c);
void						handle_quote(const char **p,
								t_tokenlist *token_list);

//---------------------------parsing
t_parser					*create_parser(void);
void						add_parser_node(t_parser **head,
								t_parser *new_node);
int							parse_tokens(t_parser **parser, t_tokenlist *list,
								t_env *env);
int							parse_tokens_helper(t_input **tokens,
								t_parser *curr, t_env *env);
int							handle_parsing_identifier(t_input *tokens,
								t_parser *curr, t_env *env);
int							handle_parsing_identifier_helper(t_input *tokens,
								t_parser *curr, char *value, t_env *env);
int							handle_parsing_identifier_helper_errors(
								t_input *tokens, t_parser *curr, t_env *env);
int							handle_parsing_identifier_helper_errors_helper(
								t_input *tokens, t_parser *curr, t_env *env);
int							handle_parsing_path(t_input *tokens, t_parser *curr,
								t_env *env);
int							handle_parsing_path_helper_1(t_input *tokens,
								t_parser *curr, t_env *env);
int							handle_parsing_path_helper_2(t_input *tokens,
								t_parser *curr, t_env *env);
char						*remove_quotes(const char *str);
bool						count_dash(char *str);
int							handle_parsing_argument(t_input *tokens,
								t_parser *curr);
int							handle_parsing_quotes_helper_2(t_input *tokens,
								char *value, t_env *env);
int							handle_parsing_quotes(t_input *tokens,
								t_parser *curr, t_env *env);
int							handle_parsing_redirection(t_input *tokens,
								t_parser *curr, t_env *env);
int							is_all_spaces(const char *str);
int							isempty(const char *str);
char						*expand_variable(const char *input, t_env env);
int							parse_tokens(t_parser **parser, t_tokenlist *list,
								t_env *env);
int							is_executable(char *cmd, t_env env);
int							is_executable_pwd(t_env env, char *cmd);
char						*get_path(t_env env, char *cmd);
int							handle_parsing_path_helper_1(t_input *tokens,
								t_parser *curr, t_env *env);
int							handle_parsing_path_helper_2(t_input *tokens,
								t_parser *curr, t_env *env);
int							handle_parsing_path(t_input *tokens, t_parser *curr,
								t_env *env);
bool						ft_check_n_operation(char *str);
int							count_rows(char **array);
char						**add_string_to_2d_array(char **array,
								char *new_string);
void						add_to_array(t_parser *parser, int new_redirection);
void						free_ptr(void *ptr);
char						*join_strs(char *str, char *add);
void						errmsg_cmd(char *command, char *detail,
								char *error_message);
void						add_to_input_args(char *value, t_parser *curr);
bool						cmd_is_dir(char *cmd);
t_env						*manage_env_i(void);
int							handle_parsing_quotes(t_input *tokens,
								t_parser *curr, t_env *env);
char						*process_variable(char *x_str, t_env *env);

//--------------------------------------printing
void						print_tokens(const t_tokenlist *list);
void						print_token_list(t_tokenlist *list);
void						print_parser(t_parser *parser);
void						print_2d_array(char **array);

//-----------------------free
void						ft_free_env(t_env **myenv);
void						free_parser(t_parser *parser);

//----------------------build-ins

//_______exit
void						builtin_exit(t_parser *parser, t_env *myenv);
int							handle_exit_argument(char *input, t_env *env);
int							handle_exit_arguments(char **input, t_env *env);
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
int							change_directory_and_update(t_parser *list,
								t_env *myenv);

void						update_pwd(t_parser *list, t_env *myenv);
void						replace_with_str(char ***array, char *new_str);
int							is_oldpwd_input(const char *input);
char						*remove_quotes_with_free(char *str);
#endif