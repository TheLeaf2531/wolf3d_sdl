/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vboissel <vboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/28 20:11:38 by vboissel          #+#    #+#             */
/*   Updated: 2019/02/28 20:11:38 by vboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

void		move_player(t_env *e, t_player *p, float delta_time)
{
	t_vector2d	mvmnt;
	t_vector2d  mv_tmp;
	t_vector2d	d;
	t_hit		hit;
	float 		a;

	mv_tmp = p->mvmt;
	//printf("p->mvmnt {%f ; %f}\n", p->mvmt.x, p->mvmt.y);
	if (p->mvmt.y != 0)
	{
		mvmnt.x = p->speed * delta_time * p->mvmt.y * p->rot.x + p->pos.x;
		mvmnt.y = p->speed * delta_time * p->mvmt.y * p->rot.y + p->pos.y;
	}
	if (p->mvmt.x != 0)
	{
		d.x = p->rot.x  * cos(degreesToRadians(p->mvmt.x * 90)) - p->rot.y * sin(degreesToRadians(p->mvmt.x * 90));
		d.y = p->rot.x  * sin(degreesToRadians(p->mvmt.x * 90)) + p->rot.y * cos(degreesToRadians(p->mvmt.x * 90));
		mvmnt.x = p->speed * delta_time  * d.x + p->pos.x;
		mvmnt.y = p->speed * delta_time  * d.y + p->pos.y;
	}
	a = atan2f(p->rot.y - p->mvmt.y, p->rot.x - p->mvmt.x);
	p->mvmt.x = cosf(a * p->pos.x) - sinf(a * p->pos.y);
	p->mvmt.y = sinf(a * p->pos.x) + cosf(a * p->pos.y);
	hit = cast_ray(p, e->m, p->mvmt, p->c_sector, -1);
	p->mvmt = mv_tmp;
	mvmnt.x = (mvmnt.x < hit.pos.x) == (p->pos.x < hit.pos.x) ? mvmnt.x : p->pos.x;
	mvmnt.y = (mvmnt.y < hit.pos.y) == (p->pos.y < hit.pos.y) ? mvmnt.y : p->pos.y;
	p->pos = mvmnt;
	//printf("mvmnt {%f ; %f}\n", mvmnt.x, mvmnt.y);
	/*
if (mvmnt.x > 0 && mvmnt.y > 0
		&& mvmnt.x < (float)e->map->size.x && mvmnt.y < (float)e->map->size.y)
	{
		if ((int)e->player->pos.x > (int)mvmnt.x)
			if (e->map->c[(int)mvmnt.x][(int)mvmnt.y].w_id[0] != 0)
				mvmnt.x = e->player->pos.x;
		if ((int)e->player->pos.x < (int)mvmnt.x)
			if (e->map->c[(int)mvmnt.x][(int)mvmnt.y].w_id[2] != 0)
				mvmnt.x = e->player->pos.x;
		if ((int)e->player->pos.y > (int)mvmnt.y)
			if (e->map->c[(int)mvmnt.x][(int)mvmnt.y].w_id[1] != 0)
				mvmnt.y = e->player->pos.y;
		if ((int)e->player->pos.y < (int)mvmnt.y)
			if (e->map->c[(int)mvmnt.x][(int)mvmnt.y].w_id[3] != 0)
				mvmnt.y = e->player->pos.y;
		e->player->pos = mvmnt;
	}*/
}

t_player	*rotate_player(t_player *p, int left, float delta_time)
{
	t_vector2d	new_rotation;
	t_vector2d	new_plane;
	double		speed;

	speed = p->r_speed * delta_time * left;
	new_rotation = (t_vector2d){
		p->rot.x * cos(speed) - p->rot.y * sin(speed),
		p->rot.x * sin(speed) + p->rot.y * cos(speed)
	};
	new_plane = (t_vector2d){
		p->plane.x * cos(speed) - p->plane.y * sin(speed),
		p->plane.x * sin(speed) + p->plane.y * cos(speed)
	};
	p->rot = new_rotation;
	p->plane = new_plane;
	p->rotate = 0;
	return (p);
}

t_player	*init_player(int fd)
{
	t_player	*p;
	char		*l;
	char		**tab;

	if (!(p = ft_memalloc(sizeof(t_player))))
		return (NULL);
	get_next_line(fd, &l);
	if (l[0] != 'P')
		return (NULL);
	tab = ft_strsplit(l, ' ');
	p->c_sector = ft_atoi(tab[1]);
	p->pos = (t_vector2d) {(float)ft_atof(tab[2]), (float)ft_atof(tab[3])};
	p->rot = (t_vector2d) {(float)atof(tab[4]), (float)atof(tab[5])};
	p->plane =(t_vector2d) {(float) 0, (float)0.66};
	p->speed = 3.01;
	p->r_speed = 0.0174533*10;
	return (p);
}