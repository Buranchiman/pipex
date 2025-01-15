/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wivallee <wivallee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 11:55:35 by wivallee          #+#    #+#             */
/*   Updated: 2025/01/15 15:31:07 by wivallee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

extern char	**environ;

void	clean_close(char *msg, t_fd tabfd, char **args, char *delete)
{
	char	buffer[1];

	if (delete)
		free(delete);
	if (args)
	{
		if (args[0])
			free(args[0]);
		free(args);
	}
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

void	ft_output(char **input, t_fd tabfd, int index)
{
	int		fd;
	char	*cmdonly;

	if (ft_strncmp(input[1], "here_doc", 8) == 0)
		fd = open(input[index + 1], O_CREAT | O_WRONLY | O_APPEND, 0644);
	else
		fd = open(input[index + 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
		clean_close(input[index + 1], tabfd, NULL, NULL);
	if (dup2(fd, STDOUT_FILENO) == -1)
		clean_close("could not dup final output", tabfd, NULL, NULL);
	close(fd);
	if (fork() == 0)
	{
		if (dup2(tabfd.input_fd, STDIN_FILENO) == -1)
			clean_close("could not dup final input", tabfd, NULL, NULL);
		cmdonly = ft_substr(input[index], 0, ft_strindex(input[index], ' '));
		execute_command(input[index], tabfd, cmdonly);
	}
	wait(NULL);
}
