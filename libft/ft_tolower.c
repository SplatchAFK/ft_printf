/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romasant <romasant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/05 16:40:00 by romasant          #+#    #+#             */
/*   Updated: 2026/10/05 16:42:44 by romasant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_tolower(int c)
{
	if (c >= 65 && c <= 90)
		c += 32;
	return (c);
}
/*
#include <stdio.h>

int	main(void)
{
	printf("Voici la minuscule de %d = %d", 'a', ft_tolower('a'));
	printf("Voici la minuscule de %d = %d", 'z', ft_tolower('z'));
	printf("Voici la minuscule de %d = %d", 'A', ft_tolower('A'));
	printf("Voici la minuscule de %d = %d", '5', ft_tolower('5'));
	printf("Voici la minuscule de %d = %d", '{', ft_tolower('{'));
	printf("Voici la minuscule de %d = %d", EOF, ft_tolower(EOF));
	return (0);
}
*/
