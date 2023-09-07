/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-meij <bde-meij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 15:10:38 by bde-meij          #+#    #+#             */
/*   Updated: 2023/01/17 15:49:48 by bde-meij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	check_input(int ac, char **av)
{
	char	c;
	int		i;

	i = 0;
	if ((ac < 5) || (ac > 6))
		return (1);
	ac--;
	while (ac > 0)
	{
		while (av[ac][i])
		{
			c = av[ac][i];
			if (c < '0' || c > '9')
				return (2);
			i++;
		}
		i = 0;
		if (ft_atoi(av[ac]) < 1)
			return (3);
		ac--;
	}
	return (0);
}

int	make_threads(t_struct *the_struct)
{
	int	i;

	i = 0;
	while (i < the_struct->n_philos)
	{
		if (pthread_create(&the_struct->philos[i].own_thread, NULL, \
			&dinner, &the_struct->philos[i]))
			return (print_error(6), 1);
		i++;
	}
	return (0);
}

void	clean_up(t_struct *the_struct)
{
	int	i;

	i = 0;
	while (i < the_struct->n_philos)
	{
		pthread_join(the_struct->philos[i].own_thread, NULL);
		pthread_mutex_destroy(the_struct->philos[i].l_fork);
		i++;
	}
	pthread_mutex_destroy(&the_struct->meal_mutex);
	pthread_mutex_destroy(&the_struct->mealtime_mutex);
}

int	main(int ac, char **av)
{
	t_struct	*the_struct;
	int			i;

	i = check_input(ac, av);
	if (i > 0)
		return (print_error(i), i);
	the_struct = malloc(sizeof(t_struct));
	if (!the_struct)
		return (print_error(4), 1);
	if (initialization(the_struct, av))
		return (1);
	if (make_threads(the_struct))
		return (1);
	monitoring(the_struct);
	clean_up(the_struct);
	return (0);
}
