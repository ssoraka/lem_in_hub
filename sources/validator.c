/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljalikak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/20 14:00:11 by ljalikak          #+#    #+#             */
/*   Updated: 2019/08/20 14:00:13 by ljalikak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_list.h"

t_val	*read_and_print_file(void)
{
	t_val	*head;
	char	*arr;

	head = NULL;
	arr = NULL;
	while (get_next_line(0, &arr))
	{
		if (!head)
			head = create_list_valid(arr);
		else
			head = creator_valid(head, arr);
		ft_memdel((void**)(&arr));
	}
	index_comments_and_num(head);
	ft_memdel((void**)(&arr));
	lemin(&head);
	return (head);
}

void	lemin(t_val **head)
{
	if (!(*head))
		return ;
	check_xy(*head);
	split_name_xy(*head);
	check_dupl_xy(*head, *head);
	check_commands(*head);
	check_dupl_start(*head);
	check_dupl_end(*head);
	*head = split_names(head);
	check_new_names(*head, *head);
	if (checker(*head))
	{
		printer_to_error_or_finish(*head);
		check_dupl_rooms(head);
	}
	else
		del_roll_valid(head);
}

int		checker(t_val *head)
{
	t_val	*t;
	int		start;
	int		end;

	start = 0;
	end = 0;
	t = head;
	if (!head)
		return (FALSE);
	while (t)
	{
		if (t->iscom + t->isnum + t->isstart
		+ t->isend + t->isxy + t->isedge != 1)
			return (FALSE);
		if (t->isstart == 1)
			start++;
		if (t->isend == 1)
			end++;
		t = t->next;
	}
	if (start == 0 || end == 0)
		return (FALSE);
	return (TRUE);
}

void	check_commands(t_val *h)
{
	t_val *t;

	t = NULL;
	if (!h)
		return ;
	while (h->next)
	{
		if (!ft_strcmp(h->line, "##start"))
		{
			t = h;
			while (h->next && h->next->iscom == 1)
				h = h->next;
			t->isstart = 1;
		}
		if (!ft_strcmp(h->line, "##end"))
		{
			t = h;
			while (h->next && h->next->iscom == 1)
				h = h->next;
			t->isend = 1;
		}
		h = h->next;
	}
}

void	check_new_names(t_val *a, t_val *b)
{
	t_val	*tmp;
	int		flag1;
	int		flag2;

	tmp = b;
	if (!a || !b)
		return ;
	while (a)
	{
		flag1 = 0;
		flag2 = 0;
		while (b)
		{
			if (a->isedge && b->isxy && !ft_strcmp(b->name, a->name1))
				flag1 = 1;
			if (a->isedge && b->isxy && !ft_strcmp(b->name, a->name2))
				flag2 = 1;
			b = b->next;
		}
		if (flag1 == 0 || flag2 == 0)
			a->isedge = 0;
		b = tmp;
		a = a->next;
	}
}
