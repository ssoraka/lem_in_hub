/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_move_ants.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/20 15:43:24 by ssoraka           #+#    #+#             */
/*   Updated: 2019/08/20 15:43:24 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_lem_in.h"

int		ft_ants_in_this_line(t_path *path,
		int *ants_on_path, int lines_in_answer)
{
	int i;
	int flow;
	int ants;

	i = 0;
	ants = 0;
	flow = path->flow_count;
	while (path && path->flow_count == flow)
	{
		if (ants_on_path[i] + path->length < lines_in_answer)
		{
			(ants_on_path[i])++;
			ants++;
		}
		i++;
		path = path->next;
	}
	return (ants);
}

/*
**	назначаем все маршруты каждому муравью
*/

void	ft_choose_path_for_all_ants(t_all *all)
{
	int		i;
	t_ant	*ant;
	t_path	*path;
	t_path	*check_path;

	ant = all->ants;
	path = all->paths;
	while (path && path->flow_count != all->flows)
		path = path->next;
	while (ant)
	{
		i = 0;
		check_path = path;
		while (ant && i < all->flows)
		{
			if (all->ant_count_on_path[i] > 0)
			{
				(all->ant_count_on_path[i])--;
				ant->step = check_path;
				ant = ant->next;
			}
			i++;
			check_path = check_path->next;
		}
	}
}

/*
** приводит муравьев в движение по их маршрутам
*/

void	ft_ants_motion(t_all *all)
{
	t_ant	*ant;
	int		first_ant;

	all->flows = ft_choose_min_flow(all);
	ft_count_of_ant_for_path_by_flow(all, all->flows);
	ft_choose_path_for_all_ants(all);
	ft_putchar('\n');
	while (all->end->empty_full < all->ant_count)
	{
		ant = all->ants;
		first_ant = TRUE;
		while (ant)
		{
			if (ft_is_ant_step(all, ant))
			{
				if (first_ant)
					first_ant = FALSE;
				else
					ft_putchar(' ');
				ft_print_ants(ant->name, ant->step->rooms->name);
			}
			ant = ant->next;
		}
		ft_putchar('\n');
	}
}

/*
**	функция перемещает одного муравья по его маршруту
*/

int		ft_is_ant_step(t_all *all, t_ant *ant)
{
	if (ant->step->rooms == all->end)
		return (FALSE);
	if (ant->step->way && (ant->step->way->rooms->empty_full == EMPTY ||
	ant->step->way->rooms == all->end))
	{
		ant->step->rooms->empty_full -= FULL;
		ant->step = ant->step->way;
		ant->step->rooms->empty_full += FULL;
		return (TRUE);
	}
	return (FALSE);
}

void	ft_print_ants(int name, char *room_name)
{
	ft_putchar('L');
	ft_putnbr(name);
	ft_putchar('-');
	ft_putstr(room_name);
}
