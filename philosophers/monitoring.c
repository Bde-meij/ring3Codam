/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-meij <bde-meij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 11:42:36 by bde-meij          #+#    #+#             */
/*   Updated: 2023/01/17 16:43:34 by bde-meij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	see_if_dead(t_struct *the_struct, u_int64_t timer)
{
	int	i;

	i = 0;
	while (i < the_struct->n_philos)
	{
		if ((timer - the_struct->philos[i].prev_mealtime > \
			the_struct->die) && (timer > \
			the_struct->philos[i].prev_mealtime))
		{
			printer(4, &the_struct->philos[i]);
			pthread_mutex_lock(&the_struct->deadlock_mutex);
			the_struct->keep_eating = NO;
			pthread_mutex_unlock(&the_struct->deadlock_mutex);
			break ;
		}
		i++;
	}
}

void	see_if_meals_left(t_struct *the_struct)
{
	int	i;

	i = 0;
	while ((the_struct->philos[i].meals == 0) && (i < the_struct->n_philos))
		i++;
	if (i == the_struct->n_philos)
	{
		pthread_mutex_lock(&the_struct->deadlock_mutex);
		the_struct->keep_eating = NO;
		pthread_mutex_unlock(&the_struct->deadlock_mutex);
	}
}

void	monitoring(t_struct *the_struct)
{
	u_int64_t	timer;

	while (the_struct->keep_eating == YES)
	{
		pthread_mutex_lock(&the_struct->mealtime_mutex);
		timer = timefunction(the_struct->epoch);
		see_if_dead(the_struct, timer);
		pthread_mutex_unlock(&the_struct->mealtime_mutex);
		pthread_mutex_lock(&the_struct->meal_mutex);
		see_if_meals_left(the_struct);
		pthread_mutex_unlock(&the_struct->meal_mutex);
	}
}
