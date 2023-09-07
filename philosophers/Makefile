NAME = philo
CC	= gcc
SRC = main.c dinner.c initialization.c time_warp.c printing.c monitoring.c

OBJFILES = $(SRC:%.c=%.o)
# B_OBJFILES = $(B_SRC:%.c=%.o)
CFLAGS = -Wall -Wextra -Werror

# ifdef BON
# OBJ = $(OBJFILES) $(B_OBJFILES)
# else
OBJ = $(OBJFILES)
# endif

all	: $(NAME)

$(NAME) : $(OBJ)
	$(CC) $(OBJ) -o $(NAME) 
	
# bonus: 
# 	make all BON=1

clean:
	rm -f $(OBJFILES)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: clean fclean all