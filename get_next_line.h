/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiwolee <jiwolee@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 23:35:14 by jiwolee           #+#    #+#             */
/*   Updated: 2022/03/02 23:35:35 by jiwolee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include	<unistd.h>
# include	<stdlib.h>

typedef struct s_gnl_list
{
	int					gnl_fd;
	char				*buff;
	char				*buff_cur;
	struct s_gnl_list	*next;
}	t_gnl_list;

char		*get_next_line(int fd);
int			gnl_read_fd(int fd, t_gnl_list	*list, char **str);
int			gnl_read_buffer(char **str, t_gnl_list *list);
int			gnl_free_list(t_gnl_list *list, t_gnl_list *head);
size_t		gnl_strlen(const char *s);
t_gnl_list	*gnl_get_fdlist(int fd, t_gnl_list *head);
t_gnl_list	*gnl_find_fdlist(int fd, t_gnl_list *head);
void		*gnl_memset(void *b, int c, size_t len);
int			gnl_strjoin(char **str, size_t str_len, t_gnl_list *list);

#endif
