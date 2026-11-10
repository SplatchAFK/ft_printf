/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romasant <romasant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/06 18:05:04 by romasant          #+#    #+#             */
/*   Updated: 2026/11/10 19:39:06 by romasant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"

static int	ft_verif_char(const char *s, int counter, va_list ap, int *flag)
{
	int		result_parsing;
	int		size_conversion;

	result_parsing = 0;
	size_conversion = 0;
	*flag = 0;
	if (s[counter] == '%' && s[counter + 1])
	{
		result_parsing = ft_parsing(s, counter);
		if (result_parsing >= 1 && result_parsing <= 8)
			size_conversion = ft_apply(ap, result_parsing);
		*flag = 1;
	}
	else
		if (write(1, &s[counter], 1) == -1)
			return (-1);
	if (size_conversion == -1)
		return (-1);
	return (size_conversion);
}

static int	ft_fail(int result, va_list ap)
{
	if (result == -1)
	{
		va_end(ap);
		return (-1);
	}
	return (0);
}

int	ft_printf(const char *s, ...)
{
	va_list (ap);
	va_start(ap, s);
	int (counter) = 0;
	int (flag) = 0;
	int (result) = 0;
	int (size_conversion_final) = 0;
	while (s[counter])
	{
		result = ft_verif_char(s, counter, ap, &flag);
		if (ft_fail(result, ap) == -1)
			return (-1);
		size_conversion_final += result;
		size_conversion_final++;
		if (size_conversion_final == -1)
			return (-1);
		if (flag == 1)
		{
			size_conversion_final--;
			counter++;
		}
		counter++;
	}
	va_end(ap);
	return (size_conversion_final);
}
