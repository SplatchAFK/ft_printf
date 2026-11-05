/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romasant <romasant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 11:43:34 by romasant          #+#    #+#             */
/*   Updated: 2026/10/30 11:43:34 by romasant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <stdarg.h>
# include <limits.h>
# include "libft/libft.h"

int		ft_printf(const char *s, ...);
int		ft_aff_int(va_list ap);
int		ft_aff_char(va_list ap);
int		ft_aff_string(va_list ap);
int		ft_aff_address(va_list ap);
int		ft_aff_hexa(va_list ap, char choice);
int		ft_aff_percentage(void);
int		ft_aff_unsigned_int(va_list ap);

#endif
