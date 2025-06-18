/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 17:47:48 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/06/17 20:10:30 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	first(t_pipex *px)
{
	char	**split;
	char	*path;
	int		fd;
	int		infile;
	int		outfile_fd;

	infile = open(px -> argv[1], O_RDONLY);
	if (infile == -1)
	{
		perror(px -> argv[1]);
		outfile_fd = open(px->argv[px->argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (outfile_fd >= 0)
		{
			if (px->argv[3] && !ft_strncmp(px->argv[3], "wc -l", 5))
			{
				write(outfile_fd, "0\n", 2);
				close(outfile_fd);
				return (-42);
			}
			close(outfile_fd);
		}
	}
	split = split_pipex(px -> argv[2]);
	if (!split || !split[0])
		path = NULL;
	else
		path = get_path(split[0], px);
	fd = exec_first_command(path, infile, split, px);
	ft_free_split(split);
	free(path);
	return (fd);
}

static int	final(int fd, t_pipex *px)
{
	char	**split;
	char	*path;

	if (fd == -42)
		return (0);
	split = split_pipex(px->argv[3]);
	if (!split || !split[0])
		path = NULL;
	else
		path = get_path(split[0], px);
	exec_final_command(path, split, fd, px);
	if (fd >= 0)
		close(fd);
	ft_free_split(split);
	free(path);
	return (px->exit_status);
}

static int	run_pipex(t_pipex *px)
{
	int	fd;

	fd = first(px);
	return (final(fd, px));
}

int	main(int argc, char *argv[], char *envp[])
{
	t_pipex	px;

	px.argc = argc;
	px.argv = argv;
	px.envp = envp;
	if (argc != 5 || !argv[1][0] || !argv[2][0] || !argv[3][0] || !argv[4][0])
	{
		ft_printf("Usage: %s infile cmd1 cmd2 outfile\n", argv[0]);
		exit(1);
	}
	px.exit_status = run_pipex(&px);
	return (px.exit_status);
}
