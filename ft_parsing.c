/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romasant <romasant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/06 18:09:26 by romasant          #+#    #+#             */
/*   Updated: 2026/11/07 21:19:36 by romasant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_apply_int(va_list ap, int result)
{
	int (result_size_int) = 0;
	if (result == 1)
		return (result);
	result_size_int = ft_manage_integer(ap);
	if (!result_size_int)
		return (-1);
	return (result_size_int);
}

static int	ft_apply_char(int result)
{
	return (result);
}

static int	ft_apply_other(int result)
{
	return (result);
}

int	ft_apply(va_list ap, int result)
{
	int (size_conversion) = 0;
	if (result == 1 || result == 2)
		size_conversion = ft_apply_int(ap, result);
	else if (result == 3 || result == 4)
		size_conversion = ft_apply_char(result);
	else if (result == 5 || result == 6 || result == 7)
		size_conversion = ft_apply_other(result);
	else if (result == 8)
	{
		ft_putchar_fd('%', 1);
		return (1);
	}
	if (!size_conversion)
		return (-1);
	return (size_conversion);
}

int	ft_parsing(const char *s, int counter)
{
	counter++;
	if (s[counter] == 'u')
		return (1);
	else if (s[counter] == 'i')
		return (2);
	else if (s[counter] == 'd')
		return (2);
	else if (s[counter] == 'c')
		return (3);
	else if (s[counter] == 's')
		return (4);
	else if (s[counter] == 'x')
		return (5);
	else if (s[counter] == 'X')
		return (6);
	else if (s[counter] == 'p')
		return (7);
	else if (s[counter] == '%')
		return (8);
	return (0);
}
