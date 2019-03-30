/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vboissel <vboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/22 00:24:14 by vboissel          #+#    #+#             */
/*   Updated: 2019/02/22 00:24:14 by vboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/wolf.h"


void	update_player(t_env *e)
{
	if (e->p->rotate != 0)
		rotate_player(e->p, -e->p->rotate, e->time.delta_time);
	if (e->p->mvmt.x || e->p->mvmt.y)
		move_player(e, e->p, e->time.delta_time);
}

void	game_loop(t_env *e)
{
	while (e->running)
	{
		e->time.frame_start = SDL_GetTicks();
		update_event(e);
		update_player(e);
		//printf("Start rendering\n");
		alter_render_frame(e, e->size);
		//printf("Stop  rendering\n");
		//render_frame(e, e->size);
		e->time.frame_end = SDL_GetTicks();
		e->time.delta_time = (float)(e->time.frame_end - e->time.frame_start) / 1000.0F;		
	}
	SDL_DestroyWindow(e->w);
	SDL_Quit();
}

int main(int argc, char *argv[])
{
	t_env	*e;

	if (argc != 2)
		return (0);
	if ((e = init_env((t_vector2i){WIDTH, HEIGHT})))
	{
		if (!(parse_map_alter(argv[1], e)))
		{
			fprintf(stderr, "Error while parsing level\n");
			exit(0);
		}
		e->time = (t_time){(int)0, (int)0, (int)0};
		e->running = 1;
		SDL_SetRelativeMouseMode(1);
		game_loop(e);
	}
	else
		fprintf(stderr, "Error initializing SDL\n");
    return (0);
}