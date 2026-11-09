/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romasant <romasant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/06 18:36:18 by romasant          #+#    #+#             */
/*   Updated: 2026/11/09 19:45:29 by romasant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/ft_printf.h"

int	main(void)
{
	printf("\nVoici la size en octet : %d", printf("%s %d %x %p %u %c %% %X %i", "s", -1, 255u, (void *)1, 7u, 'c', 10u, 0));
	printf("\n\n");
	printf("\nVoici la size en octet : %d", ft_printf("%s %d %x %p %u %c %% %X %i", "s", -1, 255u, (void *)1, 7u, 'c', 10u, 0));
	return (0);
}
