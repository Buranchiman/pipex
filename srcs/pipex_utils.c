/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wivallee <wivallee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 11:55:35 by wivallee          #+#    #+#             */
/*   Updated: 2025/01/14 16:37:15 by wivallee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

extern char	**environ;

int	clean_close(char *msg, t_fd tabfd, char **args)
{
	char	buffer[1];

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

// char	*find_env(void)
// {
// 	char	**env;

// 	env = environ;
// 	while (*env)
// 	{
// 		if (ft_strncmp(*env, "PATH=", 5) == 0)
// 		{
// 			*env = *env + 5;
// 			return (*env);
// 		}
// 		env++;
// 	}
// 	ft_printf(2, "PATH is not set.\n");
// 	return (NULL);
// }
// char	*find_next_path(char *path)
// {
// 	int		i;
// 	char	*temp;

// 	i = 0;
// 	while (path[i] && path[i] != ':')
// 		i++;
// 	temp = ft_substr(path, 0, i);
// 	path = path + i;
// 	return (temp);
// }

// char	**creating_cmd(char *cmd, t_fd tabfd)
// {
// 	char	**args;
// 	char	*cmdonly;

// 	args = (char **)malloc(sizeof(char *) * 3);
// 	if (!args)
// 		return (NULL);
// 	cmdonly = ft_substr(cmd, 0, ft_strindex(cmd, ' '));
// 	if (!cmdonly)
// 		clean_close("Malloc error in ft_substr\n", tabfd, args);
// 	args[1] = ft_strchr(cmd, ' ');
// 	if (args[1] != NULL)
// 		args[1]++;
// 	args[0] = ft_strjoin("/bin/", cmdonly);
// 	free(cmdonly);
// 	if (!args[0])
// 		clean_close("Malloc error in ft_strjoin\n", tabfd, args);
// 	args[2] = NULL;
// 	return (args);
// }

// int	execute_command(char *cmd, t_fd tabfd)
// {
// 	char	**args;

// 	if (tabfd.input_fd <= -1)
// 		exit(EXIT_FAILURE);
// 	if (dup2(tabfd.input_fd, STDIN_FILENO) == -1)
// 		clean_close("could not dup input", tabfd, NULL);
// 	close(tabfd.input_fd);
// 	if (dup2(tabfd.pipe_fd[1], STDOUT_FILENO) == -1)
// 		clean_close("could not dup output", tabfd, NULL);
// 	close(tabfd.pipe_fd[1]);
// 	args = creating_cmd(cmd, tabfd);
// 	if (!args)
// 		clean_close("Malloc error\n", tabfd, args);
// 	if (execve(args[0], args, environ) == -1)
// 		clean_close(cmd, tabfd, args);
// 	ft_clear_tab(args);
// 	exit(EXIT_SUCCESS);
// }

int	ft_output(char **input, t_fd tabfd, int index)
{
	int		fd;
	// char	**args;

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
		// if (!args)
		// 	clean_close("Malloc error\n", tabfd, args);
		if (dup2(tabfd.input_fd, STDIN_FILENO) == -1)
			clean_close("could not dup final input", tabfd, NULL);
		execute_command(input[index], tabfd);
	}
	wait(NULL);
	return (0);
}
