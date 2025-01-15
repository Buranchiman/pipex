/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wivallee <wivallee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 14:47:11 by wivallee          #+#    #+#             */
/*   Updated: 2025/01/15 15:33:12 by wivallee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char extern	**environ;

char	*find_env(void)
{
	char	**env;

	env = environ;
	while (*env)
	{
		if (ft_strncmp(*env, "PATH=", 5) == 0)
		{
			*env = *env + 5;
			return (*env);
		}
		env++;
	}
	ft_printf(2, "PATH is not set.\n");
	return (NULL);
}

char	*find_next_path(char **path)
{
	int		i;
	char	*temp;

	i = 0;
	while ((*path)[i] && (*path)[i] != ':')
		i++;
	temp = ft_substr(*path, 0, i);
	if (!temp)
		return (NULL);
	if ((*path)[i] == ':')
		i++;
	*path += i;
	temp = ft_strjoinfree(temp, "/");
	return (temp);
}

char	**creating_cmd(char *cmd)
{
	char	**args;

	args = (char **)malloc(sizeof(char *) * 3);
	if (!args)
		return (NULL);
	args[1] = ft_strchr(cmd, ' ');
	if (args[1] != NULL)
		args[1]++;
	args[2] = NULL;
	return (args);
}

void	execute_command(char *cmd, t_fd tabfd, char *cmdonly)
{
	char	**args;
	char	*env;

	args = creating_cmd(cmd);
	if (!args)
		clean_close("Error while allocating args\n", tabfd, NULL, cmdonly);
	env = find_env();
	if (!env)
		clean_close("No PATH in env\n", tabfd, args, cmdonly);
	while (*env)
	{
		args[0] = ft_strjoinfree(find_next_path(&env), cmdonly);
		if (!args[0])
			clean_close(NULL, tabfd, args, cmdonly);
		execve(args[0], args, environ);
		free(args[0]);
		args[0] = NULL;
	}
	ft_printf(2, "command not found : %s\n", cmdonly);
	clean_close(NULL, tabfd, args, cmdonly);
}

void	pipe_n_exec(char *cmd, t_fd tabfd)
{
	char	*cmdonly;

	if (tabfd.input_fd <= -1)
		exit(EXIT_FAILURE);
	if (dup2(tabfd.input_fd, STDIN_FILENO) == -1)
		clean_close("could not dup input", tabfd, NULL, NULL);
	close(tabfd.input_fd);
	if (dup2(tabfd.pipe_fd[1], STDOUT_FILENO) == -1)
		clean_close("could not dup output", tabfd, NULL, NULL);
	close(tabfd.pipe_fd[1]);
	cmdonly = ft_substr(cmd, 0, ft_strindex(cmd, ' '));
	execute_command(cmd, tabfd, cmdonly);
}
