/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 13:30:15 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/06/11 10:09:41 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "printf/ft_printf.h"
#include "get_next_line.h"
#include "sys/wait.h"

void	pipe_err(int *fd)
{
	if (pipe(fd) == -1)
	{
		perror("pipe");
		exit(1);
	}
}
char	*fill_path(char *path)
{
	int	index;
	
	index = 0;
	if (path)
	{
		while (path[index])
		{
			if (path[index] == '\n')
				path[index] = '\0';
			index++;
		}
	}
	return (path);
}
char	*get_path(char *command)
{
	pid_t	pid;
	int		fd[2];
	char	*args[3];
	char	*path;

	path = NULL;
	pipe_err(fd);
	pid = fork();
	if (pid == 0)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
		args[0] = "/usr/bin/which";
		args[1] = command;
		args[2] = NULL;
		execve(args[0], args, NULL);
		exit(1);
	}
	wait(NULL);
	path = get_next_line(fd[0]);
	close(fd[0]);
	path = fill_path(path);
	return (path);
}

int	get_outfile(char **argv)
{
	int	index;

	index = 0;
	while (argv[index])
		index++;
	return (index - 1);
}

int	exec_first_command(char *path, int infile, char **args)
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
		execve(path, args, NULL);
		perror("execve");
		exit(1);
	}
	close(infile);
	close(fd[1]);
	wait(NULL);
	return (fd[0]);
}

int	exec_commands(char *path, char **args, int infile)
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
		execve(path, args, NULL);
		perror("execve");
		exit(1);
	}
	close(infile);
	close(fd[1]);
	wait(NULL);
	return (fd[0]);
}

void	exec_final_command(char *path, char **args, int infile, char *outfile)
{
	pid_t	pid;
	int		outfile_fd;

	outfile_fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	pid = fork();
	if (pid == 0)
	{
		dup2(infile, STDIN_FILENO);
		dup2(outfile_fd, STDOUT_FILENO);
		close(infile);
		close(outfile_fd);
		execve(path, args, NULL);
		perror("execve");
		exit(1);
	}
	close(outfile_fd);
	wait(NULL);
}

/* char	*exec_awk(char *arg)
{
	int		end;
	int		index;
	char	*script;
	int		j;

	index = 1;
	end = 1;
	j = 0;
	while (arg[end])
	{
		if (arg[end] == '\'')
			break;
		end++;
	}
	script = malloc(end);
	if (!script)
		return(NULL);
	while (index < end)
		script[j++] = arg[index++];
	script[j] = '\0';
	ft_printf("%s\n",script);
	return (script);
}

char **return_awk_args(char *command)
{
	char **args;
	char *scrip_start;

	args = malloc(3 * sizeof(char *));
	args[0] = ft_strdup("awk");
	scrip_start = command + 3;
	while (*scrip_start == ' ')
		scrip_start++;
	args[1] = ft_strdup(scrip_start);
	args[2] = NULL;

	return(args);
} */

void	ft_free_split(char **split)
{
	int	index;

	index = 0;
	while (split[index])
	{
		free(split[index]);
		index++;
	}
	free(split);
}

int	first(char **argv)
{
	char	**split;
	char	*path;
	int		fd;
	int		infile;

	infile = open(argv[1], O_RDONLY);
	split = ft_split(argv[2], ' ');
	path = get_path(split[0]);
	fd = exec_first_command(path, infile, split);
	ft_free_split(split);
	free(path);
	return (fd);
}

int	middle(char **argv, int fd, int prev_fd)
{
	char	**split;
	char	*path;

	split = ft_split(argv[2], ' ');
	path = get_path(split[0]);
	fd = exec_commands(path, split, fd);
	close(prev_fd);
	ft_free_split(split);
	free(path);
	return (fd);
}

void	final(char **argv, int fd, int argc)
{
	char	**split;
	char	*path;

	split = ft_split(argv[argc -2], ' ');
	path = get_path(split[0]);
	exec_final_command(path, split, fd, argv[argc - 1]);
	ft_free_split(split);
	free(path);
	close(fd);
}

int	main(int argc, char *argv[])
{
	int		index;
	int		fd;
	int		prev_fd;

	if (argc < 5)
	{
		ft_printf("error\n");
		exit(1);
	}
	if (access(argv[1], F_OK) == -1)
	{
		perror(argv[1]);
		exit(-1);
	}
	fd = first(argv);
	index = 3;
	while (index < argc - 2)
	{
		prev_fd = fd;
		middle(argv, fd, prev_fd);
		index++;
	}
	final(argv, fd, argc);
}
