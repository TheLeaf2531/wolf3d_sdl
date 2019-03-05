/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vboissel <vboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/22 00:04:50 by vboissel          #+#    #+#             */
/*   Updated: 2019/02/22 00:04:50 by vboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_H
# define TYPES_H

# define WIDTH	1600
# define HEIGHT	900

typedef unsigned int	uint32;
typedef unsigned char	uint8;

typedef struct			s_vector2i
{
	int		x;
	int		y;
}						t_vector2i;

typedef struct			s_vector2f
{
	float	x;
	float	y;
}						t_vector2f;

typedef struct			s_vector2d
{
	double	x;
	double	y;
}						t_vector2d;

typedef struct			s_wall
{
	int			w_id;
	int			w_type;
	t_vector2f	pos[2];
	void		*gate;
}						t_wall;

typedef struct 			s_gate
{
	int			s_in;
	int			s_out;
	t_wall		*w_in;
	t_wall		*w_out;

}						t_gate;

typedef struct			s_sector
{
	int			sector_id;
	int 		wall_nbr;
	t_wall		**wall;
}						t_sector;

typedef struct			s_map
{
	int			s_nbr;
	int			gate_nbr;
	t_sector	**sector;
	t_gate		**gate;
}						t_map;

typedef struct			s_player
{
	int			c_sector;
	t_vector2f	pos;
	t_vector2f	rot;
	float		speed;
	float		r_speed;
	t_vector2f	plane;
	t_vector2f	raydir;
}						t_player;

typedef struct			s_rgba
{
	Uint8		r;
	Uint8		g;
	Uint8		b;
	Uint8		a;
}						t_rgba;

typedef struct			s_rayenv
{
	int			c_sector;
	int			first_run;
	int			ex_wall;
	t_player	*p;
	t_map		*m;
}						t_rayenv;

typedef struct			s_line
{
	int			h;
	int			x;
	t_vector2i	dr;
}						t_line;

typedef struct			s_hit
{
	int			wall_hit;
	double		dist;
	t_vector2f	pos;
	int			type;
}						t_hit;

typedef struct 			s_texture
{
	t_vector2i		s;
	SDL_Texture		*tex;
	void			*tmp;
	Uint32 			*pixels;
	SDL_PixelFormat *format;
	int				pitch;
}						t_texture;


typedef struct			s_env
{
	SDL_Window	*w;
	SDL_Renderer	*r;
	t_map		*m;
	t_player	*p;
}						t_env;
#endif