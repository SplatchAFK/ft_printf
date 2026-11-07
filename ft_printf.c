/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romasant <romasant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/06 18:05:04 by romasant          #+#    #+#             */
/*   Updated: 2026/11/07 21:49:57 by romasant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

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
		write(1, &s[counter], 1);
	if (size_conversion == -1)
		return (-1);
	return (size_conversion);
}

int	ft_printf(const char *s, ...)
{
	va_list	ap;

	va_start(ap, s);
	int (counter) = 0;
	int (flag) = 0;
	int (size_conversion_final) = 0;
	while (s[counter])
	{
		size_conversion_final += ft_verif_char(s, counter, ap, &flag);
		size_conversion_final++;
		if (size_conversion_final == -1)
			return (-1);
		if (flag == 1)
		{
			size_conversion_final--;
			counter++;
		}
		counter++;
		//printf("\n[%c][%d] [%d]\n", s[counter], counter, size_conversion_final);
	}
	va_end(ap);
	return (size_conversion_final);
}
