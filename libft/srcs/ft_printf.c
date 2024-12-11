/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wivallee <wivallee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 15:06:09 by wivallee          #+#    #+#             */
/*   Updated: 2024/11/21 14:25:14 by wivallee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <stdio.h>

int	ft_type(char c, va_list lst)
{
	if (c == 'c')
	{
		ft_putchar_fd(va_arg(lst, int), 1);
		return (1);
	}
	if (c == 's')
		return (ft_putstr_fdlen(va_arg(lst, char *)));
	if (c == 'p')
		return (ft_print_add(va_arg(lst, unsigned long)));
	if (c == 'd' || c == 'i')
		return (ft_print_number(va_arg(lst, int)));
	if (c == 'u')
		return (ft_putnbr_base(va_arg(lst, unsigned int), "0123456789", 1));
	if (c == 'x')
		return (ft_print_hexa(va_arg(lst, unsigned int), 'x'));
	if (c == 'X')
		return (ft_print_hexa(va_arg(lst, unsigned int), 'X'));
	if (c == '%')
	{
		write(1, "%", 1);
		return (1);
	}
	return (0);
}

int	ft_printf(const char *format, ...)
{
	va_list			lst;
	int				count;
	int				i;

	va_start(lst, format);
	count = 0;
	while (*format)
	{
		i = 0;
		if (*format == '%' && ft_strchr("cspdiuxX%%", *(format + 1)))
		{
			count += ft_type(*(format + 1), lst);
			format += 2;
		}
		while (*(format + i) && (*(format + i) != '%'))
			i++;
		write(1, format, i);
		format += i;
		count += i;
	}
	va_end(lst);
	return (count);
}
