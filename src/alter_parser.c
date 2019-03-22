/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alter_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vboissel <vboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/21 23:02:57 by vboissel          #+#    #+#             */
/*   Updated: 2019/03/21 23:02:57 by vboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"


t_mapfile	*load_map(char *filename)
{
	int			fd;
	t_mapfile	*file;
	t_list		*node;
	t_mapLine	*current_line;
	char		*line;

	if ((fd = open(filename, O_RDONLY)) <= 0)
		read_error("Cannot open map file.");
	if (!(file = ft_memalloc(sizeof(t_mapfile))))
		read_error("Memory allocation of file failed");
	file->line_number = 0;
	while (get_next_line(fd, &line) > 0)
	{
		if(!(current_line = ft_memalloc(sizeof(t_mapLine))))
			read_error("Memory allocation of line failed");
		current_line->number = file->line_number;
		current_line->fields = ft_strsplit(line, ' ');
		current_line->fields_nbr = elem_nbrs(line, ' ');
		file->line_number += 1;
		node = ft_lstnew(current_line, sizeof(current_line));
		free(line);
	}
	return (NULL);
}

/*
static t_map		*parse_map(char	*filename)
{

}
*/