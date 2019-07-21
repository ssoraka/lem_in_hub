/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/27 11:13:07 by ssoraka           #+#    #+#             */
/*   Updated: 2019/05/27 11:13:07 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "locale.h"


#define SUCCESS 0
#define FAIL 1
#define ERROR -1

#define ERROR_MSG "В процессе что-то пошло не так!\n"


#define INF 117901063


#define INVERS -1
#define DEACTIVE 0
#define ACTIVE 2
#define FICTIOUS 1
#define FICTIOUS_NULL 3

#define EMPTY 0
#define FULL 1

#define START 0
#define END 1
#define MOVE 2

#define NO_CONNECT -1

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
	int				s_num;
	int				e_num;
	int				flows;
	struct s_ant	*ants;
	struct s_room	*rooms;
	struct s_room	*start;
	struct s_room	*end;
	struct s_tube	*tubes;
	struct s_path	*paths;
}					t_all;




int ft_dijkstra(t_all *all);


int		ft_cost(int active)
{
	if (active == ACTIVE)
		return (2);
	if (active == INVERS)
		return (-1);
	if (active == FICTIOUS)
		return (2);
	if (active == FICTIOUS_NULL)
		return (1);
	else
		return (0);
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
	while(path)
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

void	ft_del_all(t_all *all)
{
	if (all)
	{
		ft_del_ants(&(all->ants));
		ft_del_path(&(all->paths));
		ft_del_room(&(all->rooms));
		ft_del_tubes(&(all->tubes));
	}
	free(all);
}


void ft_error(t_all *all, int msg)
{
	ft_del_all(all);
	if (msg == 1)
		ft_putstr(ERROR_MSG);
	exit(0);
}


t_ant	*ft_create_ant(int name)
{
	t_ant *tmp;

	tmp = (t_ant *)ft_memalloc(sizeof(t_ant));
	if (!tmp)
		return (NULL);
	tmp->name = name;
	tmp->motion = START;
	return (tmp);
}

t_room	*ft_create_room(char *name)
{
	t_room *tmp;

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

/*
**	отключение связи
*/

void	ft_deactive_tube(t_all *all, char *name1, char *name2)
{
	t_tube *tube;

	tube = all->tubes;
	while (tube)
	{
		if (ft_strcmp(name1, tube->room1->name) == 0 &&
			ft_strcmp(name2, tube->room2->name) == 0)
		{
			tube->active = DEACTIVE;
			return ;
		}
		tube = tube->next;
	}
}

/*
**	Алгоритм Суурбалле: переносит выходящие связи из существующей комнаты
**	на фиктивную новую
*/

void	ft_change_out_tubes(t_all *all, char *old, char *new)
{
	t_tube *tube;
	t_room *old_room;

	old_room = ft_find_room_by_name(all->rooms, old);
	tube = all->tubes;
	while (tube)
	{
		if (tube->room1 == all->start || tube->room2 == all->start ||
			tube->room1 == all->end || tube->room2 == all->end)
		{
			tube = tube->next;
			continue ;
		}
		if (old_room == tube->room1 && tube->active == ACTIVE)
		{
			tube->active = DEACTIVE;
			ft_add_tube(all, new, tube->room2->name, FICTIOUS);
		}
		tube = tube->next;
	}
	ft_add_tube(all, new, old, FICTIOUS_NULL);
}

/*
**	Алгоритм Суурбалле: переносит входящие связи с существующей комнаты
**	на фиктивную новую
*/

void	ft_change_in_tubes(t_all *all, char *old, char *new)
{
	t_tube *tube;
	t_room *old_room;

	old_room = ft_find_room_by_name(all->rooms, old);
	tube = all->tubes;
	while (tube)
	{
		if (tube->room1 == all->start || tube->room2 == all->start ||
			tube->room1 == all->end || tube->room2 == all->end)
		{
			tube = tube->next;
			continue ;
		}
		if (old_room == tube->room2 && tube->active == ACTIVE)
		{
			tube->active = DEACTIVE;
			ft_add_tube(all, tube->room1->name, new, FICTIOUS);
		}
		tube = tube->next;
	}
	ft_add_tube(all, old, new, FICTIOUS_NULL);
}

/*
**	Алгоритм Суурбалле: отключение маршрута, добавление фиктивных комнат
*/

void	ft_restruct_tube(t_all *all, char *first, char *second)
{

	ft_deactive_tube(all, first, second);
	ft_deactive_tube(all, second, first);

	char *first_out;
	char *second_in;

	//создаем имена для новых комнат
	first_out = ft_strjoin(first, "_out");
	second_in = ft_strjoin(second, "_in");

	//создаем новые комнаты
	ft_add_room(all, first_out);
	all->rooms->origin = ft_find_room_by_name(all->rooms, first);
	ft_add_room(all, second_in);
	all->rooms->origin = ft_find_room_by_name(all->rooms, second);
	//перекидываем связи с существующего узла на новый
	//ft_change_tube(all, first, first_out, OUT);
	//ft_change_tube(all, second, second_in, IN);
	ft_change_out_tubes(all, first, first_out);
	ft_change_in_tubes(all, second, second_in);

	//добавляем связь между новыми комнатами
	ft_add_tube(all, second_in, first_out, INVERS);

	free(second_in);
	free(first_out);
}

/**/



void ft_print_rooms(t_room *rooms)
{
	while (rooms)
	{
		printf("%s", rooms->name);
		if (rooms->origin)
			printf("->%s", rooms->origin->name);
		printf("\n");
		rooms = rooms->next;
	}
}

void ft_print_paths(t_path *paths)
{
	printf("__%d__\n", paths->flow_count);
	while (paths)
	{
		printf("%s", paths->rooms->name);
		if (paths->rooms->origin)
			printf("->%s", paths->rooms->origin->name);
		printf("__%d", paths->length);
		printf("\n");
		paths = paths->way;
	}
}

void ft_print_tubes(t_tube *tubes)
{
	while (tubes)
	{
		printf("%s-%s\t%d\t%d\n", tubes->room1->name, tubes->room2->name, tubes->cost, tubes->active);
		tubes = tubes->next;
	}
}

/*
**	объект, который все в себе хранит
*/

t_all	*ft_create_all(int ants_count)
{
	t_all *tmp;

	tmp = (t_all *)ft_memalloc(sizeof(t_all));
	if (!tmp)
		ft_error(NULL, 1);
	tmp->ant = ants_count;
	tmp->ant_count = ants_count;
	return (tmp);
}

/*
**	создает один шаг пути
*/

t_path	*ft_create_step_of_path(t_room *room, int flow_count)
{
	t_path *tmp;

	if (!room)
		return (NULL);
	tmp = (t_path *)ft_memalloc(sizeof(t_path));
	if (!tmp)
		return (NULL);
	tmp->rooms = room;
	tmp->flow_count = flow_count;
	return (tmp);
}

/*
**	создает маршрут
*/

t_path	*ft_create_path(t_room *room, int flow_count)
{
	t_path	*begin;
	t_path	*tmp;
	int		length;

	length = 0;
	begin = NULL;
	while (room)
	{
		if (room->origin)
			tmp = ft_create_step_of_path(room->origin, flow_count);
		else
			tmp = ft_create_step_of_path(room, flow_count);
		if (!tmp)
		{
			ft_del_path(&begin);
			return (NULL);
		}
		tmp->way = begin;
		tmp->length = length++;
		if (begin)
			begin->prev_way = tmp;
		begin = tmp;
		room = room->prev;
	}
	return (begin);
}

/*
**	создает маршрут
*/

int 	ft_add_path(t_all *all, int flow_count)
{
	t_path *path;

	if (!(all->end) || !(all->end->prev))
		return (FAIL);
	if (!(path = ft_create_path(all->end, flow_count)))
		ft_error(all, 1);
	path->next = all->paths;
	all->paths = path;
	return (SUCCESS);
}

/*
**	стирает запись кратчайшего маршрута, чтоб было понятно, есть другой или нет
*/

void	ft_clear_shorts_path(t_all *all)
{
	t_room *room;

	room = all->rooms;
	while (room)
	{
		room->prev = NULL;
		room = room->next;
	}
}

/*
t_path	*ft_copy_paths(t_path *path, int flow_count)
{
	t_room *origin;

	origin = path->room;
	while (origin->next)
		origin = origin->next;
	path_new = ft_create_path(origin, flow_count);

	return (path_new);
}
*/

/*
**	проецирует выбранный маршрут на существующие комнаты для его копирования
**	возможно это тупое решение...
*/

void	ft_restore_path_at_rooms(t_path *paths)
{
	t_room *room;

	room = paths->rooms;
	while (paths->way)
	{
		paths = paths->way;
		paths->rooms->prev = room;
		room = paths->rooms;
	}
}

/*
**	Алгоритм Суурбалле: дублирование маршрутов при увеличении потока
*/

int		ft_copy_last_paths_with_same_flows(t_all *all)
{
	t_path	*check_path;
	int		flow;

	check_path = all->paths;
	if (!check_path)
		return (FAIL);
	flow = check_path->flow_count;
	while (check_path && check_path->flow_count == flow)
	{
		ft_restore_path_at_rooms(check_path);
		ft_add_path(all, flow + 1);
		check_path = check_path->next;
	}
	return (SUCCESS);
}

/*
**	Алгоритм Суурбалле: отключение открытых маршрутов по их направлению
*/

void	ft_deactive_new_paths(t_all *all)
{
	int flow;
	t_path *check_path;
	t_path *room;

	check_path = all->paths;
	if (!check_path)
		return ;
	flow = check_path->flow_count;
	while (check_path && check_path->flow_count == flow)
	{
		room = check_path;
		//printf("__%s_%s__\n", all->start->name, all->end->name);
		while (room->way)
		{
			//printf(" _%s_%s_\n", room->name, room->next->name);
			/*if (ft_strcmp(room->rooms->name, all->start->name) &&
			ft_strcmp(room->rooms->name, all->end->name) &&
			ft_strcmp(room->way->rooms->name, all->start->name) &&
			ft_strcmp(room->way->rooms->name, all->end->name))*/
			if (room->rooms == all->start || room->rooms == all->end ||
			room->way->rooms == all->start || room->way->rooms == all->end)
				ft_deactive_tube(all, room->rooms->name, room->way->rooms->name);
			else
				ft_restruct_tube(all, room->rooms->name, room->way->rooms->name);

			room = room->way;
		}
		check_path = check_path->next;
	}
}


/*
**	Алгоритм Суурбалле: удаление фиктивных комнат и связей
*/

void	ft_del_fictious_tubes_and_rooms(t_all *all)
{
	t_tube *tube;
	t_room *room;

	while (all->tubes->active != ACTIVE && all->tubes->active != DEACTIVE)
	{
		tube = all->tubes;
		all->tubes = tube->next;
		free(tube);
	}
	tube = all->tubes;
	while (tube)
	{
		tube->active = ACTIVE;
		tube = tube->next;
	}
	while (all->rooms->origin)
	{
		all->count--;
		room = all->rooms;
		all->rooms = all->rooms->next;
		free(room->name);
		free(room);
	}
}

/*
**	Алгоритм Суурбалле: поиск одинаковых комнат в двух маршрутах для
*/

t_path	*ft_find_similar_room(t_path *path, t_path *find)
{
	if (!path || !find || !(find->way) || !(find->prev_way))
		return (NULL);
	while (path)
	{
		if (path->rooms == find->rooms)
			return (path);
		path = path->way;
	}
	return (NULL);
}

/*
**	Алгоритм Суурбалле: перестановка фрагментов маршрутов
*/

void	ft_swap_paths(t_path *path1, t_path *path2)
{
	t_path *temp_path;

	if (!path1 || !path2)
		return ;
	//ft_print_paths(path1);
	temp_path = path1->way;
	path1->way = path2->way->way;
	path1->way->prev_way = path1;
	free(path2->way);
	path2->way = temp_path->way;
	path2->way->prev_way = path2;
	free(temp_path);
}

/*
**	Алгоритм Суурбалле:
**	удаление общих участвков у всех маршрутов одного потока
*/

void	ft_merge_paths(t_path *path)
{
	t_path *check_path;
	t_path *first_path;
	t_path *room;
	int flow;

	flow = path->flow_count;
	first_path = path->next;
	if (flow == 1)
		return ;
	while (path->way)
		path = path->way;
	while (path)
	{
		check_path = first_path;
		while (check_path && flow == check_path->flow_count)
		{
			if ((room = ft_find_similar_room(check_path, path->way)))
				ft_swap_paths(room, path);
			check_path = check_path->next;
		}
		path = path->prev_way;
	}
}

/*
**	Алгоритм Суурбалле: пересчет длин маршрутов после перестановки
*/

void	ft_recount_length_of_all_paths(t_path *path)
{
	t_path *check_path;
	int flow;
	int count;

	flow = path->flow_count;
	while (path && flow == path->flow_count)
	{
		count = 0;
		check_path = path;
		while (check_path->way)
			check_path = check_path->way;
		while (check_path)
		{
			check_path->length = count;
			count++;
			check_path = check_path->prev_way;
		}
		path = path->next;
	}
}

/*
**	определение необходимого количества шагов при текущем потоке
*/

int		ft_find_count_of_step_for_ants_by_flow(t_all *all, int flow)
{
	t_path	*path;
	int		max_lenght;
	int		step_count;

	max_lenght = 0;
	path = all->paths;
	while (path)
	{
		if (path->flow_count == flow)
		{
			if (max_lenght < path->length)
				max_lenght = path->length;
		}
		else if (path->flow_count < flow)
			break ;
		path = path->next;
	}
	step_count = all->ant / flow + max_lenght - 1;
	return (step_count);
}

/*
**	определяет поток с наименьшим еоличеством шагов для перемещения
*/

int		ft_choose_flow(t_all *all)
{
	int best_flow;
	int flow;
	int step_count;
	int best_count;

	flow = all->paths->flow_count;
	best_count = INF;
	if (all->ant < flow)
		flow = all->ant;
	while (flow)
	{
		step_count = ft_find_count_of_step_for_ants_by_flow(all, flow);
		if (step_count < best_count)
		{
			best_count = step_count;
			best_flow = flow;
		}
		flow--;
	}
	return (best_flow);
}


/*
**	в случае нахождения еще одного маршрута, дублирует старые,
**	увеличивает максимальный поток и перекраивает их с новым
*/

int	ft_find_another_path(t_all *all)
{
	int answer;

	answer = FAIL;
	ft_deactive_new_paths(all);
	if (ft_dijkstra(all) == SUCCESS)
	{
		if (ft_copy_last_paths_with_same_flows(all) == SUCCESS)
			ft_dijkstra(all);
		(all->flows)++;
		ft_add_path(all, all->flows);
		ft_merge_paths(all->paths);
		if (all->paths->way->rooms != all->end)
			answer = SUCCESS;
	}
	ft_del_fictious_tubes_and_rooms(all);
	return (answer);
}

/*
**		для Дейкстры
*/

int		ft_find_index_of_not_visited_room_with_min_dist(t_all *all, int *distance, char *visited)
{
	int min_dist;
	int index;
	int i;

	i = 0;
	min_dist = INF;
	index = INF;
	while (i < all->count)
	{
		if (!visited[i] && distance[i] <= min_dist)
		{
			min_dist = distance[i];
			index = i;
		}
		i++;
	}
	if (index == INF)
		return (NO_CONNECT);
	return(index);
}

/*
**	для Дейкстры
*/

int ft_cost_of_tube(t_all *all, int room1_num, int room2_num)
{
	t_tube *tube;

	tube = all->tubes;
	while (tube)
	{
		if (room1_num == tube->room1->num && room2_num == tube->room2->num)
		{
			if (tube->active)
				return (tube->cost);
			else
				return (INF);
		}
		tube = tube->next;
	}
	return (INF);
}

/*
**	для Дейкстры
*/

void	ft_choose_min_dist_from_active_visited_room_to_not_visited(t_all *all, int *distance, char *visited, int u)
{
	int cost;
	int i;

	i = 0;
	while (i < all->count)
	{
		cost = ft_cost_of_tube(all, u, i);
		if (!visited[i] && distance[u] != INF &&
		distance[u] + cost < distance[i])
		{
			distance[i] = distance[u] + cost;
			ft_find_room_by_number(all->rooms, i)->prev = ft_find_room_by_number(all->rooms, u);
			//pred[i] = u;
		}
		i++;
	}
}

/*
**	Алгоритм Дейкстры для поиска кратчайшего маршрута от начала в конец
*/

int ft_dijkstra(t_all *all)
{
	int		distance[all->count];
	char	visited[all->count];
	int		count;
	int		near_notvisited_room;

	ft_clear_shorts_path(all);
	ft_bzero((void *)visited, all->count);
	ft_memset((void *)distance, 7, 4 * all->count);
	distance[all->start->num] = 0;
	count = 0;
	while (count < all->count)
	{
		near_notvisited_room = ft_find_index_of_not_visited_room_with_min_dist(all, distance, visited);
		if (near_notvisited_room == NO_CONNECT)
			break ;
		visited[near_notvisited_room] = TRUE;
		ft_choose_min_dist_from_active_visited_room_to_not_visited(all, distance, visited, near_notvisited_room);
		count++;
	}
	if (all->end->prev)
		return (SUCCESS);
	return (FAIL);
}

/*
**	создаем муравейник...
*/

void ft_create_antwood(t_all *all)
{
	t_ant *ants;
	int count;

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

/*
**	активирует муравьев в заисимости от количества потоков
*/

void	ft_ant_activation(t_all *all)
{
	int flows;
	t_ant *ant;
	t_path *path;

	flows = ft_choose_flow(all);
	all->ant -= flows;
	ant = all->ants;
	path = all->paths;
	while (path->flow_count != flows)
		path = path->next;
	while (ant && flows)
	{
		if (ant->motion == START)
		{
			ant->motion = MOVE;
			ant->step = path;
			if (all->paths->way->rooms != all->end)
			{//если вход и выход соединены одной трубой
				path = path->next;
				flows--;
			}
		}
		ant = ant->next;
	}
}

/*
**	функция перемещает одного муравья по его маршруту
*/

int		ft_is_ant_step(t_all *all, t_ant *ant)
{
	if (ant->motion == MOVE && ant->step->rooms == all->start &&
	ant->step->way->rooms->empty_full == FULL && all->paths->way->rooms != all->end)
	{
		ant->motion = START;
		all->ant++;
	}
	if (ant->motion == MOVE && ant->step->rooms == all->end)
		ant->motion = END;
	if (ant->motion == MOVE && ant->step->way &&
	(ant->step->way->rooms->empty_full == EMPTY ||
	ant->step->way->rooms == all->end))
	{
		ant->step->rooms->empty_full -= FULL;
		ant->step = ant->step->way;
		ant->step->rooms->empty_full += FULL;
		return(TRUE);
	}
	return(FALSE);
}



/*
** приводит муравьев в движение по их маршрутам
*/

void	ft_ants_motion(t_all *all)
{
	t_ant *ant;
	int first_ant;

	while (all->end->empty_full < all->ant_count)
	{
		if (all->ant)
			ft_ant_activation(all);
		ant = all->ants;
		first_ant = TRUE;
		while (ant)
		{
			if (ft_is_ant_step(all, ant))
			{
				if (first_ant)
					first_ant = FALSE;
				else
					printf("%s", " ");
				printf("L%d-%s", ant->name, ant->step->rooms->name);
			}
			ant = ant->next;
		}
		printf("\n");
	}
}




//главная функция
int main()
{



	t_all *all;
	//all = ft_create_all(rooms, "0", "7");
	all = ft_create_all(5);

	ft_add_room(all, "0");
	ft_add_room(all, "1");
	ft_add_room(all, "2");
	ft_add_room(all, "3");
	ft_add_room(all, "4");
	ft_add_room(all, "5");
	ft_add_room(all, "6");
	ft_add_room(all, "7");
	ft_add_room(all, "8");
	ft_add_room(all, "9");


	all->start = ft_find_room_by_name(all->rooms, "0");
	all->end = ft_find_room_by_name(all->rooms, "7");



	//ft_print_rooms(rooms);


	printf("   -0    \n");
	printf(" /  |       \n");
	printf("3---2---1  \n");
	printf("|   |   |     \n");
	printf("6---5   4  \n");
	printf("    |  /     \n");
	printf("    7-  \n");
	printf("       \n");


	//all->tubes = ft_create_tube(all->rooms, "0", "2", ACTIVE);
	ft_add_tube(all, "0", "2", ACTIVE);
	ft_add_tube(all, "2", "0", ACTIVE);
	ft_add_tube(all, "0", "3", ACTIVE);
	ft_add_tube(all, "3", "0", ACTIVE);
	ft_add_tube(all, "1", "2", ACTIVE);
	ft_add_tube(all, "2", "1", ACTIVE);
	ft_add_tube(all, "1", "4", ACTIVE);
	ft_add_tube(all, "4", "1", ACTIVE);
	ft_add_tube(all, "3", "2", ACTIVE);
	ft_add_tube(all, "2", "3", ACTIVE);
	ft_add_tube(all, "3", "6", ACTIVE);
	ft_add_tube(all, "6", "3", ACTIVE);
	ft_add_tube(all, "2", "5", ACTIVE);
	ft_add_tube(all, "5", "2", ACTIVE);
	ft_add_tube(all, "6", "5", ACTIVE);
	ft_add_tube(all, "5", "6", ACTIVE);
	ft_add_tube(all, "7", "5", ACTIVE);
	ft_add_tube(all, "5", "7", ACTIVE);
	ft_add_tube(all, "4", "7", ACTIVE);
	ft_add_tube(all, "7", "4", ACTIVE);

	ft_add_tube(all, "17", "41", ACTIVE);


/*
printf("1-------2    \n");
printf("|       |       \n");
printf("0---3---4---9-_  \n");
printf("|   |       |  |  \n");
printf("|   5-------6  |\n");
printf("7--------------8      \n");
printf("       \n");




	//all->tubes = ft_create_tube(all->rooms, "0", "1", ACTIVE);
	ft_add_tube(all, "0", "1", ACTIVE);
	ft_add_tube(all, "1", "0", ACTIVE);
	ft_add_tube(all, "0", "3", ACTIVE);
	ft_add_tube(all, "3", "0", ACTIVE);
	ft_add_tube(all, "0", "7", ACTIVE);
	ft_add_tube(all, "7", "0", ACTIVE);
	ft_add_tube(all, "1", "2", ACTIVE);
	ft_add_tube(all, "2", "1", ACTIVE);
	ft_add_tube(all, "4", "2", ACTIVE);
	ft_add_tube(all, "2", "4", ACTIVE);
	ft_add_tube(all, "7", "8", ACTIVE);
	ft_add_tube(all, "8", "7", ACTIVE);
	ft_add_tube(all, "8", "9", ACTIVE);
	ft_add_tube(all, "9", "8", ACTIVE);
	ft_add_tube(all, "3", "5", ACTIVE);
	ft_add_tube(all, "5", "3", ACTIVE);
	ft_add_tube(all, "3", "4", ACTIVE);
	ft_add_tube(all, "4", "3", ACTIVE);
	ft_add_tube(all, "4", "9", ACTIVE);
	ft_add_tube(all, "9", "4", ACTIVE);
	ft_add_tube(all, "5", "6", ACTIVE);
	ft_add_tube(all, "6", "5", ACTIVE);
	ft_add_tube(all, "6", "9", ACTIVE);
	ft_add_tube(all, "9", "6", ACTIVE);

	ft_add_tube(all, "9", "61", ACTIVE);
*/
	//ft_print_tubes(all->tubes);

	//ft_find_another_path(all);

/*	if (ft_dijkstra(all) == SUCCESS)
	{
		ft_add_path(all, 1);
		//ft_print_paths(all->paths);
		ft_find_another_path(all);
	}
*/


	ft_create_antwood(all);


	while (ft_find_another_path(all) == SUCCESS && all->ant > all->paths->flow_count)
		ft_recount_length_of_all_paths(all->paths);


/*	t_path *path = all->paths;

	while (path)
	{
		ft_print_paths(path);
		printf("_________________\n");
		path = path->next;
	}
*/

	if (all->paths)
		ft_ants_motion(all);




	ft_del_all(all);


	//ft_deactive_tube(all, "0", "2");
	//ft_deactive_tube(all, "2", "5");
	//ft_restruct_tube(all, "2", "5");
	//ft_deactive_tube(all, "5", "7");


	//ft_print_tubes(all->tubes);


	//ft_dijkstra(all);


	//ft_create_path(all, 2);
	//ft_print_rooms(all->paths->next->rooms);
	//ft_print_rooms(all->paths->rooms);

/*
	char *str = NULL;
	t_room *room = all->end;

	while (room)
	{
		str = ft_strjoin_free(room->name, str, 0, 1);
		str = ft_strjoin_free(" ", str, 0, 1);
		room = room->prev;
	}

	printf("%s\n", str);
*/



	//short_path = Dijkstra(GR, start);
	//printf("%s\n", short_path);
/*
	printf("0---1---3    \n");
	printf(" \\   \\   \\  \n");
	printf("  2---4---5     \n");
	printf("       \n");
	printf("неудобный короткий путь:\n");
	printf(" 0 1 4 5\n");
	printf("путь после отключения связей 0-1, 1-4, 4-5:\n");
	GR[0][1] = 0;
	GR[1][4] = 0;
	GR[4][5] = 0;
*/

	//GR[0][2] = 0;
	//GR[2][5] = 0;
	//GR[5][7] = 0;
	//short_path = Dijkstra(GR, start);
	//printf("%s\n", short_path);


	/*int **arr;
	int i;
	arr = GR;
	while (arr[0])
	{
		i = 0;
		while(i < V)
		{
			printf("%d\t", arr[0][i]);
			i++;
		}
		arr++;
		printf("\n");
	}*/






	return (0);
}
