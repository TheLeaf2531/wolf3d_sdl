/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alter_renderer.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vboissel <vboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/29 18:15:41 by vboissel          #+#    #+#             */
/*   Updated: 2019/03/30 19:53:13 by vboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/wolf.h"



static t_hit		**malloc_buffer(int size)
{
	t_hit		**hit_buffer;
	int			i;

	i = 0;
	if (!(hit_buffer = ft_memalloc(sizeof(t_hit) * size)))
		return (NULL);
	while (i < size)
	{
		if (!(hit_buffer[i] = ft_memalloc(sizeof(t_hit))))
			return (NULL);
		i++;
	}
	return (hit_buffer);
}


static void			**free_buffer(t_hit **hit_buffer, int size)
{
	int		i;

	i = 0;
	while (i < size)
	{
		free(hit_buffer[i]);
		i++;
	}
	free(hit_buffer);
}

static t_hit	*set_hit(t_hit hit, t_hit **buffer, int x)
{
	buffer[x]->dist = hit.dist;
	buffer[x]->pos = hit.pos;
	buffer[x]->result = hit.result;
	buffer[x]->type = hit.type;
	buffer[x]->wall = hit.wall;
	return (buffer[x]);
}

static t_hit	**wall_casting(t_env *e, int width, t_hit **hit_buff)
{
	t_ray		ray;
	t_hit		tmp;
	float		cam_x;
	int			x;

	x = 0;
	ray = set_ray(e->m, e->p->pos, e->p->rot, e->p->c_sector, 0);
	while (x < width)
	{
		cam_x = 2 * x / (float)width - (float)1;
		ray.direction = (t_vector2f) {(float)(e->p->rot.x + e->p->plane.x * cam_x),
									(float)(e->p->rot.y + e->p->plane.y * cam_x)};		
		tmp = cast_ray(ray);
		if (tmp.result)
		{
			tmp.dist *= ((e->p->rot.x * ray.direction.x + e->p->rot.y
				* ray.direction.y) / (hypotl(e->p->rot.x, e->p->rot.y)
				* hypotl(ray.direction.x, ray.direction.y)));
		}
		else
			tmp.dist = -1.0F;
		hit_buff[x] = set_hit(tmp, hit_buff, x);
		x++;
	}
	return (hit_buff);
}

static void	set_pixel(t_texture *tex, Uint32 c, t_vector2i p)
{
    Uint32 *pixels  = tex->pixels;
    //Uint32 mapped_color = SDL_MapRGBA(tex->format, c.r, c.g, c.b, c.a);
	//printf ("pop\n");
    pixels[p.y * tex->s.x + p.x] = c;
}

static void				draw_column(float dist, t_line line, t_vector2i s, SDL_Surface *surface, t_texture *tex)
{
	t_vector2i	scr_coord;
	t_vector2i	sur_coord;
	t_vector2f	wall_unit;
	t_vector2i	clamped_ln;
	uint32		*pixels;

	wall_unit.x = (dist - (int)dist);
	sur_coord.x =  (int)(wall_unit.x * (float)surface->w);
	scr_coord = (t_vector2i){line.x, (int)0};
	pixels = surface->pixels;
	clamped_ln.x = (double)-line.h / 2 + s.y / 2 < 0 ? 0 : (double)-line.h / 2 + s.y / 2;
	clamped_ln.y = (double)line.h / 2 + s.y / 2 >= s.y ? s.y - 1 : (double)line.h / 2 + s.y / 2;
	while (scr_coord.y < s.y)
	{
		if (scr_coord.y < clamped_ln.x)
		{
			uint32 pix  = SDL_MapRGBA(tex->format, 100, 100, 255, 255);
			set_pixel(tex, pix, scr_coord);
		}
		else if (scr_coord.y >= clamped_ln.x && scr_coord.y < clamped_ln.y)
		{
			wall_unit.y = (float)((float)(scr_coord.y - line.dr.x) * (1.0F / (float)(line.dr.y - line.dr.x)));
			sur_coord.y = (int)(wall_unit.y / (1.0F / (float)surface->h));
			set_pixel(tex, pixels[sur_coord.y * surface->w + sur_coord.x], scr_coord);
		}
		else
		{
			uint32 pix  = SDL_MapRGBA(tex->format, 100, 100, 100, 255);
			set_pixel(tex, pix, scr_coord);
		}
		scr_coord.y++;
	}
	
}

static t_texture		*draw_walls(t_env *e, t_vector2i size, t_hit **hit_buffer)
{
	t_texture	*texture;
    t_line		l;
	int			x;

	x = 0;
	if (!(texture = create_texture(e, size)))
		return (NULL);
	while (x < size.x)
	{
		if (hit_buffer[x]->dist >= 0)
		{
			l.x = x;
			l.h = (int)(size.y / hit_buffer[x]->dist);
			//l.dr.x = (double)-l.h / 2 + s.y / 2 < 0 ? 0 : (double)-l.h / 2 + s.y / 2;
			l.dr.x = (double)-l.h / 2 + size.y / 2;
			//l.dr.y = (double)l.h / 2 + s.y / 2 >= s.y ? s.y - 1 : (double)l.h / 2 + s.y / 2;
			l.dr.y = (double)l.h / 2 + size.y / 2;
			draw_column(distance(hit_buffer[x]->wall->pos[0],
						(t_vector2f){(float)hit_buffer[x]->pos.x, (float)hit_buffer[x]->pos.y}),
						l,
						size,
						find_surface(e->ressources ,hit_buffer[x]->type), texture);
		}
		x++;
	}
	return (texture);
}


int				alter_render_frame(t_env *e, t_vector2i size)
{
	t_hit		**hit_buffer;
	t_texture	*texture;


	if (!(hit_buffer = malloc_buffer(size.x)))
		return (0);
	if (SDL_RenderClear(e->r))
		read_error(SDL_GetError());
	wall_casting(e, size.x, hit_buffer);
	texture = draw_walls(e, size, hit_buffer);
	SDL_FreeFormat(texture->format);
	SDL_UnlockTexture(texture->tex);
	if (SDL_RenderCopy(e->r, texture->tex, NULL, NULL) == -1)
		read_error(SDL_GetError());
	SDL_RenderPresent(e->r);
	SDL_DestroyTexture(texture->tex);
	free(texture);
	free_buffer(hit_buffer, size.x);
	return (1);
}