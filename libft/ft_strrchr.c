/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romasant <romasant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 18:59:13 by romasant          #+#    #+#             */
/*   Updated: 2026/10/08 23:02:54 by romasant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	size_t	counter_str;
	size_t	last_index;
	int		flag;

	counter_str = 0;
	last_index = 0;
	flag = 0;
	while (s[counter_str])
	{
		if ((char)c == s[counter_str] && s[counter_str] != '\0')
		{
			last_index = counter_str;
			flag = 1;
		}
		counter_str++;
	}
	if ((char)c == '\0')
		return ((char *)&s[counter_str]);
	if (!s[counter_str] && flag == 1)
		return ((char *)&s[last_index]);
	return (NULL);
}
