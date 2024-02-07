/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allTests.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 10:32:11 by bazaluga          #+#    #+#             */
/*   Updated: 2024/02/07 03:15:18 by bazaluga         ###   ########.fr       */
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

	if (opt == 1)
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
	else if (opt == 2)
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
	else if (opt == 3)
	{
		close(STDOUT_FILENO);
		close(fdmap);
		fdmap = -1;
		dup2(fdout, STDOUT_FILENO);
		close(fdout);
		fdout = -1;
		shm_unlink(map_name);
	}
	else
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

void	test_ft_printf(int check_real, char *expected, char *str, ...)
{
	char res[8];
	bzero(res, 8);
	map_stdout(1, 8, 0, NULL);
	printf("a");
	fflush(stdout);
	/* map_stdout(2, 1, 0, res); */
	printf("b");
	fflush(stdout);
	/* map_stdout(2, 1, 1, res); */
	printf("c");
	fflush(stdout);
	/* map_stdout(2, 1, 2, res); */
	printf("d");
	fflush(stdout);
	/* map_stdout(2, 1, 3, res); */
	printf("e");
	fflush(stdout);
	/* map_stdout(2, 1, 4, res); */
	map_stdout(0, 7, 0, res);
	printf("%s.\n", res);
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
