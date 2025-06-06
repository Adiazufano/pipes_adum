#include "printf/ft_printf.h"
#include "get_next_line.h"
#include "sys/wait.h"

int	main(int argc, char *argv[])
{
	int	infile;
	int	outfile;
	int	index;
	int	start;
	int	end;
	char **commands;

	index = 1;
	infile = open(argv[1], O_RDONLY);
	while (argv[index])
		index++;
	end = index - 1;
	outfile = open(argv[end], O_RDWR | O_CREAT | O_TRUNC, 0644);
	(void)outfile;
	(void)infile;
	(void)argc;
	start = 2;
	index = 0;
	commands = malloc(sizeof(char *) *(( end - start) + 2));
	if (!commands)
		exit(1);
	while (start <= end)
	{
		commands[index] = argv[start];
		start++;
		index++;
	}
	commands[index] = '\0';
	free(commands);
}