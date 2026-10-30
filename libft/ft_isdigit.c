/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romasant <romasant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/01 18:42:23 by romasant          #+#    #+#             */
/*   Updated: 2026/10/07 19:23:49 by romasant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}
/*
int	main(int ac, char **av)
{
	(void) ac;
	char	c;

	c = av[1][0];
	printf("Le caractere est un nombre
	si = 1, sinon = 0\nValeur = %d\n", ft_isdigit(c));
	return (0);
}
*/
