/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_find_new_paths.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/20 15:43:39 by ssoraka           #+#    #+#             */
/*   Updated: 2019/08/20 15:43:40 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_lem_in.h"

/*
**	в случае нахождения еще одного маршрута, дублирует старые,
**	увеличивает максимальный поток и перекраивает их с новым
*/

int		ft_find_another_path(t_all *all)
{
	int answer;
	int flow;

	answer = FAIL;
	flow = 0;
	ft_deactive_new_paths(all);
	ft_copy_last_paths_with_same_flows(all);
	if (ft_dijkstra(all) == SUCCESS)
	{
		(all->flows)++;
		ft_add_path(all, all->flows);
		ft_merge_paths(all->paths);
		ft_recount_length_of_all_paths(all->paths);
		if (all->paths->way->rooms != all->end)
			answer = SUCCESS;
	}
	ft_del_fictious_tubes_and_rooms(all);
	return (answer);
}

/*
**	Алгоритм Суурбалле: дублирование маршрутов при увеличении потока
*/

int		ft_copy_last_paths_with_same_flows(t_all *all)
{
	t_path	*check_path;
	int		flow;

	check_path = all->paths;
	if (!check_path)
		return (FAIL);
	flow = check_path->flow_count;
	while (check_path && check_path->flow_count == flow)
	{
		ft_clear_shorts_path(all);
		ft_restore_path_at_rooms(check_path);
		ft_add_path(all, flow + 1);
		check_path = check_path->next;
	}
	return (SUCCESS);
}

/*
**	проецирует выбранный маршрут на существующие комнаты для его копирования
**	возможно это тупое решение...
*/

void	ft_restore_path_at_rooms(t_path *paths)
{
	t_room *room;

	room = paths->rooms;
	room->prev = NULL;
	while (paths->way)
	{
		paths = paths->way;
		paths->rooms->prev = room;
		room = paths->rooms;
	}
}

/*
**	создает маршрут
*/

int		ft_add_path(t_all *all, int flow_count)
{
	t_path *path;

	if (!(all->end) || !(all->end->prev))
		return (FAIL);
	if (!(path = ft_create_path(all->end, flow_count)))
		ft_error(all, 1);
	path->next = all->paths;
	all->paths = path;
	return (SUCCESS);
}

/*
**	Алгоритм Суурбалле: отключение открытых маршрутов по их направлению
*/

void	ft_deactive_new_paths(t_all *all)
{
	int		flow;
	t_path	*check_path;
	t_path	*stage;

	check_path = all->paths;
	if (!check_path)
		return ;
	flow = check_path->flow_count;
	while (check_path && check_path->flow_count == flow)
	{
		stage = check_path;
		while (stage->way)
		{
			ft_deactive_tube(all, stage->way->rooms, stage->rooms);
			ft_deactive_tube(all, stage->rooms, stage->way->rooms);
			stage = stage->way;
		}
		stage = check_path;
		while (stage->way->way)
		{
			ft_restruct_point(all, stage->rooms, stage->way->rooms);
			stage = stage->way;
		}
		check_path = check_path->next;
	}
}
