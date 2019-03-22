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

#include "wolf.h"

Uint32 rgba(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
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