/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chillyd <chillyd@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 14:25:38 by wivallee          #+#    #+#             */
/*   Updated: 2024/12/19 14:49:19 by chillyd          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

extern char	**environ;

char	**creating_cmd(char *cmd)
{
	char	**args;

	args = (char **)malloc(sizeof(char *) * 3);
	if (!args)
		return (NULL);
	args[0] = ft_strjoin("/bin/", ft_substr(cmd, 0, ft_strindex(cmd, ' ')));
	if (!args[0])
	{
		free(args);
		return (NULL);
	}
	args[1] = ft_strchr(cmd, ' ');
	if (args[1] != NULL)
		args[1]++;
	//ft_printf("args[1] is %s\n", args[1]);
	args[2] = NULL;
	return (args);
}

int	printing_err(void)
{
	perror(strerror(errno));
	return (-1);
}

int	ft_output(char **input, int fd_cmd1, int index)
{
	int		fd;
	char	**args;
	
	fd = open(input[index + 1], O_CREAT | O_WRONLY, 0644);
	if (fd == -1)
		return (printing_err());
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (printing_err());
	close(fd);
	if (fork() == 0)
	{
		args = creating_cmd(input[index]);
		if (dup2(fd_cmd1, STDIN_FILENO) == -1)
			return (printing_err());
		if (execve(args[0], args, environ) == -1)
			return (printing_err());
	}
	wait(NULL);
	return (0);
}

int	ft_pipex(char **input, int argc)
{
	int		fd;
	int		pipe_fd[2];
	char	**args;
	int		i;

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
		args = creating_cmd(input[2]);
		if (execve(args[0], args, environ) == -1)
			return (printing_err());
	}
	i = 3;
	while (i < argc - 2)
	{
		if (fork() == 0)
		{
			if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
			{
				close(pipe_fd[1]);
				return (printing_err());
			}
			close(pipe_fd[0]);
			if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
			{
				close(pipe_fd[1]);
				return (printing_err());
			}
			close(pipe_fd[1]);
			args = creating_cmd(input[i]);
			if (execve(args[0], args, environ) == -1)
				return (printing_err());
		}
		i++;
	}
	close(pipe_fd[1]);
	if (ft_output(input, pipe_fd[0], i) == -1)
	{
		close(pipe_fd[0]);
		return (-1);
	}
	close(pipe_fd[0]);
	return (0);
}

int	main(int arc, char **arv)
{
	
	return (ft_pipex(arv, arc));
}