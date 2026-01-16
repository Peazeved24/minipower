/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   srcserver.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peazeved <peazeved@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 01:20:16 by peazeved          #+#    #+#             */
/*   Updated: 2026/01/16 17:00:49 by peazeved         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "struct.h"

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*ptr;
	size_t			i;

	ptr = (unsigned char *)b;
	i = 0;
	while (i < len)
	{
		ptr[i] = (unsigned char)c;
		i++;
	}
	return (b);
}

void	ft_msghandler(t_struct *s)
{
	if (s->msg)
		return ;
	s->msg = malloc(s->len + 1);
	if (!s->msg)
		exit(1);
	ft_memset(s->msg, 0, s->len + 1);
}

void	ft_resetvars(t_struct *s, int *g_bits)
{
	s->len = 0;
	s->i = 0;
	s->j = 0;
	s->c = 0;
	*g_bits = 0;
	if (s->msg)
	{
		free(s->msg);
		s->msg = NULL;
	}
}

void	ft_putmsg(t_struct *s)
{
	if (!s->msg)
		return ;
	write(1, s->msg, s->len);
	ft_putchar('\n');
	free(s->msg);
	s->msg = NULL;
}
