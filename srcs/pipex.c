/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chillyd <chillyd@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 14:25:38 by wivallee          #+#    #+#             */
/*   Updated: 2025/01/13 11:21:11 by chillyd          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

extern char	**environ;

int	clean_close(char *msg, t_fd tabfd, char **args)
{
	char	buffer[1];

	if (args)
		ft_clear_tab(args);
	if (msg)
		perror(msg);
	if (read(tabfd.input_fd, buffer, 0) != -1)
		close(tabfd.input_fd);
	if (read(tabfd.pipe_fd[0], buffer, 0) != -1)
		close(tabfd.pipe_fd[0]);
	if (read(tabfd.pipe_fd[1], buffer, 0) != -1)
		close(tabfd.pipe_fd[1]);
	exit(EXIT_FAILURE);
}

char	**creating_cmd(char *cmd, t_fd tabfd)
{
	char	**args;
	char	*cmdonly;

	args = (char **)malloc(sizeof(char *) * 3);
	if (!args)
		return (NULL);
	cmdonly = ft_substr(cmd, 0, ft_strindex(cmd, ' '));
	if (!cmdonly)
		clean_close("Malloc error in ft_substr\n", tabfd, args);
	args[0] = ft_strjoin("/bin/", cmdonly);
	free(cmdonly);
	if (!args[0])
		clean_close("Malloc error in ft_strjoin\n", tabfd, args);
	args[1] = ft_strchr(cmd, ' ');
	if (args[1] != NULL)
		args[1]++;
	args[2] = NULL;
	return (args);
}

int	execute_command(char *cmd, t_fd tabfd)
{
	char	**args;

	if (tabfd.input_fd <= -1)
		exit(EXIT_FAILURE);
	args = creating_cmd(cmd, tabfd);
	if (!args)
		clean_close("Malloc error\n", tabfd, args);
	if (dup2(tabfd.input_fd, STDIN_FILENO) == -1)
		clean_close("could not dup input", tabfd, args);
	close(tabfd.input_fd);
	if (dup2(tabfd.pipe_fd[1], STDOUT_FILENO) == -1)
		clean_close("could not dup output", tabfd, args);
	close(tabfd.pipe_fd[1]);
	if (execve(args[0], args, environ) == -1)
		clean_close(cmd, tabfd, args);
	ft_clear_tab(args);
	exit(EXIT_SUCCESS);
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
		clean_close(input[index + 1], tabfd, NULL);
	if (dup2(fd, STDOUT_FILENO) == -1)
		clean_close("could not dup final output", tabfd, NULL);
	close(fd);
	if (fork() == 0)
	{
		args = creating_cmd(input[index], tabfd);
		if (!args)
			clean_close("Malloc error\n", tabfd, args);
		if (dup2(tabfd.input_fd, STDIN_FILENO) == -1)
			clean_close("could not dup final input", tabfd, args);
		if (execve(args[0], args, environ) == -1)
			clean_close(input[index], tabfd, args);
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
			return (clean_close("Could not create pipe", tabfd, NULL));
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
		return (clean_close(NULL, tabfd, NULL));
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

char	*reading_here_doc(char **arv)
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

int	inputing_here_doc(char **arv)
{
	char	*here_doc;
	int		fds[2];

	fds[0] =  0;
	if (ft_strncmp(arv[1], "here_doc", 8) == 0)
	{
		here_doc = reading_here_doc(arv);
		if (!here_doc)
			exit(1);
		if (pipe(fds) == -1)
		{
			free(here_doc);
			perror("Could not create pipe");
			exit(1);
		}
		ft_putstr_fd(here_doc, fds[1]);
		close(fds[1]);
		free(here_doc);
	}
	return (fds[0]);
}

int	main(int arc, char **arv)
{
	t_fd	tabfd;

	if (arc < 5)
	{
		perror("Incorrect number of params\n");
		return (-1);
	}
	tabfd.input_fd = inputing_here_doc(arv);
	if (tabfd.input_fd == 0)
	{
		tabfd.input_fd = open(arv[1], O_RDONLY);
		if (tabfd.input_fd == -1)
			perror(arv[1]);
	}
	return (ft_pipex(arv, arc, tabfd));
}
