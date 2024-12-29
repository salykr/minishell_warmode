/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 12:56:23 by skreik            #+#    #+#             */
/*   Updated: 2024/12/29 18:00:16 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

/*
	manage_input_output(&f, fd, parser);
		if(ft_getenv(env, "PATH")!=NULL)
		{
			if (execve(cmd_path, parser->args, env->env) == -1)
			{
				perror("execve");
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			printf("Error: PATH environment variable not set\n");
			exit(EXIT_FAILURE);
		}

*/
void	execute_command(t_parser *parser, t_fd f, t_env *env, int fd[2])
{
	char	*cmd_path;
	pid_t	pid;

	initialize_execution(parser, env, &cmd_path);
	pid = fork();
	if (pid == -1)
		exit(EXIT_FAILURE);
	if (pid == 0)
	{
		configure_child_signals();
		manage_input_output(&f, fd, parser);
		if(ft_getenv(env, "PATH")!=NULL)
		{
			if (execve(cmd_path, parser->args, env->env) == -1)
				perror("execve");
		}
		else
			printf("Error: PATH environment variable not set\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		ignore_signals();  // Ignore SIGINT and SIGQUIT for parent
		return(handle_child_exit(&f), free(cmd_path));
	}
}


void	execute_builtin_command(t_parser *parser, t_fd f, t_env *env, int fd[2])
{
	pid_t	pid;
	t_fd original_fds;

 	if (parser->next == NULL) 
	{
        if (save_original_fds(&original_fds.fd_1, &original_fds.fd_2) == -1)
            return;
        manage_input_output(&f, fd, parser);
		buitlin(parser, env);
        restore_original_fds(original_fds.fd_1, original_fds.fd_2);
    } 
	else
	{
		pid = fork();
		if (pid == -1)
			exit(EXIT_FAILURE);
		if (pid == 0)
		{
			manage_input_output(&f, fd, parser);
			buitlin(parser, env);
			exit(global_var);
		}
		else 
			handle_child_exit(&f);
	}
}




void handle_wait_status(t_parser *parser)
{
    int status;

    while (wait(&status) > 0)
    {
        if (WIFEXITED(status))  // Normal termination
        {
            if (parser && parser->next == NULL)
                global_var = WEXITSTATUS(status);
        }
        else if (WIFSIGNALED(status))  // Terminated by a signal
            global_var = WTERMSIG(status) + 128;  // Get the signal number
        else
            global_var = status;  // Handle other cases, if needed
    }
}

int	handle_io_and_execute(t_parser *parser, t_env *env, t_fd *f, int fd[]) 
{
    int val;

	val = handle_input_output(parser, f, fd);
    if (val == 0)
        return (0);
    else if (val == -1)
    {
        global_var = 130;
        return (-1);
    }
    if (is_builtin(parser))
        execute_builtin_command(parser, *f, env, fd);
    else
		execute_command(parser, *f, env, fd);
	return(1);
}

void	cmds_exec(t_parser *parser, t_env *env)
{
    t_fd f;
    int fd[2];

    f.fd_1 = STDIN_FILENO;
    while (parser)
    {
        if (parser->next)
            pipe(fd);
        f.fd_2 = STDOUT_FILENO;
        if(handle_io_and_execute(parser, env, &f, fd) != 1)
			return;
        if (parser->next)
        {
            close(fd[1]);
            f.fd_1 = fd[0];
        }
        parser = parser->next;
    }
    if (f.fd_1 != STDIN_FILENO)
        close(f.fd_1);
    handle_wait_status(parser);
    restore_signals();
}
// void	cmds_exec(t_parser *parser, t_env *env)
// {
// 	t_fd f;
// 	int fd[2];
// 	int val;

// 	f.fd_1 = STDIN_FILENO;
// 	while (parser)
// 	{
// 		if (parser->next)
// 			pipe(fd);
// 		f.fd_2 = STDOUT_FILENO;
// 		val = handle_input_output(parser,&f, fd); 
// 		if(val == 0)
// 			return;
// 		else if(val == -1)
// 		{
// 			global_var = 130;
// 			return;
// 		}
// 		if (is_builtin(parser))
// 			execute_builtin_command(parser, f, env, fd);
// 		else
// 			execute_command(parser, f, env, fd);
// 		if (parser->next) // to handle input of cmd
// 		{
// 			close(fd[1]);
// 			f.fd_1 = fd[0];
// 		}
// 		parser = parser->next;
// 	}
// 	if (f.fd_1 != STDIN_FILENO)
// 		close(f.fd_1);
//     handle_wait_status(parser);
// 	restore_signals();
// }
