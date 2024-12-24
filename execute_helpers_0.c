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

// Function to strip quotes from a string
char *strip_quotes(const char *str) {
    if (!str || strlen(str) < 2) {
        return strdup(str); // Return a copy if the string is too short or NULL
    }
    size_t len = strlen(str);
    if ((str[0] == '\'' && str[len - 1] == '\'') || (str[0] == '"' && str[len - 1] == '"')) {
        char *new_str = malloc(len - 1); // Allocate space for stripped string
        if (!new_str) {
            perror("malloc");
            return NULL;
        }
        strncpy(new_str, str + 1, len - 2);
        new_str[len - 2] = '\0'; // Null-terminate the string
        return new_str;
    }
    return strdup(str); // Return a copy if no quotes to strip
}

char **ft_create_args(t_parser *parser, t_tokenlist *token) {
    char **path;
    t_input *current;
    int operation_index = 0;
    int input_index = 0;

    path = NULL;

    if (!token || !token->head || !parser) {
        return path; // Return if token list or parser is invalid
    }

    current = token->head;

    // Traverse the token list to find the command
    while (current) {
        if (strcmp(current->value, parser->command) == 0) {
            path = add_string_to_2d_array(path, current->value);
            current = current->next;
            break;
        }
        current = current->next;
    }

    // Process subsequent tokens
    while (current && (parser->input || parser->operations)) {
        char *stripped_value = strip_quotes(current->value);
        if (parser->operations && parser->operations[operation_index] &&
            strcmp(stripped_value, parser->operations[operation_index]) == 0) {
            printf("Adding operation: %s\n", current->value);
            path = add_string_to_2d_array(path, parser->operations[operation_index]);
            operation_index++;
        } else if (parser->input && parser->input[input_index] &&
                   strcmp(stripped_value, parser->input[input_index]) == 0) {
            printf("Adding input: %s\n", current->value);
            path = add_string_to_2d_array(path, parser->input[input_index]);
            input_index++;
        }
        free(stripped_value); // Free the allocated memory
        current = current->next;
    }

    return path;
}


// char	**ft_create_args(t_parser *parser, t_tokenlist *token)
// {
// 	char	**path;
// 	int		i;

// 	path = NULL;
// 	path = add_string_to_2d_array(path, parser->command);

// 	i = 0;
// 	if (parser->operations != NULL)
// 	{
// 		while (parser->operations[i])
// 		{
// 			printf("if 1 \n");
// 			path = add_string_to_2d_array(path, parser->operations[i]);
// 			i++;
// 		}
// 	}
// 	i = 0;
// 	//&& parser->redirection == NULL
// 	if (parser->input != NULL )
// 	{
// 		while (parser->input[i])
// 		{
// 			printf("if 2 \n");
// 			path = add_string_to_2d_array(path, parser->input[i]);
// 			i++;
// 		}
// 	}
// 	// 	i = 0;
// 	// if (parser->heredoc != NULL)
// 	// {
// 	// 	while (parser->heredoc[i])
// 	// 	{
// 	// 		printf("if 3 \n");
// 	// 		path = add_string_to_2d_array(path, parser->heredoc[i]);
// 	// 		i++;
// 	// 	}
// 	// }
// 	return (path);
// }

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

void ft_redirection_delimiter(t_parser *node)
{
    char *line = NULL;
    size_t line_len = 0;
	global_var = 0;
    // Continuously prompt and read input until the delimiter is found
    while (global_var != 130)
    {
        write(STDOUT_FILENO, "> ", 2); // Write the prompt directly to stdout
        line = get_next_line(0); // Read input from stdin
        if (!line) // Handle EOF or error
            break;
        // Remove the newline character if present
        line_len = strlen(line);
        if (line[line_len - 1] == '\n')
            line[line_len - 1] = '\0';

        // Check if the input matches the delimiter
        if (strcmp(line, node->delimeter) == 0)
        {
            free(line);
            break;
        }
        // Add the input line to the 2D array
        node->heredoc = add_string_to_2d_array(node->heredoc, line);
        free(line); // Free the line buffer after processing
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