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
	//printf("			p.y : %d  ; p.x : %d ; s->w :  %d\n", p.y, p.x, s->w);
    pixels[p.y * tex->s.x + p.x] = mapped_color;
}

static void vert_line(t_texture *tex, t_line l, t_rgba c)
{
	int		y;

	//printf("line x : %d, l.dr.x : %d, l.dr.y : %d\n", l.x, l.dr.x, l.dr.y);
	y = l.dr.x;
	while (y < l.dr.y)
	{
		set_pixel(tex, c, (t_vector2i){(int)l.x, y});
		y++;
	}
}

static t_rgba	get_color(t_hit hit)
{
	t_rgba c;

	if (hit.type == 1)
		c = (t_rgba){(Uint8)120, (Uint8)120, (Uint8)120, (Uint8)255};
	else
		c = (t_rgba){(Uint8)120, (Uint8)0, (Uint8)0, (Uint8)255};
	return (c);
}

static t_texture	*create_texture(t_env *e, t_vector2i s)
{
	t_texture	*tex;

	tex = ft_memalloc(sizeof(tex));
	tex->tex = SDL_CreateTexture(e->r, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, s.x, s.y);
	tex->format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
	SDL_LockTexture(tex->tex, NULL, &tex->tmp, &tex->pitch);
	tex->pixels = tex->tmp;
	tex->s = s;
	return (tex);
}

int			render_frame(t_env  *e, t_vector2i s)
{
    int				x;
    float			camX;
    t_hit			hit;
    t_line			l;
    t_texture		*tex;

	x = 0;
	tex = create_texture(e, s);
	//sur = SDL_CreateRGBSurfaceWithFormat(0, s.x, s.y, 32, SDL_PIXELFORMAT_RGBA8888);
	//SDL_LockSurface(sur);
	while (x < s.x)
	{
		camX = 2 * x / (double)s.x - 1;
		e->p->raydir = (t_vector2f) {(float)(e->p->rot.x + e->p->plane.x * camX),
									(float)(e->p->rot.y + e->p->plane.y * camX)}; 
		hit = cast_ray(e->p, e->m, e->p->c_sector, -1);
		l.x = x;
		l.h = (int)(s.y / hit.dist);
		l.dr.x = -l.h / 2 + s.y / 2 < 0 ? 0 : -l.h / 2 + s.y / 2;
		l.dr.y = l.h / 2 + s.y / 2 >= s.y ? s.y - 1 : l.h / 2 + s.y / 2;
		vert_line(tex, l, get_color(hit));
		x++;
	}
	SDL_RenderCopy(e->r, tex->tex, NULL, NULL);
	SDL_RenderPresent(e->r);
	//SDL_UnlockSurface(sur);
	//if (SDL_RenderReadPixels(e->r, NULL, SDL_PIXELFORMAT_ARGB8888, sur->pixels, sizeof(Uint32) * (s.x )) == 0)
	//	printf("%s\n",SDL_GetError());
	return (0);
}
