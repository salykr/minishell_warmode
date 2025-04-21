NAME = libft.a
SRCS = ft_atoi.c \
	ft_bzero.c \
	ft_isalnum.c \
	ft_isalpha.c \
	ft_isascii.c \
	ft_isdigit.c \
	ft_isprint.c \
	ft_memchr.c \
	ft_memcmp.c \
	ft_memcpy.c \
	ft_memmove.c \
	ft_memset.c \
	ft_strcmp.c \
	ft_strchr.c \
	ft_strncat.c \
	ft_strlcat.c \
	ft_strlcpy.c \
	ft_strlen.c \
	ft_strtoll.c\
	ft_strncmp.c \
	ft_strnstr.c \
	ft_strrchr.c \
	ft_tolower.c \
	ft_toupper.c \
	ft_calloc.c \
	ft_strdup.c \
	ft_putchar_fd.c \
	ft_putendl_fd.c \
	ft_putnbr_fd.c \
	ft_putstr_fd.c \
	ft_split.c \
    ft_itoa.c \
	ft_strtrim.c \
	ft_substr.c \
	ft_striteri.c \
	ft_strjoin.c \
	ft_strmapi.c \
	ft_realloc.c \
	ft_strcat.c\
	ft_strcpy.c\
	ft_strncpy.c\
	ft_strstr.c \
	ft_strpbrk.c \
	ft_strndup.c\
	ft_strtok.c\
	ft_isspace.c\
	ft_str_join.c 


OBJS = $(SRCS:.c=.o)

CC = gcc
CFLAGS = -Wall -Wextra -Werror
RM = rm -f

.PHONY: all clean fclean re bonus

all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)

bonus: $(OBJS) 
	ar rcs $(NAME) $(OBJS) 

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS) 

fclean: clean
	$(RM) $(NAME)

re: fclean all