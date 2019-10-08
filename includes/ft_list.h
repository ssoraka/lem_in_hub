/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_list.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljalikak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/19 14:14:21 by ljalikak          #+#    #+#             */
/*   Updated: 2019/06/19 14:14:22 by ljalikak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LIST_H
# define FT_LIST_H
# include <string.h>
# include <stdlib.h>
# include <stdio.h>
# include "ft_lem_in_struct.h"
# include "libft.h"

/*
**	main.c
*/

t_val	*read_and_print_file(void);
void	lemin(t_val **head);
int		checker(t_val *head);
void	check_commands(t_val *head);
void	check_new_names(t_val *a, t_val *b);

/*
**	list_val.c
*/

t_val	*create_list_valid(char *s);
t_val	*creator_valid(t_val *head, char *s);
void	printer_valid(t_val *head);
void	del_list_valid(t_val **del);
void	del_roll_valid(t_val **head);

/*
**	check_comment_and_realnum.c
*/

int		not_comment(char *s);
int		check_realnum(char *s);
int		check_realnum_add(char *s, int n, int len);
void	index_comments_and_num(t_val *h);
void	printer_to_error_or_finish(t_val *h);

/*
**	check_xy_and_edge.c
*/

void	check_xy(t_val *head);
void	check_edge(t_val *head);
int		num_w(char **av);
int		num_c(char *s, int ch);

/*
**	splits.c
*/

void	split_name_xy(t_val *head);
t_val	*split_names(t_val **head);
void	del_split(char **s);

/*
**	duplicate.c
*/

void	check_dupl_xy(t_val *a, t_val *b);
void	check_dupl_start(t_val *head);
void	check_dupl_end(t_val *head);
void	check_dupl_rooms(t_val **head);
#endif
