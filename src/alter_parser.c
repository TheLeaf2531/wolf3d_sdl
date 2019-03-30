/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alter_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vboissel <vboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/21 23:02:57 by vboissel          #+#    #+#             */
/*   Updated: 2019/03/29 17:36:13 by vboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/wolf.h"


static int			add_line_node(char *line, t_mapfile *map)
{
	t_list		*tmp_node;
	t_list		*new_node;
	t_mapLine	*line_parsed;

	if (!(line_parsed = ft_memalloc(sizeof(t_mapLine))))
		return (0);
	line_parsed->number = map->line_number;
	line_parsed->fields_nbr = elem_nbrs(line, ' ');
	if (!(line_parsed->fields = ft_strsplit(line, ' ')))
		return (0);
	if (!(new_node = ft_memalloc(sizeof(t_list))))
		return (0);
	new_node->content_size = sizeof(t_mapLine);
	new_node->content = line_parsed;
	if (map->list == NULL)
		map->list = new_node;
	else
	{
		tmp_node = map->list;
		while (tmp_node->next != NULL)
			tmp_node = tmp_node->next;
		tmp_node->next = new_node;
	}
	return (1);
}

static t_mapfile			*load_map(char *filename)
{
	int			fd;
	t_mapfile	*file;
	char		*line;

	if ((fd = open(filename, O_RDONLY)) <= 0)
		read_error("Cannot open map file.");
	if (!(file = ft_memalloc(sizeof(t_mapfile))))
		read_error("Memory allocation of file failed");
	file->line_number = 0;
	while (get_next_line(fd, &line) > 0)
	{
		file->line_number += 1;
		if (!add_line_node(line, file))
			read_error("Error while reading line");
		free(line);
	}
	return (file);
}

static void 				free_mapfile(t_mapfile		*file)
{
	t_list		*current;
	t_mapLine	*line;
	int			i;

	while (file->list)
	{
		current = file->list;
		line = current->content;
		i = 0;
		while (i < line->fields_nbr)
		{
			free(line->fields[i]);
			i++;
		}
		free(line->fields);
		free(current);
		file->list = file->list->next;
	}
	free(file);
}

t_map				*parse_map_alter(char	*filename, t_env *e)
{
	t_mapfile	*file;
	t_list		*node;
	t_mapLine	*line;
	t_map		*map;

	if (!(file = load_map(filename)))
		read_error("File loading failed");
	node = file->list;
	line = node->content;
	if (!(map = ft_memalloc(sizeof(t_map))))
		read_error("Memory allocation of map failed");
	map->s_nbr = ft_atoi(line->fields[0]);
	map->sector = ft_memalloc(sizeof(t_sector*) * map->s_nbr);
	map->gate_nbr = ft_atoi(line->fields[1]);
	map->gate = ft_memalloc(sizeof(t_gate*) * map->gate_nbr);
	node = node->next;
	if (!(node = parse_sectors(map, node)))
		read_error("Error while parsing sectors");
	if (!(node = parse_gates(map, node)))
		read_error("Error while parsing gates");
	if (!(e->p = init_player_alter(node)))
		read_error("Error while player info");
	e->m = map;
	free_mapfile(file);
	return (map);
}
