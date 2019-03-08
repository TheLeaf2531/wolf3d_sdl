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


t_player	*rotate_player(t_player *p, int left, float delta_time)
{
	t_vector2d	new_rotation;
	t_vector2d	new_plane;
	double		speed;

	speed = p->r_speed * delta_time * left;
	//printf("Speed : %f\n", speed);
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
	//printf("Player :\n");
	//printf("			Current sector	: %d\n", p->c_sector);
	//printf("			Pos             : {%f ; %f}\n", p->pos.x, p->pos.y);
	//printf("			Rot             : {%f ; %f}\n", p->rot.x, p->rot.y);
	return (p);
}
/*
void		rotate_player(t_env	*e, int a)
{
	t_vector2f	new_rotation;
	float		speed;

	speed = degreesToRadians(PLAYER_ROTATION_SPEED * a);
	new_rotation = (t_vector2f){
		(float)e->player->dir.x * cos(speed) - e->player->dir.y * sin(speed),
		(float)e->player->dir.x * sin(speed) + e->player->dir.y * cos(speed)
	};
	e->player->dir = new_rotation;
	new_rotation = (t_vector2f){
		(float)e->player->canvas_plane.x * cos(speed) - e->player->canvas_plane.y * sin(speed),
		(float)e->player->canvas_plane.x * sin(speed) + e->player->canvas_plane.y * cos(speed)
	};
	e->player->canvas_plane = new_rotation;
}%
*/

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
	p->r_speed = 0.0174533*10;
	//printf("Player :\n");
//	printf("			Current sector	: %d\n", p->c_sector);
//	printf("			Pos             : {%f ; %f}\n", p->pos.x, p->pos.y);
	//printf("			Rot             : {%f ; %f}\n", p->rot.x, p->rot.y);
	return (p);
}