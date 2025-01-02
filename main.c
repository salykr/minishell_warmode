/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skreik <skreik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 14:10:16 by skreik            #+#    #+#             */
/*   Updated: 2025/01/02 15:45:57 by skreik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

int global_var = 0;
void    ft_free_env(t_env **my_env)
{
        int     i;

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
t_env *manage_env_i()
{
	t_env	*myenv;
        char *wd;
	char	cwd[2048];

	myenv = (t_env *)malloc(sizeof(t_env));
	if (!myenv)
		return (NULL);
	myenv->env = (char **)malloc(sizeof(char *) * (4));
	if (!myenv->env)
	{
		free(myenv);
		return (NULL);
	}
        wd = ft_strdup("PWD=");
	myenv->env[0] = ft_strdup("SHLVL=1");
        myenv->env[1] = ft_str_join(wd, getcwd(cwd, sizeof(cwd)));
        free(wd);
        myenv->env[2] = ft_strdup("_=/usr/bin/env");
	myenv->env[3] = NULL;
	return (myenv);
}

int     main(int argc, char **argv, char **envp)
{
        (void)argc;
        (void)argv;
        char *line = NULL;
        t_env *my_env;
        t_tokenlist *token_list;
        t_parser *parser;
        int value ;
        //_=/usr/bin/env
        if(envp == NULL || *envp == NULL)
                my_env = manage_env_i();
        else
                my_env = init_env(envp);
        restore_signals();
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
                        printf("Error: Failed to tokenize input.\n");
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
                        printf("Error: Failed to create parser.\n");
                        free(line);
                        free_token_list(token_list); // Free token list
                        continue ;
                }
                value = parse_tokens(&parser, token_list, *my_env);
                if (!parser || !parser->command || value == -1)
                {
                        free(line);
                        free_token_list(token_list);
                        free_parser(parser);
                        continue ;
                }
                printf("\nParsed commands:\n");
                print_parser(parser);
                free_token_list(token_list);
                cmds_exec(parser, my_env);
                free(line);
                free_parser(parser);
        }
        ft_free_env(&my_env);
        return (EXIT_SUCCESS);
}










// int main(int argc, char **argv,char **envp)
// {
//      char *line = NULL;
//      (void)argc;
//      (void)argv;
//      t_env *my_env;
//      t_tokenlist *token_list;
//      t_parser *parser;
//      int value ;

//      line = "hi$HOME";

//      my_env = init_env(envp);
//      token_list = tokenize_input(line);
//              if (!token_list)
//              {
//                      fprintf(stderr,"Error: Failed to tokenize input.\n");
//              }
//              // Print tokens (for debugging)
//              printf("\nTokens:______________________\n");
//              print_token_list(token_list);
//              // Parse tokens
//              parser = create_parser(); // Initialize parser
//              if (!parser)
//              {
//                      fprintf(stderr, "Error: Failed to create parser.\n");
//                      free_token_list(token_list); // Free token list
//                      ft_free_env(&my_env);
//                      return 0;
//              }

//              value = parse_tokens(&parser, token_list, *my_env);
//              // Check if parser is valid
//              if (!parser || !parser->command || value == -1)
//              {
//                      // fprintf(stderr, "Error: Invalid parser or command.\n");
//                      free_token_list(token_list);
//                      free_parser(parser);
//                      ft_free_env(&my_env);

//                      return 0;
//              }

//              // Print parser nodes (for debugging)
//              printf("\nParsed commands:\n");