/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romasant <romasant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/11 12:40:02 by romasant          #+#    #+#             */
/*   Updated: 2026/10/11 12:51:41 by romasant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t		counter_dst;
	const char	*src_origin;

	counter_dst = 0;
	src_origin = src;
	if (dstsize == 0)
		return (ft_strlen(src));
	while (*src && counter_dst < dstsize - 1)
	{
		*dst = *src;
		counter_dst++;
		dst++;
		src++;
	}
	*dst = '\0';
	return (ft_strlen(src_origin));
}
