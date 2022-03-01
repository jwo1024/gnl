#include	"get_next_line_bonus.h"

char	*get_next_line(int fd)
{
	static t_gnl_list	gnl_head;
	t_gnl_list			*now;
	int					a;
	char				*str;

	a = 1;
	str = NULL;
	now = NULL;
	if (fd >= 0)
	{
		now = gnl_get_fdlist(fd, &gnl_head);
		while (now && a > 0)
			a = gnl_read_fd(fd, now, &str);
		if (a == 0 || a == -1)
			gnl_free_list(now, &gnl_head);
	}
	return (str);
}

int	gnl_read_fd(int fd, t_gnl_list	*now, char **str)
{
	int		i;

	i = 1;
	if (now->buff_cur == NULL)
	{
		i = read(fd, now->buff, BUFFER_SIZE);
		if (i == 0 || i == -1)
			return (i);
		else
			now->buff_cur = now->buff;
	}
	i = gnl_read_buffer(str, now);
	return (i);
}

int	gnl_read_buffer(char **str, t_gnl_list *now)
{
	size_t	str_len;
	size_t	i;
	int		rtn;

	i = 0;
	str_len = 0;
	rtn = 1;
	while (now->buff_cur[str_len] != 0 && now->buff_cur[str_len] != '\n')
		str_len++;
	if (now->buff_cur[str_len] == '\n')
	{
		rtn = -2;
		str_len++;
	}
	gnl_strjoin(str, str_len, now);
	if (now->buff_cur[0] == 0)
	{
		gnl_memset(now->buff, 0, BUFFER_SIZE);
		now->buff_cur = NULL;
	}
	return (rtn);
}

int	gnl_strjoin(char **str, size_t str_len, t_gnl_list *now)
{
	char	*tmp;
	size_t	tmp_len;
	size_t	i;

	i = 0;
	tmp = *str;
	tmp_len = gnl_strlen(tmp);
	*str = (char *)malloc(sizeof(char) * (tmp_len + str_len + 1));
	if (*str)
	{
		while (i < tmp_len + str_len)
		{
			if (i < tmp_len)
				(*str)[i] = tmp[i];
			else
				(*str)[i] = now->buff_cur[i - tmp_len];
			i++;
		}
		(*str)[i] = '\0';
		free(tmp);
	}
	now->buff_cur += (i - tmp_len);
	if (!str)
		return (0);
	return (1);
}
