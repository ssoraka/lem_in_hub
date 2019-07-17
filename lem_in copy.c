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


#define INF 1000000
#define V 6

//const int V = 6;
//алгоритм Дейкстры
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
	return (path[5]);

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
//главная функция
int main()
{
	int start = 0;
	int *GR[V + 1];

	int GR0[V] = {0, 1, 1, 0, 0, 0};
	int GR1[V] = {1, 0, 0, 1, 1, 0};
	int GR2[V] = {1, 0, 0, 0, 1, 0};
	int GR3[V] = {0, 1, 0, 0, 0, 1};
	int GR4[V] = {0, 1, 1, 0, 0, 1};
	int GR5[V] = {0, 0, 0, 1, 1, 0};

	GR[0] = GR0;
	GR[1] = GR1;
	GR[2] = GR2;
	GR[3] = GR3;
	GR[4] = GR4;
	GR[5] = GR5;
	GR[6] = NULL;

	char *short_path;

	//short_path = Dijkstra(GR, start);
	//printf("%s\n", short_path);

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

	short_path = Dijkstra(GR, start);
	printf("%s\n", short_path);

	GR[0][2] = 0;
	printf("путь после отключения связей 0-2, ...:\n");
	short_path = Dijkstra(GR, start);
	printf("%s\n", short_path);
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
