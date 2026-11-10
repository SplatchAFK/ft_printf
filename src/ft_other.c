/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_other.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romasant <romasant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/09 13:10:13 by romasant          #+#    #+#             */
/*   Updated: 2026/11/10 19:16:02 by romasant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"

int	ft_put_unsigned_int(unsigned int n)
{
	if (n > UINT_MAX)
		return (-1);
	if (n <= 9)
		if (ft_putchar_fd(n + '0', 1) == -1)
			return (-1);
	if (n >= 10)
	{
		if (ft_putnbr_fd(n / 10, 1) == -1)
			return (-1);
		if (ft_putchar_fd((n % 10) + '0', 1) == -1)
			return (-1);
	}
	return (0);
}

int	ft_unsigned_intlen(unsigned int n)
{
	int	counter;

	counter = 0;
	if (n == 0)
		return (1);
	while (n)
	{
		n /= 10;
		counter++;
	}
	return (counter);
}

int	ft_intlen(int n)
{
	int	counter;

	counter = 0;
	if (n < 0)
		counter++;
	if (n == 0)
		return (1);
	while (n)
	{
		n /= 10;
		counter++;
	}
	return (counter);
}

char	*ft_itoa_unsigned_int(unsigned int n)
{
	char	*res;

	int (counter) = 1;
	int (size) = 0;
	unsigned int (nbr) = n;
	size = ft_unsigned_intlen(n);
	res = malloc(sizeof(char) * size + 1);
	if (!res)
		return (NULL);
	res[size] = '\0';
	nbr = n;
	while (counter > 0)
	{
		counter--;
		res[counter] = nbr % 10 + '0';
		nbr = nbr / 10;
	}
	return (res);
}
