/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sector.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vboissel <vboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/27 16:21:41 by vboissel          #+#    #+#             */
/*   Updated: 2019/03/29 17:36:50 by vboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/wolf.h"


static int		init_sector(t_map *map, int current_sector,  t_mapLine *line)
{
	t_sector	*sector;
	int			i;

	if (!(sector = ft_memalloc(sizeof(t_sector))))
		return(0);
	sector->sector_id = ft_atoi(line->fields[1]);
	sector->wall_nbr = ft_atoi(line->fields[2]);
	if (!(sector->wall = ft_memalloc(sizeof(t_wall*) * sector->wall_nbr)))
		return (0);
	i = 0;
	while (i < sector->wall_nbr)
	{
		if (!(sector->wall[i] = ft_memalloc(sizeof(t_wall))))
			return (0);
		i++;
	}
	map->sector[current_sector] = sector;
	return (1);
}

static int		add_wall(t_sector	*sector, int current_wall, t_mapLine *line)
{
	t_vector2f	pos[2];

	if (line->fields_nbr != 6)
		return (0);
	pos[0] = (t_vector2f){(float)ft_atof(line->fields[1]),
							(float)ft_atof(line->fields[2])};
	pos[1] = (t_vector2f){(float)ft_atof(line->fields[3]),
							(float)ft_atof(line->fields[4])};
	sector->wall[current_wall]->pos[0] = pos[0];
	sector->wall[current_wall]->pos[1] = pos[1];
	sector->wall[current_wall]->w_type = ft_atoi(line->fields[5]);
	sector->wall[current_wall]->w_id = current_wall;
	return (0); 
}

t_list			*parse_sectors(t_map *map, t_list *node)
{
	t_mapLine	*line;
	int			current_sector;
	int			current_wall;

	current_wall = 0;
	current_sector = 0;
	while (node)
	{
		line = node->content;
		if (line->fields[0][0] == 'G' || line->fields[0][0] == 'P')
			return (node);
		if (line->fields[0][0] == 'E')
		{
			current_sector++;
			current_wall = 0;
		}
		if (line->fields[0][0] == 'S')
			init_sector(map, current_sector, line);
		if (line->fields[0][0] == 'W')
		{
			add_wall(map->sector[current_sector], current_wall, line);
			current_wall++;
		}
		node = node->next;
	}
	return (NULL);
}