/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wivallee <wivallee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:42:57 by wivallee          #+#    #+#             */
/*   Updated: 2025/01/15 15:22:51 by wivallee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "ultralibft.h"
# include <errno.h>
# include <sys/wait.h>

typedef struct s_fd
{
	int	pipe_fd[2];
	int	input_fd;
}				t_fd;

void	clean_close(char *msg, t_fd tabfd, char **args, char *delete);
char	*find_env(void);
char	*find_next_path(char **path);
char	**creating_cmd(char *cmd);
void	execute_command(char *cmd, t_fd tabfd, char *cmdonly);
void	pipe_n_exec(char *cmd, t_fd tabfd);
void	ft_output(char **input, t_fd tabfd, int index);

#endif
