/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skreik <skreik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 12:56:23 by skreik            #+#    #+#             */
/*   Updated: 2024/12/27 12:11:47 by skreik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// void manage_redirection_input(t_parser *parser, int *fd)
// {
// 	int i;
// 	int j;
// 	int fd_heredoc;
// 	int fd_file;

// 	fd_file = -1 ;
// 	fd_heredoc  = -1;
// 	i = 0;
// 	j = 0;
// 	if(parser->redirection && check_heredoc_existence(parser->redirection))
// 	{
// 		write_in_heredoc(parser);
// 		fd_heredoc = handle_heredoc(parser->heredoc);	
// 	}
// 	while(parser->redirection && parser->redirection[i]!='\0') //increment i here eza badi aamel pre increment
// 	{
// 		if(parser->redirection[i] == T_INPUT)
// 		{
// 			if (parser->infile != NULL && parser->infile[j] != NULL && parser->redirection[i] == T_INPUT)
// 			{
// 				if(j != 0)
// 					close(fd_file);
// 				fd_file = open(parser->infile[j], O_RDONLY);
// 				if(fd_file == -1)
// 					perror("Error: Input file");
// 				j++;
// 			}
// 		}
// 		i++;
// 	}
// 	if(parser->redirection[get_last_input_redirection(parser->redirection)] == T_HEREDOC)
// 	{
// 		if(fd_file != -1)
// 			close(fd_file);
// 		*fd = fd_heredoc;
// 	}
// 	else if (parser->redirection[get_last_input_redirection(parser->redirection)] == T_INPUT)
// 	{
// 		if(fd_heredoc != -1)
// 			close(fd_heredoc);
// 		*fd = fd_file;
// 	}
// }
#include "mini_shell.h"

void	execute_command(t_parser *parser, t_fd f, t_env *env, int fd[2])
{
	char	*cmd_path;
	// char	**args;
	// int		heredoc_fd;
	pid_t	pid;
	(void)fd;

	initialize_execution(parser, env, &cmd_path);
	printf("args\n");
	print_2d_array(parser->args);
	pid = fork();
	if (pid == -1)
		exit(EXIT_FAILURE);
	if (pid == 0)
	{
		printf("cmd: %s\n",parser->command);
		manage_input_output(&f, fd, parser);
		if(f.fd_1 == -1 ||  f.fd_2 == -1)
			return;
		if(ft_getenv(env, "PATH")!=NULL)
			execve(cmd_path, parser->args, env->env);
		perror("Error");
		exit(EXIT_FAILURE);
	}
	else
	{
		handle_child_exit(pid,&f, parser);
		// free_input(args);
		free(cmd_path);
	}
}

int save_original_fds(int *original_stdin, int *original_stdout)
{
    *original_stdin = dup(STDIN_FILENO);
    *original_stdout = dup(STDOUT_FILENO);
    if (*original_stdin == -1 || *original_stdout == -1)
	{
        perror("Error saving original file descriptors");
        return -1;
    }
    return 0;
}
void restore_original_fds(int original_stdin, int original_stdout)
{
    if (dup2(original_stdin, STDIN_FILENO) == -1)
        perror("Error restoring STDIN");
    if (dup2(original_stdout, STDOUT_FILENO) == -1)
        perror("Error restoring STDOUT");
    close(original_stdin);
    close(original_stdout);
}

void	execute_builtin_command(t_parser *parser, t_fd f, t_env *env, int fd[2])
{
	pid_t	pid;
	int original_stdin;
	int original_stdout;

 	if (parser->next == NULL) 
	{
        if (save_original_fds(&original_stdin, &original_stdout) == -1)
            return; // Handle error
        manage_input_output(&f, fd, parser);
		buitlin(parser, env);
        restore_original_fds(original_stdin, original_stdout);
    } 
	else
	{
		pid = fork();
		if (pid == -1)
			exit(EXIT_FAILURE);
		if (pid == 0) // Child process
		{
			manage_input_output(&f, fd, parser);
			buitlin(parser, env);
			exit(global_var);
		}
		else // Parent process
			handle_child_exit(pid, &f, parser);
	}
}


int check_permissions(const char *filepath, int flag)
{
	//input flag = 0
	//output flag =1
    struct stat file_stat;

    if (flag == 0 && stat(filepath, &file_stat) == -1)
	{
        perror("Error");
        return (0);
    }
	if (flag == 0 && !(file_stat.st_mode & S_IRUSR))
	{
        printf("Permission denied\n");
		return (0);
    }
	if ( flag ==1 && stat(filepath, &file_stat) != -1)
	{
		if (!(file_stat.st_mode & (S_IRUSR | S_IWUSR | S_IXUSR)))
		{
			printf("Permission denied\n");
			return (0);
		}
	}
	return(1);
}

// int check_permissions(const char *filepath)
// {
//     // Check read, write, and execute permissions
//     if (access(filepath, R_OK | W_OK | X_OK) != 0)
// 	{
//         printf("Permission denied\n");
// 		return (0);
// 	}
// 	return(1);
// }
int manage_redirection_output(t_parser *parser, int *fd)
{
	int i;
	int j;

	i = -1;
	j = -1;
	while(parser->redirection && parser->redirection[++i] != '\0')
	{
		if(parser->redirection[i] == T_OUTPUT || parser->redirection[i] == T_APPEND)
		{
			if (parser->outfile != NULL && parser->outfile[++j] != NULL && parser->redirection[i] == T_OUTPUT)
			{
				if (!check_permissions(parser->outfile[j], 1))
               	{
					global_var = 1;
                   return(1);
				}
				if(j!=0)
					close(*fd);
				*fd = open(parser->outfile[j], O_WRONLY | O_CREAT | O_TRUNC, 0644);
				// j++;
			}
			else if (parser->outfile != NULL && parser->outfile[j] != NULL && parser->redirection[i] == T_APPEND)
			{
				if (!check_permissions(parser->outfile[j], 1))
               	{
					global_var = 1;
                   return(1);
				}
				if(j != 0)
					close(*fd);
				*fd = open(parser->outfile[j], O_WRONLY | O_CREAT | O_APPEND, 0644);
			}
		}
	}
	return(0);
}


int check_heredoc_existence(int *redirection)
{
	int i;
	
	i = 0;
	while(redirection[i]!='\0')
	{
		if(redirection[i] == T_HEREDOC)
			return (1);
		i++;
	}
	return(0);
}

int get_last_input_redirection(int *redirection)
{
    int i;
    int last_index; // Initialize to -1 to indicate no valid redirection found

	i = 0;
	last_index = -1;
    while (redirection && redirection[i] != '\0') // Iterate through the array
    {
        if (redirection[i] == T_HEREDOC || redirection[i] == T_INPUT) // Check for T_HEREDOC or T_INPUT
            last_index = i; // Update the last index whenever we find a match
        i++; // Move to the next element
    }
    return (last_index); // Return the last found index, or -1 if none found
}



int manage_redirection_input(t_parser *parser, int *fd)
{
    int i;
    int j;
    int fd_heredoc;
    int fd_file;

    fd_file = -1;
    fd_heredoc = -1;
    i = -1;  // Initialize to -1 for pre-increment logic
    j = -1;
    if (parser->redirection && check_heredoc_existence(parser->redirection))
    {
        write_in_heredoc(parser);
        fd_heredoc = handle_heredoc(parser->heredoc);
    }
    while (parser->redirection && parser->redirection[++i] != '\0')  // Pre-increment i
    {
        if (parser->redirection[i] == T_INPUT)
        {
            if (parser->infile != NULL && parser->infile[++j] != NULL)  // Pre-increment j
            {
				if (!check_permissions(parser->infile[j], 0))
				{
					global_var = 1;
                    return(1);
				}
                if (j != 0)
                    close(fd_file);
                fd_file = open(parser->infile[j], O_RDONLY);
                if (fd_file == -1)
                    perror("Error: Input file");
            }
        }
    }
    if (parser->redirection[get_last_input_redirection(parser->redirection)] == T_HEREDOC)
    {
        if (fd_file != -1)
            close(fd_file);
        *fd = fd_heredoc;
    }
    else if (parser->redirection[get_last_input_redirection(parser->redirection)] == T_INPUT)
    {
        if (fd_heredoc != -1)
            close(fd_heredoc);
        *fd = fd_file;
    }
	return(0);
}
void manage_pipe(t_parser *parser,t_fd *f, int fd[2])
{
	if(parser->next != NULL)
		f->fd_2 = fd[1];
}
int	handle_input_output(t_parser *parser,t_fd *f, int fd[2])
{	
	manage_pipe(parser,f, fd);
	if(parser->redirection)
	{
		if(parser->infile != NULL || parser->delimeter != NULL)
		{
			if(manage_redirection_input(parser,&f->fd_1) == 1)
				return (0);
		}
		if(parser->outfile !=NULL)
		{
			if(manage_redirection_output(parser,&f->fd_2) == 1)	
				return (0);
		}
	}
	return(1);
}

void	cmds_exec(t_parser *parser, t_env *env)
{
	t_fd f;
	int fd[2];
	int status;

	f.fd_1 = STDIN_FILENO;
	while (parser)
	{
		if (parser->next)
			pipe(fd);
		f.fd_2 = STDOUT_FILENO;
		if(handle_input_output(parser,&f, fd) == 0)
			return;
		if (is_builtin(parser))
			execute_builtin_command(parser, f, env, fd);
		else
			execute_command(parser, f, env, fd);
		if (parser->next) // to handle input of cmd
		{
			close(fd[1]);
			f.fd_1 = fd[0];
		}
		parser = parser->next;
	}
	if (f.fd_1 != STDIN_FILENO)
		close(f.fd_1);
	while (wait(&status) > 0)
	{
		if (WIFEXITED(status))
			global_var = WEXITSTATUS(status);
	}
}
