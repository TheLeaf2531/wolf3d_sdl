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

static	int			is_coherent(t_player *p, t_wall *w, t_hit hit)
{
	int			is_legit;
	t_vector2f	ran_x;
	t_vector2f	ran_y;

	ran_x.x = w->pos[0].x < w->pos[1].x ? w->pos[0].x : w->pos[1].x;
	ran_x.y = w->pos[0].x < w->pos[1].x ? w->pos[1].x : w->pos[0].x;
	ran_y.x = w->pos[0].y < w->pos[1].y ? w->pos[0].y : w->pos[1].y;
	ran_y.y = w->pos[0].y < w->pos[1].y ? w->pos[1].y : w->pos[0].y;
	is_legit = 1;
	if ((p->raydir.x < 0 && hit.pos.x > p->pos.x) || (p->raydir.y < 0 && hit.pos.y > p->pos.y)
		|| (p->raydir.x > 0 && hit.pos.x < p->pos.x) || (p->raydir.y > 0 && hit.pos.y < p->pos.y))
	{
		/*if (w->w_type == 3)
			printf("			The hit is not on the correct direction\n");*/
		is_legit = 0;
	}
	else if (hit.pos.x < ran_x.x || hit.pos.x > ran_x.y)
	{
		/*if (w->w_type == 3)
			printf("			The hit is not in the x range of the wall\n");*/
		is_legit = 0;
	}
	else if (hit.pos.y < ran_y.x || hit.pos.y > ran_y.y)
	{
		/*if (w->w_type == 3)
			printf("			The hit is not in the y range of the wall\n");*/
		is_legit = 0;
	}
	return (is_legit);	
}

static t_hit		intersection(int c_sector, t_wall *w,
							double mat[3][2], t_player *p, t_map *m) 
{
	t_hit		hit;
	t_gate		*g;

	if (w->pos[0].x != w->pos[1].x)
	{
		mat[0][1] = (w->pos[1].y - w->pos[0].y) / (w->pos[1].x - w->pos[0].x);
		mat[1][1] = -1;
		mat[2][1] = -(w->pos[0].y - mat[0][1] * w->pos[0].x);
	}
	else
	{
	//	printf("		X egals\n");
		mat[0][1] = 1;
		mat[1][1] = 0;
		mat[2][1] = w->pos[0].x;

	}
	if (mat[0][0] * mat[1][1] - mat[1][0] * mat[0][1] != 0)
	{
		//printf("	Matrice : \n");
		//printf("		%f   %f   %f\n",mat[0][0],mat[1][0],mat[2][0]);
		//printf("		%f   %f   %f\n",mat[0][1],mat[1][1],mat[2][1]);

		hit.pos.x = (mat[2][0] * mat[1][1] - mat[2][1] * mat[1][0]) /
			(mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0]);

		hit.pos.y = (mat[0][0] * mat[2][1] - mat[2][0] * mat[0][1] ) /
			(mat[0][0] * mat[1][1] - mat[1][0] * mat[0][1]);

		//printf("			Pos          : {%f ; %f}\n",hit.pos.x,hit.pos.y);
		if (!is_coherent(p, w, hit))
		{
			hit.wall_hit = 0;
			return (hit);
		}
		if (w->w_type == 0 && w->gate != NULL)
		{
			//printf("Hit on gate\n");
			g = (t_gate*)w->gate;
			c_sector = g->s_in == c_sector ? g->s_out : g->s_in;
			hit = cast_ray(p, m, c_sector, g->s_in == c_sector ? g->w_in->w_id : g->w_out->w_id);
		}
		else
		{
			//if (w->w_type == 3)
			//	printf("Hit on type %d\n", w->w_type);
 			hit.dist = sqrtl(powl(p->pos.x - hit.pos.x, 2) + powf(p->pos.y - hit.pos.y, 2));
			//if (w->w_type == 3)
			//	printf("With a dist of %f\n", hit.dist);
			hit.dist = hit.dist * ((p->rot.x * p->raydir.x + p->rot.y * p->raydir.y) / (hypotl(p->rot.x, p->rot.y) * hypotl(p->raydir.x, p->raydir.y)));
			hit.wall_hit = 1;
			hit.type = w->w_type;
		}
	}
	else
		hit.wall_hit = 0;
	return (hit);
}

/**
	t_gate *g;
				g = (t_gate*)(m->sector[i]->wall[y]->gate);
				printf("		This wall is linked to :\n");
				if (g->s_in == m->sector[i]->sector_id)
					printf("			Sector %d, Wall %d\n", g->s_out, g->w_out->w_id);
				else
					printf("			Sector %d, Wall %d\n", g->s_in, g->w_in->w_id);
 **/
t_hit		cast_ray(t_player *p, t_map *m, int c_sector, int ex)
{
	double			mat[3][2];
	int 			i;
	t_hit			hit;

	mat[0][0] =  p->raydir.y;
	mat[1][0] = -p->raydir.x;
	mat[2][0] = -(p->raydir.x * p->pos.y - p->raydir.y * p->pos.x);
	hit.wall_hit = 0;
	i = 0;
	//printf("Test in sector : %d\n", c_sector);
	while (i < m->sector[c_sector]->wall_nbr && !hit.wall_hit)
	{
		
		if (i != ex)
		{
	//		printf("	Test intersection with wall : %i\n", i);
			hit = intersection(c_sector, m->sector[c_sector]->wall[i], mat, p, m);
		}
		i++;
	}
	return (hit);
}