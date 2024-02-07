/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allTests.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 10:32:11 by bazaluga          #+#    #+#             */
/*   Updated: 2024/02/07 02:27:02 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf-breaker.h"
#include <sys/mman.h>
#include <fcntl.h>

/****************************/
/*        RUN TESTS         */
/****************************/

void	map_stdout(char opt, size_t size, size_t start, char *res)
{
	static int	fdout = -1;
	static int	fdmap = -1;
	char		map_name[] = "/map_out";
	char		*map;

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
			close(STDOUT_FILENO);
			dup2(fdmap, STDOUT_FILENO);
		}
	}
	else if (opt == 2)
	{
		map = (char *)mmap(NULL, size, PROT_READ, MAP_PRIVATE, fdmap, start);
		if (map == MAP_FAILED)
		{
			perror("Error with mmap");
			exit(1);
		}
		strncpy(res, map, size);
		munmap(map, size);
		map = NULL;
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
		map = (char *)mmap(NULL, size, PROT_READ, MAP_PRIVATE, fdmap, start);
		if (map == MAP_FAILED)
		{
			perror("Error with mmap");
			exit(1);
		}
		strncpy(res, map, size);
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
	/* char	res[9]; */
	/* char	res2[10]; */

	/* bzero(res, 9); */
	/* bzero(res2, 10); */
	/* printf("coucou1\n"); */
	/* fflush(stdout); */
	/* map_stdout(1, 8, res); */
	/* printf("coucou2\n"); */
	/* fflush(stdout); */
	/* map_stdout(0, 8, res); */
	/* printf("coucou3\n"); */
	/* fflush(stdout); */
	/* map_stdout(1, 9, res2); */
	/* printf("coucou23\n"); */
	/* fflush(stdout); */
	/* map_stdout(0, 9, res2); */
	/* printf("middle = %s", res); */
	/* printf("end = %s", res2); */
	/* fflush(stdout); */
	char *res = calloc(2147483648, sizeof(char));
	char *res2 = calloc(2147483648, sizeof(char));
	if (!res || !res2)
	{
		perror("calloc error.");
		exit(1);
	}
	map_stdout(1, 2147483647, 0, NULL);
	printf("%02147483647d", 23);
	fflush(stdout);
	bzero(res, 2147483648);
	map_stdout(2, 2147483647, 0, res);
	/* map_stdout(1, 2147483647, NULL); */
	printf("%02147483647d", 23);
	fflush(stdout);
	bzero(res2, 2147483648);
	map_stdout(0, 2147483647, 2147483647, res2);
	if (strncmp(res, res2, 2147483647))
		printf("Not the same !!!\n");
	else
		printf("The same !!!\n");
	/* int fd = open("tmp", O_WRONLY | O_CREAT, 0666); */
	/* write(fd, res, 2147483648); */
	/* close(fd); */
	free(res);
	free(res2);
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
