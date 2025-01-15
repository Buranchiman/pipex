/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wivallee <wivallee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 11:40:44 by wivallee          #+#    #+#             */
/*   Updated: 2025/01/15 15:18:59 by wivallee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	ft_pipex(char **input, t_fd tabfd)
{
	if (pipe(tabfd.pipe_fd) == -1)
		clean_close("Could not create pipe", tabfd, NULL, NULL);
	if (fork() == 0)
	{
		close(tabfd.pipe_fd[0]);
		pipe_n_exec(input[2], tabfd);
	}
	close(tabfd.pipe_fd[1]);
	if (tabfd.input_fd > 2)
		close(tabfd.input_fd);
	tabfd.input_fd = tabfd.pipe_fd[0];
	ft_output(input, tabfd, 3);
	close(tabfd.pipe_fd[0]);
	return (0);
}

int	main(int arc, char **arv)
{
	t_fd	tabfd;

	if (arc != 5)
	{
		ft_putstr_fd("Incorrect number of params\n", 2);
		return (-1);
	}
	tabfd.input_fd = open(arv[1], O_RDONLY);
	if (tabfd.input_fd == -1)
		perror(arv[1]);
	return (ft_pipex(arv, tabfd));
}
