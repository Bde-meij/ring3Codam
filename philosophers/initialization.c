/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-meij <bde-meij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 14:35:16 by bde-meij          #+#    #+#             */
/*   Updated: 2023/01/18 09:45:07 by bde-meij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_atoi(const char *str)
{
	int	value;
	int	index;
	int	sign;

	value = 0;
	index = 0;
	sign = 1;
	while ((str[index] == 32) || (str[index] >= 9 && str[index] <= 13))
		index++;
	if ((str[index] == '-') || (str[index] == '+'))
	{
		if (str[index] == '-')
			sign = -1;
		index++;
	}
	while ((str[index] >= '0') && (str[index] <= '9'))
	{
		value *= 10;
		value += (str[index] - 48);
		index++;
	}
	return (value * sign);
}

int	init_mutexes(t_struct *the_struct)
{
	int	i;

	i = 0;
	while (i < the_struct->n_philos)
	{
		if (pthread_mutex_init(&the_struct->forks[i], NULL))
			return (1);
		i++;
	}
	if (pthread_mutex_init(&the_struct->meal_mutex, NULL))
		return (1);
	if (pthread_mutex_init(&the_struct->mealtime_mutex, NULL))
		return (1);
	if (pthread_mutex_init(&the_struct->deadlock_mutex, NULL))
		return (1);
	return (0);
}

void	init_philos(t_struct *the_struct)
{
	int	i;

	i = 0;
	while (i < the_struct->n_philos)
	{
		the_struct->philos[i].phil_num = i + 1;
		the_struct->philos[i].meals = the_struct->max_rounds;
		the_struct->philos[i].prev_mealtime = 0;
		the_struct->philos[i].the_struct = the_struct;
		the_struct->philos[i].l_fork = &the_struct->forks[i];
		the_struct->philos[i].r_fork = \
		&the_struct->forks[(i + 1) % the_struct->n_philos];
		i++;
	}
}

int	initialization(t_struct *the_struct, char **av)
{
	the_struct->n_philos = ft_atoi(av[1]);
	the_struct->die = ft_atoi(av[2]);
	the_struct->eat = ft_atoi(av[3]);
	the_struct->sleep = ft_atoi(av[4]);
	if (av[5])
		the_struct->max_rounds = ft_atoi(av[5]);
	else
		the_struct->max_rounds = -1;
	if ((the_struct->eat < 10) || (the_struct->sleep < 10))
		printf("Warning: Philo won't work properly \
with eat or sleep time below 10\n");
	the_struct->keep_eating = YES;
	the_struct->print_condition = YES;
	the_struct->epoch = 0;
	the_struct->epoch = timefunction(the_struct->epoch);
	the_struct->forks = malloc(sizeof(pthread_mutex_t) * \
		(the_struct->n_philos + 1));
	the_struct->philos = malloc(sizeof(t_philos) * (the_struct->n_philos + 1));
	if ((!the_struct->forks) || (!the_struct->philos))
		return (print_error(4), 1);
	init_philos(the_struct);
	if (init_mutexes(the_struct))
		return (print_error(5), 1);
	return (0);
}
