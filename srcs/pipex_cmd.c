/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wivallee <wivallee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 14:47:11 by wivallee          #+#    #+#             */
/*   Updated: 2025/01/14 17:36:46 by wivallee         ###   ########.fr       */
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
	//ft_printf(2, "path[i] is %c\n", *path[i]);
	while ((*path)[i] && (*path)[i] != ':')
		i++;
	//ft_printf(2, "i is %d and env[i] is %s\n", i, *path[i]);
	temp = ft_substr(*path - i, 0, i);
	//ft_printf(2, "temps is %s\n", temp);
	if (!temp)
		return (NULL);
	*path += (i + ((*path)[i] == ':' ? 1 : 0));
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
int	execute_command(char *cmd, t_fd tabfd)
{
	char	**args;
	char	*env;
	char	*cmdonly;

	args = creating_cmd(cmd);
	if (!args)
		clean_close("Error while allocating args\n", tabfd, NULL);
	env = find_env();
	ft_printf(1, "PATH IS %s\n", env);
	if (!env)
		clean_close("No PATH in env\n", tabfd, args);
	cmdonly = ft_substr(cmd, 0, ft_strindex(cmd, ' '));
	if (!cmdonly)
		clean_close("Malloc error in ft_substr\n", tabfd, args);
	while (*env)
	{
		args[0] = ft_strjoinfree(find_next_path(&env), cmdonly);
		ft_printf(2, "args[0] is %s\n", args[0]);
		ft_printf(2, "env is %s\n", env);
		if (!args[0])
		{
			free(cmdonly);
			clean_close(NULL, tabfd, args);
		}
		execve(args[0], args, environ);
	}
	ft_printf(2, "command not found : %s\n", cmdonly);
	exit(EXIT_FAILURE);
}
void	pipe_n_exec(char *cmd, t_fd tabfd)
{
	if (tabfd.input_fd <= -1)
		exit(EXIT_FAILURE);
	if (dup2(tabfd.input_fd, STDIN_FILENO) == -1)
		clean_close("could not dup input", tabfd, NULL);
	close(tabfd.input_fd);
	if (dup2(tabfd.pipe_fd[1], STDOUT_FILENO) == -1)
		clean_close("could not dup output", tabfd, NULL);
	close(tabfd.pipe_fd[1]);
	execute_command(cmd, tabfd);
}
