/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   duplicate.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljalikak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/18 14:19:58 by ljalikak          #+#    #+#             */
/*   Updated: 2019/08/18 14:20:01 by ljalikak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_list.h"

void	check_dupl_xy(t_val *a, t_val *b)
{
	t_val *tmp;

	tmp = b;
	if (!a || !b)
		return ;
	while (a->next)
	{
		while (b->next)
		{
			if (a->isxy && b->isxy && ft_strcmp(a->line, b->line) && a != b)
				if (((ft_strcmp(a->name, b->name) && !ft_strcmp(a->x_y, b->x_y))
				|| (!ft_strcmp(a->name, b->name) && ft_strcmp(a->x_y, b->x_y))))
					a->isxy = 0;
			b = b->next;
		}
		b = tmp;
		a = a->next;
	}
}

void	check_dupl_start(t_val *head)
{
	char	*tmp;
	t_val	*t;

	tmp = NULL;
	t = NULL;
	while (head->next && !head->isstart)
		head = head->next;
	t = head;
	while (head->next && head->next->iscom == 1)
		head = head->next;
	if (head->next)
	{
		tmp = ft_strdup(head->next->line);
		head = head->next;
	}
	while (head && head->next)
	{
		if (head->isstart && ft_strcmp(head->next->line, tmp))
			head->isstart = 0;
		head = head->next;
	}
	ft_memdel((void**)(&tmp));
}

void	check_dupl_end(t_val *head)
{
	char	*tmp;
	t_val	*t;

	tmp = NULL;
	t = NULL;
	while (head->next && !head->isend)
		head = head->next;
	t = head;
	while (head->next && head->next->iscom == 1)
		head = head->next;
	if (head->next)
	{
		tmp = ft_strdup(head->next->line);
		head = head->next;
	}
	while (head && head->next)
	{
		if (head->isend && ft_strcmp(head->next->line, tmp))
			head->isend = 0;
		head = head->next;
	}
	ft_memdel((void**)(&tmp));
}

void	check_dupl_rooms(t_val **head)
{
	t_val	*map;
	int		start;
	int		end;

	start = 0;
	end = 0;
	map = *head;
	while (map)
	{
		if (map->isedge)
			break ;
		if (map->isstart && start == 0)
			start = 1;
		else if (map->isstart && start == 1)
			map->next->isdup = 1;
		if (map->isend && end == 0)
			end = 1;
		else if (map->isend && end == 1)
			map->next->isdup = 1;
		map = map->next;
	}
}
