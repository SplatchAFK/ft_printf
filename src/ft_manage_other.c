/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_manage_other.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romasant <romasant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/09 17:42:43 by romasant          #+#    #+#             */
/*   Updated: 2026/11/09 19:25:49 by romasant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"

static int	ft_mini_convert_hexa(unsigned int nb, char *base, int result)
{
	int		counter;
	char	*base_up;

	counter = 0;
	base_up = "0123456789ABCDEF";
	if (nb / 16)
		counter += ft_mini_convert_hexa(nb / 16, base, result);
	if (result == 6)
	{
		write(1, &base_up[nb % 16], 1);
		return (counter + 1);
	}
	write(1, &base[nb % 16], 1);
	return (counter + 1);
}

static int	ft_mini_convert_hexa_void(unsigned long n)
{
	int		counter;
	char	*base;

	counter = 0;
	base = "0123456789abcdef";
	if (n / 16)
		counter += ft_mini_convert_hexa_void(n / 16);
	write(1, &base[n % 16], 1);
	return (counter + 1);
}

int	ft_manage_hexa(va_list ap, int result)
{
	char			*base;
	unsigned int	nb;
	int				size_hexa;

	size_hexa = 0;
	base = "0123456789abcdef";
	nb = va_arg(ap, unsigned int);
	if (nb == 0)
	{
		ft_putchar_fd('0', 1);
		return (1);
	}
	size_hexa = ft_mini_convert_hexa(nb, base, result);
	return (size_hexa);
}

int	ft_manage_void(va_list ap)
{
	unsigned long	n;
	int				size_void;

	size_void = 0;
	n = (unsigned long)va_arg(ap, void *);
	if (n == 0)
	{
		ft_putstr_fd("(nil)", 1);
		return (5);
	}
	ft_putstr_fd("0x", 1);
	size_void = ft_mini_convert_hexa_void(n);
	return (size_void + 2);
}
