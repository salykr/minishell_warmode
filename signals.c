#include "mini_shell.h"

void	ctrl_c_press(int sig)
{
	(void)sig;
	// Move to a new line, as pressing Ctrl+C should interrupt the current input
	write(STDOUT_FILENO, "\n", 1);
	// Redisplay the prompt
	rl_on_new_line();
	  rl_replace_line("", 0); // Clear the current input line
	rl_redisplay(); // Redisplay the prompt
  global_var = sig + 128;
}

