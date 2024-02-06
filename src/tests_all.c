/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allTests.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 10:32:11 by bazaluga          #+#    #+#             */
/*   Updated: 2024/02/06 15:18:24 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf-breaker.h"

/****************************/
/*        RUN TESTS         */
/****************************/
int			fds[2];
char		pipe_buff[2147483648];
#include <fcntl.h>
#include <sys/statvfs.h>
void	pipe_stdout(int start)
{
	static int	stdout = -1;
	static int f = -1;
	if (start)
	{
		/* pipe_buff = (char *)(calloc(2147483648, sizeof(char))); */
		/* if (!pipe_buff) */
		/* { */
		/* 	perror("ERROR MALLOC\n"); */
		/* 	exit(EXIT_FAILURE); */
		/* } */
		stdout = dup(STDOUT_FILENO);
		struct statvfs stats;
		char *user = getenv("USER");
		char *path1 = ft_strjoin("/home/", user);
		char *path = ft_strjoin(path1, "/sgoinfre/");
		int ret = statvfs(path, &stats);
		if (!ret)
		{
			const unsigned int GB = 1024 * 1024 * 1024;
			if ((stats.f_bsize * stats.f_bavail)/GB >= 3)
			{
				write(2, "here\n", 5);
				f = open(ft_strjoin(path, "tmp"), O_CREAT | O_WRONLY, 0666);
				dup2(f, STDOUT_FILENO);
			}
		}


		/* close(STDOUT_FILENO); */
		/* if (pipe(fds) == -1) */
		/* { */
		/* 	perror("Pipe error."); */
		/* 	exit(EXIT_FAILURE); */
		/* } */
		/* dup2(fds[1], STDOUT_FILENO); */
	}
	else
	{
		close(f);
		/* bzero(pipe_buff, 2147483648); */
		/* close(fds[1]); */
		/* fds[1] = -1; */
		/* read(fds[0], pipe_buff, 2147483647); */
		/* close(fds[0]); */
		/* fds[0] = -1; */
		dup2(stdout, STDOUT_FILENO);
	}
}
void	test_ft_printf(int check_real, char *expected, char *str, ...)
{
	/* struct statvfs stats; */
	/* char *user = getenv("USER"); */
	/* char *path1 = ft_strjoin("/home/", user); */
	/* char *path = ft_strjoin(path1, "/sgoinfre"); */
	/* printf("%s\n", path); */
	/* int ret = statvfs(path, &stats); */
	/* if (!ret) */
	/* { */
	/* 	const unsigned int KB = 1024 * 1024 * 1024; */
	/* 	printf("block size = %lu\n", stats.f_bsize); */
	/* 	printf("Fragment size = %lu\n", stats.f_frsize); */
	/* 	printf("f_blocks = %lu\n", stats.f_blocks); */
	/* 	printf("free space = %lu\n", (stats.f_bsize * stats.f_bavail)/KB); */
	/* } */
	printf("coucou1\n");
	fflush(stdout);
	pipe_stdout(1);
	/* /\* printf("coucou2\n"); *\/ */
	printf("%02147483647d", 23);
	fflush(stdout);
	pipe_stdout(0);
	printf("coucou3\n");
	fflush(stdout);
	/* printf("%s", pipe_buff); */
}

/* void	run_all(char *particular_fun, void *fun) */
/* { */
/* 	CuString	*output = CuStringNew(); */
/* 	CuSuite		*suite = CuSuiteNew(); */
/* 	int			i; */

/* 	if (particular_fun) */
/* 		CuSuiteAddSuite(suite, ((CuSuite *(*)(void))fun)()); */
/* 	else */
/* 	{ */
/* 		for (i = 0; i < N_TESTS_FUN; i++) */
/* 			if (fcts[i].fun_name && fcts[i].fun) */
/* 				if (fcts[i].test_fun) */
/* 					CuSuiteAddSuite(suite, ((CuSuite * (*)(void))fcts[i].test_fun)()); */
/* 	} */
/* 	CuSuiteRun(suite); */
/* 	CuSuiteSummary(suite, output); */
/* 	CuSuiteDetails(suite, output); */
/* 	if (particular_fun) */
/* 		printf("\n\n%s: ", particular_fun); */
/* 	else */
/* 		printf("\n\nlibft: "); */
/* 	for (i = 0; output->buffer[i] && output->buffer[i] != '\n'; i++) */
/* 	{ */
/* 		if (output->buffer[i] == '.') */
/* 			printf("%s%c%s", ANSI_COLOR_GREEN, output->buffer[i], ANSI_COLOR_RESET); */
/* 		else */
/* 			printf("%s%c%s", ANSI_COLOR_RED, output->buffer[i], ANSI_COLOR_RESET); */
/* 	} */
/* 	if (!strncmp(&output->buffer[i], "\n\nOK", 3)) */
/* 		printf("%s%s%s",ANSI_COLOR_GREEN, &output->buffer[i], ANSI_COLOR_RESET); */
/* 	else */
/* 		printf("%s%s%s\n",ANSI_COLOR_RED, &output->buffer[i], ANSI_COLOR_RESET); */
/* 	if (!particular_fun) */
/* 		for (i = 0; i < N_TESTS_FUN; i++) */
/* 			if (fcts[i].fun_name && !fcts[i].fun) */
/* 				printf("%sMISSING %s.%s\n", ANSI_COLOR_RED, fcts[i].fun_name, ANSI_COLOR_RESET); */
/* 	CuStringDelete(output); */
/* 	CuSuiteDelete(suite); */
/* } */

int	main(int argc, char *argv[])
{
	/* char	*fun_name = NULL; */
	/* void	*lib; */
	/* void	*test_fun; */

	/* if (argc > 1) */
	/* 	fun_name = strdup(argv[1]); */
	/* lib = init_fcts(); */
	/* printf("\n\n"); */
	/* test_fun = get_test_fun(fun_name); */
	/* if (fun_name && test_fun && !get_fun(fun_name)) */
	/* 	printf("%sfunction %s missing.\n%s", ANSI_COLOR_RED,fun_name, ANSI_COLOR_RESET); */
	/* else if (fun_name && test_fun) */
	/* 	run_all(fun_name,test_fun); */
	/* else if (fun_name) */
	/* 	printf("%sfunction %s not found.\n%s", ANSI_COLOR_RED,fun_name, ANSI_COLOR_RESET); */
	/* else */
	/* 	run_all(NULL, NULL); */
	/* dlclose(lib); */
	test_ft_printf(0,NULL,"coucou, %d\n", 23);
	return (0);
}
