/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vboissel <vboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/22 00:02:44 by vboissel          #+#    #+#             */
/*   Updated: 2019/02/22 00:02:44 by vboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WOLF_H
# define WOLF_H

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <math.h>
#include "types.h"
#include "libft.h"


#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

t_env			*init_env(t_vector2i s);
t_env			*load_file(char *filename, t_env *e);
t_player		*init_player(int fd);
t_hit           cast_ray(t_player *p, t_map *m, int c_sector, int ex);
int				render_frame(t_env  *e, t_vector2i s);
#endif