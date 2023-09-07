/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_warp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-meij <bde-meij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 14:38:47 by bde-meij          #+#    #+#             */
/*   Updated: 2023/01/13 17:36:07 by bde-meij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	sleep_in_bits(u_int64_t time_to_wait, t_struct *the_struct)
{
	u_int64_t	current_time;

	current_time = timefunction(the_struct->epoch);
	while ((timefunction(the_struct->epoch) - current_time) < time_to_wait)
		usleep(time_to_wait / 2);
}

u_int64_t	timefunction(u_int64_t start_of_program)
{
	u_int64_t		time_since_epoch;
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	time_since_epoch = (tv.tv_sec * 1000) + (tv.tv_usec / 1000) \
	- start_of_program;
	return (time_since_epoch);
}
