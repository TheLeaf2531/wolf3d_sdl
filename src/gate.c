/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gate.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vboissel <vboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/27 18:09:45 by vboissel          #+#    #+#             */
/*   Updated: 2019/03/29 17:36:24 by vboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/wolf.h"


static int		add_gate(t_map	*map, int current_gate, t_mapLine	*line)
{
	t_gate	*g;

	if (!(g = ft_memalloc(sizeof(t_gate))))
		return (0);
	g->s_in = ft_atoi(line->fields[1]);
	g->w_in	= map->sector[g->s_in]->wall[ft_atoi(line->fields[2])];
	g->w_in->gate = g;
	g->s_out = ft_atoi(line->fields[3]);
	g->w_out	= map->sector[g->s_out]->wall[ft_atoi(line->fields[4])];
	g->w_out->gate = g;
	map->gate[current_gate] = g;
	return (1);
}

t_list			*parse_gates(t_map *map, t_list *node)
{
	t_mapLine	*line;
	int			curr_gate;

	line = node->content;
	if (line->fields[0] && line->fields[0][0] == 'P')
		return (node);
	curr_gate = 0;
	while (node)
	{
		line = node->content;
		if (line->fields[0][0] == 'G' && line->fields_nbr != 5)
			return (0);
		else if (line->fields[0][0] == 'G')
		{
			add_gate(map, curr_gate, line);
			curr_gate++;
		}
		else if (line->fields[0][0] == 'P')
			return (node);
		else
			return (NULL);
		node = node->next;
	}
	return (NULL);
}