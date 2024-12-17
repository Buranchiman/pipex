/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chillyd <chillyd@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 14:25:38 by wivallee          #+#    #+#             */
/*   Updated: 2024/12/17 16:40:28 by chillyd          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	printing_err(void)
{
	perror(strerror(errno));
	return (-1);
}

int	second_cmd(char **input, int	fd_cmd1)
{
	int		fd;
	char	*env[] = {"PATH=/bin:/usr/bin:/usr/local/bin:/sbin:/usr/sbin:/opt/bin", NULL};
	char	*args[] = {"/bin/sh", "-c", input[3], NULL};

	if (access(input[4], W_OK) == -1)
		return (printing_err());
	fd = open(input[4], O_WRONLY);
	if (fork() == 0)
	{
		if (dup2(fd, STDOUT_FILENO) == -1)
			return (printing_err());
		if (dup2(fd_cmd1, STDIN_FILENO) == -1)
			return (printing_err());
		if (execve("/bin/sh", args, env) == -1)
			return (printing_err());
	}
	wait(NULL);
	return (0);
}
int	ft_pipex(char **input)
{
	int		fd;
	int		pipe_fd[2];
	char	*env[] = {"PATH=/bin:/usr/bin:/usr/local/bin:/sbin:/usr/sbin:/opt/bin", NULL};
	char	*args[] = {"/bin/sh", "-c", input[2], input[1], NULL};
	
	if (access(input[1], R_OK) == -1)
		return (printing_err());
	fd = open(input[1], O_RDONLY);
	if (fd == -1)
		return (printing_err());
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		close(fd);
		return (printing_err());
	}
	close(fd);
	if (pipe(pipe_fd) == -1) 
		return (printing_err());
	if (fork() == 0)
	{
		close(pipe_fd[0]);
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1) 
		{
			close(pipe_fd[1]);
			return (printing_err());
        }
		close(pipe_fd[1]);
		if (execve("/bin/sh", args, env) == -1)
			return (printing_err());
	}
	close(pipe_fd[1]);
	if (second_cmd(input, pipe_fd[0]) == -1)
	{
		close(pipe_fd[0]);
		return (-1);
	}
	close(pipe_fd[0]);
	return (0);
}

int	main(int arc, char **arv)
{
	(void)arc;
	return (ft_pipex(arv));
}