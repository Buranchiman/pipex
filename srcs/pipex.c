/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wivallee <wivallee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 14:25:38 by wivallee          #+#    #+#             */
/*   Updated: 2025/01/09 16:25:05 by wivallee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

extern char	**environ;

int	clean_close(char *msg, t_fd tabfd)
{
	char	buffer[1];

	if (msg)
		perror(msg);
	if (read(tabfd.input_fd, buffer, 0) != -1)
		close(tabfd.input_fd);
	if (read(tabfd.pipe_fd[0], buffer, 0) != -1)
		close(tabfd.pipe_fd[0]);
	if (read(tabfd.pipe_fd[1], buffer, 0) != -1)
		close(tabfd.pipe_fd[1]);
	exit (1);
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

int	execute_command(char *cmd, t_fd tabfd)
{
	char	**args;

	args = creating_cmd(cmd);
	if (dup2(tabfd.input_fd, STDIN_FILENO) == -1)
		return (clean_close(NULL, tabfd));
	close(tabfd.input_fd);
	if (dup2(tabfd.pipe_fd[1], STDOUT_FILENO) == -1)
		return (clean_close(NULL, tabfd));
	close(tabfd.pipe_fd[1]);
	if (execve(args[0], args, environ) == -1)
		perror(cmd);
	return (0);
}

int	ft_output(char **input, t_fd tabfd, int index)
{
	int		fd;
	char	**args;

	if (ft_strncmp(input[1], "here_doc", 8) == 0)
		fd = open(input[index + 1], O_CREAT | O_WRONLY | O_APPEND, 0644);
	else
		fd = open(input[index + 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
		perror(input[index + 1]);
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (clean_close(NULL, tabfd));
	close(fd);
	if (fork() == 0)
	{
		args = creating_cmd(input[index]);
		if (dup2(tabfd.input_fd, STDIN_FILENO) == -1)
			clean_close(NULL, tabfd);
		if (execve(args[0], args, environ) == -1)
			perror(input[index]);
	}
	wait(NULL);
	return (0);
}

int	ft_pipex(char **input, int argc, t_fd tabfd)
{
	int		i;

	i = 0;
	if (ft_strncmp(input[1], "here_doc", 8) == 0)
		i = 1;
	while (i < argc - 4)
	{
		if (pipe(tabfd.pipe_fd) == -1)
			return (clean_close("Could not create pipe", tabfd));
		if (fork() == 0)
		{
			close(tabfd.pipe_fd[0]);
			execute_command(input[i + 2], tabfd);
		}
		close(tabfd.pipe_fd[1]);
		close(tabfd.input_fd);
		tabfd.input_fd = tabfd.pipe_fd[0];
		i++;
	}
	if (ft_output(input, tabfd, i + 2) == -1)
		return (clean_close(NULL, tabfd));
	close(tabfd.pipe_fd[0]);
	return (0);
}

int	delimitercmp(char *delimiter, char *s)
{
	int	i;

	i = 0;
	while (delimiter[i] && s[i] && s[i] == delimiter[i])
		i++;
	if (delimiter[i] == '\0' && s[i] == '\n')
		return (1);
	return (0);
}

char	*working_here_doc(char **arv)
{
	char	*here;
	char	*line;

	here = NULL;
	line = get_next_line(0);
	if (!line)
		return (NULL);
	while (line)
	{
		if (delimitercmp(arv[2], line))
			break ;
		here = ft_strjoinfree(here, line);
		if (!here)
			break ;
		free(line);
		line = get_next_line(0);
		if (!line)
		{
			free(here);
			return (NULL);
		}
	}
	free(line);
	return (here);
}

int	main(int arc, char **arv)
{
	char	*here_doc;
	int		fds[2];
	t_fd	tabfd;

	if (ft_strncmp(arv[1], "here_doc", 8) == 0)
	{
		here_doc = working_here_doc(arv);
		if (pipe(fds) == -1)
		{
			free(here_doc);
			perror("Could not create pipe");
			return (-1);
		}
		ft_putstr_fd(here_doc, fds[1]);
		close(fds[1]);
		free(here_doc);
	}
	else
	{
		fds[0] = open(arv[1], O_RDONLY);
		if (fds[0] == -1)
			perror(arv[1]);
	}
	tabfd.input_fd = fds[0];
	return (ft_pipex(arv, arc, tabfd));
}
