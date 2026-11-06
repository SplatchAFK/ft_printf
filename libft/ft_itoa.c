/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romasant <romasant@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/23 12:00:47 by romasant          #+#    #+#             */
/*   Updated: 2026/11/03 10:20:19 by romasant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_verif_neg(long n, int *neg, int *len)
{
	if (n < 0)
	{
		*neg = 1;
		n = n * -1;
	}
	while (n / 10 != 0)
	{
		n = n / 10;
		(*len)++;
	}
}

char	*ft_itoa(int n)
{
	char	*res;

	int (len) = 1;
	int (neg) = 0;
	long (nbr) = n;
	ft_verif_neg(nbr, &neg, &len);
	res = malloc(sizeof(char) * (len + neg + 1));
	if (!res)
		return (NULL);
	res[len + neg] = '\0';
	nbr = n;
	if (nbr < 0)
	{
		res[0] = '-';
		nbr = nbr * -1;
	}
	while (len > 0)
	{
		len--;
		res[len + neg] = nbr % 10 + '0';
		nbr = nbr / 10;
	}
	return (res);
}
