#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/02 10:23:20 by bazaluga          #+#    #+#              #
#    Updated: 2024/02/02 13:01:30 by bazaluga         ###   ########.fr        #
#                                                                              #
#******************************************************************************#

################################### VARIABLES ##################################

PRINTFDIR	=	../ft_printf

PRINTFSRC	=	../ft_printf/src

PRINTFINCL	=	../ft_printf/includes

PRINTF		=	$(PRINTFDIR)/libftprintf.so

PRINTFSO	=	obj/libftprintf.so

TESTRULE	=	breaker

NAME 		=	run-breaker

SRCN 		=	$(shell find * -type f -name "*.c" ! -name "*static*")

SRCS 		=	$(shell find * -type f -name "*static*.c") src/CuTest.c src/utils.c

MALLOC		=	src/malloc.c

LMALLOC		=	obj/libmalloc.so

CC	 		=	cc

CFLAGS		=	-Wall -Wextra -Werror

ifeq ($(shell uname), Darwin)
	MACOS = 1
	LIBINCLUDES = -Lobj -lmalloc
	LIBFLAGS = -dynamiclib
else
	LIBINCLUDES = $(LMALLOC) -lbsd
	LIBFLAGS = -shared -nostartfiles -fPIC -ldl
endif

ANSI_COLOR_RED		=	"\033[31m"
ANSI_COLOR_GREEN	=	"\033[32m"
ANSI_COLOR_RESET	=	"\033[0m"

##################################### RULES #####################################

all:		$(NAME)

$(NAME):	$(LMALLOC) $(PRINTFSO) $(SRCN)
			$(CC) $(CFLAGS) -o $@ $(SRCN) $(LIBINCLUDES)

static:		$(LMALLOC) $(PRINTFSO) $(SRCS)
			$(CC) $(CFLAGS) -o $(NAME) $(SRCS) $(LIBINCLUDES)

run:		$(NAME)
			./$(NAME) $(filter-out $@, $(MAKECMDGOALS))

srun:		static
			./$(NAME) $(filter-out $@, $(MAKECMDGOALS))

$(LMALLOC):	$(MALLOC)
			mkdir -p obj
			$(CC) $(LIBFLAGS) -o $@ $<

ifdef MACOS
$(PRINTFSO):	$(PRINTFDIR)
			rm -rf ft_printf/*
			mkdir -p ft_printf
			cp $(PRINTFDIR)/* ./ft_printf
			make -C ft_printf/ $(PRINTFRULE)
			mv ft_printf/libftprintf.so $(PRINTFSO)
else
$(PRINTFSO):	$(PRINTFDIR)
			make -C $(PRINTFDIR) $(PRINTFRULE)
			cp $(PRINTF) $(PRINTFSO)
endif

$(PRINTFDIR):
			@echo $(ANSI_COLOR_RED)\
				PLEASE PROVIDE THE CORRECT PATH TO YOUR FT_PRINTF\
				$(ANSI_COLOR_RESET)
			@false

%:
	@true

clean:
			rm -f $(NAME) $(SRCN:.c=.o) $(SRCS:.c=.o)

fclean:		clean
			rm -rf obj ft_printf

re:			fclean all

.PHONY:		all clean force fclean static run srun $(PRINTFSO)
