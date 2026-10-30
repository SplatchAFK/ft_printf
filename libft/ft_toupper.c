/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romasant <romasant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/05 16:22:45 by romasant          #+#    #+#             */
/*   Updated: 2026/10/05 16:22:45 by romasant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_toupper(int c)
{
	if (c >= 97 && c <= 122)
		c -= 32;
	return (c);
}
/*
#include <stdio.h>

int	main(void)
{
	printf("Voici la majuscule de %d = %d", 'a', ft_toupper('a'));
	printf("Voici la majuscule de %d = %d", 'z', ft_toupper('z'));
	printf("Voici la majuscule de %d = %d", 'A', ft_toupper('A'));
	printf("Voici la majuscule de %d = %d", '5', ft_toupper('5'));
	printf("Voici la majuscule de %d = %d", '{', ft_toupper('{'));
	printf("Voici la majuscule de %d = %d", EOF, ft_toupper(EOF));
	return (0);
}
*/
