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

# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <math.h>
# include <SDL2/SDL.h>
# include "../libft/libft.h"
# include "../include/types.h"

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
void			read_error(const char *error);
//t_mapfile		*load_map(char *filename);
size_t			elem_nbrs(char const *s, char c);

void			update_event(t_env *e);
t_texture		*create_texture(t_env *e, t_vector2i s);

t_map			*parse_map_alter(char *filename, t_env *e);
t_list			*parse_sectors(t_map *map, t_list *node);
t_list			*parse_gates(t_map *map, t_list *node);
t_player		*init_player_alter(t_list	*node);

SDL_Surface		*find_surface(t_ressources	*ressources, int texture_id);

int				alter_render_frame(t_env *e, t_vector2i size);

int				evaluate_distance(t_vector2f o, t_vector2f p_1, t_vector2f p_2);
float			distance(t_vector2f a, t_vector2f b);


#endif