/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_conversion_others.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romasant <romasant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/04 14:25:31 by romasant          #+#    #+#             */
/*   Updated: 2026/11/05 14:03:00 by romasant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_mini_aff_hexa(unsigned long n, char *base, char choice)
{
	int		size_str_hexa;
	char	c_local;

	size_str_hexa = 0;
	if (n >= 16)
		size_str_hexa += ft_mini_aff_hexa(n / 16, base, choice);
	c_local = base[n % 16];
	if (choice == 'X')
		c_local = ft_toupper(base[n % 16]);
	write(1, &c_local, 1);
	size_str_hexa++;
	return (size_str_hexa);
}

int	ft_aff_address(va_list ap)
{
	unsigned long	n;
	int				size_str;

	n = (unsigned long)va_arg(ap, void *);
	size_str = 0;
	if (!n)
	{
		ft_putstr_fd("(nil)", 1);
		return (5);
	}
	ft_putstr_fd("0x", 1);
	size_str = ft_mini_aff_hexa((unsigned long)n, "0123456789abcdef", '0');
	return (size_str + 2);
}

int	ft_aff_hexa(va_list ap, char choice)
{
	unsigned int	n;
	int				size_hexa;

	n = (unsigned int)va_arg(ap, int);
	size_hexa = 0;
	if (!n)
	{
		ft_putstr_fd("0", 1);
		return (1);
	}
	size_hexa = ft_mini_aff_hexa(n, "0123456789abcdef", choice);
	return (size_hexa);
}

int	ft_aff_percentage(void)
{
	ft_putstr_fd("%", 1);
	return (1);
}
