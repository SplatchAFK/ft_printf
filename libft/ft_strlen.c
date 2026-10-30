/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romasant <romasant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 16:30:07 by romasant          #+#    #+#             */
/*   Updated: 2026/10/04 17:05:04 by romasant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlen(const char *s)
{
	size_t	counter_size;

	counter_size = 0;
	while (s[counter_size] != '\0')
		counter_size++;
	return (counter_size);
}
/*
#include <stdio.h>

int	main(int ac, char **av)
{
	(void) ac;
	printf("Voici la size de %s : %zu", av[1], ft_strlen(av[1]));
	return (0);
}
*/
