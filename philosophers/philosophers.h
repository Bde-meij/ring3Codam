/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-meij <bde-meij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 15:11:09 by bde-meij          #+#    #+#             */
/*   Updated: 2023/01/18 09:48:51 by bde-meij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdlib.h>
# include <unistd.h>
# include <stdarg.h>
# include <stdio.h>
# include <pthread.h>
# include <time.h>
# include <sys/time.h>

# define YES 100
# define NO -100

typedef struct a_struct	t_struct;
typedef struct b_struct	t_philos;

typedef struct b_struct
{
	int				meals;
	int				phil_num;
	t_struct		*the_struct;
	u_int64_t		prev_mealtime;
	pthread_t		own_thread;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*r_fork;
}	t_philos;

typedef struct a_struct
{
	int				print_condition;
	int				keep_eating;
	int				max_rounds;
	int				n_philos;
	t_philos		*philos;
	u_int64_t		eat;
	u_int64_t		die;
	u_int64_t		sleep;
	u_int64_t		epoch;
	pthread_mutex_t	*forks;
	pthread_mutex_t	meal_mutex;
	pthread_mutex_t	mealtime_mutex;
	pthread_mutex_t	deadlock_mutex;
}	t_struct;

void		sleep_in_bits(u_int64_t waiting_time, t_struct *the_struct);
int			initialization(t_struct *the_struct, char **av);
u_int64_t	timefunction(u_int64_t start_of_program);
void		printer(int msg, t_philos *philo);
void		monitoring(t_struct *the_struct);
int			ft_atoi(const char *str);
void		*dinner(void *philos);
void		print_error(int code);

#endif