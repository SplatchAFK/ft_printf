/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romasant <romasant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 11:54:30 by romasant          #+#    #+#             */
/*   Updated: 2026/11/05 14:26:44 by romasant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_parsing(va_list ap, const char *s, size_t counter)
{
	int	result_nb;

	result_nb = 0;
	if (s[counter] == 'd' || s[counter] == 'i')
		result_nb = ft_aff_int(ap);
	else if (s[counter] == 'c')
		result_nb = ft_aff_char(ap);
	else if (s[counter] == 's')
		result_nb = ft_aff_string(ap);
	else if (s[counter] == 'p')
		result_nb = ft_aff_address(ap);
	else if (s[counter] == 'x')
		result_nb = ft_aff_hexa(ap, 'x');
	else if (s[counter] == 'X')
		result_nb = ft_aff_hexa(ap, 'X');
	else if (s[counter] == '%')
		return (ft_aff_percentage());
	else if (s[counter] == 'u')
		result_nb = ft_aff_unsigned_int(ap);
	else
		ft_putchar_fd(s[counter], 1);
	return (result_nb);
}

static int	ft_conversion_main(const char *s, va_list ap)
{
	int		nb_str_return;
	int		result_nb_parsing;
	size_t	counter;

	nb_str_return = 0;
	counter = 0;
	result_nb_parsing = 0;
	while (counter < ft_strlen(s))
	{
		if (s[counter] == '%' && s[counter + 1])
		{
			counter++;
			result_nb_parsing = result_nb_parsing + ft_parsing(ap, s, counter);
		}
		else
		{
			ft_putchar_fd(s[counter], 1);
			nb_str_return++;
		}
		counter++;
	}
	return (nb_str_return + result_nb_parsing);
}

int	ft_printf(const char *s, ...)
{
	va_list	ap;
	int		nb;

	va_start(ap, s);
	nb = ft_conversion_main(s, ap);
	va_end(ap);
	return (nb);
}
