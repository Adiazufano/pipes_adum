/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 17:54:07 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/06/17 20:12:43 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	command_not_found(char *path, char **args, int fd1, int fd2, int fd3)
{
	ft_printf("%s: command not found\n",args[0]);
	ft_free_split(args);
	free(path);
	if (fd1 >= 0)
		close(fd1);
	if (fd2 >= 0)
		close(fd2);
	if (fd3 >= 0)
		close(fd3);
	exit(127);
}

int	exec_first_command(char *path, int infile, char **args, t_pipex *px)
{
	pid_t	pid;
	int		fd[2];
	int		status;
	
	if (pipe(fd) == -1)
	{
		perror("pipe");
		exit(1);
	}
	pid = fork();
	if (pid == 0)
	{
		if (!path || access(path, X_OK) != 0)
			command_not_found(path, args, infile, fd[0], fd[1]);
		if (infile >= 0)
			dup2(infile, STDIN_FILENO);
		else
			close(STDIN_FILENO);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
		execve(path, args, px -> envp);
		if (infile >= 0)
			close(infile);
		exit(1);
	}
	if (infile >= 0)
		close(infile);
	close(fd[1]);
	waitpid(pid, &status, 0);
	return (fd[0]);
}

void	exec_final_command(char *path, char **args, int infile, t_pipex *px)
{
	pid_t	pid;
	int		outfile_fd;
	int		status;

	outfile_fd = open(px -> argv[px -> argc - 1],
		O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (outfile_fd < 0)
	{
		perror(px -> argv[px->argc - 1]);
		ft_free_split(args);
		free(path);
		if (infile >= 0)
			close(infile);
		exit(1);
	}
	if (!path || access(path, X_OK) != 0)
		command_not_found(path, args, infile, -1, outfile_fd);
	pid = fork();
	if (pid == 0)
	{
		if (infile >= 0)
			dup2(infile, STDIN_FILENO);
		else
			close(STDIN_FILENO);
		dup2(outfile_fd, STDOUT_FILENO);
		if (infile >= 0)
			close(infile);
		close(outfile_fd);
		execve(path, args, px -> envp);
		exit(1);
	}
	close(outfile_fd);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		px -> exit_status = WEXITSTATUS(status);
	else
		px -> exit_status = 127;
}
