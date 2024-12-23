/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adokmak <adokmak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 12:55:26 by adokmak           #+#    #+#             */
/*   Updated: 2024/12/23 12:53:40 by adokmak          ###   ########.fr       */
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
        input_redirection(parser, &f);
        output_redirection(parser, &f, fd);
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

int ft_redirection_delimiter(t_parser *parser, int re)
{
    int fd;
    
    fd = -1;
    /*Input (<) via manage_T_input.
    Output (>) via manage_T_output.
    Append (>>) via manage_T_append*/
    manage_T_input(parser, re, &fd);
    manage_T_output(parser, re, &fd);
    manage_T_append();
    if (fd == -1)
    {
        /*If no valid file descriptor is obtained (fd == -1), 
        it reports an error and exits.*/
        perror("Error opening file");
        exit(1);
    }
    return (fd);
}

void    manage_T_input(t_parser *parser, int re,int *fd)
{
    if (re == T_INPUT)
    {
       if (parser->infile != NULL)
            *fd = open(parser->infile,O_RDONLY);
        else
            perror("Error: Input file not specified");
    }
}

void    manage_T_output(t_parser *parser, int re, int *fd)
{
    int i;

    i = -1;
    if (re == T_OUTPUT)
    {
        /*pointer l2n bade 3adel 3ala file b2albo
        why? ye3ne ana b2alb lfd fi l address lal fd
        fa ana eza htyt fd bala pointer y3ne bkun 3am 3adel 
        3a shu 3amle pointer l fd
        kermel hyk lezem hot *fd l2n bade 3adel aal 
        file directly li b2albo ye3nee*/
        if (parser->outfile != NULL)
        {   
            while(parser->outfile[++i])
            { 
                *fd = open(parser->outfile,O_WRONLY, O_CREAT, O_TRUNC, 0644);
                if(parser->outfile[i + 1] != '\0')
                    close(*fd);    
            }
        }
        else
            perror("Error: Output file not specified");
    }
}

void    manage_T_append(t_parser *parser, int re, int *fd)
{
    int i;

    i = -1;
    if (re == T_APPEND)
    {
        while (parser->outfile[++i] != NULL)
        {
            *fd = open(parser->outfile, )
        }
    }
}

/* Summary of ft_handle_redirections
Purpose: Centralizes logic for handling all types of redirections.
Flexibility: Supports multiple redirection types (<, >, >>).
Integration: Used by input_redirection 
and output_redirection to open files and manage file descriptors.
*/
