/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiwolee <jiwolee@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 23:34:24 by jiwolee           #+#    #+#             */
/*   Updated: 2022/03/02 23:34:55 by jiwolee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"get_next_line.h"

char	*get_next_line(int fd)
{
	static t_gnl_list	head;
	t_gnl_list			*list;
	int					a;
	char				*str;

	a = 1;
	str = NULL;
	list = NULL;
	if (fd >= 0)
	{
		list = gnl_get_fdlist(fd, &head);
		while (list && a > 0)
			a = gnl_read_fd(fd, list, &str);
		if (a == 0 || a == -1)
			gnl_free_list(list, &head);
	}
	return (str);
}

int	gnl_read_fd(int fd, t_gnl_list	*list, char **str)
{
	int		i;

	i = 1;
	if (list->buff_cur == NULL)
	{
		i = read(fd, list->buff, BUFFER_SIZE);
		if (i == 0 || i == -1)
			return (i);
		else
			list->buff_cur = list->buff;
	}
	i = gnl_read_buffer(str, list);
	return (i);
}

int	gnl_read_buffer(char **str, t_gnl_list *list)
{
	size_t	len;
	size_t	i;
	int		rtn;

	i = 0;
	len = 0;
	rtn = 1;
	while (list->buff_cur[len] != 0 && list->buff_cur[len] != '\n')
		len++;
	if (list->buff_cur[len] == '\n')
	{
		rtn = -2;
		len++;
	}
	gnl_strjoin(str, len, list);
	if (list->buff_cur[0] == 0)
	{
		gnl_memset(list->buff, 0, BUFFER_SIZE);
		list->buff_cur = NULL;
	}
	return (rtn);
}

int	gnl_strjoin(char **str, size_t str_len, t_gnl_list *list)
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
				(*str)[i] = list->buff_cur[i - tmp_len];
			i++;
		}
		(*str)[i] = '\0';
		free(tmp);
	}
	list->buff_cur += (i - tmp_len);
	if (!str)
		return (0);
	return (1);
}
