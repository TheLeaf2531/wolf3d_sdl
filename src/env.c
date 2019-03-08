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



static SDL_Window		*init_window(t_vector2i s)
{
	SDL_Window	*w;

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		return (NULL);
	if (!(w = SDL_CreateWindow("WOLF_3D", SDL_WINDOWPOS_CENTERED,
								SDL_WINDOWPOS_CENTERED,
								s.x,
								s.y,
								SDL_WINDOW_SHOWN)))
		return (NULL);
	return (w);
}

t_env					*init_env(t_vector2i s)
{
	t_env	*e;

	if (!(e = ft_memalloc(sizeof(t_env))))
		return (NULL);
	if (!(e->w = init_window(s)))
		return (NULL);
	if (!(e->r = SDL_CreateRenderer(e->w, -1, SDL_RENDERER_SOFTWARE)))
		return (NULL);
	//SDL_SetRenderDrawColor(e->r, 0, 0, 0, 255);
	//SDL_RenderClear(e->r);
	//SDL_RenderPresent(e->r);
	return (e);
}
