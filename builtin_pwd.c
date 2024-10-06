/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 12:50:51 by saoun             #+#    #+#             */
/*   Updated: 2024/10/06 12:33:15 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

// void builtin_pwd(t_env *env)
// {
//     char *buffer;
//     size_t size;
//     char *pwd_env;
//     char *new_buffer ;

//     size = INITIAL_SIZE;
//     buffer = (char *)malloc(size);
//     if (buffer == NULL)
//     {
//         perror("malloc");
//         exit(EXIT_FAILURE);
//     }
//     if (getcwd(buffer, size) == NULL)
//     {
//         if (errno == ERANGE)
//         {            size *= 2;
//             new_buffer = (char *)ft_realloc(buffer, size / 2, size); // Old size is size / 2
//             if (new_buffer == NULL)
//             {
//                 perror("ft_realloc");
//                 free(buffer); // Free old buffer
//                 exit(EXIT_FAILURE);
//             }
//             buffer = new_buffer;
//         }
//         else
//         {
//             pwd_env = ft_getenv(env,"PWD"); // Fetch the PWD environment variable
//             if (pwd_env != NULL)
//             {
//                 char *path = strstr(pwd_env, "PATH=");  // Look for "PATH=" in the PWD value
//                 if (path != NULL)
//                     printf("%s\n", path + 5);
//                 else
//                     printf("%s\n", pwd_env);  // Just print PWD if "PATH=" not found
//             }
//             else
//                 perror("getcwd and PWD not set");
//             free(buffer);
//             return;  // Exit the function since we have printed the result
//         }
//     }
//     printf("%s\n", buffer);
//     free(buffer);
// }
#include "mini_shell.h"


char *resize_buffer(char *buffer, size_t *size)
{
    *size *= 2;
    char *new_buffer;

    new_buffer = (char *)ft_realloc(buffer, *size / 2, *size);  // Old size is size / 2
    if (new_buffer == NULL)
    {
        perror("ft_realloc");
        free(buffer);
        exit(EXIT_FAILURE);
    }
    return new_buffer;
}

void handle_pwd_env(t_env *env)
{
    char *pwd_env;  // Fetch the PWD environment variable
    char *path;

    pwd_env = ft_getenv(env, "PWD");
    if (pwd_env != NULL)
    {
        path = strstr(pwd_env, "PATH=");  // Look for "PATH=" in the PWD value
        if (path != NULL)
            printf("%s\n", path + 5);  // Print value after "PATH="
        else
            printf("%s\n", pwd_env);  // Print PWD if "PATH=" not found
    }
    else
        perror("getcwd and PWD not set");
}

void builtin_pwd(t_env *env)
{
    size_t size;
    char *buffer;

    size = INITIAL_SIZE;
    buffer = (char *)malloc(size);
    if (buffer == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    while (getcwd(buffer, size) == NULL)
    {
        if (errno == ERANGE)
            buffer = resize_buffer(buffer, &size);
        else
        {
            handle_pwd_env(env);  // Handle using PWD from environment variables
            free(buffer);
            return;  // Exit the function since we have printed the result
        }
    }
    printf("%s\n", buffer);
    free(buffer);
}
