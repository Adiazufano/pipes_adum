/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 13:30:15 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/06/09 14:02:28 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "printf/ft_printf.h"
#include "get_next_line.h"
#include "sys/wait.h"

int	get_end(char **argv)
{
	int	index;

	index = 1;
	while (argv[index])
		index++;
	return (index - 1);
}

char	**extract_commands(char **argv)
{
	char	**commands;
	int		index;
	int		start;
	int		end;

	start = 2;
	end = get_end(argv);
	commands = malloc(sizeof(char *) * ((end - start) + 2));
	if (!commands)
		exit(1);
	index = 0;
	while (start < end)
	{
		commands[index] = argv[start];
		start++;
		index++;
	}
	commands[index] = NULL;
	return (commands);
}

char	**split_command(char *command)
{
	char	**command_splited;

	command_splited = ft_split(command, ' ');
	return (command_splited);
}

void	is_valid_command(char **comands, int otfile)
{
	pid_t	pid;
	char	*args[3];
	char	**scommand;
	char	*comand;

	pid = fork();
	if (pid == 0)
	{
		scommand = ft_split(comands[0], ' ');
		comand = scommand[0];
		args[0] = "/usr/bin/which";
		args[1] = comand;
		args[2] = NULL;
		dup2(otfile, STDOUT_FILENO);
		execve("/usr/bin/which", args, NULL);
		perror("execve");
		exit(1);
	}
	wait(NULL);
	close(otfile);
}
char	*get_path(char **argv)
{
	char	*path;
	int		fd;

	fd = open(argv[get_end(argv)], O_RDONLY);
	if (fd < 0)
	{
		perror("open");
		exit(1);
	}
	path = get_next_line(fd);
	return (path);
}
int	main(int argc, char *argv[])
{
	int		infile;
	int		outfile;
	char	**commands;
	int		index;
	char	*path;

	if (argc < 3)
	{
		ft_printf("error\n");
		exit(1);
	}
	outfile = open(argv[get_end(argv)], O_RDWR | O_CREAT | O_TRUNC, 0644);
	infile = open(argv[1], O_RDONLY);
	if (access(argv[1], F_OK) == -1)
	{
		perror(argv[1]);
		exit(-1);
	}
	commands = extract_commands(argv);
	is_valid_command(commands, outfile);
	path = get_path(argv);
	index = 0;
	ft_printf("%s", path);	
	index = 0;
	free(commands);
}
