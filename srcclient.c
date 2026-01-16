/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   srcclient.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peazeved <peazeved@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 17:03:24 by peazeved          #+#    #+#             */
/*   Updated: 2026/01/16 17:21:56 by peazeved         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "struct.h"

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	ft_pidverify(int s_pid)
{
	if (kill(s_pid, 0) == -1)
	{
		ft_putstr("Invalid PID.\n");
		exit(1);
	}
}
