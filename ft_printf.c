/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romasant <romasant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 11:54:30 by romasant          #+#    #+#             */
/*   Updated: 2026/11/01 21:51:43 by romasant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_redirection(va_list ap, const char *s, size_t counter)
{
	counter++;
	if (s[counter] == 'd' || s[counter] == 'i')
		ft_conversion_int(ap);
	else if (s[counter] == 'c')
		ft_conversion_char(ap);
	else
		ft_putchar_fd(s[counter], 1);
	return (counter);
}

static void	ft_conversion_main(const char *s, va_list ap)
{
	size_t (counter) = 0;
	while (counter < ft_strlen(s))
	{
		if (s[counter] == '%' && s[counter + 1])
			counter = ft_redirection(ap, s, counter);
		else
			ft_putchar_fd(s[counter], 1);
		counter++;
	}
}

int	ft_printf(const char *s, ...)
{
	va_list	ap;

	va_start(ap, s);
	ft_conversion_main(s, ap);
	va_end(ap);
	return (0);
}
