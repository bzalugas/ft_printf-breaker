/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests_c.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 10:35:14 by bazaluga          #+#    #+#             */
/*   Updated: 2024/02/06 11:16:13 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_printf-breaker.h"



CuSuite	*tests_c_get_suite()
{
	CuSuite	*s = CuSuiteNew();

	return (s);
}
