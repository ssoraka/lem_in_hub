/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lem_in_struct.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljalikak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/19 14:14:21 by ljalikak          #+#    #+#             */
/*   Updated: 2019/06/19 14:14:22 by ljalikak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LEM_IN_STRUCT_H
# define FT_LEM_IN_STRUCT_H
# define MAX_PATH 100

typedef struct		s_val
{
	struct s_val	*next;
	char			*line;
	char			*name;
	char			*x_y;
	char			*name1;
	char			*name2;
	int				isnum;
	int				isstart;
	int				isend;
	int				isdup;
	int				isxy;
	int				isedge;
	int				iscom;
}					t_val;

typedef struct		s_room
{
	int				x;
	int				y;
	int				num;
	int				empty_full;
	char			*name;
	struct s_room	*origin;
	struct s_room	*prev;
	struct s_room	*next;
}					t_room;

typedef struct		s_tube
{
	int				cost;
	int				active;
	struct s_room	*room1;
	struct s_room	*room2;
	struct s_tube	*next;
}					t_tube;

typedef struct		s_path
{
	int				flow_count;
	int				length;
	struct s_room	*rooms;
	struct s_path	*way;
	struct s_path	*prev_way;
	struct s_path	*next;
}					t_path;

typedef struct		s_ant
{
	int				name;
	int				motion;
	struct s_path	*step;
	struct s_ant	*next;
}					t_ant;

typedef struct		s_all
{
	int				ant;
	int				ant_count;
	int				count;
	int				path_count;
	int				flows;
	int				ant_count_on_path[MAX_PATH];
	struct s_list	*list;
	struct s_val	*head;
	struct s_ant	*ants;
	struct s_room	*rooms;
	struct s_room	*start;
	struct s_room	*end;
	struct s_tube	*tubes;
	struct s_path	*paths;
}					t_all;

#endif
