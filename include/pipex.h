/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: buranchiman <buranchiman@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 14:25:57 by wivallee          #+#    #+#             */
/*   Updated: 2025/01/08 11:22:47 by buranchiman      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "ultralibft.h"
# include <errno.h>
# include <sys/wait.h>

typedef struct s_fd
{
    int pipe_fd[2];
    int input_fd;
}               t_fd;

#endif