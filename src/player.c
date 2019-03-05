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
	p->pos = (t_vector2f) {(float)ft_atof(tab[2]), (float)ft_atof(tab[3])};
	p->rot = (t_vector2f) {(float)atof(tab[4]), (float)atof(tab[5])};
	p->plane =(t_vector2f) {(float) 0, (float)0.66};
	printf("Player :\n");
	printf("			Current sector	: %d\n", p->c_sector);
	printf("			Pos             : {%f ; %f}\n", p->pos.x, p->pos.y);
	printf("			Rot             : {%f ; %f}\n", p->rot.x, p->rot.y);
	return (p);
}