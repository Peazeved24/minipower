/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peazeved <peazeved@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 17:05:40 by peazeved          #+#    #+#             */
/*   Updated: 2026/01/16 17:04:03 by peazeved         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

# include <signal.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

typedef struct s_struct
{
	int		j;
	int		i;
	int		len;
	char	c;
	char	*msg;
}			t_struct;

// client.
int			ft_strlen(char *str);
int			ft_atoi(char *str);
void		ft_putstr(char *str);
void		ft_putchar(char c);
void		ft_putnbr(unsigned int n);
int			ft_isdigit(char *str);
void		ft_pidverify(int s_pid);
// server
void		ft_msghandler(t_struct *s);
void		ft_resetvars(t_struct *s, int *g_bits);
void		ft_putmsg(t_struct *s);
void		*ft_memset(void *b, int c, size_t len);
#endif
