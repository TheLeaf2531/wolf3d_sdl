/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vboissel <vboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/28 23:28:50 by vboissel          #+#    #+#             */
/*   Updated: 2019/02/28 23:28:50 by vboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"



static double newPrecision(double n, double i) 
{ 
    return floor(pow(10,i)*n)/pow(10,i); 
}


t_ray				set_ray(t_map *map, t_vector2f origin,
							t_vector2f direction, int origin_sector,
							int gate_stop)
{
	t_ray		ray;

	ray.map = map;
	ray.origin = origin;
	ray.direction = direction;
	ray.current_sector = origin_sector;
	ray.origin_sector = origin_sector;
	ray.excluded_wall = -1;
	ray.gate_stop = gate_stop;
	return (ray);
}

static	int			coherent_hit(t_ray ray, t_wall *w, t_hit hit)
{
	int			is_legit;
	t_vector2f	ran_x;
	t_vector2f	ran_y;

	ran_x.x = w->pos[0].x < w->pos[1].x ? w->pos[0].x : w->pos[1].x;
	ran_x.y = w->pos[0].x < w->pos[1].x ? w->pos[1].x : w->pos[0].x;
	ran_y.x = w->pos[0].y < w->pos[1].y ? w->pos[0].y : w->pos[1].y;
	ran_y.y = w->pos[0].y < w->pos[1].y ? w->pos[1].y : w->pos[0].y;
	is_legit = 1;
	if ((ray.direction.x < 0.0F && hit.pos.x > ray.origin.x) ||
			(ray.direction.y < 0.0F && hit.pos.y > ray.origin.y) ||
			(ray.direction.x > 0.0F && hit.pos.x < ray.origin.x) ||
			(ray.direction.y > 0.0F && hit.pos.y < ray.origin.y))
		is_legit = 0;
	else if (hit.pos.x < ran_x.x - 0.00001F|| hit.pos.x > ran_x.y + 0.00001F)
		is_legit = 0;
	else if (hit.pos.y < ran_y.x - 0.00001F|| hit.pos.y > ran_y.y + 0.00001F)
		is_legit = 0;
	return (is_legit);	
}

static t_hit		intersection(t_ray ray, t_wall *w)
{
	t_hit		hit;
	t_gate		*gate;
	double		det;

	det = ray.mat[0][0] * ray.mat[1][1] - ray.mat[1][0] * ray.mat[0][1];
	if (det > 0.00001 || det < 0.00001)
	{
		hit.pos.x = (ray.mat[2][0] * ray.mat[1][1] - ray.mat[2][1] * ray.mat[1][0]) / det;
		hit.pos.y = (ray.mat[0][0] * ray.mat[2][1] - ray.mat[2][0] * ray.mat[0][1]) / det;
		if (coherent_hit(ray, w, hit))
		{
			if (w->w_type == 0 && w->gate && !ray.gate_stop)
			{
				gate = (t_gate*)w->gate;
				ray.current_sector = gate->s_in == ray.current_sector ?
					gate->s_out : gate->s_in;
				ray.excluded_wall = gate->s_in == ray.current_sector ?
					gate->w_in->w_id : gate->w_out->w_id; 
				hit = cast_ray(ray);
			}
			else
			{
				hit.dist = sqrtf(powl(ray.origin.x - hit.pos.x, 2.0F)
							+ powl(ray.origin.y - hit.pos.y, 2.0F));
				hit.result = !w->gate ? 1 : 2;
				hit.type = w->w_type;
				hit.wall = w;
			}		
		}
		else
			hit.result = 0;
	}
	else
		hit.result = 0;
	return (hit);
}


t_hit		cast_ray(t_ray ray)
{
	t_hit		hit;
	int			i;
	t_wall		*w;

	ray.mat[0][0] = ray.direction.y;
	ray.mat[1][0] = -ray.direction.x;
	ray.mat[2][0] = -(ray.direction.x * ray.origin.y
						- ray.direction.y * ray.origin.x);
	hit.result = 0;
	i = 0;
	while (i < ray.map->sector[ray.current_sector]->wall_nbr && !hit.result)
	{
		if (i != ray.excluded_wall)
		{
			w = ray.map->sector[ray.current_sector]->wall[i];
			ray.mat[0][1] = (w->pos[1].y - w->pos[0].y) / (w->pos[1].x - w->pos[0].x);
			ray.mat[1][1] = -1.0F;
			ray.mat[2][1] =	(w->pos[0].y - ray.mat[0][1] * w->pos[0].x) * -1.0F;				
			if (fabs(w->pos[0].x - w->pos[1].x) < 0.00001F)
			{
				ray.mat[0][1] = 1.0F;
				ray.mat[1][1] = 0.0F;
				ray.mat[2][1] = w->pos[0].x;
			}
			hit = intersection(ray,
					ray.map->sector[ray.current_sector]->wall[i]);
			
		}
		i++;
	}
	return (hit);
}