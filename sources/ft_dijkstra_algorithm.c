/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dijkstra_algorithm.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/20 15:43:14 by ssoraka           #+#    #+#             */
/*   Updated: 2019/08/20 15:43:14 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_lem_in.h"

/*
**	Алгоритм Дейкстры для поиска кратчайшего маршрута от начала в конец
*/

int		ft_dijkstra(t_all *all)
{
	int		distance[MAX_ROOMS];
	char	visited[MAX_ROOMS];
	int		count;
	int		near_notvisited_room;

	ft_clear_shorts_path(all);
	ft_bzero((void *)visited, all->count);
	ft_memset((void *)distance, 7, 4 * all->count);
	distance[all->start->num] = 0;
	count = 0;
	while (!all->end->prev)
	{
		near_notvisited_room = ft_find_index_of_not_visited_room_with_min_dist(
		all, distance, visited);
		if (near_notvisited_room == NO_CONNECT)
			break ;
		visited[near_notvisited_room] = TRUE;
		ft_choose_min_dist_from_active_visited_room_to_not_visited(all,
		distance, visited, near_notvisited_room);
		count++;
	}
	if (all->end->prev)
		return (SUCCESS);
	return (FAIL);
}

/*
**		для Дейкстры
*/

int		ft_find_index_of_not_visited_room_with_min_dist(t_all *all,
		int *distance, char *visited)
{
	int min_dist;
	int index;
	int i;

	i = 0;
	min_dist = INF;
	index = INF;
	while (i < all->count)
	{
		if (!visited[i] && distance[i] <= min_dist)
		{
			min_dist = distance[i];
			index = i;
		}
		i++;
	}
	if (index == INF)
		return (NO_CONNECT);
	return (index);
}

/*
**	для Дейкстры
*/

void	ft_choose_min_dist_from_active_visited_room_to_not_visited(
		t_all *all, int *distance, char *visited, int u)
{
	t_tube	*tube;
	int		i;

	tube = all->tubes;
	while (tube)
	{
		if (tube->room1->num == u && tube->active)
		{
			i = tube->room2->num;
			if (!visited[i] && distance[u] != INF &&
			distance[u] + tube->cost < distance[i])
			{
				distance[i] = distance[u] + tube->cost;
				tube->room2->prev = tube->room1;
			}
		}
		tube = tube->next;
	}
}

/*
**	стирает запись кратчайшего маршрута, чтоб было понятно, есть другой или нет
*/

void	ft_clear_shorts_path(t_all *all)
{
	t_room *room;

	room = all->rooms;
	while (room)
	{
		room->prev = NULL;
		room = room->next;
	}
}
