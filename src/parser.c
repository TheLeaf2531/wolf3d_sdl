/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vboissel <vboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/25 18:35:25 by vboissel          #+#    #+#             */
/*   Updated: 2019/02/25 18:35:25 by vboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

static t_sector		*init_sector(int fd)
{
	char		*l;
	char		**tab;
	t_sector	*s;
	int			i;

	if (get_next_line(fd, &l) <= 0)
		return (NULL);
	if (l[0] != 'S')
		return (NULL);
	if (!(s = ft_memalloc(sizeof(t_sector))))
		return (NULL);
	if (!(tab = ft_strsplit(l, ' ')))
		return (NULL);
	s->sector_id = ft_atoi(tab[1]);
	s->wall_nbr = ft_atoi(tab[2]);
	if (!(s->wall = ft_memalloc(sizeof(t_wall*) * s->wall_nbr)))
		return(NULL);
	i = 0;
	while (i < s->wall_nbr)
	{
		if (!(s->wall[i] = ft_memalloc(sizeof(t_wall))))
			return(NULL);
		i++;
	}
	free(l);
	free(tab);
	return (s);
}

static t_sector	*parse_sector(int fd)
{
	char		*l;
	char		**tab;
	int			i;
	t_sector	*s;

	if (!(s = init_sector(fd)))
		return (NULL);
	i = 0;
	(void)tab
;	while (i < s->wall_nbr)
	{
		get_next_line(fd, &l);
		if (l[0] != 'W')
			return (NULL);
		tab = ft_strsplit(l, ' ');
		s->wall[i]->pos[0] = (t_vector2d){(double)ft_atof(tab[1]), (double)ft_atof(tab[2])};
		s->wall[i]->pos[1] = (t_vector2d){(double)ft_atof(tab[3]), (double)ft_atof(tab[4])};
		s->wall[i]->w_type = ft_atoi(tab[5]);
		s->wall[i]->w_id = i;
		free(l);
		free(tab);
		i++;
	}
	get_next_line(fd, &l);
	if (l[0] != 'E')
		return (NULL);
	free(l);
	return (s);
}

static t_gate	*parse_gate(int fd, t_map *m)
{
	t_gate		*g;
	char		*l;
	char		**tab;

	if (!(g = ft_memalloc(sizeof(t_gate))))
		return (NULL);
	//printf("	Alloc gate ok\n");
	get_next_line(fd, &l);
	if (l[0] != 'G')
		return (NULL);
	tab = ft_strsplit(l, ' ');

	g->s_in = ft_atoi(tab[1]);
	g->w_in = m->sector[g->s_in]->wall[ft_atoi(tab[2])];
	g->w_in->gate = g;
	//printf("	In gate ok\n");

	g->s_out = ft_atoi(tab[3]);
	g->w_out = m->sector[g->s_out]->wall[ft_atoi(tab[4])];
	g->w_out->gate = g;
	//printf("	Out gate ok\n");

	return (g);
}

static t_map	*init_map(int fd)
{
	int		i;
	char	*l;
	char	**tab;
	t_map	*m;

	i = 0;
	if (!(m = ft_memalloc(sizeof(t_map))))
		return(NULL);
	get_next_line(fd, &l);
	tab = ft_strsplit(l, ' ');
	if ((m->s_nbr = ft_atoi(tab[0])) <= 0)
		return(NULL);
	m->gate_nbr = ft_atoi(tab[1]);
	if (!(m->sector = ft_memalloc(sizeof(t_sector*) * m->s_nbr)))
		return (NULL);
	if (!(m->gate = ft_memalloc(sizeof(t_gate*) * m->gate_nbr)))
		return (NULL);	
		
	while (i < m->s_nbr)
	{
		//printf("Parse %d\n", i );
		if (!(m->sector[i] = parse_sector(fd)))
			return (NULL);
	//	printf("sec ok\n");
		i++;
	}
	i = 0;
	while (i < m->gate_nbr)
	{
		//printf("Parse %d\n", i );
		if (!(m->gate[i] = parse_gate(fd, m)))
			return (NULL);
		//printf("gate ok\n");		
		i++;
	}
	return (m);
}

t_env		*load_file(char *filename, t_env *e)
{
	int		fd;
	t_map	*m;

	fd = open(filename, O_RDONLY);
	if (fd <= 0)
		return (NULL);
	if(!(m = init_map(fd)))
		return (NULL);
	if (!(e->p = init_player(fd)))
		return (NULL);
	/*
	int i = 0;
	printf("Sector count : %d\n", m->s_nbr);
	while (i < m->s_nbr)
	{
		printf("	Sector : %d\n", i);
		printf("	Sector ID    : %d\n", m->sector[i]->sector_id);
		printf("	Wall numbers : %d\n", m->sector[i]->wall_nbr);
		int y = 0;
		while (y < m->sector[i]->wall_nbr)
		{
			printf("		id    : %d\n", m->sector[i]->wall[y]->w_id);
			printf("		type  : %d\n", m->sector[i]->wall[y]->w_type);
			printf("		start : %f %f\n", m->sector[i]->wall[y]->pos[0].x, m->sector[i]->wall[y]->pos[0].y);
			printf("		end   : %f %f\n", m->sector[i]->wall[y]->pos[1].x, m->sector[i]->wall[y]->pos[1].y);
			if (m->sector[i]->wall[y]->w_type == 0)
			{
				t_gate *g;
				g = (t_gate*)(m->sector[i]->wall[y]->gate);
				printf("		This wall is linked to :\n");
				if (g->s_in == m->sector[i]->sector_id)
					printf("			Sector %d, Wall %d\n", g->s_out, g->w_out->w_id);
				else
					printf("			Sector %d, Wall %d\n", g->s_in, g->w_in->w_id);
			}
			y++;
			printf("		-----\n");
		}
		i++;
	}
	*/
	e->m = m;
	return (e);
}