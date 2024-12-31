/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skreik <skreik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 12:26:55 by marvin            #+#    #+#             */
/*   Updated: 2024/12/30 11:21:23 by skreik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

void	ignore_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;
	struct sigaction	sa_stp;

	sa_int.sa_handler = SIG_IGN;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);

	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);

	sa_stp.sa_handler = SIG_IGN;
	sigemptyset(&sa_stp.sa_mask);
	sa_stp.sa_flags = 0;
	sigaction(SIGTSTP, &sa_stp, NULL);
}

void	set_signal_handler_heredoc(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;
	struct sigaction	sa_stp;

	sa_int.sa_handler = ctrl_c_press_heredoc;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);

	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);

	sa_stp.sa_handler = SIG_IGN;
	sigemptyset(&sa_stp.sa_mask);
	sa_stp.sa_flags = 0;
	sigaction(SIGTSTP, &sa_stp, NULL);
}

void	restore_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;
	struct sigaction	sa_stp;

	sa_int.sa_handler = ctrl_c_press;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);
	
	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);

	// signal(SIGTSTP, SIG_IGN);
	sa_stp.sa_handler = SIG_IGN;
	sigemptyset(&sa_stp.sa_mask);
	sa_stp.sa_flags = 0;
	sigaction(SIGTSTP, &sa_stp, NULL);
}

void	configure_child_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;
	struct sigaction	sa_stp;
	
	sa_int.sa_handler = ctrl_c_press;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);

	sa_quit.sa_handler = SIG_DFL;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);

	sa_stp.sa_handler = SIG_IGN;
	sigemptyset(&sa_stp.sa_mask);
	sa_stp.sa_flags = 0;
	sigaction(SIGTSTP, &sa_stp, NULL);
}

void	ctrl_c_press(int sig)
{
	if (sig == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		global_var = 130;
		printf("global var is %d\n",global_var);
		rl_redisplay();
	}
}

void	ctrl_c_press_heredoc(int sig)
{
	if (sig == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		global_var = 130;
		printf("global var is %d\n",global_var);
	}
	// if (sig == SIGQUIT)
	// 	return ;
}
