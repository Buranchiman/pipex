/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wivallee <wivallee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 14:25:38 by wivallee          #+#    #+#             */
/*   Updated: 2024/12/20 17:48:27 by wivallee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

extern char	**environ;

int	printing_err(void)
{
	perror(strerror(errno));
	return (-1);
}

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

int	execute_command(char *cmd, int input_fd, int output_fd)
{
	char	**args;

	args = creating_cmd(cmd);
	if (dup2(input_fd, STDIN_FILENO) == -1)
	{
		printing_err();
		return (-1);
	}
	close(input_fd);
	if (dup2(output_fd, STDOUT_FILENO) == -1)
	{
		printing_err();
		return (-1);
	}
	close(output_fd);
	if (execve(args[0], args, environ) == -1)
	{
		printing_err();
		return (-1);
	}
	return (0);
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
	int		input_fd;
	int		pipe_fd[2];
	int		i;

	input_fd = open(input[1], O_RDONLY);
	if (input_fd == -1)
		return (printing_err());
	i = 0;
	while (i < argc - 4)
	{
		if (pipe(pipe_fd) == -1)
		{
			printing_err();
			return (-1);
		}
		if (fork() == 0)
		{
			close(pipe_fd[0]);
			if (execute_command(input[i + 2], input_fd, pipe_fd[1]) == -1)
				return (-1);
		}
		close(pipe_fd[1]);
		close(input_fd);
		input_fd = pipe_fd[0];
		i++;
	}
	//fd = pipe_fd[0];
	close(pipe_fd[1]);
	if (ft_output(input, pipe_fd[0], i + 2) == -1)
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