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

#include "wolf.h"


void	update_player(t_env *e)
{
	if (e->p->rotate != 0)
		rotate_player(e->p, -e->p->rotate, e->time.delta_time);
	if (e->p->mvmt.x || e->p->mvmt.y)
		move_player(e, e->p, e->time.delta_time);
}

void	mouse_event(t_env *e, SDL_Event ev)
{
	if (ev.type == SDL_MOUSEMOTION)
	{
		e->p->rotate = ev.motion.xrel;
		e->p->rotate = !ev.motion.xrel ? 0 : e->p->rotate;
	}
	else
		e->p->rotate = 0;
}

void	keyboard_event(t_env *e, SDL_Event ev)
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

void	update_event(t_env *e)
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

int main(int argc, char *argv[])
{
	t_env	*e;
	int 	fps;
	int counter;
	counter = 0;
	fps = 0;
	//t_hit	hit;
	if (argc != 2)
		return (0);
	if ((e = init_env((t_vector2i){WIDTH, HEIGHT})))
	{
		if (!(load_file(argv[1], e)))
		{
			fprintf(stderr, "Error while parsing level\n");
			exit(0);
		}
		e->time = (t_time){(int)0, (int)0, (int)0};
		e->running = 1;
		SDL_SetRelativeMouseMode(1);
		//SDL_ShowCursor(SDL_DISABLE);
		while (e->running)
		{
			counter += SDL_GetTicks();
			fps++;
			if (counter > 1000)
			{
				counter = 0;
				//printf("FPS : %d\n", fps);
				fps = 0;
			}
			//printf("DeltaTime : %f\n", e->time.delta_time);
			e->time.frame_start = SDL_GetTicks();
			update_event(e);
			update_player(e);
			render_frame(e, e->size);
			e->time.frame_end = SDL_GetTicks();
			printf("Delta : %f\n", e->time.delta_time);
			e->time.delta_time = (float)(e->time.frame_end - e->time.frame_start) / 1000;
		}
		//hit = cast_ray(e->p, e->m, e->p->c_sector, -1);
		//printf("Hit info :\n");
		//printf("		Hit        : %d\n", hit.wall_hit);
		//printf("		Pos        : {%f ; %f}\n", hit.pos.x, hit.pos.y);
		//printf("		Wall type  : %d\n", hit.type);
		//printf("		Distance   : %f\n", hit.dist);
		/*
		while (i < 180)
		{
			e->p = rotate_player(e->p, 1);
			i++;
		}
		i = 0;
		while (i < 1)
		{
		printf("Player :\n");
		printf("			Current sector	: %d\n", e->p->c_sector);
		printf("			Pos             : {%f ; %f}\n", e->p->pos.x, e->p->pos.y);
		printf("			Rot             : {%f ; %f}\n", e->p->rot.x, e->p->rot.y);
			//printf("%d\n",i);
			e->p = rotate_player(e->p, -1);
			render_frame(e, (t_vector2i){(int)WIDTH, (int)HEIGHT});
			SDL_Delay(10000);
			i++;
		}
		//SDL_RenderPresent(e->r);
		//SDL_Delay(10000);*/
		SDL_DestroyWindow(e->w);
		SDL_Quit();
	}
	else
		fprintf(stderr, "Error initializing SDL\n");
    return (0);
}