/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_conversion_int.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romasant <romasant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/01 12:51:08 by romasant          #+#    #+#             */
/*   Updated: 2026/11/01 21:29:34 by romasant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_conversion_int(va_list ap)
{
	int (n) = 0;

	n = va_arg(ap, int);
	ft_putnbr_fd(n, 1);
}
