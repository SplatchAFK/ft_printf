/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romasant <romasant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/09 00:35:58 by romasant          #+#    #+#             */
/*   Updated: 2026/10/09 12:12:19 by romasant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char		*dest_cast;
	const unsigned char	*src_cast;

	dest_cast = (unsigned char *)dest;
	src_cast = (const unsigned char *)src;
	if (dest < src)
	{
		while (n--)
			*dest_cast++ = *src_cast++;
	}
	else
	{
		while (n--)
			dest_cast[n] = src_cast[n];
	}
	return (dest);
}
