/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wivallee <wivallee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 14:25:38 by wivallee          #+#    #+#             */
/*   Updated: 2025/01/14 11:58:25 by wivallee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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

	fds[0] = 0;
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
		ft_putstr_fd("Incorrect number of params\n", 2);
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
