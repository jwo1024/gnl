#include	"get_next_line_bonus.h"

t_gnl_list	*gnl_get_fdlist(int fd, t_gnl_list *gnl_head)
{
	t_gnl_list	*rtn;

	rtn = NULL;
	rtn = gnl_find_fdlist(fd, gnl_head);
	if (rtn != NULL)
		return (rtn);
	rtn = (t_gnl_list *)malloc(sizeof(t_gnl_list));
	if (rtn)
	{
		rtn = gnl_memset(rtn, 0, sizeof(t_gnl_list));
		rtn->gnl_fd = fd;
		rtn->buff = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
		if (rtn->buff != NULL)
		{
			while (gnl_head->next != NULL)
				gnl_head = gnl_head->next;
			gnl_head->next = rtn;
		}
		else
		{
			free(rtn);
			return (NULL);
		}
	}
	return (rtn);
}

t_gnl_list	*gnl_find_fdlist(int fd, t_gnl_list *gnl_head)
{
	t_gnl_list	*tmp;

	tmp = gnl_head;
	while (tmp->next != NULL)
	{
		tmp = tmp->next;
		if (tmp->gnl_fd == fd)
			return (tmp);
	}
	return (NULL);
}

int	gnl_free_list(t_gnl_list *now, t_gnl_list *gnl_head)
{
	t_gnl_list	*tmp;

	tmp = gnl_head;
	while (tmp->next != NULL)
	{
		if (tmp->next->gnl_fd == now->gnl_fd)
		{
			free(now->buff);
			tmp->next = now->next;
			free(now);
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

size_t	gnl_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s && s[i])
		i++;
	return (i);
}

void	*gnl_memset(void *b, int c, size_t len)
{
	unsigned char	*str;
	size_t			i;

	str = b;
	i = 0;
	while (i < len)
		str[i++] = (unsigned char)c;
	return (str);
}
