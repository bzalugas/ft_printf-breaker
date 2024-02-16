/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allTests.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 10:32:11 by bazaluga          #+#    #+#             */
/*   Updated: 2024/02/16 17:31:12 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf-breaker.h"
#include <sys/mman.h>
#include <fcntl.h>

/****************************/
/*        RUN TESTS         */
/****************************/

void	map_stdout(char opt, size_t size, int start, char *res)
{
	static int	fdout = -1;
	static int	fdmap = -1;
	static char	map_name[] = "/map_out";
	static char	*map = NULL;

	if (opt == 1) // start mapping stdout
	{
		if (fdout == -1)
			fdout = dup(STDOUT_FILENO);
		if (fdmap == -1)
		{
			fdmap = shm_open(map_name, O_RDWR | O_CREAT, 0666);
			if (fdmap == -1)
			{
				perror("Error with shm_open");
				exit(1);
			}
			ftruncate(fdmap, size);
			map = (char *)mmap(NULL, size, PROT_WRITE, MAP_PRIVATE, fdmap, (off_t)start);
			if (map == MAP_FAILED)
			{
				perror("Error with mmap opt 0");
				exit(1);
			}
			bzero(map, size);
			munmap(map, size);
			map = NULL;
			close(STDOUT_FILENO);
			dup2(fdmap, STDOUT_FILENO);
		}
	}
	else if (opt == 2) // get result
	{
		if (!map)
		{
			map = (char *)mmap(NULL, size, PROT_READ, MAP_PRIVATE, fdmap, (off_t)start);
			if (map == MAP_FAILED)
			{
				perror("Error with mmap");
				exit(1);
			}
			strncpy(res, map, size);
		}
		else
			strncpy(res, &map[start], size);
	}
	else if (opt == 3) // close everything
	{
		close(STDOUT_FILENO);
		close(fdmap);
		fdmap = -1;
		dup2(fdout, STDOUT_FILENO);
		close(fdout);
		fdout = -1;
		shm_unlink(map_name);
	}
	else // get result & close everything
	{
		if (!map)
		{
			map = (char *)mmap(NULL, size, PROT_READ, MAP_PRIVATE, fdmap, (off_t)start);
			if (map == MAP_FAILED)
			{
				perror("Error with mmap opt 0");
				exit(1);
			}
			strncpy(res, map, size);
		}
		else
			strncpy(res, &map[start], size);
		munmap(map, size);
		map = NULL;
		close(STDOUT_FILENO);
		close(fdmap);
		fdmap = -1;
		dup2(fdout, STDOUT_FILENO);
		close(fdout);
		fdout = -1;
		shm_unlink(map_name);
	}
}

void	test_int_max_ft_printf(char *str, va_list args)
{
	char	*res1 = calloc((size_t)INT_MAX + 1, sizeof(char));
	char	*res2 = calloc((size_t)INT_MAX + 1, sizeof(char));
	va_list	args2;

	if (!res1 || !res2)
	{
		perror("calloc error.");
		exit(EXIT_FAILURE);
	}
	va_copy(args2, args);
	map_stdout(1, INT_MAX, 0, NULL);
	vprintf(str, args);
	fflush(stdout);
	map_stdout(0, INT_MAX, 0, res1);
	map_stdout(1, INT_MAX, 0, NULL);
	vprintf(str, args2);
	fflush(stdout);
	map_stdout(0, INT_MAX, 0, res2);
	if (strncmp(res1, res2, INT_MAX))
		printf("Strings are different.\n");
	else
		printf("Strings are the same.\n");
}

void	test_ft_printf(int check_real, char *expected, int int_max, const char *str, ...)
{
	va_list	args;
	va_start(args, str);
	if (int_max)
		test_int_max_ft_printf(str, args);
	else
	{
		char	res1[BUFFSIZE];
		char	res2[BUFFSIZE];

		map_stdout(1, BUFFSIZE * 2, 0, NULL);
		printf("coucou %d\n", 23);
		fflush(stdout);
		map_stdout(2, BUFFSIZE, 0, res1);
		printf("Bonjour %d\n", 23);
		fflush(stdout);
		map_stdout(0, BUFFSIZE, strlen(res1), res2);
		printf("I want to compare : \n1: %s\n2: %s\n", res1, res2);
	}
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
	test_ft_printf(0, NULL, 1, "%02147483647d", 23);
	return (0);
}
