/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chillyd <chillyd@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 14:25:38 by wivallee          #+#    #+#             */
/*   Updated: 2024/12/16 17:38:12 by chillyd          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	second_cmd(char **input, int	fd_cmd1)
{
	int		fd;
	//int		fildes[2];
	// char	*buffer;
	// char	*cmd1 = NULL;
	char	*env[] = {"PATH=/bin:/usr/bin:/usr/local/bin:/sbin:/usr/sbin:/opt/bin", NULL};
	char	*args[] = {"/bin/sh", "-c", input[3], NULL};

	if (access(input[4], W_OK) == -1)
	{
		perror(strerror(errno));
		return (-1);
	}
	// fildes[0] = STDOUT_FILENO;
	fd = open(input[4], O_WRONLY);
	// if (fildes[1] == -1 || fildes[0] == -1)
	// {
	// 	perror(strerror(errno));
	// 	return (-1);
	// }
	//pipe(fildes);
	// buffer = (char *)ft_calloc(43, sizeof(char));
	// if (!buffer)
	// 	return (-1);
	// while (read(fd_cmd1, buffer, 42) > 0)
	// 	cmd1 = ft_strjoinfree(cmd1, buffer);
	// ft_printf("cmd1 is %s\n", cmd1);
	// close(pipe_fd[0]);
	// free(buffer);
	if (fork() == 0)
	{
		if (dup2(fd, STDOUT_FILENO) == -1)
		{
			perror("Failed to redirect stdin");
			//close(fildes[1]);
			return (-1);
		}
		if (dup2(fd_cmd1, STDIN_FILENO) == -1)
		{
			perror("Failed to redirect stdin");
			//close(fildes[1]);
			return (-1);
		}
		if (execve("/bin/sh", args, env) == -1)
		{
			perror(strerror(errno));
			return (-1);
		}
	}
	wait(NULL);
	return (0);
}
int	ft_pipex(char **input)
{
	int		fd;
	int		pipe_fd[2];
	char	*env[] = {"PATH=/bin:/usr/bin:/usr/local/bin:/sbin:/usr/sbin:/opt/bin", NULL};
	char	*args[] = {"/bin/sh", "-c", input[2], input[1], NULL};
	if (access(input[1], R_OK) == -1)
	{
		perror(strerror(errno));
		return (-1);
	}
	fd = open(input[1], O_RDONLY);
	if (fd == -1)
	{
		perror(strerror(errno));
		return (-1);
	}
	if (pipe(pipe_fd) == -1) 
	{
        perror("pipe");
        return 1;
    }
	if (fork() == 0)
	{
		close(pipe_fd[0]);
		if (dup2(fd, STDIN_FILENO) == -1)
		{
			perror("Failed to redirect stdin");
			close(fd);
			return (-1);
		}
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1) 
		{
            perror("dup2");
            exit(1);
        }
		close(pipe_fd[1]);
		if (execve("/bin/sh", args, env) == -1)
		{
			perror(strerror(errno));
			return (-1);
		}
	}
	close(pipe_fd[1]);
	if (second_cmd(input, pipe_fd[0]) == -1)
		return (-1);
	return (0);
}

int	main(int arc, char **arv)
{
	(void)arc;
	return (ft_pipex(arv));
}