/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 11:19:23 by bazaluga          #+#    #+#             */
/*   Updated: 2024/02/06 14:10:09 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf-breaker.h"

int			g_exit_code;
pid_t		g_pid;
t_ft		fcts[N_TESTS_FUN];
print_buff	buff;
int			g_in_fun;
int			fds[2];
/* char		pipe_buff[2147483648]; */

void	pipe_stdout(int start)
{
	static int	stdout = -1;

	if (start)
	{
		stdout = dup(STDOUT_FILENO);
		/* close(STDOUT_FILENO); */
		if (pipe(fds) == -1)
		{
			perror("Pipe error.");
			exit(EXIT_FAILURE);
		}
		dup2(fds[1], STDOUT_FILENO);
	}
	else
	{
		close(fds[1]);
		fds[1] = -1;
		read(fds[0], &pipe_buff, BUFFSIZE);
		close(fds[0]);
		fds[0] = -1;
		dup2(stdout, STDOUT_FILENO);
	}
}

void	manage_outputs(int to_close)
{
	static int	stdout = -1;
	static int	stderr = -1;

	if (to_close)
	{
		stdout = dup(STDOUT_FILENO);
		stderr = dup(STDERR_FILENO);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);
	}
	else if (stdout != -1 && stderr != -1)
	{
		dup2(stdout, STDOUT_FILENO);
		dup2(stderr, STDERR_FILENO);
		close(stdout);
		close(stderr);
	}
}

void	manage_pipes(int to_open, int to_close)
{
	if (to_open)
	{
		if (pipe(fds) == -1)
		{
			perror("Pipe error.");
			exit(EXIT_FAILURE);
		}
	}
	else if (to_close)
	{
		close(fds[1]);
		fds[1] = -1;
		read(fds[0], &pipe_buff, BUFFSIZE);
		close(fds[0]);
		fds[0] = -1;
	}
}

void	add_color_buff(char c)
{
	buff.color = c;
}

void	printbuff()
{
	char	*color;

	if (buff.txt[0])
	{
		color = buff.color == 'r' ? ANSI_COLOR_RED : ANSI_COLOR_GREEN;
		printf("%s%s%s", color, buff.txt, ANSI_COLOR_RESET);
		buff.txt[0] = '\0';
		buff.color = '\0';
	}
}
