#include "minishell.h"

int	sig_exit(void)
{
	return (EXIT_SUCCESS);
}

static void	handle_signal_exec(int sig)
{
	g_signal_received = 128 + sig;
}

void	init_signal_exec(void)
{
	struct sigaction	sa;

	rl_event_hook = sig_exit;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = &handle_signal_exec;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGTSTP, &sa, NULL);
}

static void	sigint_interactive_mode(int sig)
{
	g_signal_received = 128 + sig;
	write(STDERR_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	init_signal_interactive_mode(void)
{
	struct sigaction	sa;

	rl_event_hook = sig_exit;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = &sigint_interactive_mode;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
	sigaction(SIGTSTP, &sa, NULL);
}
