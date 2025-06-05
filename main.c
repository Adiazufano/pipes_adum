#include "printf/ft_printf.h"

int	main(int argc, char *argv[])
{
	int	fd_read;
	int fd_write;
	char	buffer[1024];
	int	bytes;
	if (argc < 3)
		return (-1);
	fd_read = open(argv[1], O_RDONLY);
	if (argc >= 5)
	{	fd_write = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd_write == -1)
			return (-1);
		dup2(fd_write, 1);
	}
	else
		fd_write = 1;
	while ((bytes = read(fd_read, buffer, 1024)) > 0)
	{
		write(fd_write, buffer, bytes);
	}
	return (0);
}