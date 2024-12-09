/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 12:26:55 by marvin            #+#    #+#             */
/*   Updated: 2024/12/09 22:36:31 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

void	ctrl_c_press(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	// global_var = sig + 128;
	global_var = 1;
	    rl_done = 1;     // Force readline to exit immediately, simulating EOF
	// kill(getpid(), SIGKILL  );  // Send SIGINT (Ctrl+C) to the current process
	printf("ctrl_c_press, global var: %d\n",global_var);
}
