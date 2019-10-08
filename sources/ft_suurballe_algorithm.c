/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_suurballe_algorithm.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/20 15:42:44 by ssoraka           #+#    #+#             */
/*   Updated: 2019/08/20 15:42:45 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_lem_in.h"

/*
**	Алгоритм Суурбалле: отключение маршрута, добавление фиктивных комнат
*/

void	ft_restruct_point(t_all *all, t_room *first, t_room *second)
{
	char second_in[MAX_NAME];

	ft_memcpy((void *)second_in, second->name, ft_strlen(second->name) + 1);
	ft_strcat(second_in, "_in ");
	ft_add_room(all, second_in);
	all->rooms->origin = second;
	ft_change_in_tubes(all, second, second_in);
	ft_add_tube(all, second_in, first->name, INVERS);
	ft_add_tube(all, second->name, second_in, FICTIOUS_NULL);
}

/*
**	Алгоритм Суурбалле: переносит выходящие связи из существующей комнаты
**	на фиктивную новую
*/

void	ft_change_out_tubes(t_all *all, t_room *old, char *new)
{
	t_tube *tube;

	tube = all->tubes;
	while (tube)
	{
		if (tube->room1 == all->start || tube->room2 == all->start ||
			tube->room1 == all->end || tube->room2 == all->end)
		{
			tube = tube->next;
			continue ;
		}
		if (old == tube->room1 && tube->active == ACTIVE)
		{
			tube->active = DEACTIVE;
			ft_add_tube(all, new, tube->room2->name, FICTIOUS);
		}
		tube = tube->next;
	}
}

/*
**	Алгоритм Суурбалле: переносит входящие связи с существующей комнаты
**	на фиктивную новую
*/

void	ft_change_in_tubes(t_all *all, t_room *old, char *new)
{
	t_tube *tube;

	tube = all->tubes;
	while (tube)
	{
		if (old == tube->room2 && tube->active == ACTIVE)
		{
			tube->active = DEACTIVE;
			ft_add_tube(all, tube->room1->name, new, FICTIOUS);
		}
		tube = tube->next;
	}
}

/*
**	отключение связи
*/

void	ft_deactive_tube(t_all *all, t_room *room1, t_room *room2)
{
	t_tube *tube;

	tube = all->tubes;
	while (tube)
	{
		if (room1 == tube->room1 && room2 == tube->room2)
		{
			tube->active = DEACTIVE;
			return ;
		}
		tube = tube->next;
	}
}

int		ft_cost(int active)
{
	if (active == ACTIVE)
		return (2);
	if (active == INVERS)
		return (-1);
	if (active == FICTIOUS)
		return (2);
	if (active == FICTIOUS_NULL)
		return (1);
	else
		return (0);
}
