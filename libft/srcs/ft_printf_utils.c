/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wivallee <wivallee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 15:59:43 by wivallee          #+#    #+#             */
/*   Updated: 2024/11/21 14:26:22 by wivallee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <stdio.h>

int	ft_putstr_fdlen(const char *s)
{
	if (s == NULL)
	{
		ft_putstr_fd("(null)", 1);
		return (6);
	}
	ft_putstr_fd((char *)s, 1);
	return (ft_strlen(s));
}

int	ft_putnbr_base(unsigned long nbr, char *base, int sign)
{
	unsigned long	lenbase;
	unsigned long	power;
	unsigned long	holder;
	int				count;

	holder = nbr;
	power = 1;
	lenbase = (unsigned long)ft_strlen(base);
	count = 1;
	if (sign < 0)
	{
		write(1, "-", 1);
		count++;
	}
	while (power <= holder / lenbase)
		power = power * lenbase;
	while (power >= lenbase)
	{
		ft_putchar_fd(base[holder / power], 1);
		count++;
		holder = holder % power;
		power = power / lenbase;
	}
	ft_putchar_fd(base[holder], 1);
	return (count);
}

int	ft_print_number(int nb)
{
	long long int	holder;

	holder = nb;
	if (nb < 0)
	{
		holder = -holder;
		return (ft_putnbr_base(holder, "0123456789", -1));
	}
	return (ft_putnbr_base(holder, "0123456789", 1));
}

int	ft_print_hexa(unsigned int nb, char c)
{
	if (c == 'x')
		return (ft_putnbr_base((unsigned long)nb, "0123456789abcdef", 1));
	if (c == 'X')
		return (ft_putnbr_base((unsigned long)nb, "0123456789ABCDEF", 1));
	return (0);
}

int	ft_print_add(unsigned long add)
{
	if (add == 0)
		return (ft_putstr_fdlen("(nil)"));
	ft_putstr_fd("0x", 1);
	return (ft_putnbr_base(add, "0123456789abcdef", 1) + 2);
}
