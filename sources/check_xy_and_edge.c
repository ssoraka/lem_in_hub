/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_xy_and_edge.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljalikak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/18 14:04:53 by ljalikak          #+#    #+#             */
/*   Updated: 2019/08/18 14:04:55 by ljalikak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_list.h"

void	check_xy(t_val *h)
{
	char	**av;

	if (!h)
		return ;
	while (h)
	{
		if (!h->isnum && !h->iscom && ft_strcmp(h->line, "##start")
		&& ft_strcmp(h->line, "##end"))
		{
			av = ft_strsplit(h->line, ' ');
			if (num_w(av) == 3 && av[0][0] != 'L' && check_realnum(av[1])
			&& check_realnum(av[2]))
				h->isxy = 1;
			else
			{
				del_split(av);
				break ;
			}
			del_split(av);
		}
		h = h->next;
	}
	return (check_edge(h));
}

void	check_edge(t_val *h)
{
	char	**av;

	if (!h)
		return ;
	while (h)
	{
		if (!h->iscom)
		{
			av = ft_strsplit(h->line, '-');
			if (num_w(av) == 2 && num_c(h->line, '-') == 1)
				h->isedge = 1;
			else
			{
				del_split(av);
				break ;
			}
			del_split(av);
		}
		h = h->next;
	}
}

int		num_w(char **av)
{
	int i;

	i = 0;
	while (av[i])
		i++;
	return (i);
}

int		num_c(char *s, int ch)
{
	int i;
	int count;

	i = 0;
	count = 0;
	while (s[i])
	{
		if (s[i] == ch)
			count++;
		i++;
	}
	return (count);
}
