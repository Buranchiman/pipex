/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: buranchiman <buranchiman@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 14:25:38 by wivallee          #+#    #+#             */
/*   Updated: 2025/01/07 17:36:43 by buranchiman      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

extern char	**environ;

int	printing_err(char *msg)
{
	perror(msg);
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
	args[2] = NULL;
	return (args);
}

int	execute_command(char *cmd, int input_fd, int output_fd)
{
	char	**args;

	args = creating_cmd(cmd);
	if (dup2(input_fd, STDIN_FILENO) == -1)
	{
		printing_err("Could not dup");
		return (-1);
	}
	close(input_fd);
	if (dup2(output_fd, STDOUT_FILENO) == -1)
	{
		printing_err("Could not dup");
		return (-1);
	}
	close(output_fd);
	if (execve(args[0], args, environ) == -1)
		printing_err(cmd);
	return (0);
}

int	ft_output(char **input, int fd_cmd1, int index)
{
	int		fd;
	char	**args;

	if (ft_strncmp(input[1], "here_doc", 8) == 0)
		fd = open(input[index + 1], O_CREAT | O_WRONLY | O_APPEND, 0644);
	else
		fd = open(input[index + 1], O_CREAT | O_WRONLY, 0644);
	if (fd == -1)
		return (printing_err(input[index + 1]));
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (printing_err("Could not dup"));
	close(fd);
	if (fork() == 0)
	{
		args = creating_cmd(input[index]);
		if (dup2(fd_cmd1, STDIN_FILENO) == -1)
			return (printing_err("Could not dup"));
		if (execve(args[0], args, environ) == -1)
			printing_err(input[index]);
	}
	wait(NULL);
	return (0);
}

int	ft_pipex(char **input, int argc, int input_fd)
{
	int		pipe_fd[2];
	int		i;

	i = 0;
	if (ft_strncmp(input[1], "here_doc", 8) == 0)
		i = 1;
	while (i < argc - 4)
	{
		if (pipe(pipe_fd) == -1)
		{
			printing_err("Could not create pipe");
			return (-1);
		}
		if (fork() == 0)
		{
			close(pipe_fd[0]);
			if (execute_command(input[i + 2], input_fd, pipe_fd[1]) == -1)
				close(pipe_fd[1]);
		}
		close(pipe_fd[1]);
		close(input_fd);
		input_fd = pipe_fd[0];
		i++;
	}
	close(pipe_fd[1]);
	if (ft_output(input, pipe_fd[0], i + 2) == -1)
	{
		close(pipe_fd[0]);
		return (-1);
	}
	close(pipe_fd[0]);
	return (0);
}
char	*working_here_doc(char **arv)
{
	char	*here;
	char	*line;
	int		i;
	
	here = NULL;
	line = get_next_line(0);
	if (!line)
		return (NULL);
	while (line)
	{
		here = ft_strjoinfree(here, line);
		if (!here)
			break ;
		i = 0;
		free(line);
		line = get_next_line(0);
		if (!line)
		{
			free(here);
			return (NULL);
		}
		while (line[i] && arv[2][i] && arv[2][i] == line[i])
			i++;
		if (arv[2][i] == '\0' && line[i] == '\n')
			break;
	}
	free(line);
	return (here);
}

int	main(int arc, char **arv)
{
	char	*here_doc;
	int		fds[2];
	
	if (ft_strncmp(arv[1], "here_doc", 8) == 0)
	{
		here_doc = working_here_doc(arv);
		if (pipe(fds) == -1)
		{
			free(here_doc);
			return (printing_err("Could not create pipe"));
		}
		ft_putstr_fd(here_doc, fds[1]);
		close(fds[1]);
		free(here_doc);
	}
	else
	{
		fds[0] = open(arv[1], O_RDONLY);
		if (fds[0] == -1)
			printing_err(arv[1]);
	}
	return (ft_pipex(arv, arc, fds[0]));
}
