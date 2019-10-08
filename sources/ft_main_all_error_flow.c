/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_main_all_error_flow.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/27 11:13:07 by ssoraka           #+#    #+#             */
/*   Updated: 2019/05/27 11:13:07 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "locale.h"
#include "ft_lem_in.h"
#include "ft_list.h"

/*
**	объект, который все в себе хранит
*/

t_all	*ft_create_all(t_val *head)
{
	t_all *tmp;

	tmp = (t_all *)ft_memalloc(sizeof(t_all));
	if (!tmp)
	{
		del_roll_valid(&head);
		ft_error(NULL, 1);
	}
	tmp->head = head;
	while (head)
	{
		if (head->isnum)
		{
			tmp->ant = ft_atoi(head->line);
			break ;
		}
		head = head->next;
	}
	tmp->ant_count = tmp->ant;
	return (tmp);
}

void	ft_error(t_all *all, int msg)
{
	ft_del_all(all);
	if (msg == 1)
		ft_putstr(ERROR_MSG);
	if (msg == 2)
		ft_putstr(ERROR_MSG2);
	exit(0);
}

/*
**	определяет поток с наименьшим еоличеством шагов для перемещения
*/

int		ft_choose_min_flow(t_all *all)
{
	int best_flow;
	int flow;
	int steps_count;
	int best_count;

	flow = all->paths->flow_count;
	best_count = INF;
	if (all->ant < flow)
		flow = all->ant;
	while (flow)
	{
		steps_count = ft_count_of_ant_for_path_by_flow(all, flow);
		if (steps_count < best_count)
		{
			best_count = steps_count;
			best_flow = flow;
		}
		flow--;
	}
	return (best_flow);
}

int		main(void)
{
	t_all	*all;
	t_val	*head;
	int		flow;
	int		flow2;

	flow = INF;
	head = read_and_print_file();
	if (!head)
		ft_error(NULL, 2);
	all = ft_create_all(head);
	ft_create_all_rooms(all);
	ft_create_all_tubes(all);
	ft_create_antwood(all);
	while (ft_find_another_path(all) == SUCCESS &&
	all->ant > all->flows &&
	(flow2 = ft_count_of_ant_for_path_by_flow(all, all->flows)) < flow + 1)
		flow = flow2;
	if (all->paths)
		ft_ants_motion(all);
	else
		ft_error(all, 2);
	ft_del_all(all);
	return (0);
}
