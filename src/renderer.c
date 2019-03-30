/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vboissel <vboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/28 20:26:26 by vboissel          #+#    #+#             */
/*   Updated: 2019/02/28 20:26:26 by vboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/wolf.h"

static void	set_pixel(t_texture *tex, Uint32 c, t_vector2i p)
{
    Uint32 *pixels  = tex->pixels; 
    //Uint32 mapped_color = SDL_MapRGBA(tex->format, c.r, c.g, c.b, c.a);
    pixels[p.y * tex->s.x + p.x] = c;
}
/*
static void vert_line(t_texture *tex, t_line l, t_rgba c)
{
	int			y;
	t_rgba		sk;
	t_rgba		fl;

	sk = (t_rgba){(Uint8)150, (Uint8)150, (Uint8)255, (Uint8)255};
	fl = (t_rgba){(Uint8)50, (Uint8)50, (Uint8)50, (Uint8)255};
	y = 0;
	while (y < l.dr.x)
	{
		set_pixel(tex, sk, (t_vector2i){(int)l.x, y});
		y++;
	}
	while (y < l.dr.y)
	{
		set_pixel(tex, c, (t_vector2i){(int)l.x, y});
		y++;
	}
	while (y < tex->s.y)
	{
		set_pixel(tex, fl, (t_vector2i){(int)l.x, y});
		y++;
	}

}*/

static t_rgba	get_color(int type)
{
	t_rgba		c;

	if (type == 1)
		c = (t_rgba){(Uint8)120, (Uint8)120, (Uint8)120, (Uint8)255};
	else
		c = (t_rgba){(Uint8)120, (Uint8)130, (Uint8)0, (Uint8)255};
	return (c);
}

// https://guy-grave.developpez.com/tutoriels/jeux/doom-wolfenstein-raycasting/

static void		wall_casting(t_env *e, int size, t_hit **buff)
{
	t_ray		ray;
	float		cam_x;
	int			x;
	
	ray = set_ray(e->m, e->p->pos, e->p->rot, e->p->c_sector, 0);
	x = -1;
	while (x++ < size)
	{
		cam_x = 2 * x / (float)size - (float)1;
		ray.direction = (t_vector2f) {(float)(e->p->rot.x + e->p->plane.x * cam_x),
									(float)(e->p->rot.y + e->p->plane.y * cam_x)};
		(*buff)[x] = cast_ray(ray);
		if ((*buff)[x].result)
		{
			(*buff)[x].dist *= ((e->p->rot.x * ray.direction.x + e->p->rot.y
				* ray.direction.y) / (hypotl(e->p->rot.x, e->p->rot.y)
				* hypotl(ray.direction.x, ray.direction.y)));
		}
	}
}

static void				draw_column(float dist, t_line line, t_vector2i s, SDL_Surface *surface, t_texture *tex)
{
	t_vector2i	scr_coord;
	t_vector2i	sur_coord;
	t_rgba		fl;
	t_rgba		sk;
	Uint32 		*pixels;


	scr_coord = (t_vector2i){line.x, (int)0};
	sur_coord.x = (int)((dist - (int)dist) / (1 / surface->w));
	sk = (t_rgba){(Uint8)150, (Uint8)150, (Uint8)255, (Uint8)255};
	fl = (t_rgba){(Uint8)50, (Uint8)50, (Uint8)50, (Uint8)255};
	pixels = surface->pixels;
	printf ("Dist : %f", dist);
	printf("sur_coord.x %d\n", sur_coord.x);
	/*
		Uint32 *pixels  = tex->pixels; 
		Uint32 mapped_color = SDL_MapRGBA(tex->format, c.r, c.g, c.b, c.a);
		pixels[p.y * tex->s.x + p.x] = mapped_color;
	*/
	/*
	while (scr_coord.y < s.y)
	{
		if (scr_coord.y < line.dr.x)
		{

		}
		else if (scr_coord.y >= line.dr.x && scr_coord.y <= line.dr.y)
		{
			sur_coord.y = scr_coord.y * (1 / line.dr.x);
			printf("sur_coord {%d ; %d}\n", sur_coord.x, sur_coord.y);
			//printf("%d", pixels[sur_coord.y * surface->w + sur_coord.y]);
			set_pixel(tex, pixels[sur_coord.y * surface->w + sur_coord.y], scr_coord);
		}
		else
		{
			
		}
		scr_coord.y++;
	}
	*/
}

static t_texture		*draw_walls(t_env *e, t_vector2i s, t_hit *buff)
{
	t_texture	*tex;
    t_line		l;
	int			x;

	x = 0;
	if (!(tex = create_texture(e, s)))
		return (NULL);
	get_color(0);
	while (x < s.x)
	{
		if (buff[x].dist >= 0)
		{
			l.x = x;
			l.h = (int)(s.y / buff[x].dist);
			//l.dr.x = (double)-l.h / 2 + s.y / 2 < 0 ? 0 : (double)-l.h / 2 + s.y / 2;
			l.dr.x = (double)-l.h / 2 + s.y / 2;
			//l.dr.y = (double)l.h / 2 + s.y / 2 >= s.y ? s.y - 1 : (double)l.h / 2 + s.y / 2;
			l.dr.y = (double)l.h / 2 + s.y / 2;
			draw_column(distance(buff[x].wall->pos[0],
						(t_vector2f){(float)buff[x].pos.x, (float)buff[x].pos.y}),
						l,
						s,
						find_surface(e->ressources ,buff[x].type), tex);
		}
		x++;
	}
	return (tex);
}

int			render_frame(t_env  *e, t_vector2i s)
{
	t_hit		*wall_buffer;
	t_texture	*tex;
	int			i;

	wall_buffer = ft_memalloc(sizeof(t_hit) * s.x);
	i = 0;
	wall_casting(e, s.x, &wall_buffer);
	if (SDL_RenderClear(e->r))
		read_error(SDL_GetError());
	/*
	tex = draw_walls(e, s, wall_buffer);
	SDL_FreeFormat(tex->format);
	SDL_UnlockTexture(tex->tex);
	if (SDL_RenderCopy(e->r, tex->tex, NULL, NULL) == -1)
		read_error(SDL_GetError());
	SDL_RenderPresent(e->r);
	*/
	free(wall_buffer);
	return (0);
}
