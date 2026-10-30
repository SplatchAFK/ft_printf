/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romasant <romasant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/11 12:58:32 by romasant          #+#    #+#             */
/*   Updated: 2026/10/11 22:56:56 by romasant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	dst_len;
	size_t	src_len;
	size_t	counter_dst;

	dst_len = ft_strlen(dst);
	src_len = ft_strlen(src);
	counter_dst = 0;
	if (dst_len >= dstsize)
		return (dstsize + ft_strlen(src));
	dst = dst + dst_len;
	while (*src != '\0' && counter_dst < dstsize - dst_len - 1)
	{
		*dst = *src;
		dst++;
		src++;
		counter_dst++;
	}
	*dst = '\0';
	return (dst_len + src_len);
}
