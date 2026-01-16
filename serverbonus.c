/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverbonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peazeved <peazeved@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 13:01:45 by peazeved          #+#    #+#             */
/*   Updated: 2026/01/16 16:58:51 by peazeved         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "struct.h"

static int	g_bits;

int	ft_charhanlder(t_struct *s, int sig)
{
	s->c <<= 1;
	if (sig == SIGUSR2)
		s->c |= 1;
	s->i++;
	if (s->i == 8)
	{
		s->i = 0;
		return (1);
	}
	return (0);
}

int	ft_lenhandler(t_struct *s, int sig)
{
	if (g_bits < 32)
	{
		s->len <<= 1;
		if (sig == SIGUSR2)
			s->len |= 1;
		g_bits++;
		if (g_bits == 32)
		{
			if (s->len == 0)
			{
				s->len = 0;
				g_bits = 0;
				return (1);
			}
			ft_msghandler(s);
		}
		return (1);
	}
	return (0);
}

void	ft_serverhandler(int sig, siginfo_t *info, void *ctx)
{
	static t_struct	s;

	(void)ctx;
	if (ft_lenhandler(&s, sig))
	{
		kill(info->si_pid, SIGUSR2);
		return ;
	}
	if (ft_charhanlder(&s, sig))
	{
		if (s.j < s.len)
			s.msg[s.j++] = s.c;
		s.c = 0;
		if (s.j == s.len)
		{
			ft_putmsg(&s);
			ft_resetvars(&s, &g_bits);
		}
	}
	kill(info->si_pid, SIGUSR2);
}

static void	ft_singalsetup(void)
{
	struct sigaction	sa;

	sa.sa_sigaction = ft_serverhandler;
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
}

int	main(void)
{
	unsigned int	pid;

	pid = getpid();
	write(1, "PID: ", 5);
	ft_putnbr(pid);
	write(1, "\n", 1);
	ft_putstr("________________TEXT_ZONE________________\n");
	ft_singalsetup();
	while (1)
		pause();
	return (0);
}
