/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romasant <romasant@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/27 21:34:42 by romasant          #+#    #+#             */
/*   Updated: 2026/11/03 10:20:19 by romasant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	ft_striteri(char *s, void (*f) (unsigned int, char *))
{
	unsigned int	counter;

	counter = 0;
	if (!s || !f)
		return ;
	while (s[counter] != '\0')
	{
		f(counter, &s[counter]);
		counter++;
	}
}
