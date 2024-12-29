/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 12:26:55 by marvin            #+#    #+#             */
/*   Updated: 2024/12/29 13:22:43 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"


void ignore_signals() {
    // Set SIGINT and SIGQUIT in child process
    struct sigaction sa_int;
    sa_int.sa_handler = SIG_IGN;  // Custom handler for SIGINT
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = 0;
    sigaction(SIGINT, &sa_int, NULL);

    struct sigaction sa_quit;
    sa_quit.sa_handler = SIG_IGN;  // Default behavior for SIGQUIT (core dump)
    sigemptyset(&sa_quit.sa_mask);
    sa_quit.sa_flags = 0;
    sigaction(SIGQUIT, &sa_quit, NULL);
}
void set_signal_handler_heredoc()
{
    // Set SIGINT and SIGQUIT in child process
    struct sigaction sa_int;
    sa_int.sa_handler = ctrl_c_press_heredoc;  // Custom handler for SIGINT
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = 0;
    sigaction(SIGINT, &sa_int, NULL);

    struct sigaction sa_quit;
    sa_quit.sa_handler = SIG_IGN;  // Default behavior for SIGQUIT (core dump)
    sigemptyset(&sa_quit.sa_mask);
    sa_quit.sa_flags = 0;
    sigaction(SIGQUIT, &sa_quit, NULL);

}
void restore_signals() {
    // Set SIGINT and SIGQUIT in child process
    struct sigaction sa_int;
    sa_int.sa_handler = ctrl_c_press;  // Custom handler for SIGINT
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = 0;
    sigaction(SIGINT, &sa_int, NULL);

    struct sigaction sa_quit;
    sa_quit.sa_handler = SIG_IGN;  // Default behavior for SIGQUIT (core dump)
    sigemptyset(&sa_quit.sa_mask);
    sa_quit.sa_flags = 0;
    sigaction(SIGQUIT, &sa_quit, NULL);
}
void configure_child_signals() {
    // Set SIGINT to use custom handler
    struct sigaction sa_int;
    sa_int.sa_handler = ctrl_c_press;  // Custom handler for SIGINT
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = 0;
    sigaction(SIGINT, &sa_int, NULL);

    // Set SIGQUIT to default behavior (core dump)
    struct sigaction sa_quit;
    sa_quit.sa_handler = SIG_DFL;  // Default behavior for SIGQUIT (core dump)
    sigemptyset(&sa_quit.sa_mask);
    sa_quit.sa_flags = 0;
    sigaction(SIGQUIT, &sa_quit, NULL);
}
// Signal handler for Ctrl+C
void ctrl_c_press(int sig)
{
    // if (sig == SIGQUIT) // Check if the signal is SIGQUIT (Ctrl+\)
    // {
    //     return;
    // }
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