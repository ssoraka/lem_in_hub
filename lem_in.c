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


#define INF 117901063
#define V 10

#define OUT 1
#define IN 2

#define INVERS -1
#define DEACTIVE 0
#define ACTIVE 2
#define FICTIOUS 1
#define FICTIOUS 1


typedef struct		s_room
{
	int				x;
	int				y;
	int				num;
	char			*path;
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

typedef struct		s_all
{
	int				ants;
	int				count;
	int				s_num;
	int				e_num;
	int				flows;
	struct s_room	*rooms;
	struct s_room	*start;
	struct s_room	*end;
	struct s_tube	*tubes;
	struct s_path	*paths;
}					t_all;



int ft_dijkstra(t_all *all);


#define INVERS -1
#define DEACTIVE 0
#define ACTIVE 2
#define FICTIOUS 1
#define FICTIOUS_NULL 3


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
	t_room *tmp;

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
	t_room *tmp;

	while (rooms)
	{
		if (rooms->num == num)
			return (rooms);
		rooms = rooms->next;
	}
	return (NULL);
}

t_tube	*ft_create_tube(t_room *rooms, char *name1, char *name2, int active)
{
	t_tube *tmp;
	t_room *room1;
	t_room *room2;

	room1 = ft_find_room_by_name(rooms, name1);
	room2 = ft_find_room_by_name(rooms, name2);
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


int		ft_add_room(t_room **rooms, char *name)
{
	t_room *new_room;

	if (!rooms)
		return (FAIL);
	new_room = ft_create_room(name);
	if (!new_room)
		return (FAIL);
	new_room->next = *rooms;
	if (*rooms)
	{
		new_room->num = (*rooms)->num + 1;
		(*rooms)->prev = new_room;
	}
	*rooms = new_room;
	return (SUCCESS);
}


int		ft_add_tube(t_all *all, char *name1, char *name2, int active)
{
	t_tube **tubes;
	t_room *rooms;
	t_tube *new_tube;

	tubes = &(all->tubes);
	rooms = all->rooms;
	if (!tubes || !rooms)
		return (FAIL);
	new_tube = ft_create_tube(rooms, name1, name2, active);
	if (!new_tube)
		return (FAIL);
	new_tube->next = *tubes;
	*tubes = new_tube;
	return (SUCCESS);
}


void	ft_deactive_tube(t_all *all, char *name1, char *name2)
{
	t_tube *tube;

	tube = all->tubes;
	while (tube)
	{
		if (ft_strcmp(name1, tube->room1->name) == 0 &&
			ft_strcmp(name2, tube->room2->name) == 0)
		{
			tube->active = 0;
			return ;
		}
		tube = tube->next;
	}
}




void	ft_change_tube(t_all *all, char *old, char *new, int in_out)
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
		if (old_room == tube->room1 && in_out == OUT && tube->active == ACTIVE)
		{
			tube->active = DEACTIVE;
			ft_add_tube(all, new, tube->room2->name, FICTIOUS);
		}
		if (old_room == tube->room2 && in_out == IN && tube->active == ACTIVE)
		{
			tube->active = DEACTIVE;
			ft_add_tube(all, tube->room1->name, new, FICTIOUS);
		}
		tube = tube->next;
	}

	if (in_out == OUT)
		ft_add_tube(all, new, old, FICTIOUS_NULL);
	else if (in_out == IN)
		ft_add_tube(all, old, new, FICTIOUS_NULL);
}



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
	ft_add_room(&(all->rooms), first_out);
	all->rooms->origin = ft_find_room_by_name(all->rooms, first);
	ft_add_room(&(all->rooms), second_in);
	all->rooms->origin = ft_find_room_by_name(all->rooms, second);
	all->count += 2;
	//перекидываем связи с существующего узла на новый
	ft_change_tube(all, first, first_out, OUT);
	ft_change_tube(all, second, second_in, IN);

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

t_all	*ft_create_all(t_room *rooms, char *start, char *end)
{
	t_all *tmp;

	tmp = (t_all *)ft_memalloc(sizeof(t_all));
	if (!tmp)
		return (NULL);
	tmp->rooms = rooms;
	tmp->start = ft_find_room_by_name(rooms, start);
	tmp->end = ft_find_room_by_name(rooms, end);
	tmp->s_num = tmp->start->num;
	tmp->e_num = tmp->end->num;
	tmp->count = rooms->num + 1;
	return (tmp);
}


t_path	*ft_create_path(t_room *room, int flow_count)
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

t_path	*ft_create_way(t_room *room, int flow_count)
{
	t_path *begin;
	t_path *tmp;

	if (!room || !room->prev)
		return (NULL);
	begin = NULL;
	while (room)
	{
		if (room->origin)
			tmp = ft_create_path(room->origin, flow_count);
		else
			tmp = ft_create_path(room, flow_count);
		if (!tmp)
			return (NULL);//надо удалить созданную цепочку и все похерить
		tmp->way = begin;
		if (begin)
			begin->prev_way = tmp;
		begin = tmp;
		room = room->prev;
	}
	return (begin);
}

/*
t_path	*ft_create_way(t_room *room, int flow_count)
{
	t_path *tmp;

	if (!room || !room->prev)
		return (NULL);
	tmp = (t_path *)ft_memalloc(sizeof(t_path));
	if (!tmp)
		return (NULL);
	while (room)
	{
		ft_add_room(&(tmp->rooms), room->name);
		tmp->rooms->origin = room->origin;//так себе способ ссылаться из пути на список оригинальных комнат...
		(tmp->length)++;
		room = room->prev;
	}
	tmp->flow_count = flow_count;
	return (tmp);
}
*/

int 	ft_add_path(t_all *all, int flow_count)
{
	t_path *path;

	if (!(path = ft_create_way(all->end, flow_count)))
		return (FAIL);
	path->next = all->paths;
	all->paths = path;
	return (SUCCESS);
}

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


int		ft_copy_last_paths_with_same_flows(t_all *all)
{
	t_path *check_path;
	t_path *path_new;
	t_room *origin;
	int flow;

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
			if (ft_strcmp(room->rooms->name, all->start->name) && ft_strcmp(room->rooms->name, all->end->name) &&
			ft_strcmp(room->way->rooms->name, all->start->name) && ft_strcmp(room->way->rooms->name, all->end->name))
				ft_restruct_tube(all, room->rooms->name, room->way->rooms->name);
			else
				ft_deactive_tube(all, room->rooms->name, room->way->rooms->name);
			room = room->way;
		}
		check_path = check_path->next;
	}

	//ft_print_tubes(all->tubes);
}



void	ft_del_fictious_rooms(t_all *all)
{
	t_room *room;

	while (all->rooms->origin)
	{
		room = all->rooms;
		all->rooms = all->rooms->next;
		free(room->name);
		free(room);
	}
}

void	ft_del_fictious_tubes(t_all *all)
{
	t_tube *tube;

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
}

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
if (ft_dijkstra(all) == SUCCESS)
{
	ft_add_path(all, 1);
	ft_find_another_path(all);
}
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
		answer = SUCCESS;
	}
	ft_del_fictious_rooms(all);
	ft_del_fictious_tubes(all);
	//ft_print_tubes(all->tubes);
	//ft_active_new_paths(all);

	//ft_print_paths(all->paths->next->next);
//	printf("_________________\n");
//	ft_print_paths(all->paths->next);
//	printf("_________________\n");
//	ft_print_paths(all->paths);

	return (answer);
}


//const int V = 6;
//алгоритм Дейкстры
/*
char *Dijkstra(int **GR, int start)
{
	int distance[V], count, index, i, u, min;
	int visited[V];
	int pred[V];
	char *path[V];

	for (i = 0; i < V; i++)
	{
		distance[i] = INF;
		visited[i] = FALSE;
		path[i] = NULL;
		pred[i] = -1;
	}
	path[start] = ft_strdup("");
	distance[start] = 0;
	pred[start] = 0;

	for (count = 0; count < V; count++)
	{
		min = INF;
		for (i = 0; i < V; i++)
			if (!visited[i] && distance[i]<=min)
			{
				min = distance[i];
				index = i;
			}

		u = index;
		visited[u] = TRUE;

		if (pred[u] >= 0)
			path[u] = ft_strjoin_free( " " , ft_itoa(u), 0, 1);
		else
			path[u] = ft_strjoin_free( "маршрут недоступен до " , ft_itoa(u), 0, 1);

		if (u != 0 && pred[u] >= 0)
			path[u] = ft_strjoin_free( path[pred[u]] , path[u], 0, 1);





		index = -1;
		for (i = 0; i < V; i++)
		{
			if (!visited[i] && GR[u][i] && distance[u] != INF &&
			distance[u] + GR[u][i] < distance[i])
			{
				distance[i] = distance[u] + GR[u][i];
				pred[i] = u;
			}
		}
	}
	return (path[7]);

	printf("Стоимость пути из начальной вершины до остальных:\n");
	for (i=1; i<V; i++)
	{
		if (distance[i] != INF)
		{
			printf("расстояние от 0 до %d = %d", i, distance[i]);
			printf(" кратчайший путь -> %s\n", path[i]);
		}
		else
			printf("%d > %d = %s\n", start, i, "маршрут недоступен");
	}

}
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
		return (-1);
	return(index);
}

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




int ft_dijkstra(t_all *all)
{
	int distance[all->count], i, u;
	char visited[all->count];

	ft_clear_shorts_path(all);
	ft_bzero((void *)visited, all->count);
	ft_memset((void *)distance, 7, 4 * all->count);
	distance[all->s_num] = 0;


	int count;

	count = 0;
	while (count < all->count)
	{
		u = ft_find_index_of_not_visited_room_with_min_dist(all, distance, visited);
		if (u == -1)//поставить константу
			break ;
		visited[u] = TRUE;


		int cost;

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

		count++;
	}

	/*i = 0;
	while (i < all->count)
	{
		printf("%d__%d\n", i, distance[i]);
		i++;
	}*/
	if (all->end->prev)
		return (SUCCESS);
	return (FAIL);
}





//главная функция
int main()
{

	t_room *rooms;

	rooms = ft_create_room("0");
	ft_add_room(&rooms, "1");
	ft_add_room(&rooms, "2");
	ft_add_room(&rooms, "3");
	ft_add_room(&rooms, "4");
	ft_add_room(&rooms, "5");
	ft_add_room(&rooms, "6");
	ft_add_room(&rooms, "7");
//	ft_add_room(&rooms, "8");
//	ft_add_room(&rooms, "9");

	t_all *all;
	all = ft_create_all(rooms, "0", "7");

	//ft_print_rooms(rooms);


	all->tubes = ft_create_tube(all->rooms, "0", "2", 2);
	ft_add_tube(all, "2", "0", 2);
	ft_add_tube(all, "0", "3", 2);
	ft_add_tube(all, "3", "0", 2);
	ft_add_tube(all, "1", "2", 2);
	ft_add_tube(all, "2", "1", 2);
	ft_add_tube(all, "1", "4", 2);
	ft_add_tube(all, "4", "1", 2);
	ft_add_tube(all, "3", "2", 2);
	ft_add_tube(all, "2", "3", 2);
	ft_add_tube(all, "3", "6", 2);
	ft_add_tube(all, "6", "3", 2);
	ft_add_tube(all, "2", "5", 2);
	ft_add_tube(all, "5", "2", 2);
	ft_add_tube(all, "6", "5", 2);
	ft_add_tube(all, "5", "6", 2);
	ft_add_tube(all, "7", "5", 2);
	ft_add_tube(all, "5", "7", 2);
	ft_add_tube(all, "4", "7", 2);
	ft_add_tube(all, "7", "4", 2);

/*
all->tubes = ft_create_tube(all->rooms, "0", "1", ACTIVE);
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
*/

	//ft_print_tubes(tubes);

	//ft_find_another_path(all);

/*	if (ft_dijkstra(all) == SUCCESS)
	{
		ft_add_path(all, 1);
		//ft_print_paths(all->paths);
		ft_find_another_path(all);
	}
*/
	while (ft_find_another_path(all) == SUCCESS)
	{
		;
	}


	while (all->paths)
	{
	ft_print_paths(all->paths);
	printf("_________________\n");
	all->paths = all->paths->next;
	}


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






	exit (0);
	return (0);
}
