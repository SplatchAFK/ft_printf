/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romasant <romasant@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/09 00:03:09 by romasant          #+#    #+#             */
/*   Updated: 2026/11/03 10:20:19 by romasant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char			*dest_cast;
	const unsigned char		*src_cast;

	dest_cast = (unsigned char *)dest;
	src_cast = (unsigned char *)src;
	while (n--)
	{
		*dest_cast++ = *src_cast++;
	}
	return (dest);
}
