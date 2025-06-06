#include "printf/ft_printf.h"
#include "get_next_line.h"
#include "sys/wait.h"
char	*get_outfile(int argc, char *argv[])
{
	int index;

	index = 0;
	while (index < argc)
		index++;
	return (argv[index - 1]);
}

void infile_error(char *infile)
{
	if (access(infile, F_OK) == -1)
	{
		perror(infile);
		exit(1);
	}
}
void	command_exist(char *command, char *flags, int fd_infile, int fd_outfile)
{
	pid_t pid;
	char *argv[3];
	int	fd[2], fd2[2];

	pipe(fd);
	pid = fork();
	if (pid == 0)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		close(fd[0]);
		argv[0] = "which";
		argv[1] = command;
		argv[2] = NULL;
		execve("/usr/bin/which", argv, NULL);
		perror("which");
		exit(1);
	}
	wait(NULL);
	close(fd[1]);
	char *path = get_next_line(fd[0]);
	if(!path)
	{
		close(fd[0]);
		return ;
	}
	int j;
	j = 0;
	while (path[j])
	{
		if(path[j] == '\n')
		{
			path[j] = '\0';
			break;
		}
		j++;
	}
	close(fd[0]);
	pipe(fd2);
	pid = fork();
	if (pid == 0)
	{
		dup2(fd_outfile, STDOUT_FILENO);
		dup2(fd_infile, STDIN_FILENO);
		close(fd2[1]);
		argv[0] = command;
		argv[1] = flags;
		argv[2] = NULL;
		execve(path, argv, NULL);
		perror(argv[0]);
		exit(1);
	}
	wait(NULL);
	close(fd2[1]);
	char *line;
	while ((line = get_next_line(fd[0])))
	{
		ft_printf(line);
		free(line);
	}
	close(fd2[0]);
	free(path);
}
int	main(int argc, char *argv[])
{
	char *infile;
	char *outfile;
	int	index;
	int end;
	int start;
	int	fd_infile;
	int fd_outfile;
	char **command;

	infile = argv[1];
	outfile = get_outfile(argc, argv);
	fd_infile = open(infile, O_RDONLY);
	fd_outfile = open(outfile, O_CREAT | O_WRONLY | O_TRUNC, 0446);
	infile_error(infile);
	index = 2;
	start = index;
	while (ft_strncmp(argv[index] , "|", sizeof(argv[index])) != 0)
		index++;
	command 
}