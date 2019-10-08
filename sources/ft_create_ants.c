/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_create_ants.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/20 15:41:54 by ssoraka           #+#    #+#             */
/*   Updated: 2019/08/20 15:41:54 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_lem_in.h"

/*
**	эта функция определяет по каким маршрутам сколько пойдет муравьев
**	и возвращает количество строк ответа
*/

int		ft_count_of_ant_for_path_by_flow(t_all *all, int flow)
{
	t_path	*path;
	int		ant_count;
	int		lines_in_answer;

	ft_bzero((void *)all->ant_count_on_path, 4 * MAX_PATH);
	path = all->paths;
	while (path && path->flow_count != flow)
		path = path->next;
	ant_count = all->ant;
	lines_in_answer = 0;
	while (ant_count > 0)
	{
		lines_in_answer++;
		ant_count -= ft_ants_in_this_line(path, all->ant_count_on_path,
		lines_in_answer);
	}
	return (lines_in_answer);
}

t_ant	*ft_create_ant(int name)
{
	t_ant *tmp;

	tmp = (t_ant *)ft_memalloc(sizeof(t_ant));
	if (!tmp)
		return (NULL);
	tmp->name = name;
	return (tmp);
}

/*
**	создаем муравейник...
*/

void	ft_create_antwood(t_all *all)
{
	t_ant	*ants;
	int		count;

	count = all->ant;
	all->ant_count = count;
	ants = NULL;
	while (count > 0)
	{
		ants = ft_create_ant(count);
		if (!ants)
			ft_error(all, 1);
		ants->next = all->ants;
		all->ants = ants;
		count--;
	}
}
