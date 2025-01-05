/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isalayan <isalayan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 14:10:16 by skreik            #+#    #+#             */
/*   Updated: 2024/12/26 09:37:50 by isalayan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

int global_var = 0;
volatile sig_atomic_t g_interrupt = 0;
void	ft_free_env(t_env **my_env)
{
	int	i;

	i = 0;
	if (!my_env || !(*my_env))
		return ;
	while ((*my_env)->env[i])
	{
		free((*my_env)->env[i]);
		i++;
	}
	printf("(((((just freed  %d))))))\n",i);
	free((*my_env)->env);
	free(*my_env);
	*my_env = NULL;
}

// void setup_signal_handlers(void)
// {
//     struct sigaction sa;

//     // Initialize the sigaction structure
//     sa.sa_handler = ctrl_c_press;       // Set the signal handler function
//     sigemptyset(&sa.sa_mask);           // Do not block any signals during the handler execution
//     sa.sa_flags = 0;                    // No special flags

//     // Set up the handler for SIGINT (Ctrl+C)
//     if (sigaction(SIGINT, &sa, NULL) == -1) {
//         perror("sigaction");
//         _exit(1);
//     }

//     // You can add more signal handlers here if needed
// }


void setup_signal_handlers(void)
{
    struct sigaction sa;

    // Handle SIGINT (Ctrl+C) in the shell
    sa.sa_handler = ctrl_c_press;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    // Ignore SIGQUIT (Ctrl+\)
    sa.sa_handler = SIG_IGN;
    if (sigaction(SIGQUIT, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }
}
void handle_child_signals(void)
{
    struct sigaction sa;

    // Restore default behavior for SIGINT in child processes
    sa.sa_handler = SIG_DFL;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    if (sigaction(SIGQUIT, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }
}
int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	char *line = NULL;
	t_env *my_env;
	t_tokenlist *token_list;
	t_parser *parser;
	int value ;
	my_env = init_env(envp);
	setup_signal_handlers();
	// signal(SIGINT, ctrl_c_press);
	while (1)
	{
		line = readline("minishell>");
		if (!line)
			break ;
		if (ft_strlen(line) == 0)
        {
            free(line);
            continue;
        }
		if (*line)
			add_history(line);
		token_list = tokenize_input(line);
		if (!token_list)
		{
			fprintf(stderr,"Error: Failed to tokenize input.\n");
			free(line);
			continue ;
		}
		// Print tokens (for debugging)
		printf("\nTokens:______________________\n");
		print_token_list(token_list);
		// Parse tokens
		parser = create_parser(); // Initialize parser
		if (!parser)
		{
			fprintf(stderr, "Error: Failed to create parser.\n");
			free(line);
			free_token_list(token_list); // Free token list
			continue ;
		}
		value = parse_tokens(&parser, token_list, *my_env);
		// Check if parser is valid
		if (!parser || !parser->command || value == -1)
		{
			// fprintf(stderr, "Error: Invalid parser or command.\n");
			free(line);
			free_token_list(token_list);
			free_parser(parser);
			continue ;
		}
		// Print parser nodes (for debugging)
		printf("\nParsed commands:\n");
		print_parser(parser);
		free_token_list(token_list);
		// Execute the parsed command
		cmds_exec(parser, my_env);
		free(line);
		free_parser(parser);
	}
	ft_free_env(&my_env);
	return (EXIT_SUCCESS);
}










// int main(int argc, char **argv,char **envp)
// {
// 	char *line = NULL;
// 	(void)argc;
// 	(void)argv;
// 	t_env *my_env;
// 	t_tokenlist *token_list;
// 	t_parser *parser;
// 	int value ;

// 	line = "hi$HOME";

// 	my_env = init_env(envp);
// 	token_list = tokenize_input(line);
// 		if (!token_list)
// 		{
// 			fprintf(stderr,"Error: Failed to tokenize input.\n");
// 		}
// 		// Print tokens (for debugging)
// 		printf("\nTokens:______________________\n");
// 		print_token_list(token_list);
// 		// Parse tokens
// 		parser = create_parser(); // Initialize parser
// 		if (!parser)
// 		{
// 			fprintf(stderr, "Error: Failed to create parser.\n");
// 			free_token_list(token_list); // Free token list
// 			ft_free_env(&my_env);
// 			return 0;
// 		}

// 		value = parse_tokens(&parser, token_list, *my_env);
// 		// Check if parser is valid
// 		if (!parser || !parser->command || value == -1)
// 		{
// 			// fprintf(stderr, "Error: Invalid parser or command.\n");
// 			free_token_list(token_list);
// 			free_parser(parser);
// 			ft_free_env(&my_env);

// 			return 0;
// 		}

// 		// Print parser nodes (for debugging)
// 		printf("\nParsed commands:\n");
// 		print_parser(parser);

// 		// Execute the parsed command
// 		// cmds_exec(parser, my_env);
// 		free_token_list(token_list);
// 		free_parser(parser);
	
// 	ft_free_env(&my_env);
// 	return 0;
// }