/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romasant <romasant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 16:13:27 by romasant          #+#    #+#             */
/*   Updated: 2026/10/04 16:27:12 by romasant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isprint(int c)
{
	if (c >= 32 && c < 127)
		return (1);
	return (0);
}
/*
#include <stdio.h>

int	main(int ac, char **av)
{
	(void) ac;
	char	c;
	
	c = av[1][0];
	printf("return 1 si printable sinon
	return 0\nReturn : %d\nValeur : %c", ft_isprint(c), c);
	return (0);
}
*/
