/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romasant <romasant@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 22:49:23 by romasant          #+#    #+#             */
/*   Updated: 2026/11/03 10:20:19 by romasant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*result;
	size_t	size_str;

	size_str = ft_strlen(s1) + ft_strlen(s2);
	result = malloc(sizeof(char) * (size_str + 1));
	if (result == NULL)
		return (NULL);
	ft_strlcpy(result, s1, size_str + 1);
	ft_strlcat(result, s2, size_str + 1);
	return (result);
}
