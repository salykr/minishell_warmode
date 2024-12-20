/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adokmak <adokmak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 12:26:55 by marvin            #+#    #+#             */
/*   Updated: 2024/12/17 17:26:06 by adokmak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"


// Signal handler for Ctrl+C
void ctrl_c_press(int sig)
{
    (void)sig;
    g_interrupt = 1; // Set the interrupt flag
    write(STDOUT_FILENO, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    global_var = 130;
    rl_done = 1; // Exit readline immediately
    rl_redisplay();
    
}