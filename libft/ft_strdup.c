/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romasant <romasant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/12 16:34:30 by romasant          #+#    #+#             */
/*   Updated: 2026/10/12 16:48:47 by romasant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char		*dup;
	char		*dup_first;
	size_t		s_len;

	s_len = ft_strlen(s);
	dup = malloc(sizeof(char) * s_len + 1);
	dup_first = dup;
	if (dup == NULL)
		return (NULL);
	while (s_len--)
	{
		*dup = *s;
		dup++;
		s++;
	}
	*dup = '\0';
	return (dup_first);
}
