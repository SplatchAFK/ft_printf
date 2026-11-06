/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romasant <romasant@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 18:13:23 by romasant          #+#    #+#             */
/*   Updated: 2026/11/03 10:20:19 by romasant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*str;
	size_t	counter;
	size_t	result;

	counter = 0;
	result = ft_strlen(s) - start;
	if (len > result)
		len = result;
	if (start >= ft_strlen(s))
		return (ft_strdup(""));
	str = malloc(sizeof(char) * len + 1);
	if (str == NULL)
		return (NULL);
	while (len-- && s[start + counter] != '\0')
	{
		str[counter] = s[start + counter];
		counter++;
	}
	str[counter] = '\0';
	return (str);
}
