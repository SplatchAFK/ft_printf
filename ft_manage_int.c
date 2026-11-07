/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_manage_int.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romasant <romasant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/06 19:35:25 by romasant          #+#    #+#             */
/*   Updated: 2026/11/07 21:18:45 by romasant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_manage_integer(va_list ap)
{
	char	*int_to_str;
	int		n;
	int		size_int_str;

	n = va_arg(ap, int);
	ft_putnbr_fd(n, 1);
	int_to_str = ft_itoa(n);
	if (!int_to_str)
		return (-1);
	size_int_str = ft_strlen(int_to_str);
	return (size_int_str);
}
