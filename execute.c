/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adokmak <adokmak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 12:55:26 by adokmak           #+#    #+#             */
/*   Updated: 2024/12/22 18:35:01 by adokmak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

// void manage_T_input(t_parser *parser, int re, int *fd)
// {
    
// }

void	cmds_exec(t_parser *parser, t_env *env)
{
    t_fd f;
    int fd[2];
    int status;
    
    f.fd_1 = STDIN_FILENO;
    f.fd_2 = STDOUT_FILENO;
    while (parser)
    {
        //If parser->next exists:
        //Create a pipe to connect the current command to the next.
        //Redirect the output of the current command to fd[1].
        if (parser->next && pipe(fd))
        {
            perror("Pipe failed");
            exit(EXIT_FAILURE);
        }
        input_redirection();
        output_redirection();
        if (is_builtin(parser))
            execute_builtin_command(parser, f, env, fd);
        else
            execute_command(parser, f, env, fd);
        
        // Manage pipe for the next command
        // Close write end of pipe
        // Pass read end to next command
        if(parser->next)
        {
            close(fd[1]);
            f.fd_1 = fd[0];
        }
        parser = parser->next;
    }
    // Wait for all child processes to finish
    while (wait(&status) > 0) 
    {
        if (WIFEXITED(status)) 
        global_var = WEXITSTATUS(status);
    }
}

void input_redirection(t_parser *parser, t_fd *f)
{
    if (parser->prev == NULL)
    {
        //parser->input == NULL: Indicates no prior input is being redirected.
        //parser->delimeter != NULL: Ensures a delimiter is defined for heredoc.
        //parser->redirection[0] == T_HEREDOC: Ensures this specific command has heredoc
        if (parser->delimeter != NULL && parser->redirection[0] == T_HEREDOC)
        {
            //ft_redirection_delimiter(parser) is called.
            //The function reads input from the user line-by-line until "EOF" is encountered.
            //Captured input is stored in parser->heredoc.
            ft_redirection_delimiter(parser);
        }
        //Input redirection (<) from a file.
        //Heredoc (<<) redirection from previously captured lines.
        if (parser->infile != NULL && parser->redirection != NULL && 
        (parser->redirection[0] == T_INPUT || parser->redirection[0] == T_HEREDOC))
        {
            f->fd_1 = ft_handle_redirections(parser, parser->redirection[0]);
        }  
    }
}
//f->fd_2 : determines the destination of the command's output
/*parser->redirection[0]: Represents the first redirection type associated with the command.
parser->redirection[1]: Represents the second redirection type (if any).*/
void output_redirection(t_parser *parser, t_fd *f, int fd[2])
{
    if (parser->next)
    {
        /*for last command in pipeline ,If no redirection is specified, 
        output goes to the default standard output ex: echo "hello world" no output
        redirection so iss printed in the terminal*/
        if (parser->outfile == NULL)
            f->fd_2 = STDOUT_FILENO;
        /*If an output file is specified, handle it based on the redirection type (> or >>).
        T_OUTPUT: Write to the file, overwriting its contents.
        T_APPEND: Append to the file without overwriting.*/    
        else if (parser->outfile != NULL && parser->redirection != NULL &&
        ((parser->redirection[0] == T_OUTPUT ||
        parser->redirection[0] == T_APPEND) ||
        (parser->redirection[1] != '\0' &&
        (parser->redirection[1] == T_OUTPUT ||
        parser->redirection[1] == T_APPEND)))) 
        {
            /*echo "Hello" > output1.txt >> output2.txt
            parser->redirection[0] == T_OUTPUT (for > output1.txt)
            parser->redirection[1] == T_APPEND (for >> output2.txt)
            parser->outfile = {"output1.txt", "output2.txt"}*/
            if (parser->redirection[1] != '\0')
            {
                f->fd_2 = ft_handle_redirections(parser, parser->redirection[1]);                           
            }
            else
            {
                f->fd_2 = ft_handle_redirections(parser, parser->redirection[0]);
            }        
        }
    }
    /*part of pipeline : If there is a next command in the pipeline, 
    redirect output to the pipe's write end
    the first command become the input (fd[0]) for second command */
    else
    {
        f->fd_2 = fd[1];
    }
}
