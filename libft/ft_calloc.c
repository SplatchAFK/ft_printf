/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romasant <romasant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/12 14:47:58 by romasant          #+#    #+#             */
/*   Updated: 2026/10/12 16:14:32 by romasant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	char	*p;
	char	*p_cast;
	size_t	nb_el;

	nb_el = nmemb * size;
	if (nmemb == 0 || size == 0)
	{
		p = malloc(1);
		return (p);
	}
	if (nmemb > SIZE_MAX / size)
		return (NULL);
	p = malloc(sizeof(char) * nb_el);
	p_cast = p;
	if (p_cast == NULL)
		return (NULL);
	while (nb_el--)
	{
		*p_cast = 0;
		p_cast++;
	}
	return (p);
}
