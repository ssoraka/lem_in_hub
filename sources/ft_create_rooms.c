/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_create_rooms.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/20 15:42:04 by ssoraka           #+#    #+#             */
/*   Updated: 2019/08/20 15:42:04 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_lem_in.h"

void	ft_create_all_rooms(t_all *all)
{
	t_val *list;
	t_val *list_prev;

	list = all->head;
	list_prev = NULL;
	while (list && !(list->isedge))
	{
		if (list->iscom == 0 && list->isdup == 0 && list_prev)
		{
			if (!(list->isend) && !(list->isstart))
				ft_add_room(all, list->name);
			if (list_prev->isend || (all->end &&
			!ft_strcmp(all->end->name, all->rooms->name)))
				all->end = all->rooms;
			if (list_prev->isstart || (all->start &&
			!ft_strcmp(all->start->name, all->rooms->name)))
				all->start = all->rooms;
		}
		if (list->iscom == 0)
			list_prev = list;
		list = list->next;
	}
}

int		ft_add_room(t_all *all, char *name)
{
	t_room *new_room;
	t_room *rooms;

	rooms = all->rooms;
	new_room = ft_create_room(name);
	if (!new_room)
		ft_error(all, 1);
	if (all->rooms)
	{
		new_room->num = all->rooms->num + 1;
		all->rooms->prev = new_room;
	}
	new_room->next = all->rooms;
	all->rooms = new_room;
	all->count = new_room->num + 1;
	return (SUCCESS);
}

t_room	*ft_create_room(char *name)
{
	t_room	*tmp;

	if (!name)
		return (NULL);
	tmp = (t_room *)ft_memalloc(sizeof(t_room));
	if (!tmp)
		return (NULL);
	tmp->name = ft_strdup(name);
	if (!(tmp->name))
		ft_memdel((void *)tmp);
	return (tmp);
}

t_room	*ft_find_room_by_name(t_room *rooms, char *name)
{
	while (rooms)
	{
		if (ft_strcmp(name, rooms->name) == 0)
			return (rooms);
		rooms = rooms->next;
	}
	return (NULL);
}

t_room	*ft_find_room_by_number(t_room *rooms, int num)
{
	while (rooms)
	{
		if (rooms->num == num)
			return (rooms);
		rooms = rooms->next;
	}
	return (NULL);
}
