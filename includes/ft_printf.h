/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romasant <romasant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/06 18:02:52 by romasant          #+#    #+#             */
/*   Updated: 2026/11/10 19:14:57 by romasant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include "../libft/libft.h"
# include <stdio.h>
# include <stdarg.h>

int		ft_printf(const char *s, ...);
int		ft_parsing(const char *s, int counter);
int		ft_apply(va_list ap, int result);
int		ft_manage_integer(va_list ap);
int		ft_manage_unsigned_int(va_list ap);
int		ft_put_unsigned_int(unsigned int n);
char	*ft_itoa_unsigned_int(unsigned int n);
int		ft_unsigned_intlen(unsigned int n);
int		ft_manage_char(va_list ap);
int		ft_manage_string(va_list ap);
int		ft_manage_hexa(va_list ap, int result);
int		ft_manage_void(va_list ap);
int		ft_intlen(int n);

#endif
