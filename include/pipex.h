/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wivallee <wivallee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:42:57 by wivallee          #+#    #+#             */
/*   Updated: 2025/01/08 14:49:43 by wivallee         ###   ########.fr       */
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

#endif
