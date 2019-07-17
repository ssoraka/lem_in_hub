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


typedef struct		s_room
{
	int				x;
	int				y;
	int				num;
	char			*path;
	char			*name;
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

typedef struct		s_all
{
	int				ants;
	int				count;
	int				s_num;
	int				e_num;
	struct s_room	*rooms;
	struct s_room	*start;
	struct s_room	*end;
	struct s_tube	*tubes;
}					t_all;

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

t_tube	*ft_create_tube(t_room *rooms, char *name1, char *name2)
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
	tmp->cost = 1;
	tmp->active = 1;
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
		new_room->num = (*rooms)->num + 1;
	*rooms = new_room;
	return (SUCCESS);
}


int		ft_add_tube(t_tube **tubes, t_room *rooms, char *name1, char *name2)
{
	t_tube *new_tube;

	if (!tubes || !rooms)
		return (FAIL);
	new_tube = ft_create_tube(rooms, name1, name2);
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

void ft_print_rooms(t_room *rooms)
{
	while (rooms)
	{
		printf("%s\n", rooms->name);
		rooms = rooms->next;
	}
}

void ft_print_tubes(t_tube *tubes)
{
	while (tubes)
	{
		printf("%s-%s\n", tubes->room1->name, tubes->room2->name);
		tubes = tubes->next;
	}
}

t_all	*ft_create_all(t_room *rooms, t_tube *tubes, char *start, char *end)
{
	t_all *tmp;

	tmp = (t_all *)ft_memalloc(sizeof(t_all));
	if (!tmp)
		return (NULL);
	tmp->rooms = rooms;
	tmp->tubes = tubes;
	tmp->start = ft_find_room_by_name(rooms, start);
	tmp->end = ft_find_room_by_name(rooms, end);
	tmp->s_num = tmp->start->num;
	tmp->e_num = tmp->end->num;
	tmp->count = rooms->num + 1;
	return (tmp);
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




char *ft_dijkstra(t_all *all)
{
	int distance[V], i, u;
	char visited[V];

	ft_bzero((void *)visited, all->count);
	ft_memset((void *)distance, 7, 4 * all->count);
	all->start->path = ft_strdup("");
	distance[all->s_num] = 0;


	int count;

	count = 0;
	while (count < all->count)
	{
		u = ft_find_index_of_not_visited_room_with_min_dist(all, distance, visited);
		if (u == -1)//поставить константу
			break ;
		visited[u] = TRUE;

	/*	if (pred[u] >= 0)
			path[u] = ft_strjoin_free( " " , ft_itoa(u), 0, 1);
		else
			path[u] = ft_strjoin_free( "маршрут недоступен до " , ft_itoa(u), 0, 1);

		if (u != 0 && pred[u] >= 0)
			path[u] = ft_strjoin_free( path[pred[u]] , path[u], 0, 1);
*/

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
		printf("%d\n", distance[i]);
		i++;
	}*/

	return (all->end->path);
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

	//ft_print_rooms(rooms);

	t_tube *tubes;

	tubes = ft_create_tube(rooms, "0", "1");
	ft_add_tube(&tubes, rooms, "1", "0");
	ft_add_tube(&tubes, rooms, "0", "2");
	ft_add_tube(&tubes, rooms, "2", "0");
	ft_add_tube(&tubes, rooms, "1", "3");
	ft_add_tube(&tubes, rooms, "3", "1");
	ft_add_tube(&tubes, rooms, "2", "4");
	ft_add_tube(&tubes, rooms, "4", "2");
	ft_add_tube(&tubes, rooms, "1", "4");
	ft_add_tube(&tubes, rooms, "4", "1");
	ft_add_tube(&tubes, rooms, "3", "5");
	ft_add_tube(&tubes, rooms, "5", "3");
	ft_add_tube(&tubes, rooms, "4", "5");
	ft_add_tube(&tubes, rooms, "5", "4");

	//ft_print_tubes(tubes);

	t_all *all;
	all = ft_create_all(rooms, tubes, "0", "5");

	char *short_path;


	ft_deactive_tube(all, "0", "1");
	ft_deactive_tube(all, "4", "4");
	ft_deactive_tube(all, "4", "5");


	short_path = ft_dijkstra(all);

	char *str = NULL;
	t_room *room = all->end;

	while (room)
	{
		str = ft_strjoin_free(room->name, str, 0, 1);
		str = ft_strjoin_free(" ", str, 0, 1);
		room = room->prev;
	}

	printf("%s\n", str);




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
