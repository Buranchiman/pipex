/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wivallee <wivallee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 14:10:28 by wivallee          #+#    #+#             */
/*   Updated: 2024/12/11 14:10:44 by wivallee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include "ultralibft.h"

int	ft_putstr_fdlen(const char *s);
int	ft_putnbr_base(unsigned long nbr, char *base, int sign);
int	ft_print_hexa(unsigned int nb, char c);
int	ft_print_add(unsigned long add);
int	ft_print_number(int nb);
int	ft_printf(const char *format, ...);

#endif
