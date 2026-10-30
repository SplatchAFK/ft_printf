/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romasant <romasant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/05 16:45:03 by romasant          #+#    #+#             */
/*   Updated: 2026/10/08 22:58:20 by romasant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	if ((char)c == '\0')
		return ((char *)s);
	return (NULL);
}
/*
#include <stdio.h>

int	main(int ac, char **av)
{
	(void) ac;
	char	*str;
	char	c;
	char	*result;
	
	str = av[1];
	c = av[2][0];
	result = ft_strchr(str, c);
	if (result == NULL)
	{
		printf("Aucune occurence trouve pour %c\n", c);
		return (0);
	}
	printf("%s\n", result);
	return (0);
}
*/
