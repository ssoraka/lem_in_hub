/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_create_paths.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/20 15:42:29 by ssoraka           #+#    #+#             */
/*   Updated: 2019/08/20 15:42:29 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_lem_in.h"

/*
**	создает маршрут
*/

t_path	*ft_create_path(t_room *room, int flow_count)
{
	t_path	*begin;
	t_path	*tmp;
	int		length;

	length = 0;
	begin = NULL;
	while (room)
	{
		if (!(room->origin && room->origin == room->prev))
		{
			tmp = ft_create_step_of_path(room, flow_count);
			if (!tmp)
			{
				ft_del_path(&begin);
				return (NULL);
			}
			tmp->way = begin;
			tmp->length = length++;
			if (begin)
				begin->prev_way = tmp;
			begin = tmp;
		}
		room = room->prev;
	}
	return (begin);
}

/*
**	создает один шаг пути
*/

t_path	*ft_create_step_of_path(t_room *room, int flow_count)
{
	t_path *tmp;

	if (!room)
		return (NULL);
	if (room->origin)
		room = room->origin;
	tmp = (t_path *)ft_memalloc(sizeof(t_path));
	if (!tmp)
		return (NULL);
	tmp->rooms = room;
	tmp->flow_count = flow_count;
	return (tmp);
}

/*
**	Алгоритм Суурбалле: пересчет длин маршрутов после перестановки
*/

void	ft_recount_length_of_all_paths(t_path *path)
{
	t_path	*check_path;
	int		flow;
	int		count;

	flow = path->flow_count;
	while (path && flow == path->flow_count)
	{
		count = 0;
		check_path = path;
		while (check_path->way)
			check_path = check_path->way;
		while (check_path)
		{
			check_path->length = count;
			count++;
			check_path = check_path->prev_way;
		}
		path = path->next;
	}
}
