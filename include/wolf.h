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

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

# define degreesToRadians(angleDegrees) ((angleDegrees) * M_PI / 180.0)
# define radiansToDegrees(angleRadians) ((angleRadians) * 180.0 / M_PI)

t_env			*init_env(t_vector2i s);
t_env			*load_file(char *filename, t_env *e);
t_player		*init_player(int fd);
t_hit			cast_ray(t_ray ray);
t_ray			set_ray(t_map *map, t_vector2f origin,
						t_vector2f direction, int origin_sector,
						int gate_stop);
//t_hit			cast_ray(t_player *p, t_map *m, t_vector2d dir, int c_sector, int ex);
int				render_frame(t_env  *e, t_vector2i s);
t_player		*rotate_player(t_player *p, int left, float delta_time);
void			move_player(t_env *e, t_player *p, float delta_time);

#endif