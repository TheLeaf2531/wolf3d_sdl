/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vboissel <vboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/28 16:39:36 by vboissel          #+#    #+#             */
/*   Updated: 2019/03/29 17:36:21 by vboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/wolf.h"


static void		mouse_event(t_env *e, SDL_Event ev)
{
	if (ev.type == SDL_MOUSEMOTION)
	{
		e->p->rotate = ev.motion.xrel;
		e->p->rotate = !ev.motion.xrel ? 0 : e->p->rotate;
	}
	else
		e->p->rotate = 0;
}

static void		keyboard_event(t_env *e, SDL_Event ev)
{

	if (ev.type == SDL_KEYDOWN)
	{
		if (ev.key.keysym.sym == SDLK_w)
			e->p->mvmt.y = 1;
		if (ev.key.keysym.sym == SDLK_s)
			e->p->mvmt.y = -1;
		if (ev.key.keysym.sym == SDLK_a)
			e->p->mvmt.x = 1;
		if (ev.key.keysym.sym == SDLK_d)
			e->p->mvmt.x = -1;
	}
	if (ev.type == SDL_KEYUP)
	{
		if (ev.key.keysym.sym == SDLK_w)
			e->p->mvmt.y = 0;
		if (ev.key.keysym.sym == SDLK_s)
			e->p->mvmt.y = 0;
		if (ev.key.keysym.sym == SDLK_a)
			e->p->mvmt.x = 0;
		if (ev.key.keysym.sym == SDLK_d)
			e->p->mvmt.x = 0;
		if (ev.key.keysym.sym == SDLK_ESCAPE)
			e->running = 0;
	}
}

void		update_event(t_env *e)
{
	SDL_Event	ev;

	while (SDL_PollEvent(&ev))
	{
		if (ev.type == SDL_KEYDOWN || ev.type == SDL_KEYUP)
			keyboard_event(e, ev);
		if (ev.type == SDL_MOUSEMOTION)
			mouse_event(e, ev);
	}
}