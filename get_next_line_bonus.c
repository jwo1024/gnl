
#include	"get_next_line_bonus.h"

/*
#include	<fcntl.h>
#include	<stdio.h>

int	main(void)
{
	int	a;
	char	*s;

	a = open("tst.txt", O_RDONLY);
	s = get_next_line(a);
	printf("result @%s@\n", s);
	//s = get_next_line(a);
	//printf("result @%s@\n", s);
	return (0);
}
*/


char	*get_next_line(int fd)
{
	t_gnl_list *now;
	int		a;
	char	*str;

	a = 1;
	str = NULL;
	now = NULL;
	if (fd > 0)
	{
		now = gnl_get_fdlist(fd);
	//	printf("%s\n", now->buff);
		while (now && a > 0) //(now && (now->buff == now->buff_cur || (now->buff_cur != NULL && now->buff_cur[- 1] != '\n'))) //buff[0] ==  buff_cur[0] 일때 어떻게 주의하지? 
			a = gnl_read_fd(fd, now, &str);
	}
	return (str);
}

///////////////////////////////
int	gnl_read_fd(int fd, t_gnl_list	*now, char **str)
{
	int		i;

	i = 1;
//	printf("\n++\n");
	if (now->buff_cur == NULL) // 버퍼를 다 읽어서 buff_cur이 초기화 되었을경우
	{
		i = read(fd, now->buff, BUFFER_SIZE);
//		printf("read i = %d\n", i);
		if (i == 0 || i == -1) // eof || read 실패 시
		{
			gnl_free_list(now);
			return (i);
		}
		else
			now->buff_cur = now->buff;
	}
	i = gnl_strjoin(str, now);
//	printf("->i = %d\n", i);
	return (i); //다시 돌아가는거 어떻게 하지? // 양수일 경우에 이 함수 반복
}



int	gnl_strjoin(char **str, t_gnl_list *now)
{
	int		len;
	int		i;
	int		rtn;
	char	*tmp;
	int		tmp_len; //size_t

	i = 0;
	len = 0;
	rtn = 1;
	while (now->buff_cur[len] != 0 && now->buff_cur[len] != '\n')
		len++;
	if (now->buff_cur[len] == '\n')
	{
		rtn = -2;
		len++;
	}
	tmp = *str; // NULL 일때?
	tmp_len = gnl_strlen(tmp); // NULL = 0 , 1개라도 있으면 2개부터 시작하겟지
	*str = (char *)malloc(sizeof(char) * (tmp_len + len + 1)); // len - 1 을 해야겟지?
	if (*str)
	{
		while (i < tmp_len)
		{
			(*str)[i] = tmp[i];
			i++;
		}
		while (i < tmp_len + len)
		{
			(*str)[i] = now->buff_cur[i - tmp_len];
			i++;
		}
		(*str)[i] = '\0';
		free(tmp);
	}
	now->buff_cur += (i - tmp_len); //\n
	if (now->buff_cur[0] == 0) //buff에 들은 str을 끝까지 다 저장했을때
	{
		gnl_memset(now->buff, 0, BUFFER_SIZE);
		now->buff_cur = NULL;
	//	rtn = 1;
	}
	return (rtn);
}



int	gnl_free_list(t_gnl_list *now)
{
	t_gnl_list	*tmp;

	tmp = &gnl_head;
	while (tmp->next != NULL)
	{
		if (tmp->next->gnl_fd == now->gnl_fd)
		{
			free(now->buff);
			tmp->next = now->next;
			free(now);
	//		printf("free ok?\n");
			return (1); //굳이 반환값이 필요한가?
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

//////////////////////////////





////////////////////////////
t_gnl_list	*gnl_get_fdlist(int fd)
{
	t_gnl_list	*rtn;
	t_gnl_list	*tmp;

	rtn = NULL;
	fd++;
	rtn = gnl_find_fdlist(fd);
	if (rtn == NULL) //여기서 만든 gnl_list leak 발생;
	{
		rtn = (t_gnl_list *)malloc(sizeof(t_gnl_list)); // 배열이 아니니까 + 1 안해도 괜찮것지 대신 memset 으로 
		if (rtn)
		{
			rtn = gnl_memset(rtn, 0, sizeof(t_gnl_list));
			rtn->gnl_fd = fd;
			rtn->buff = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
			if (rtn->buff != NULL)
			{
				tmp = &gnl_head;
				while (tmp->next != NULL)
					tmp = tmp->next;
				tmp->next = rtn;
			}
			else
			{
				free(rtn);
				return (NULL);
			}
		}
	}
	return (rtn);
}

t_gnl_list	*gnl_find_fdlist(int fd)
{
	t_gnl_list	*tmp;

	tmp = &gnl_head;
	while(tmp->next != NULL)
	{
		tmp = tmp->next;
		if (tmp->gnl_fd == fd)
			return (tmp);
	}
	return (NULL);
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
////////////////////////////