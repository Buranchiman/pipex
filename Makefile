#####################################PUSH_SWAP#####################################

NAME        = pipex
NAME_BONUS  = pipex_bonus

#####################################SRCS#########################################

PATH_SRCS       = srcs/
PATH_SRCS_BONUS = srcs_bonus/

SRCS        = pipex.c
UTILS_SRCS		+= pipex_utils.c
UTILS_SRCS		+= pipex_cmd.c
SRCS_BONUS  = pipex_bonus.c

vpath %.c $(PATH_SRCS)
vpath %.c $(PATH_SRCS_BONUS)

###################################OBJS###########################################

PATH_OBJS       = objs
PATH_OBJS_BONUS = objs_bonus

OBJS        = $(patsubst %.c, $(PATH_OBJS)/%.o, $(SRCS))
OBJS_BONUS  = $(patsubst %.c, $(PATH_OBJS_BONUS)/%.o, $(SRCS_BONUS))
UTILS       = $(patsubst %.c, $(PATH_OBJS)/%.o, $(UTILS_SRCS))

################################COMPILATION#######################################

INCLUDE     = -Ilibft/includes -Iinclude
LINK        = libft/libft.a
CFLAGS      = -Wall -Wextra -Werror -g3

##################################RULES###########################################

all: libft $(NAME)

libft:
	${MAKE} -sC libft

$(NAME): $(OBJS) $(UTILS)
	$(CC) $(CFLAGS) -o $(NAME) $(UTILS) $(OBJS) $(INCLUDE) $(LINK)

$(NAME_BONUS): $(OBJS_BONUS) $(UTILS)
	$(CC) $(CFLAGS) -o $(NAME_BONUS) $(UTILS) $(OBJS_BONUS) $(INCLUDE) $(LINK)

$(UTILS): $(PATH_OBJS)/%.o: $(PATH_SRCS)/%.c
	mkdir -p $(PATH_OBJS)
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDE)

$(OBJS): $(PATH_OBJS)/%.o: %.c
	mkdir -p $(PATH_OBJS)
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDE)

$(OBJS_BONUS): $(PATH_OBJS_BONUS)/%.o: %.c
	mkdir -p $(PATH_OBJS_BONUS)
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDE)

bonus: libft $(NAME_BONUS)

clean:
	rm -rf $(PATH_OBJS)

cleanbonus:
	rm -rf $(PATH_OBJS_BONUS)

fclean: clean
	rm -rf $(NAME)

fcleanbonus: cleanbonus
	rm -rf $(NAME_BONUS)

re: fclean all

rebonus: fcleanbonus bonus

.PHONY: all libft clean fclean re
