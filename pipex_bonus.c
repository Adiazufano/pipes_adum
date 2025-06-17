/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 13:30:15 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/06/17 10:05:07 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static int	first(t_pipex *px)
{
	char	**split;
	char	*path;
	int		fd;
	int		infile;

	infile = open(px -> argv[1], O_RDONLY);
	split = split_pipex(px -> argv[2]);
	path = get_path(split[0], px);
	fd = exec_first_command(path, infile, split, px);
	ft_free_split(split);
	free(path);
	return (fd);
}

static int	middle(t_pipex *px, int fd, int prev_fd, int index)
{
	char	**split;
	char	*path;

	split = split_pipex(px -> argv[index]);
	path = get_path(split[0], px);
	fd = exec_commands(path, split, fd, px);
	close(prev_fd);
	ft_free_split(split);
	free(path);
	return (fd);
}

static int	final(int fd, t_pipex *px)
{
	char	**split;
	char	*path;

	split = split_pipex(px -> argv[px -> argc -2]);
	path = get_path(split[0], px);
	exec_final_command(path, split, fd, px);
	ft_free_split(split);
	free(path);
	close(fd);
	return (px -> exit_status);
}

static int	run_pipex(t_pipex *px)
{
	int	index;
	int	fd;
	int	prev_fd;

	fd = first(px);
	index = 3;
	while (index < px -> argc - 2)
	{
		prev_fd = fd;
		fd = middle(px, fd, prev_fd, index);
		index++;
	}
	return (final(fd, px));
}

int	main(int argc, char *argv[], char *envp[])
{
	t_pipex	px;

	px.argc = argc;
	px.argv = argv;
	px.envp = envp;
	if (argc < 5)
	{
		ft_printf("Usage: %s infile cmd1 cmd2 ... outfile\n", argv[0]);
		exit(1);
	}
	if (access(argv[1], F_OK) == -1)
		perror(argv[1]);
	px.exit_status = run_pipex(&px);
	return (px.exit_status);
}
