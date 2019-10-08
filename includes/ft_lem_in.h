/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lem_in.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljalikak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/19 14:14:21 by ljalikak          #+#    #+#             */
/*   Updated: 2019/06/19 14:14:22 by ljalikak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LEM_IN_H
# define FT_LEM_IN_H
# include <string.h>
# include <stdlib.h>
# include <stdio.h>
# include "libft.h"
# include "ft_lem_in_struct.h"
# include "ft_list.h"

# define SUCCESS 0
# define FAIL 1
# define ERROR -1

# define ERROR_MSG "Error\n"
# define ERROR_MSG2 "Error\n"

# define INF 117901063
# define NO_CONNECT -1
# define MAX_PATH 100
# define MAX_ROOMS 20000
# define MAX_NAME 300

/*
**	cost of tube
*/

# define INVERS -1
# define DEACTIVE 0
# define ACTIVE 2
# define FICTIOUS 1
# define FICTIOUS_NULL 3

/*
**	rooms
*/

# define EMPTY 0
# define FULL 1

/*
**	ft_main_all_error_flow.c
*/

t_all	*ft_create_all(t_val *head);
void	ft_error(t_all *all, int msg);
int		ft_choose_min_flow(t_all *all);

/*
**	ft_del_functions.c
*/

void	ft_del_ants(t_ant **ants);
void	ft_del_path(t_path **paths);
void	ft_del_tubes(t_tube **tubes);
void	ft_del_room(t_room **rooms);
void	ft_del_all(t_all *all);

/*
**	ft_create_ants.c
*/

void	ft_create_antwood(t_all *all);
t_ant	*ft_create_ant(int name);
int		ft_count_of_ant_for_path_by_flow(t_all *all, int flow);

/*
**	ft_create_rooms.c
*/

void	ft_create_all_rooms(t_all *all);
int		ft_add_room(t_all *all, char *name);
t_room	*ft_create_room(char *name);
t_room	*ft_find_room_by_name(t_room *rooms, char *name);
t_room	*ft_find_room_by_number(t_room *rooms, int num);

/*
**	ft_create_tubes.c
*/

void	ft_create_all_tubes(t_all *all);
int		ft_add_tube(t_all *all, char *name1, char *name2, int active);
t_tube	*ft_create_tube(t_room *room1, t_room *room2, int active);

/*
**	ft_create_paths.c
*/

t_path	*ft_create_path(t_room *room, int flow_count);
t_path	*ft_create_step_of_path(t_room *room, int flow_count);
void	ft_recount_length_of_all_paths(t_path *path);

/*
**	ft_suurballe_algorithm.c
*/

void	ft_restruct_point(t_all *all, t_room *first, t_room *second);
void	ft_change_out_tubes(t_all *all, t_room *old, char *new_name);
void	ft_change_in_tubes(t_all *all, t_room *old, char *new_name);
void	ft_deactive_tube(t_all *all, t_room *room1, t_room *room2);
int		ft_cost(int active);

/*
**	ft_suurballe_algorithm2.c
*/

void	ft_merge_paths(t_path *path);
t_path	*ft_find_similar_room(t_path *path, t_path *find);
void	ft_swap_paths(t_path *path1, t_path **path);
void	ft_del_fictious_tubes_and_rooms(t_all *all);

/*
**	ft_dijkstra_algorithm.c
*/

int		ft_dijkstra(t_all *all);
void	ft_clear_shorts_path(t_all *all);
int		ft_find_index_of_not_visited_room_with_min_dist(t_all *all,
		int *distance, char *visited);
void	ft_choose_min_dist_from_active_visited_room_to_not_visited(
		t_all *all, int *distance, char *visited, int u);

/*
**	ft_move_ants.c
*/

int		ft_ants_in_this_line(t_path *path,
		int *ants_on_path, int lines_in_answer);
void	ft_choose_path_for_all_ants(t_all *all);
void	ft_ants_motion(t_all *all);
int		ft_is_ant_step(t_all *all, t_ant *ant);
void	ft_print_ants(int name, char *room_name);

/*
**	ft_find_new_paths.c
*/

int		ft_find_another_path(t_all *all);
int		ft_copy_last_paths_with_same_flows(t_all *all);
void	ft_restore_path_at_rooms(t_path *paths);
int		ft_add_path(t_all *all, int flow_count);
void	ft_deactive_new_paths(t_all *all);

#endif
