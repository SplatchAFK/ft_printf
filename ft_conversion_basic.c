/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_conversion_basic.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romasant <romasant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/01 20:57:51 by romasant          #+#    #+#             */
/*   Updated: 2026/11/05 13:55:48 by romasant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_intlen(int nb)
{
	int		size_nb;
	char	*result_size_nb;

	if (nb == 0)
		return (1);
	result_size_nb = ft_itoa(nb);
	size_nb = ft_strlen(result_size_nb);
	return (size_nb);
}

int	ft_aff_char(va_list ap)
{
	char	c;

	c = (char)va_arg(ap, int);
	ft_putchar_fd(c, 1);
	return (1);
}

int	ft_aff_string(va_list ap)
{
	char	*s;

	s = va_arg(ap, char *);
	if (s == NULL)
	{
		ft_putstr_fd("(null)", 1);
		return (6);
	}
	ft_putstr_fd(s, 1);
	return (ft_strlen(s));
}

int	ft_aff_int(va_list ap)
{
	int	n;
	int	size_int;

	n = 0;
	size_int = 0;
	n = va_arg(ap, int);
	ft_putnbr_fd(n, 1);
	size_int = ft_intlen(n);
	return (size_int);
}
