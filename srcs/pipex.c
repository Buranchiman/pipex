/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wivallee <wivallee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 14:25:38 by wivallee          #+#    #+#             */
/*   Updated: 2024/12/13 16:19:56 by wivallee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	unsigned int	i;
	unsigned int	j;
	char			*cat;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	cat = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (cat == NULL)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		cat[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
	{
		cat[i + j] = s2[j];
		j++;
	}
	cat[i + j] = '\0';
	return (cat);
}

int	second_cmd(char **input)
{
	int		fildes[2];
	char	*env[] = {"PATH=/bin:/usr/bin:/usr/local/bin:/sbin:/usr/sbin:/opt/bin", NULL};
	char	*args[] = {"/bin/sh", "-c", input[3], NULL};

	//ft_printf("it's alive");
	if (access(input[4], W_OK) == -1)
	{
		perror(strerror(errno));
		return (-1);
	}
	fildes[0] = STDOUT_FILENO;
	fildes[1] = open(input[4], O_RDONLY);
	if (fildes[1] == -1 || fildes[0] == -1)
	{
		perror(strerror(errno));
		return (-1);
	}
	pipe(fildes);
	ft_printf("gnl is %s", get_next_line(1));
	if (fork() == 0)
	{
		if (dup2(STDOUT_FILENO, STDIN_FILENO) == -1)
		{
			perror("Failed to redirect stdin");
			close(fildes[1]);
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
	int	fd;
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
	if (fork() == 0)
	{
		if (dup2(fd, STDIN_FILENO) == -1)
		{
			perror("Failed to redirect stdin");
			close(fd);
			return (-1);
		}
		if (execve("/bin/sh", args, env) == -1)
		{
			perror(strerror(errno));
			return (-1);
		}
	}
	wait(NULL);
	//ft_printf("still here lol\n");
	if (second_cmd(input) == -1)
		return (-1);
	return (0);
}

int	main(int arc, char **arv)
{
	(void)arc;
	return (ft_pipex(arv));
}