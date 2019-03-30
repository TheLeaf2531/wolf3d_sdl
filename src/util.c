/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vboissel <vboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/24 19:16:37 by vboissel          #+#    #+#             */
/*   Updated: 2019/02/24 19:16:37 by vboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/wolf.h"


float		distance(t_vector2f a, t_vector2f b)
{
	return (sqrtf(powf(a.x - b.x, 2.0F) + powf(a.y - b.y, 2.0F)));
}

Uint32		rgba(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
   return (r << 24 | g << 16 | b << 8 | a);
}

size_t		elem_nbrs(char const *s, char c)
{
	size_t	n;

	n = 0;
	while (*s)
	{
		if (*s != c && (*(s + 1) == c || *(s + 1) == '\0'))
			n++;
		s++;
	}
	return (n);
}

t_texture	*create_texture(t_env *e, t_vector2i s)
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