/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skreik <skreik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 12:26:55 by marvin            #+#    #+#             */
/*   Updated: 2024/12/27 16:52:04 by skreik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"


// Signal handler for Ctrl+C
void ctrl_c_press(int sig)
{
    if (sig == SIGQUIT) // Check if the signal is SIGQUIT (Ctrl+\)
    {
        return;
    }
    if(sig == SIGINT)
    {
        write(STDOUT_FILENO, "\n", 1);
        rl_on_new_line();
        rl_replace_line("", 0);
        global_var = 130;
        // rl_done = 1; // Exit readline immediately
        rl_redisplay();
    }
}
void ctrl_c_press_heredoc(int sig)
{
    if(sig == SIGINT)
    {
        write(STDOUT_FILENO, "\n", 1);
        rl_on_new_line();
        rl_replace_line("", 0);
        global_var = 130;
        // rl_done = 1; // Exit readline immediately
        // rl_redisplay();
    }
    if (sig == SIGQUIT) // Check if the signal is SIGQUIT (Ctrl+\)
    {
        return;
    }
}
void ctrl_backslash(int sig)
{
    if (sig == SIGQUIT) // Check if the signal is SIGQUIT (Ctrl+\)
    {
        exit(131);
    }
}