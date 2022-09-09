/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_intersections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaouil <itaouil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/06 12:17:05 by itaouil           #+#    #+#             */
/*   Updated: 2022/09/09 21:22:10 by itaouil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	is_wall(t_cub *data, t_point *point, int direction)
{
	int	x;
	int	y;

	y = get_y_coordinate(data, point->y);
	x = get_x_coordinate(data, point->x, y);
	if (data->cubmap[y][x] == '1')
	{
		// printf("x = %d et y = %d\n", x, y);
		return (1);
	}
	else if (direction == DOWN)
	{
		if (data->cubmap[y][x + 1] == '1')
		{
			// printf("x = %d et y = %d\n", x, y);
			return (1);
		}
	}
	else if (direction == RIGHT)
	{
		if (data->cubmap[y][x + 1] == '1')
		{
			// printf("x = %d et y = %d\n", x, y);
			return (1);
		}
	}
	else if (direction == LEFT)
	{
		if (data->cubmap[y][x - 1] == '1')
		{
			// printf("x = %d et y = %d\n", x, y);
			return (1);
		}
	}
	// printf("x = %d et y = %d\n", x, y);
	return (0);
}

t_point	*first_hor_intersection(t_cub *data, int direction)
{
	t_point	*intersection;

	intersection = malloc(sizeof(t_point));
	if (direction == UP)
		intersection->y = ((int)(data->poz[1]) / 64) * 64 - 1;
	else
		intersection->y = ((int)(data->poz[1]) / 64) * 64 + 63;
	intersection->x = (int)(data->poz[0]) + ((int)(data->poz[1]) - intersection->y) / tan(data->ray);
	return (intersection);
}

void	next_hor_intersection(t_cub *data, int direction, t_point **point)
{
	int		Ya;
	int		Xa;

	if (direction == UP)
		Ya = -64;
	else
		Ya = 64;
	Xa = Ya / tan(data->ray);
	(*point)->y = (*point)->y + Ya;
	(*point)->x = (*point)->x - Xa;
}

t_point	*first_ver_intersection(t_cub *data, int direction)
{
	t_point	*intersection;

	intersection = malloc(sizeof(t_point));
	if (direction == RIGHT)
		intersection->x = ((int)(data->poz[0]) / 64) * 64 + 63;
	else
		intersection->x = ((int)(data->poz[0]) / 64) * 64 - 1;
	intersection->y = (int)data->poz[1] + ((int)(data->poz[0]) - intersection->x) / tan(data->ray);
	return (intersection);
}

void	next_ver_intersection(t_cub *data, int direction, t_point **point)
{
	int		Xa;
	int		Ya;

	if (direction == RIGHT)
		Xa = 64;
	else
		Xa = -64;
	Ya = Xa * tan(data->ray);
	(*point)->x = (*point)->x + Xa;
	(*point)->y = (*point)->y - Ya;
}

t_point	*paint_hor_intersections(t_cub *data, int direction)
{
	t_point	*intersection;

	intersection = malloc(sizeof(t_point));
	intersection = first_hor_intersection(data, direction);
	if (direction == UP || direction == NONE)
	{
		printf("direction = UP\n");
		while (intersection->y >= 63 && intersection->x >= 63
			&& !(is_wall(data, intersection, direction)))
			next_hor_intersection(data, direction, &intersection);
	}
	if (direction == DOWN || direction == NONE)
	{
		printf("direction = DOWN\n");
		while (intersection->y <= data->map_height && intersection->x >= 63
			&& !(is_wall(data, intersection, direction)))
			next_hor_intersection(data, direction, &intersection);
	}
	// printf("now intersection's x = %f et intersection's y = %f\n", intersection->x, intersection->y);
	return (intersection);
	// dda(data, player, intersection);
}

t_point	*paint_ver_intersections(t_cub *data, int direction)
{
	t_point	*intersection;
	// int		direction;

	// if ((data->ray > 0 && data->ray < PI / 2) || (data->ray > 3 * PI / 2 && data->ray < (2 * PI)))
		// direction = RIGHT;
	// else
		// direction = LEFT;
	intersection = first_ver_intersection(data, direction);
	if (direction == RIGHT || direction == NONE)
	{
		printf("direction = RIGHT\n");
		while (intersection->y >= 63 && intersection->y <= data->map_height
			&& intersection->x <= data->map_len
			&& !(is_wall(data, intersection, direction)))
		{
			// printf("intersection's x = %f et intersection's y = %f\n", intersection->x, intersection->y);
			next_ver_intersection(data, direction, &intersection);
		}
	}
	if (direction == LEFT || direction == NONE)
	{
		printf("direction = LEFT\n");
		while (intersection->x >= 0 && intersection->y >= 0 && intersection->y < data->map_height && intersection->x < data->map_len
			&& !(is_wall(data, intersection, direction)))
			next_ver_intersection(data, direction, &intersection);
	}
	// printf("now intersection's x = %f et intersection's y = %f\n", intersection->x, intersection->y);
	return (intersection);
}

float	get_distance(float ray, t_point *player, t_point *wall)
{
	float	distance;

	distance = abs_val(player->x - wall->x) / (cos(ray));
	return (abs_val(distance));
}

int	lateral_direction(t_cub *data)
{
	int	direction;

	// printf("angle = %f\n", data->ray);
	if ((data->ray >= 0 && data->ray < PI / 2) || (data->ray > 3 * PI / 2 && data->ray <= (2 * PI)))
		direction = RIGHT;
	else if (data->ray == PI / 2 || data->ray == 3 * PI / 2)
		direction = NONE;
	else
		direction = LEFT;
	return (direction);
}

int	forward_direction(t_cub *data)
{
	int	direction;

	// printf("angle = %f\n", data->ray);
	if (data->ray == PI || data->ray == 0 || data->ray == 2 * PI)
		direction = NONE;
	else if (data->ray < PI)
		direction = UP;
	else
		direction = DOWN;
	return (direction);
}

void	paint_ray(t_cub *data)
{
	float	ver_distance;
	float	hor_distance;
	t_point	*ver_intersection;
	t_point	*hor_intersection;
	t_point	*player;

	player = malloc(sizeof(t_point));
	player->x = (data->poz[0]);
	player->y = (data->poz[1]);
	ver_intersection = paint_ver_intersections(data, lateral_direction(data));
	hor_intersection = paint_hor_intersections(data, forward_direction(data));
	hor_distance = get_distance(data->ray, player, hor_intersection);
	ver_distance = get_distance(data->ray, player, ver_intersection);
	if (hor_distance < ver_distance)
	{
		// printf("hor distance = %f amd ver distance = %f\n", hor_distance, ver_distance);
		dda(data, player, hor_intersection);
	}
	else
	{
		printf("ver distance = %f and coordinate x = %f and y =%f\n", ver_distance, ver_intersection->x, ver_intersection->y);
		dda(data, player, ver_intersection);
	}
}

void	paint_fov(t_cub *data)
{
	float	fov_ray;
	float	incr;
	float	tmp;

	fov_ray = data->angle + 0.52;
	incr = 1.05 / 320;
	tmp = data->angle;
	// data->ray = fov_ray;
	// paint_ray(data);
	while(fov_ray > tmp - 0.52)
	{
		data->ray = fov_ray;
		paint_ray(data);
		fov_ray = fov_ray - incr;
	}
	data->ray = tmp - 0.52;
	paint_ray(data);
	data->ray = tmp;
}
