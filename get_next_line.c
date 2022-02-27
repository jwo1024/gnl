
#include	"get_next_line.h"

char	*get_next_line(int fd)
{
	t_gnl_list *now;
	int		a;
	char	*str;
	
	a = 0;
	str = NULL;
	now = NULL;
	if (fd > 0)
	{
		now = gnl_get_fdlist(fd);
		while (now != NULL && a) //(now && (now->buff == now->buff_cur || (now->buff_cur != NULL && now->buff_cur[- 1] != '\n'))) //buff[0] ==  buff_cur[0] 일때 어떻게 주의하지? 
			a = gnl_read_fd(fd, now, &str);
	}
	return (str);
}

///////////////////////////////
int	gnl_read_fd(int fd, t_gnl_list	*now, char **str)
{
	int		i;

	*str = NULL;
	i = 0;
	if (now->buff_cur == NULL) // 여기서 재생? nono,,
	{
		i = read(fd, now->buff, BUFFER_SIZE);
		if (i == -1) //read 실패
		{
			if (now)
				gnl_free_list(now);
			return (i); // + 메모리 해제해야함.. !!!!!!!!!!!! new ego 적대적인 고난과 슬픔 널 더 팝인 진화 시켜 u wiwi fallow watch me workit worit out
		}
		now->buff_cur = now->buff;
	}
	gnl_strjoin(str, now);
	if (i == 0)
		gnl_free_list(now);
	return (i); //다시 돌아가는거 어떻게 하지?
}


int	gnl_strjoin(char **str, t_gnl_list *now)
{
	int		len;
	int		i;
	char	*tmp;
	int		tmp_len; //size_t

	len = 0;
	while (now->buff_cur[len] != 0 && now->buff_cur[len] != '\n') // 0과 \n을 구분하는법,,? // \n 일때랑 '\0'일때랑 저장되는 str에는 차이가 있을듯
		len++;
	if (now->buff_cur[len] == '\n')
		len++;
	tmp = *str; // NULL 일때
	tmp_len = gnl_strlen(tmp);
	*str = (char *)malloc(sizeof(char) * (tmp_len + len + 1)); // len - 1 을 해야겟지?
	if (*str)
	{
		i = 0;
		while (i < tmp_len)
		{
			(*str)[i] = tmp[i];
			i++;
		}
		while (i < tmp_len + len)
		{
			(*str)[i] = now->buff_cur[i - tmp_len]; // i == 0; tmp_len == 0 ; 
			i++;
		}
		(*str)[i] = '\0';
		free(tmp);
	}
	if (now->buff_cur == 0)
	{
		gnl_memset(now->buff, 0, BUFFER_SIZE); // clear buffer 상위함수 i==0일때랑 겹치나?
		now->buff_cur = NULL; ///<-
	}
	else
		now->buff_cur += (i - tmp_len); //\n
	return (1); // 설정 ;; 
}

int	gnl_free_list(t_gnl_list *now)
{
	t_gnl_list	*tmp;

	tmp = &gnl_head;
	while (tmp->next != NULL)
	{
		if (tmp->next->gnl_fd == now->gnl_fd)
		{
			tmp->next = now->next;
			free(now);
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
	while (s && s[i++])
		i++;
	return (i);
}

//////////////////////////////









////////////////////////////
t_gnl_list	*gnl_get_fdlist(int fd)
{
	t_gnl_list	*rtn;
//	t_gnl_list	*tmp;

	rtn = NULL;
	fd++;
//	rtn = gnl_find_fdlist(fd);
/*	if (rtn == NULL) //여기서 만든 gnl_list leak 발생;
	{
		rtn = (t_gnl_list *)malloc(sizeof(t_gnl_list)); // 배열이 아니니까 + 1 안해도 괜찮것지 대신 memset 으로 
		if (rtn)
		{
			rtn = gnl_memset(rtn, 0, sizeof(t_gnl_list));
			rtn->gnl_fd = fd;
			//
			tmp = &gnl_head;
			while (tmp->next != NULL)
				tmp = tmp->next;
			//
			tmp->next = rtn;
		}
	}
*/
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