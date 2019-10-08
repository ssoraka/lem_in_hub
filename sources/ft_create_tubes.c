/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_create_tubes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/20 15:42:16 by ssoraka           #+#    #+#             */
/*   Updated: 2019/08/20 15:42:16 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_lem_in.h"

void	ft_create_all_tubes(t_all *all)
{
	t_val *list;

	list = all->head;
	while (list && !(list->isedge))
		list = list->next;
	while (list)
	{
		if (list->isedge)
		{
			ft_add_tube(all, list->name1, list->name2, ACTIVE);
			ft_add_tube(all, list->name2, list->name1, ACTIVE);
		}
		list = list->next;
	}
}

int		ft_add_tube(t_all *all, char *name1, char *name2, int active)
{
	t_room *room1;
	t_room *room2;
	t_tube *new_tube;

	if (!all)
		return (FAIL);
	room1 = ft_find_room_by_name(all->rooms, name1);
	room2 = ft_find_room_by_name(all->rooms, name2);
	if (!room1 || !room2)
		return (FAIL);
	new_tube = ft_create_tube(room1, room2, active);
	if (!new_tube)
		ft_error(all, 1);
	new_tube->next = all->tubes;
	all->tubes = new_tube;
	return (SUCCESS);
}

t_tube	*ft_create_tube(t_room *room1, t_room *room2, int active)
{
	t_tube *tmp;

	if (!room1 || !room2)
		return (NULL);
	tmp = (t_tube *)ft_memalloc(sizeof(t_tube));
	if (!tmp)
		return (NULL);
	tmp->active = active;
	tmp->cost = ft_cost(active);
	tmp->room1 = room1;
	tmp->room2 = room2;
	return (tmp);
}
