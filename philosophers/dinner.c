/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-meij <bde-meij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 14:39:54 by bde-meij          #+#    #+#             */
/*   Updated: 2023/06/26 16:35:32 by bde-meij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	eating(t_philos *philo)
{
	pthread_mutex_lock(philo->l_fork);
	printer(0, philo);
	pthread_mutex_lock(philo->r_fork);
	printer(0, philo);
	printer(1, philo);
	sleep_in_bits(philo->the_struct->eat, philo->the_struct);
	pthread_mutex_unlock(philo->l_fork);
	pthread_mutex_unlock(philo->r_fork);
}

void	sleeping(t_philos *philo)
{
	printer(2, philo);
	sleep_in_bits(philo->the_struct->sleep, philo->the_struct);
	printer(3, philo);
}

void	*dinner(void *philos)
{
	t_philos	*philo;

	philo = philos;
	if (philo->phil_num % 2 == 0)
		sleep_in_bits(philo->the_struct->eat / 2, philo->the_struct);
	while (1)
	{
		if (philo->the_struct->n_philos < 2)
		{
			pthread_mutex_lock(philo->l_fork);
			printer(0, philo);
			break ;
		}
		eating(philo);
		pthread_mutex_lock(&philo->the_struct->meal_mutex);
		philo->meals--;
		if ((philo->meals == 0) || (philo->the_struct->keep_eating == NO))
		{
			pthread_mutex_unlock(&philo->the_struct->meal_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->the_struct->meal_mutex);
		sleeping(philo);
	}
	return (NULL);
}
