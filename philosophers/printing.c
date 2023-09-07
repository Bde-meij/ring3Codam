/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-meij <bde-meij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 15:56:45 by bde-meij          #+#    #+#             */
/*   Updated: 2023/06/26 16:35:31 by bde-meij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	print_error(int code)
{
	if (code == 1)
		printf("Error: incorrect number of args");
	if (code == 2)
		printf("Error: incorrect input");
	if (code == 3)
		printf("Error: each value must be 1 or higher");
	if (code == 4)
		printf("Error: Malloc failed");
	if (code == 5)
		printf("Error: Couldn't initiate mutex");
	if (code == 6)
		printf("Error: Couldn't create thread");
}

void	printing(t_philos *philo, char *str)
{
	pthread_mutex_lock(&philo->the_struct->deadlock_mutex);
	if (philo->the_struct->print_condition == YES)
		printf("%llu %d %s\n", timefunction(philo->the_struct->epoch), \
			philo->phil_num, str);
	pthread_mutex_unlock(&philo->the_struct->deadlock_mutex);
}

void	printer(int msg, t_philos *philo)
{
	if (msg == 0)
		printing(philo, "has taken a fork");
	if (msg == 1)
	{
		printing(philo, "is eating");
		pthread_mutex_lock(&philo->the_struct->mealtime_mutex);
		philo->prev_mealtime = timefunction(philo->the_struct->epoch);
		pthread_mutex_unlock(&philo->the_struct->mealtime_mutex);
	}
	if (msg == 2)
		printing(philo, "is sleeping");
	if (msg == 3)
		printing(philo, "is thinking");
	if (msg == 4)
	{
		pthread_mutex_lock(&philo->the_struct->deadlock_mutex);
		philo->the_struct->print_condition = NO;
		pthread_mutex_unlock(&philo->the_struct->deadlock_mutex);
		printf("%llu %d %s\n", timefunction(philo->the_struct->epoch), \
			philo->phil_num, "died");
	}
}
