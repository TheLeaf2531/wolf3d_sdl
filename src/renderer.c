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

#include "wolf.h"

static void	set_pixel(t_texture *tex, t_rgba c, t_vector2i p)
{
    Uint32 *pixels  = tex->pixels; 
    Uint32 mapped_color = SDL_MapRGBA(tex->format, c.r, c.g, c.b, c.a);
    pixels[p.y * tex->s.x + p.x] = mapped_color;
}

static void vert_line(t_texture *tex, t_line l, t_rgba c)
{
	int		y;\
	y = l.dr.x;
	while (y < l.dr.y)
	{
		set_pixel(tex, c, (t_vector2i){(int)l.x, y});
		y++;
	}
}

static t_rgba	get_color(t_hit hit)
{
	t_rgba		c;

	if (hit.type == 1)
		c = (t_rgba){(Uint8)120, (Uint8)120, (Uint8)120, (Uint8)255};
	else
		c = (t_rgba){(Uint8)120, (Uint8)130, (Uint8)0, (Uint8)255};
	return (c);
}

static t_texture	*create_texture(t_env *e, t_vector2i s)
{
	t_texture	*tex;

	if (!(tex = ft_memalloc(sizeof(t_texture))))
		read_error("Counldn't malloc texture in create_texture");
	tex->tex = SDL_CreateTexture(e->r, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, s.x, s.y);
	if (tex->tex == NULL)
		read_error(SDL_GetError());
	tex->format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
	if (SDL_LockTexture(tex->tex, NULL, &(tex->tmp), &(tex->pitch)) == -1)
		read_error(SDL_GetError());
	tex->pixels = (Uint32 *)tex->tmp;
	if (tex->pixels == NULL)
		read_error("Couldn't get pixels from texture in create_texture");	
	tex->s = s;
	return (tex);
}

// https://guy-grave.developpez.com/tutoriels/jeux/doom-wolfenstein-raycasting/

int			render_frame(t_env  *e, t_vector2i s)
{
    float			x;
    float			camX;
    t_hit			hit;
    t_line			l;
    t_texture		*tex;
	t_ray			ray;
	
	x = 0;
	if (SDL_RenderClear(e->r))
		read_error(SDL_GetError());
	tex = create_texture(e, s);
	ray = set_ray(e->m, e->p->pos, e->p->rot, e->p->c_sector, 0);
	while (x < s.x)
	{
		camX = 2 * x / (float)s.x - (float)1;
		ray.direction = (t_vector2f) {(float)(e->p->rot.x + e->p->plane.x * camX),
									(float)(e->p->rot.y + e->p->plane.y * camX)};
		hit = cast_ray(ray);
		hit.dist *= ((e->p->rot.x * ray.direction.x + e->p->rot.y * ray.direction.y) /
			(hypotl(e->p->rot.x, e->p->rot.y) * hypotl(ray.direction.x, ray.direction.y)));
		if (hit.result)
		{
			l.x = x;
			l.h = (int)(s.y / hit.dist);
			l.dr.x = (double)-l.h / 2 + s.y / 2 < 0 ? 0 : (double)-l.h / 2 + s.y / 2;
			l.dr.y = (double)l.h / 2 + s.y / 2 >= s.y ? s.y - 1 : (double)l.h / 2 + s.y / 2;
			vert_line(tex, l, get_color(hit));
		}
		x++;
	}
	SDL_FreeFormat(tex->format);
	SDL_UnlockTexture(tex->tex);
	if (SDL_RenderCopy(e->r, tex->tex, NULL, NULL) == -1)
		read_error(SDL_GetError());
	SDL_RenderPresent(e->r);
	SDL_DestroyTexture(tex->tex);
	free(tex);
	return (0);
}
