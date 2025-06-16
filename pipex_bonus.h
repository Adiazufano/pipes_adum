/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 14:31:33 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/06/16 18:31:38 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H
# include "get_next_line.h"
# include "printf/ft_printf.h"
# include "sys/wait.h"

typedef struct s_pipex
{
	int		argc;
	char	**argv;
	char	**envp;
	int		exit_status;
}				t_pipex;
char	**split_pipex(char *str);
char	*get_path(char *command, t_pipex *px);
int		exec_first_command(char *path, int infile, char **args, t_pipex *px);
int		exec_commands(char *path, char **args, int infile, t_pipex *px);
void	exec_final_command(char *path, char **args, int infile, t_pipex *px);
void	pipe_err(int *fd);
void	ft_free_split(char **split);
#endif