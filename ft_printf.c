/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romasant <romasant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 11:54:30 by romasant          #+#    #+#             */
/*   Updated: 2026/10/31 15:13:55 by romasant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_printf(const char *s, ...)
{
	va_list ap;
	va_start(ap, s);
	char	*address_s;
	int		n;

	address_s = ft_strchr(s, '%');
	address_s++;
	if (*address_s == 'd')
	{
		n = va_arg(ap, int);
		ft_putnbr_fd(n, 1);
	}
	va_end(ap);
	return (0);
}

int	main(int ac, char **av)
{
	int n;

	(void) ac;
	n = ft_atoi((const char *)av[1]);
	ft_printf("voici un test : %d", n);
	return (0);
}
