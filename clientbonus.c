/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clientbonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peazeved <peazeved@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 12:09:48 by peazeved          #+#    #+#             */
/*   Updated: 2026/01/16 17:21:47 by peazeved         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "struct.h"

volatile sig_atomic_t	g_ack = 0;

void	ft_wakeup(void)
{
	while (!g_ack)
		pause();
	g_ack = 0;
}

void	ft_clienthandler(int sig)
{
	if (sig == SIGUSR2)
		g_ack = 1;
}

void	ft_lenprocess(int len, int bit, int s_pid)
{
	if (len == 0)
	{
		while (bit >= 0)
		{
			kill(s_pid, SIGUSR1);
			ft_wakeup();
			bit--;
		}
		return ;
	}
	while (bit >= 0)
	{
		if ((len >> bit) & 1)
			kill(s_pid, SIGUSR2);
		else
			kill(s_pid, SIGUSR1);
		ft_wakeup();
		bit--;
	}
}

void	ft_charprocess(int len, char *msg, int s_pid)
{
	int		i;
	char	c;
	int		b;

	i = 0;
	while (i < len)
	{
		c = msg[i];
		b = 7;
		while (b >= 0)
		{
			if ((c >> b) & 1)
				kill(s_pid, SIGUSR2);
			else
				kill(s_pid, SIGUSR1);
			ft_wakeup();
			b--;
		}
		i++;
	}
}

int	main(int ac, char **av)
{
	int					s_pid;
	char				*msg;
	int					len;
	struct sigaction	sa;

	if (ac != 3)
		return (1);
	if (!ft_isdigit(av[1]))
	{
		ft_putstr("NUMBERS only!\n");
		return (1);
	}
	s_pid = ft_atoi(av[1]);
	msg = av[2];
	len = ft_strlen(msg);
	ft_pidverify(s_pid);
	sa.sa_handler = ft_clienthandler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGUSR2, &sa, NULL);
	ft_lenprocess(len, 31, s_pid);
	ft_charprocess(len, msg, s_pid);
	return (0);
}
