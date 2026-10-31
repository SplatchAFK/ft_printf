/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romasant <romasant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 11:54:30 by romasant          #+#    #+#             */
/*   Updated: 2026/10/31 15:51:17 by romasant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void	ft_conversions_main(const char *s, va_list ap)
{
	int		n;
	char	*address_s;

	address_s = ft_strchr(s, '%');
	address_s++;
	if (*address_s == 'd')
	{
		n = va_arg(ap, int);
		ft_putnbr_fd(n, 1);
	}
}

int	ft_printf(const char *s, ...)
{
	va_list	ap;

	va_start(ap, s);
	ft_conversions_main(s, ap);
	va_end(ap);
	return (0);
}
