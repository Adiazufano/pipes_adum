/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 17:38:05 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/06/15 13:44:54 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 4
# endif
# include <fcntl.h>
# include <stdlib.h>
# include <unistd.h>
# include "printf/ft_printf.h"

typedef struct s_pipex
{
	int		argc;
	char	**argv;
	char	**envp;
	int		exit_status;
}				t_pipex;
char	*get_next_line(int fd);
char	*ft_strdup(const char *s);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_substr(char const *s, unsigned int start, size_t len);
size_t	ft_strlen(const char *s);
#endif