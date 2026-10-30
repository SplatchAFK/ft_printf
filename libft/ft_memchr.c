/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romasant <romasant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/09 13:12:17 by romasant          #+#    #+#             */
/*   Updated: 2026/10/12 14:16:01 by romasant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	const unsigned char	*s_cast;
	unsigned char		c_cast;

	s_cast = (const unsigned char *)s;
	c_cast = (unsigned char)c;
	while (n--)
	{
		if (c_cast == *s_cast)
			return ((void *)s_cast);
		s_cast++;
	}
	return (NULL);
}
