/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wivallee <wivallee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 14:25:38 by wivallee          #+#    #+#             */
/*   Updated: 2024/12/11 16:53:48 by wivallee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	ft_pipex(char **input)
{
	int	fd;

	if (access(input[1], R_OK) == -1 || access(input[4], W_OK) != -1)
	{
		perror(strerror(errno));
		return (-1);
	}
	fd = open(input[1], O_RDONLY);
	if (fd == -1)
	{
		perror(strerror(errno));
		return (-1);
	}
	return (0);
}

int	main(int arc, char **arv)
{
	(void)arc;
	return (ft_pipex(arv));
}