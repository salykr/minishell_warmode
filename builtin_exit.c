/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 11:25:53 by skreik            #+#    #+#             */
/*   Updated: 2024/10/15 19:22:22 by root             ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "mini_shell.h"

int is_numeric(const char *str)
{
    if (!str || !*str)
        return (0);
    if (*str == '-' || *str == '+') // Handle sign
        str++;
    if (*str == '\0') 
        return (0);
    while (*str)
    {
        if (!isdigit(*str))
            return (0);
        str++;
    }
    return (1);
}

void handle_exit_input(t_parser *parser)
{
    if (parser->input != NULL)
        parser->input[0] = remove_quotes(parser->input[0]);
}

int handle_exit_arguments(char *input)
{
    if (!is_numeric(input))
    {
        fprintf(stderr, "minishell: exit: %s: numeric argument required\n", input);
        global_var = 2; // Return error
    }
    else
    {
        fprintf(stderr, "minishell: exit: too many arguments\n");
        global_var = 1;
        return (1); // Indicate too many arguments, return without exiting
    }
    return (0); // Normal flow, continue to exit
}
int adjust_number(int num)
{
    if (num < 0)
    {
        while (num < 0)
            num = num + 256;
    }
    else if (num >= 256)
    {
        while (num >= 256)
            num = num - 256;
    }
    return (num);
}

void validate_numeric_input(char *input)
{
    char *endptr;
    long long num;
   
    num = strtoll(input, &endptr, 10);
    if (is_numeric(input))
    {
        if (*endptr != '\0')
        {
            fprintf(stderr, "minishell: exit: %s: numeric argument required\n", input);
            global_var = 2;
        }
        else
        {
            num = adjust_number(num);
            global_var = (int)num;
        }
    }
    else
    {
        fprintf(stderr, "minishell: exit: %s: numeric argument required\n", input);
        global_var = 2;
    }
}


void cleanup_and_exit(t_env *myenv)
{
    ft_free_env(&myenv); // Free environment variables
    exit(global_var); // Exit the shell with the specified exit code
}
void builtin_exit(t_parser *parser, t_env *myenv)
{
    printf("exit\n"); // Print "exit" before exiting the shell
    handle_exit_input(parser);

    if (parser->input == NULL)
        global_var = 0;
    else if (parser->input[1])
    {
        if (handle_exit_arguments(parser->input[0]))
            return; // Too many arguments, do not exit
    }
    else
        validate_numeric_input(parser->input[0]);
    cleanup_and_exit(myenv);
}


// void builtin_exit(t_parser *parser, t_env *myenv)
// {
//     printf("exit\n"); // Print "exit" before exiting the shell
//     if (parser->input != NULL)
//     {
//        parser->input[0] = remove_quotes(parser->input[0]);
//        printf("inputtttt: %s\n\n",parser->input[0]);
//     }
//     if (parser->input == NULL)
//         global_var = 0;
//     else if (parser->input[1]) // More than one argument
//     {
//         if (!is_numeric(parser->input[0]))
//             fprintf(stderr, "minishell: exit: %s: numeric argument required\n", parser->input[0]);
//         else
//         {
//             fprintf(stderr, "minishell: exit: too many arguments\n");
//             global_var = 1;
//             return; // Do not exit, just return
//         }
//     }
//     else if (is_numeric(parser->input[0])) // Check if the argument is numeric
//     {
//         char *endptr;
//         errno = 0; // Clear errno before conversion
//         long long num = strtoll(parser->input[0], &endptr, 10);

//         if (errno != 0 || *endptr != '\0' || num < 0 || num > 255)
//         {
//             fprintf(stderr, "minishell: exit: %s: numeric argument required\n", parser->input[0]);
//             global_var = 2;
//         }
//         else
//             global_var = (int)num; // Set global_var to the valid numeric value
//     }
//     else // Non-numeric argument
//     {
//         fprintf(stderr, "minishell: exit: %s: numeric argument required\n", parser->input[0]);
//         global_var = 2;
//     }
//     ft_free_env(&myenv); // Free environment variables
//     exit(global_var); // Exit the shell with the specified exit code
// }

/*
exit  : prints exit and exits (normal behavior)
exit 42 : prints exit and exits (normal behavior)
exit 000042: prints exit and exits (normal behavior)
exit 666: prints exit and exits (normal behavior)
exit +666, exit -666 , exit -259 , exit -0000042, exit -42, exit -4 ,
exit 259, exit 0, exit -0 , exit +0 : prints exit and exits(normal behavior)
 
 
exit exit: numeric arg required and exits
exit hola: numeric arg required and exits
exit hola que tal : numeric arg required and exits
exit hola 666 666: numeric argument required nad exits
exit hola 666  : numeric required and exits
exit --666, exit ++++666 ,
exit -----0, exit +++++0 : numeric argument required, and exits


exit 666 666 : too many args and doesnt exit
exit -666 666 : too many args and doesnt exit
exit 666 666 666 666: too many args and doesnt exit
exit 666 hola 666: too many args and doesnt exit
exit -69 -96 :  too many arguments and doesnt exit
*/