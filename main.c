#include <fcntl.h>//TO_DEL
#include "get_next_line.h"

void	ft_putnbr_fd(int n, int fd)
{
	long long	x;

	x = n;
	if (x < 0)
	{
		x = -x;
		write(fd, "-", 1);
	}
	if (x / 10)
		ft_putnbr_fd(x / 10, fd);
	x %= 10;
	x += '0';
	write(fd, &x, 1);
}

static char	*get_tail(char *tail)
{
	char	*new_tail;
	char	*start;

	if (!(*tail))
		return (NULL);
	start = ft_strchr(tail, '\n') + 1;
	if (!start)
		return (NULL);
	new_tail = ft_strdup(start);
	free(tail);
	return (new_tail);
}

static char	*get_line(const char *tail)
{
	char	*line;
	int		size;

	size = 0;
	while (tail[size] != '\n' && tail[size])
		size++;
	if (tail[size] == '\n')
		line = malloc(size + 2);
	else
		line = malloc(size + 1);
	if (!line)
		return (NULL);
	if (tail[size] == '\n')
		line[size + 1] = '\0';
	while (size != -1)
	{
		line[size] = tail[size];
		size--;
	}
	return (line);
}

static char	*get_str(char *tail, int fd)
{
	char	*buff;
	char	*temp;
	size_t	readed;

	buff = malloc(BUFFER_SIZE + 1);
	if (!buff)
		return (NULL);
	readed = 1;
	while (!ft_strchr(tail, '\n') && readed != 0)
	{
		readed = read(fd, buff, BUFFER_SIZE);
		if (readed < 0)
		{
			free(buff);
			return (NULL);
		}
		buff[readed] = '\0';
		temp = ft_strjoin(tail, buff);
		free(tail);
		tail = temp;
	}
	free(buff);
	return (tail);
}

char	*get_next_line(int fd)
{
	static char	*tail;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || fd > FOPEN_MAX)
		return (NULL);
	if (!tail)
	{
		tail = malloc(1);
		tail[0] = '\0';
	}
	tail = get_str(tail, fd);
	if (!tail[0])
		return (NULL);
	line = get_line(tail);
	if (line == NULL)
		printf("NULL\n");
	tail = get_tail(tail);
	return (line);
}

int	main()
{
	char	*line;
	int		i;
	int		fd1;

	fd1 = open("/home/ndillon/CLionProjects/get-next-line/test.txt", O_RDONLY);
	i = 1;
	while (i < 10)
	{
		line = get_next_line(fd1);
		printf("line [%02d]: %s", i, line);
		//printf("%d", line[0]);
		if (line == NULL)
			printf("NULL\n");
		free(line);
		i++;
	}
	close(fd1);
	while (1);
	return (0);
}
