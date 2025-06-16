/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_commands_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 14:08:17 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/06/16 17:47:25 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	exec_first_command(char *path, int infile, char **args, t_pipex *px)
{
	pid_t	pid;
	int		fd[2];

	if (pipe(fd) == -1)
	{
		perror("pipe");
		exit(1);
	}
	pid = fork();
	if (pid == 0)
	{
		dup2(infile, STDIN_FILENO);
		dup2(fd[1], STDOUT_FILENO);
		close(infile);
		close(fd[0]);
		close(fd[1]);
		execve(path, args, px -> envp);
		perror("execve");
		exit(1);
	}
	close(infile);
	close(fd[1]);
	wait(NULL);
	return (fd[0]);
}

int	exec_commands(char *path, char **args, int infile, t_pipex *px)
{
	pid_t	pid;
	int		fd[2];

	if (pipe(fd) == -1)
	{
		perror("pipe");
		exit(1);
	}
	pid = fork();
	if (pid == 0)
	{
		dup2(infile, STDIN_FILENO);
		dup2(fd[1], STDOUT_FILENO);
		close(infile);
		close(fd[0]);
		close(fd[1]);
		execve(path, args, px -> envp);
		perror("execve");
		exit(1);
	}
	close(infile);
	close(fd[1]);
	wait(NULL);
	return (fd[0]);
}

void	exec_final_command(char *path, char **args, int infile, t_pipex *px)
{
	pid_t	pid;
	int		outfile_fd;
	int		status;

	outfile_fd = open(px -> argv[px -> argc - 1],
			O_WRONLY | O_CREAT | O_TRUNC, 0644);
	pid = fork();
	if (pid == 0)
	{
		dup2(infile, STDIN_FILENO);
		dup2(outfile_fd, STDOUT_FILENO);
		close(infile);
		close(outfile_fd);
		execve(path, args, px -> envp);
		perror("execve");
		exit(1);
	}
	close(outfile_fd);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		px -> exit_status = WEXITSTATUS(status);
	else
		px -> exit_status = 1;
}
