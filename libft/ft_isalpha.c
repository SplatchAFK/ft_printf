/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romasant <romasant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/11 12:27:18 by romasant          #+#    #+#             */
/*   Updated: 2026/10/11 12:28:47 by romasant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isalpha(int c)
{
	return ((c >= 65 && c <= 90) || (c >= 97 && c <= 122));
}
/*
#include <stdio.h>

int	main(int ac, char **av)
{
	(void) ac;
	char	c;

	c = av[1][0];
	printf("Le caractere est une lettre
	si = 1, sinon = 0\nValeur = %d", ft_isalpha(c));
	return (0);
}
*/
