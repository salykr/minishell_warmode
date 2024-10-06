/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skreik <skreik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 13:39:47 by skreik            #+#    #+#             */
/*   Updated: 2024/09/04 16:04:38 by skreik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "pipex.h"


// void	check_files(int argc, char *argv[], int *fd_1, int *fd_2)
// {
// 	if (argc != 5)
// 	{
// 		perror("Error: Incorrect number of arguments");
// 		exit(-1);
// 	}
// 	*fd_1 = open(argv[1], O_RDONLY);
// 	if (*fd_1 == -1)
// 	{
// 		perror("Error opening file");
// 		exit(1);
// 	}
// 	*fd_2 = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 	if (*fd_2 == -1)
// 	{
// 		perror("Error opening file");
// 		close(*fd_1);
// 		exit(1);
// 	}
// }

// void	handle_command(t_pipe p, char *cmd, char **envp, int *pipefd)
// {
// 	char	*cmd_path;

// 	dup2(p.pipe1, STDIN_FILENO);
// 	dup2(p.pipe2, STDOUT_FILENO);
// 	close(pipefd[0]);
// 	close(pipefd[1]);
// 	close(p.pipe1);
// 	close(p.pipe2);
// 	cmd_path = check_access(get_path(envp), ft_split(cmd, ' '));
// 	if (!cmd_path)
// 	{
// 		perror("Error command not found");
// 		exit(1);
// 	}
// 	execve(cmd_path, ft_split(cmd, ' '), envp);
// 	perror("Error executing command");
// 	exit(1);
// }

// int	main(int argc, char *argv[], char *envp[])
// {
// 	t_pipe		fd;
// 	t_pipe		p;
// 	int			pipefd[2];

// 	check_files(argc, argv, &fd.pipe1, &fd.pipe2);
// 	pipe(pipefd);
// 	if (fork() == 0)
// 	{
// 		p.pipe1 = fd.pipe1;
// 		p.pipe2 = pipefd[1];
// 		handle_command(p, argv[2], envp, pipefd);
// 	}
// 	if (fork() == 0)
// 	{
// 		p.pipe1 = pipefd[0];
// 		p.pipe2 = fd.pipe2;
// 		handle_command(p, argv[3], envp, pipefd);
// 	}
// 	close(pipefd[0]);
// 	close(pipefd[1]);
// 	close(fd.pipe1);
// 	close(fd.pipe2);
// 	wait(NULL);
// 	wait(NULL);
// 	return (0);
// }
