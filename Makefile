#####################################PUSH_SWAP#####################################

NAME	=	pipex
NAME_BONUS	=	pipex_bonus

#####################################SRCS########################################

PATH_SRCS	=	srcs/

SRCS	+=	pipex.c

vpath	%.c	$(PATH_SRCS)

PATH_SRCS_BONUS	=	srcs_bonus/

vpath	%.c	$(PATH_SRCS_BONUS)

###################################OBJS#########################################

PATH_OBJS	=	objs
OBJS	=	$(patsubst	%.c,	$(PATH_OBJS)/%.o,	$(SRCS))
MAIN	=	$(patsubst	%.c,	$(PATH_OBJS)/%.o,	main.c)

PATH_OBJS_BONUS	=	objs_bonus
OBJS_BONUS	=	$(patsubst	%.c,	$(PATH_OBJS_BONUS)/%.o,	$(SRCS_BONUS))

################################COMPILATION####################################

INCLUDE	=	-Ilibft/includes	-Iinclude

LINK	=	libft/libft.a

CC		=	clang
CFLAGS	=	-Wall	-Wextra	-Werror -g3

##################################RULES#######################################

all:		libft	$(NAME)

libft:
	${MAKE}	-sC	libft

$(NAME):	$(OBJS)	$(MAIN)
	$(CC)	$(CFLAGS)	-o	$(NAME)	$(OBJS)	objs/main.o $(INCLUDE)	$(LINK)

$(MAIN):
	$(CC)	$(CFLAGS)	-c	srcs/main.c	-o	objs/main.o	$(INCLUDE)

$(OBJS):	$(PATH_OBJS)/%.o:%.c
	mkdir	-p	$(PATH_OBJS)
	$(CC)	$(CFLAGS)	-c	$<	-o	$@	$(INCLUDE)

bonus:	$(NAME_BONUS)

$(NAME_BONUS):	$(OBJS)	$(OBJS_BONUS)
	$(CC)	$(CFLAGS)	-o	$(NAME_BONUS)	$(OBJS)	$(OBJS_BONUS)	$(INCLUDE)	$(LINK)

$(OBJS_BONUS):	$(PATH_OBJS_BONUS)/%.o:%.c
	mkdir	-p	$(PATH_OBJS_BONUS)
	$(CC)	$(CFLAGS)	-c	$<	-o	$@	$(INCLUDE)

clean:
	rm	-rf	$(PATH_OBJS)

fclean:	clean
	rm	-rf	$(NAME)

re:	fclean
	${MAKE}

cleanbonus:
	rm	-rf	$(PATH_OBJS)
	rm	-rf	$(PATH_OBJS_BONUS)

fcleanbonus: cleanbonus
	rm	-rf	$(NAME_BONUS)

rebonus: fcleanbonus bonus

.PHONY:	all	libft	clean	fclean	re