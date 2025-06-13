/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 13:30:15 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/06/12 18:36:45 by aldiaz-u         ###   ########.fr       */
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
			break;
		if (str[i] == '\'')
		{
			i++;
			while (str[i] && str[i] != '\'')
				i++;
			if (str[i] == '\'')
				count++;
		}
		else if (str[i] != ' ' && str[i] != '\'')
				count++;
		i++;
	}
	return(count);
}
char	*extract_word(const char *str, int *i)
{
	int	start;
	char quote;
	char *word;

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

char **split_pipex(char *str)
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
			break;
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
	close(fd[1]);
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

int	exec_first_command(char *path, int infile, char **args,t_pipex *px)
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

	outfile_fd = open(px -> argv[px -> argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
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

int	first(t_pipex *px)
{
	char	**split;
	char	*path;
	int		fd;
	int		infile;

	infile = open(px -> argv[1], O_RDONLY);
	split = split_pipex(px -> argv[2]);
	path = get_path(split[0]);
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
	path = get_path(split[0]);
	fd = exec_commands(path, split, fd, px);
	close(prev_fd);
	ft_free_split(split);
	free(path);
	return (fd);
}

void	final(int fd, t_pipex *px)
{
	char	**split;
	char	*path;

	split = split_pipex(px -> argv[px -> argc -2]);
	path = get_path(split[0]);
	exec_final_command(path, split, fd, px);
	ft_free_split(split);
	free(path);
	close(fd);
}

void	run_pipex(t_pipex *px)
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
	final(fd, px);
}

int	main(int argc, char *argv[], char *envp[ ])
{
	t_pipex px;

	px.argc = argc;
	px.argv = argv;
	px.envp = envp;
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
	run_pipex(&px);
}
