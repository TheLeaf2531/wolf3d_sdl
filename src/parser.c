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

#include "../include/wolf.h"


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
;	while (i < s->wall_nbr)
	{
		get_next_line(fd, &l);
		if (l[0] != 'W')
			return (NULL);
		tab = ft_strsplit(l, ' ');
		s->wall[i]->pos[0] = (t_vector2f){(float)SDL_atof(tab[1]), (float)SDL_atof(tab[2])};
		s->wall[i]->pos[1] = (t_vector2f){(float)SDL_atof(tab[3]), (float)SDL_atof(tab[4])};
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
	if (get_next_line(fd, &l) <= 0)
		return (NULL);
	if (l[0] != 'G')
		return (NULL);
	tab = ft_strsplit(l, ' ');
	g->s_in = ft_atoi(tab[1]);
	g->w_in = m->sector[g->s_in]->wall[ft_atoi(tab[2])];
	g->w_in->gate = g;
	g->s_out = ft_atoi(tab[3]);
	g->w_out = m->sector[g->s_out]->wall[ft_atoi(tab[4])];
	g->w_out->gate = g;
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
		read_error("m malloc failed | init_map");
	get_next_line(fd, &l);
	tab = ft_strsplit(l, ' ');
	if ((m->s_nbr = ft_atoi(tab[0])) <= 0)
		return(NULL);
	m->gate_nbr = ft_atoi(tab[1]);
	if (!(m->sector = ft_memalloc(sizeof(t_sector*) * m->s_nbr)))
		read_error("m->sector malloc failed | init_map");
	if (!(m->gate = ft_memalloc(sizeof(t_gate*) * m->gate_nbr)))
		read_error("m->sector malloc failed | init_map");
	while (i < m->s_nbr)
	{
		if (!(m->sector[i] = parse_sector(fd)))
			read_error("Parsing Error : sector");
		i++;
	}
	i = 0;
	while (i < m->gate_nbr)
	{
		if (!(m->gate[i] = parse_gate(fd, m)))
			read_error("Parsing Error : gate");
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
		read_error("Cannot open file");
	if(!(m = init_map(fd)))
		read_error("Cannot init map");
	if (!(e->p = init_player(fd)))
		read_error("Cannot init player");
	e->m = m;
	return (e);
}