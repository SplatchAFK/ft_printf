/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_conversion_unsigned_int.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romasant <romasant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 13:56:29 by romasant          #+#    #+#             */
/*   Updated: 2026/11/05 14:23:27 by romasant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void	ft_aff_unsigned_int(unsigned int n)
{
	if (n == UINT_MAX)
	{
		ft_putstr_fd("4294967295", 1);
		return ;
	}
	if (n < 0)
	{
		n *= -1;
		ft_putchar_fd('-', 1);
	}
	if (n >= 0 && n <= 9)
	{
		ft_putchar_fd(n + '0', 1);
		return ;
	}
	if (n >= 10)
	{
		ft_putnbr_fd(n / 10, 1);
		ft_putchar_fd((n % 10) + '0', 1);
	}
}

static int	ft_unsigned_int_len(unsigned int nb)
{
	int		size_nb;
	char	*result_size_nb;

	if (nb == 0)
		return (1);
	result_size_nb = ft_itoa(nb);
	size_nb = ft_strlen(result_size_nb);
	return (size_nb);
}

int	ft_aff_unsigned_int(va_list ap)
{
	int	n;
	int	size_int;

	n = 0;
	size_int = 0;
	n = va_arg(ap, unsigned int);
	ft_aff_unsigned_int(n);
	size_int = ft_unsigned_int_len(n);
	return (size_int);
}
