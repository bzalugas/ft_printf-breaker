/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allTests.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 10:32:11 by bazaluga          #+#    #+#             */
/*   Updated: 2024/02/02 10:32:32 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/lftest.h"

/****************************/
/*        RUN TESTS         */
/****************************/

void	run_all(char *particular_fun, void *fun)
{
	CuString	*output = CuStringNew();
	CuSuite		*suite = CuSuiteNew();
	int			i;

	if (particular_fun)
		CuSuiteAddSuite(suite, ((CuSuite *(*)(void))fun)());
	else
	{
		for (i = 0; i < N_TESTS_FUN; i++)
			if (fcts[i].fun_name && fcts[i].fun)
				if (fcts[i].test_fun)
					CuSuiteAddSuite(suite, ((CuSuite * (*)(void))fcts[i].test_fun)());
	}
	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	if (particular_fun)
		printf("\n\n%s: ", particular_fun);
	else
		printf("\n\nlibft: ");
	for (i = 0; output->buffer[i] && output->buffer[i] != '\n'; i++)
	{
		if (output->buffer[i] == '.')
			printf("%s%c%s", ANSI_COLOR_GREEN, output->buffer[i], ANSI_COLOR_RESET);
		else
			printf("%s%c%s", ANSI_COLOR_RED, output->buffer[i], ANSI_COLOR_RESET);
	}
	if (!strncmp(&output->buffer[i], "\n\nOK", 3))
		printf("%s%s%s",ANSI_COLOR_GREEN, &output->buffer[i], ANSI_COLOR_RESET);
	else
		printf("%s%s%s\n",ANSI_COLOR_RED, &output->buffer[i], ANSI_COLOR_RESET);
	if (!particular_fun)
		for (i = 0; i < N_TESTS_FUN; i++)
			if (fcts[i].fun_name && !fcts[i].fun)
				printf("%sMISSING %s.%s\n", ANSI_COLOR_RED, fcts[i].fun_name, ANSI_COLOR_RESET);
	CuStringDelete(output);
	CuSuiteDelete(suite);
}

int	main(int argc, char *argv[])
{
	char	*fun_name = NULL;
	void	*lib;
	void	*test_fun;

	if (argc > 1)
		fun_name = strdup(argv[1]);
	lib = init_fcts();
	printf("\n\n");
	test_fun = get_test_fun(fun_name);
	if (fun_name && test_fun && !get_fun(fun_name))
		printf("%sfunction %s missing.\n%s", ANSI_COLOR_RED,fun_name, ANSI_COLOR_RESET);
	else if (fun_name && test_fun)
		run_all(fun_name,test_fun);
	else if (fun_name)
		printf("%sfunction %s not found.\n%s", ANSI_COLOR_RED,fun_name, ANSI_COLOR_RESET);
	else
		run_all(NULL, NULL);
	dlclose(lib);
	return (0);
}
