/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_del_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/20 14:13:45 by ssoraka           #+#    #+#             */
/*   Updated: 2019/08/20 14:13:45 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_lem_in.h"

void	ft_del_all(t_all *all)
{
	if (all)
	{
		ft_del_ants(&(all->ants));
		ft_del_path(&(all->paths));
		ft_del_room(&(all->rooms));
		ft_del_tubes(&(all->tubes));
		del_roll_valid(&(all->head));
	}
	free(all);
}

void	ft_del_ants(t_ant **ants)
{
	t_ant *tmp;
	t_ant *ant;

	ant = *ants;
	while (ant)
	{
		tmp = ant;
		ant = ant->next;
		free(tmp);
	}
	*ants = NULL;
}

void	ft_del_path(t_path **paths)
{
	t_path *path;
	t_path *check_step;
	t_path *check_path;

	path = *paths;
	while (path)
	{
		check_path = path;
		path = path->next;
		while (check_path)
		{
			check_step = check_path;
			check_path = check_path->way;
			free(check_step);
		}
	}
	*paths = NULL;
}

void	ft_del_tubes(t_tube **tubes)
{
	t_tube *tmp;
	t_tube *tube;

	tube = *tubes;
	while (tube)
	{
		tmp = tube;
		tube = tube->next;
		free(tmp);
	}
	*tubes = NULL;
}

void	ft_del_room(t_room **rooms)
{
	t_room *tmp;
	t_room *room;

	room = *rooms;
	while (room)
	{
		tmp = room;
		room = room->next;
		free(tmp->name);
		free(tmp);
	}
	*rooms = NULL;
}
