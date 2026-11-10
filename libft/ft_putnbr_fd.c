/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romasant <romasant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/27 23:23:37 by romasant          #+#    #+#             */
/*   Updated: 2026/11/10 19:07:15 by romasant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putnbr_fd(int n, int fd)
{
	if (n == INT_MIN)
	{
		if (ft_putstr_fd("-2147483648", fd) == -1)
			return (-1);
		return (0);
	}
	if (n < 0)
	{
		n *= -1;
		if (ft_putchar_fd('-', fd) == -1)
			return (-1);
	}
	if (n >= 10)
	{
		if (ft_putnbr_fd(n / 10, fd) == -1)
			return (-1);
	}
	if (ft_putchar_fd((n % 10) + '0', fd) == -1)
		return (-1);
	return (0);
}
