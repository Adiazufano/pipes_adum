/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 13:30:15 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/06/15 13:49:28 by aldiaz-u         ###   ########.fr       */
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

int	count_words(char *str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		while (str[i] == ' ')
			i++;
		if (!str[i])
			break ;
		if (str[i] == '\'' || str[i] == '\"')
		{
			i++;
			while (str[i] && str[i] != '\'' && str[i] != '\"')
				i++;
			if (str[i] == '\'' || str[i] == '\"')
				count++;
		}
		else if (str[i] != ' ' && str[i] != '\'' && str[i] != '\"')
			count++;
		i++;
	}
	return (count);
}

char	*extract_word(const char *str, int *i)
{
	int		start;
	char	quote;
	char	*word;

	if (str[*i] == '\'' || str[*i] == '\"')
	{
		quote = str[*i];
		start = ++(*i);
		while (str[*i] && str[*i] != quote)
			(*i)++;
		word = ft_substr(str, start, *i - start);
		if (str[*i] == quote)
			(*i)++;
		return (word);
	}
	else
	{
		start = *i;
		while (str[*i] && str[*i] != ' ' && str[*i] != '\'' && str[*i] != '\"')
			(*i)++;
		return (ft_substr(str, start, *i - start));
	}
}

char	**split_pipex(char *str)
{
	char	**splited;
	int		j;
	int		i;

	j = 0;
	i = 0;
	splited = malloc(sizeof (char *) * (count_words(str) + 1));
	while (str[i])
	{
		while (str[i] == ' ')
			i++;
		if (!str[i])
			break ;
		splited[j++] = extract_word(str, &i);
	}
	splited[j] = NULL;
	return (splited);
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

char	*find_command(char **path, char *command)
{
	int		j;
	char	*full_path;

	j = 0;
	while (path[j])
	{
		full_path = malloc(ft_strlen(path[j]) + ft_strlen(command) + 2);
		ft_strlcpy(full_path, path[j], ft_strlen(path[j]) + 2);
		ft_strlcat(full_path, "/", ft_strlen(path[j]) + 2);
		ft_strlcat(full_path, command, ft_strlen(command)
			+ ft_strlen(path[j]) + 2);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		j++;
	}
	return (NULL);
}

char	*get_path(char *command, t_pipex *px)
{
	int		i;
	char	*path_env;
	char	**path;
	char	*result;

	i = 0;
	path_env = NULL;
	while (px->envp[i] && ft_strncmp(px->envp[i],
			"PATH=", ft_strlen("PATH=")) != 0)
		i++;
	if (!px->envp[i])
		return (NULL);
	path_env = px->envp[i] + 5;
	path = ft_split(path_env, ':');
	result = find_command(path, command);
	ft_free_split(path);
	return (result);
}

int	get_outfile(char **argv)
{
	int	index;

	index = 0;
	while (argv[index])
		index++;
	return (index - 1);
}

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

int	first(t_pipex *px)
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

int	middle(t_pipex *px, int fd, int prev_fd, int index)
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

int	final(int fd, t_pipex *px)
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

int	run_pipex(t_pipex *px)
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
	{
		perror(argv[1]);
		exit(1);
	}
	px.exit_status = run_pipex(&px);
	return (px.exit_status);
}
