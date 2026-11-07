/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romasant <romasant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/06 18:02:52 by romasant          #+#    #+#             */
/*   Updated: 2026/11/06 19:51:50 by romasant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include "libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <stdarg.h>

int	ft_printf(const char *s, ...);
int	ft_parsing(const char *s, int counter);
int	ft_apply(va_list ap, int result);
int	ft_manage_integer(va_list ap);

#endif
