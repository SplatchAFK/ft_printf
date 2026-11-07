/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romasant <romasant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/06 18:36:18 by romasant          #+#    #+#             */
/*   Updated: 2026/11/07 21:49:46 by romasant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	main(void)
{
	printf("%d", printf("test de fou%%%dcoucou ca va%d ?%d", 42, 89052, 14));
	printf("\n\n");
	printf("%d", ft_printf("test de fou%%%dcoucou ca va%d ?%d", 42, 89052, 14));
	return (0);
}
