/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lftest.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 10:31:56 by bazaluga          #+#    #+#             */
/*   Updated: 2024/02/06 22:43:27 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */

#ifndef LFTEST_H
# define LFTEST_H
# ifdef __linux__
#  define _GNU_SOURCE
#  include <bsd/string.h>
# endif
# include "CuTest.h"
# include <stdio.h>
# include <ctype.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <errno.h>
# include <sys/wait.h>
# include <limits.h>
# include <dlfcn.h>
# include <stdarg.h>
# define BUFFSIZE 30
# define BUFFBSIZE 2048
# define BUFFBBSIZE 8192
# define ANSI_COLOR_RED     "\033[31m"
# define ANSI_COLOR_GREEN   "\033[32m"
# define ANSI_COLOR_RESET	"\033[0m"
# define N_TESTS_FUN 0

/********************************** STRUCTS ***********************************/

typedef struct	s_ft
{
	char	*fun_name;
	void	*fun;
	void	*test_fun;
}				t_ft;

typedef struct	print_buff
{
	char	color;
	char	txt[BUFFBBSIZE];
}				print_buff;

typedef struct	s_malloc t_malloc;
struct	s_malloc
{
	unsigned long	addr;
	size_t			size;
	size_t			rank;
	t_malloc		*next;
};

typedef struct	s_leaks_tracer t_leaks_tracer;
struct	s_leaks_tracer
{
	t_malloc	*first_malloc;
	int			count;
	int			in_use;
};

/****************************** GLOBAL VARIABLES ******************************/

extern int				g_exit_code;
extern pid_t			g_pid;
extern int				g_malloc_fail;
extern size_t			g_last_malloc_size;
extern t_leaks_tracer	g_leaks;
extern char				*g_leaks_text;
extern t_ft				fcts[N_TESTS_FUN];
extern print_buff		buff;
extern int				g_in_fun;
extern int				fds[2];
extern char				pipe_buff[BUFFSIZE];
extern int				g_free_called;

/*********************************** MACROS ***********************************/

/* Testing mallocs */
# define FAIL_MALLOC g_malloc_fail = 1;
# define END_FAIL g_malloc_fail = 0;
# define LEAKS_TRACER_START if (g_leaks_text) {	\
		free(g_leaks_text);						\
		g_leaks_text = NULL;}					\
	leaks_tracer_reset(&g_leaks);				\
	leaks_tracer_start(&g_leaks);
# define LEAKS_TRACER_STOP leaks_tracer_stop(&g_leaks);
# define LEAKS_TRACER_CONTINUE leaks_tracer_start(&g_leaks);
# define LEAKS_TRACER_RESULT LEAKS_TRACER_STOP;						\
	g_leaks_text = leaks_tracer_text(&g_leaks);						\
	leaks_tracer_reset(&g_leaks);
# define LEAKS_OK g_leaks_text == NULL

/* Getting fds & protecting stdout + stderr */
# define CLOSE_OUTPUTS manage_outputs(1);
# define OPEN_OUTPUTS manage_outputs(0);
# define OPEN_PIPE manage_pipes(1, 0);
# define CLOSE_PIPE manage_pipes(0, 1);

/* Preventing tester from crashs */
# define SANDBOX(X)								\
	if ((g_pid = fork()) == -1){				\
		perror("Error during fork");			\
		exit(EXIT_FAILURE);						\
	}											\
	if (!g_pid){								\
		X;										\
		exit(EXIT_SUCCESS);						\
	}											\
	wait(&g_exit_code);

/*********************** MALLOCS, FREE & LEAKS FUNCTIONS **********************/

void		leaks_tracer_start(t_leaks_tracer *lst);
void		leaks_tracer_stop(t_leaks_tracer *lst);
char		*leaks_tracer_text(t_leaks_tracer *lst);
void		leaks_tracer_reset(t_leaks_tracer *lst);
t_malloc	*leaks_tracer_find_by_addr(t_leaks_tracer *lst, void *addr);

/****************************** UTILS FUNCTIONS *******************************/

void	*init_fcts();
void	*get_fun(char *name);
void	*get_test_fun(char *name);
void	add_txt_buff(char *txt);
void	add_color_buff(char c);
void	printbuff();
void	manage_outputs(int to_close);
void	manage_pipes(int get_read, int get_write);
char	*ft_strjoin(char const *s1, char const *s2);

/****************************** TESTS FUNCTIONS *******************************/

#endif
