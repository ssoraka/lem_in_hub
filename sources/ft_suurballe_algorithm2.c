/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_suurballe_algorithm2.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/20 15:43:02 by ssoraka           #+#    #+#             */
/*   Updated: 2019/08/20 15:43:03 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_lem_in.h"

/*
**	Алгоритм Суурбалле:
**	удаление общих участвков у всех маршрутов одного потока
*/

void	ft_merge_paths(t_path *path)
{
	t_path	*check_path;
	t_path	*first_path;
	t_path	*room;
	int		flow;

	flow = path->flow_count;
	first_path = path->next;
	if (flow == 1)
		return ;
	while (path->way->way)
		path = path->way;
	while (path->prev_way)
	{
		check_path = first_path;
		while (check_path && flow == check_path->flow_count)
		{
			if ((room = ft_find_similar_room(check_path, path)))
				ft_swap_paths(room, &path);
			check_path = check_path->next;
		}
		path = path->prev_way;
	}
}

/*
**	Алгоритм Суурбалле: поиск одинаковых комнат в двух маршрутах для
*/

t_path	*ft_find_similar_room(t_path *path, t_path *find)
{
	if (!path || !find)
		return (NULL);
	while (path)
	{
		if (path->rooms == find->rooms)
			return (path);
		path = path->way;
	}
	return (NULL);
}

/*
**	Алгоритм Суурбалле: перестановка фрагментов маршрутов
*/

void	ft_swap_paths(t_path *path1, t_path **path)
{
	t_path *temp_path;
	t_path *del_path;
	t_path *path2;

	path2 = *path;
	if (!path1 || !path2)
		return ;
	temp_path = path1->way;
	path1->way = path2->way;
	path1->way->prev_way = path1;
	path2->way = temp_path;
	temp_path->prev_way = path2;
	while (path2->prev_way->rooms == temp_path->rooms)
	{
		temp_path = temp_path->way;
		path2 = path2->prev_way;
	}
	del_path = path2->way;
	temp_path->prev_way->way = NULL;
	path2->way = temp_path;
	temp_path->prev_way = path2;
	ft_del_path(&del_path);
	*path = path2;
}

/*
**	Алгоритм Суурбалле: удаление фиктивных комнат и связей
*/

void	ft_del_fictious_tubes_and_rooms(t_all *all)
{
	t_tube *tube;
	t_room *room;

	while (all->tubes->active != ACTIVE && all->tubes->active != DEACTIVE)
	{
		tube = all->tubes;
		all->tubes = tube->next;
		free(tube);
	}
	tube = all->tubes;
	while (tube)
	{
		tube->active = ACTIVE;
		tube = tube->next;
	}
	while (all->rooms->origin)
	{
		all->count--;
		room = all->rooms;
		all->rooms = all->rooms->next;
		free(room->name);
		free(room);
	}
}
