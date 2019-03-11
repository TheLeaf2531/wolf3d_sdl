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
	//printf("			p.y : %d  ; p.x : %d ; tex->s.x  :  %d nmbr : %d\n", p.y, p.x, tex->s.x,p.y * tex->s.x + p.x );
    pixels[p.y * tex->s.x + p.x] = mapped_color;
}

static void vert_line(t_texture *tex, t_line l, t_rgba c)
{
	int		y;
	//printf("l\n");
	//printf("line x : %d, l.dr.x : %d, l.dr.y : %d\n", l.x, l.dr.x, l.dr.y);
	//printf("	Drawing :\n");
	//printf("		Start drawing line at : {%d, %d}\n", l.x, l.dr.x);
	y = l.dr.x;
	while (y < l.dr.y)
	{
		//printf("			Set pixel drawing line at : {%d, %d}\n", l.x, y);
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
		c = (t_rgba){(Uint8)120, (Uint8)130, (Uint8)0, (Uint8)255};
	//printf("		Color selected is : %d %d %d %d\n", c.r, c.g, c.b, c.a);
	return (c);
}

static t_texture	*create_texture(t_env *e, t_vector2i s)
{
	t_texture	*tex;

	//printf("	Create Texture : \n");
	if (!(tex = ft_memalloc(sizeof(t_texture))))
	{
		//printf("Malloc failed on : create texture\n");
		return (NULL);
	}
	tex->tex = SDL_CreateTexture(e->r, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, s.x, s.y);
	if (tex->tex == NULL)
		printf("SDL_Create texture failed\n");
	tex->format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
	if (SDL_LockTexture(tex->tex, NULL, &(tex->tmp), &(tex->pitch)) == -1)
		printf("SDL_LockTexture failed: %s\n", SDL_GetError());
	//printf("Pitch Value : %d\n", tex->pitch);
	tex->pixels = (Uint32 *)tex->tmp;
	if (tex->pixels == NULL)
	{
		printf("Cannot get pixels\n");
		return (NULL);
	}
	tex->s = s;
	return (tex);
}

int			render_frame(t_env  *e, t_vector2i s)
{
    float				x;
    float			camX;
    t_hit			hit;
    t_line			l;
    t_texture		*tex;
	t_ray			ray;

	x = 0;
	SDL_RenderClear(e->r);
	tex = create_texture(e, s);
	//sur = SDL_CreateRGBSurfaceWithFormat(0, s.x, s.y, 32, SDL_PIXELFORMAT_RGBA8888);
	//SDL_LockSurface(sur);
	//printf("\nRendering start :\n");
	ray = set_ray(e->m, e->p->pos, e->p->rot, e->p->c_sector, 0);
	while (x < s.x)
	{
		//printf("	For x = %f\n",x);
		camX = 2 * x / (float)s.x - (float)1;
		//printf("		Camx : %f", camX);
		ray.direction = (t_vector2f) {(float)(e->p->rot.x + e->p->plane.x * camX),
									(float)(e->p->rot.y + e->p->plane.y * camX)};
		//printf("		plane : {%f, %f}\n", e->p->plane.x, e->p->plane.y); 

	//	printf("		Raydir : {%f, %f}\n", ray.direction.x, ray.direction.y);
		hit = cast_ray(ray);
	//	printf("hit dist uncorrected %f\n", hit.dist);
		hit.dist *= ((e->p->rot.x * ray.direction.x + e->p->rot.y * ray.direction.y) /
			(hypotl(e->p->rot.x, e->p->rot.y) * hypotl(ray.direction.x, ray.direction.y)));
		//hit = cast_ray(e->p, e->m, e->p->raydir, e->p->c_sector, -1);
	//	printf("		Hit info :\n");
	//	printf("			Dist         : %f\n",hit.dist);
	//	printf("			Pos          : {%f ; %f}\n",hit.pos.x,hit.pos.y);
	//	printf("			Wall type    : %d\n",hit.type);
	//	printf("			Hit append   : %d\n",hit.result);
		if (hit.result)
		{
			//printf("Hit :)\n");
			l.x = x;
			l.h = (int)(s.y / hit.dist);
			l.dr.x = (double)-l.h / 2 + s.y / 2 < 0 ? 0 : (double)-l.h / 2 + s.y / 2;
			l.dr.y = (double)l.h / 2 + s.y / 2 >= s.y ? s.y - 1 : (double)l.h / 2 + s.y / 2;
			//printf("		Line y start at  : %d\n", l.dr.x);
			//printf("		Line y finish at : %d\n", l.dr.y);
			vert_line(tex, l, get_color(hit));
		}
		x++;
	}
	SDL_FreeFormat(tex->format);
	SDL_UnlockTexture(tex->tex);
	if (SDL_RenderCopy(e->r, tex->tex, NULL, NULL) == -1)
		printf("SDL_RenderCopy failed: %s\n",SDL_GetError());
	SDL_RenderPresent(e->r);
	SDL_DestroyTexture(tex->tex);
	free(tex);
	//SDL_UnlockSurface(sur);
	//if (SDL_RenderReadPixels(e->r, NULL, SDL_PIXELFORMAT_ARGB8888, sur->pixels, sizeof(Uint32) * (s.x )) == 0)
	//	printf("%s\n",SDL_GetError());
	return (0);
}
