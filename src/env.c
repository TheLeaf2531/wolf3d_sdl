/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vboissel <vboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/22 00:02:24 by vboissel          #+#    #+#             */
/*   Updated: 2019/02/22 00:02:24 by vboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

void					read_error(const char	*error)
{
	perror(error);
	exit(0);
}

static void				load_textures(t_env *e)
{
	e->asset[0].id	  = 0;
	if (!(e->asset[0].surface = SDL_LoadBMP("./textures/placeholder.bmp")))
		read_error(SDL_GetError());
	e->asset[0].id	  = 1;
	if (!(e->asset[0].surface = SDL_LoadBMP("./textures/ground.bmp")))
		read_error(SDL_GetError());;
}

static SDL_Window		*init_window(t_vector2i s)
{
	SDL_Window	*w;

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		read_error(SDL_GetError());
	if (!(w = SDL_CreateWindow("WOLF_3D", SDL_WINDOWPOS_CENTERED,
								SDL_WINDOWPOS_CENTERED,
								s.x,
								s.y,
								SDL_WINDOW_SHOWN)))
		read_error(SDL_GetError());
	return (w);
}

t_env					*init_env(t_vector2i s)
{
	t_env	*e;

	if (!(e = ft_memalloc(sizeof(t_env))))
		read_error("e malloc failed | init_env");
	e->size = s;
	e->w = init_window(s);
	if (!(e->r = SDL_CreateRenderer(e->w, -1, SDL_RENDERER_ACCELERATED)))
		perror(SDL_GetError());
	//TODO : switch type renderer
	//else if (!(e->r = SDL_CreateRenderer(e->w, -1, SDL_RENDERER_SOFTWARE)))
	//	read_error(SDL_GetError());
	load_textures(e);
	return (e);
}
