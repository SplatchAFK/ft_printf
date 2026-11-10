/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_manage_basic.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romasant <romasant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/06 19:35:25 by romasant          #+#    #+#             */
/*   Updated: 2026/11/10 19:14:36 by romasant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"

int	ft_manage_integer(va_list ap)
{
	int		n;
	int		size_int_str;

	n = va_arg(ap, int);
	if (ft_putnbr_fd(n, 1) == -1)
		return (-1);
	size_int_str = ft_intlen(n);
	return (size_int_str);
}

int	ft_manage_unsigned_int(va_list ap)
{
	unsigned int		n;
	int					size_unsigned_int_str;

	n = va_arg(ap, unsigned int);
	if (ft_put_unsigned_int(n) == -1)
		return (-1);
	size_unsigned_int_str = ft_unsigned_intlen(n);
	return (size_unsigned_int_str);
}

int	ft_manage_char(va_list ap)
{
	char	c;

	c = (char)va_arg(ap, int);
	if (ft_putchar_fd(c, 1) == -1)
		return (-1);
	return (1);
}

int	ft_manage_string(va_list ap)
{
	char	*string;
	int		size_str;

	string = va_arg(ap, char *);
	if (string == NULL)
	{
		if (ft_putstr_fd("(null)", 1) == -1)
			return (-1);
		return (6);
	}
	if (ft_putstr_fd(string, 1) == -1)
		return (-1);
	size_str = ft_strlen((const char *)string);
	return (size_str);
}
