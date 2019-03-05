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

int main(int argc, char *argv[])
{
	t_env	*e;
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
		//hit = cast_ray(e->p, e->m, e->p->c_sector, -1);
		//printf("Hit info :\n");
		//printf("		Hit        : %d\n", hit.wall_hit);
		//printf("		Pos        : {%f ; %f}\n", hit.pos.x, hit.pos.y);
		//printf("		Wall type  : %d\n", hit.type);
		//printf("		Distance   : %f\n", hit.dist);
		render_frame(e, (t_vector2i){(int)WIDTH, (int)HEIGHT});
		SDL_RenderPresent(e->r);
		SDL_Delay(5000);
		SDL_DestroyWindow(e->w);
		SDL_Quit();
	}
	else
		fprintf(stderr, "Error initializing SDL\n");
    return (0);
}