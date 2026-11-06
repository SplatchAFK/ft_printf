/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romasant <romasant@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 23:37:00 by romasant          #+#    #+#             */
/*   Updated: 2026/11/03 10:20:19 by romasant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	first;
	size_t	last;
	size_t	size;

	first = 0;
	last = ft_strlen(s1);
	while (s1[first] != '\0' && ft_strchr(set, s1[first]))
		first++;
	while (last > first && ft_strchr(set, s1[last - 1]))
		last--;
	size = last - first;
	return (ft_substr(s1, first, size));
}
