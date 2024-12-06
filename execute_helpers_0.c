/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_helpers_0.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 15:08:51 by marvin            #+#    #+#             */
/*   Updated: 2024/11/24 15:08:51 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"mini_shell.h"


char	**ft_create_args(t_parser *parser)
{
	char	**path;
	int		i;

	path = NULL;
	path = add_string_to_2d_array(path, parser->command);
	i = 0;
	if (parser->operations != NULL)
	{
		while (parser->operations[i])
		{
			path = add_string_to_2d_array(path, parser->operations[i]);
			i++;
		}
	}
	if(strcmp(parser->command, "set")==0 || strcmp(parser->command, "declare")==0)
	{
		printf("hiiiiiiiiiiiii\\n\n");
		path = add_string_to_2d_array(path, ft_strdup("-c"));
	}
	i = 0;
	if (parser->input != NULL && parser->redirection == NULL)
	{
		while (parser->input[i])
		{
			printf("command : %s\n", parser->command);
			path = add_string_to_2d_array(path, parser->input[i]);
			i++;
		}
	}
	return (path);
}

char	*get_path(t_env env, char *cmd)
{
	int		i;
	char	*p;
	char	*back_slash;
	char	**path;
	char	*env_path;

	env_path = ft_getenv(&env, "PATH");
	if (!env_path)
		return (NULL);
	path = ft_split(env_path, ':');
	// path = add_string_to_2d_array(path, ft_strdup("/bin/bash"));
	// print_2d_array(path);
	if (!path)
		return (NULL);
	i = 0;
	while (path[i])
	{
		back_slash = ft_strjoin("/", cmd);
		p = ft_strjoin(path[i], back_slash);
		// printf("path is: %s\n", p);
		free(back_slash);
		if (access(p, X_OK) == 0)
			return (free_input(path), p);
		else
			free(p);
		i++;
	}
	return (free_input(path), NULL);
}
char	*get_path_PWD(t_env env, char *cmd)
{
	char	*p;
	char	*env_path;

	env_path = ft_getenv(&env, "PWD");
	if (!env_path)
		return (NULL);
	p = ft_strjoin(env_path, &cmd[1]);
	printf("str: %s\n", p);
	if (access(p, X_OK) == 0)
		return (p);
	else
		free(p);
	return (NULL);
}
/*
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <termios.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>

volatile sig_atomic_t signal_received = 0;
struct termios old_tio, new_tio;

// Function to set the terminal to raw mode (disables canonical mode and echo)
void set_raw_mode() {
    tcgetattr(STDIN_FILENO, &old_tio);  // Save current terminal settings
    new_tio = old_tio;  // Copy current settings
    new_tio.c_lflag &= ~(ICANON | ECHO);  // Disable canonical mode and echo
    new_tio.c_cc[VMIN] = 1;  // Minimum number of characters to read
    new_tio.c_cc[VTIME] = 0;  // No timeout

    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);  // Set terminal to raw mode
}

// Function to restore the terminal to its original settings
void restore_terminal() {
    tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);  // Restore terminal settings
}

// Signal handler for SIGINT (Ctrl+C)
void handle_sigint(int sig)
{
	// (void)sig;
	if(sig == SIGINT)
	{
		signal_received = 1;  // Set flag when Ctrl+C is pressed
		set_raw_mode();       // Switch to raw mode for immediate input capture

		// Clear current input (if any) and break the loop
		// rl_replace_line("", 0); 
		// rl_on_new_line();
		// rl_redisplay();
		printf("SIGINT\n\n");
		restore_terminal();
	}

}

// Core function where readline is used to simulate the heredoc behavior
void ft_redirection_delimiter(t_parser *node)
{
    char *lineread;

    // Set up the signal handler for SIGINT (Ctrl+C)
    signal(SIGINT, handle_sigint);

    while (1)
    {
        // If Ctrl+C was pressed, break the loop
        if (signal_received)
        {
            signal_received = 0;  // Reset flag
            break;  // Exit the loop immediately after Ctrl+C
        }

        lineread = readline("> ");
        if (!lineread) // If EOF is reached, break the loop
        {
            break;
        }

        if (ft_strcmp(lineread, node->delimeter) == 0)
        {
            free(lineread);
            break;
        }

        node->input = add_string_to_2d_array(node->input, lineread);
        free(lineread);
    }

    // Ensure terminal settings are restored before returning
    restore_terminal();

    // Reset signal handler for SIGINT to default after processing
    signal(SIGINT, SIG_DFL);
}
*/

void	ft_redirection_delimiter(t_parser *node)
{
	char	*lineread;

	while (1)
	{
		lineread = readline(">");
		if (!lineread) // If EOF is reached, break the loop
			break ;
		// printf("line read: %s\n", lineread);
		if (ft_strcmp(lineread, node->delimeter) == 0)
		{
			free(lineread);
			break ;
		}
		node->input = add_string_to_2d_array(node->input, lineread);
		free(lineread);
	}
}


int	handle_heredoc(char **heredoc_content)
{
	int	pipefd[2];
	int	i;

	i = 0;
	if (pipe(pipefd) == -1)
	{
		perror("Error creating pipe");
		exit(EXIT_FAILURE);
	}
	while (heredoc_content != NULL && heredoc_content[i] != NULL)
	{
		write(pipefd[1], heredoc_content[i], strlen(heredoc_content[i]));
		write(pipefd[1], "\n", 1);
		i++;
	}

	close(pipefd[1]);
	return (pipefd[0]);
}

/*
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    // Prepare arguments for execve
    char *args[] = {"/bin/bash", "-c", "echo $abc", NULL};  // Run echo to print the value of $abc
    
    // Set environment variables (including abc=hi)
    char *envp[] = {
        "abc=hi",  // Set abc to "hi"
        NULL       // End of environment variables
    };

    // Execute the command using execve
    if (execve("/bin/bash", args, envp) == -1) {
        perror("execve");
        exit(EXIT_FAILURE);
    }

    return 0;
}

*/