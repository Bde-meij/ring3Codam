/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-meij <bde-meij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 14:48:15 by bde-meij          #+#    #+#             */
/*   Updated: 2023/01/20 14:53:10 by bde-meij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <stdlib.h>
# include <unistd.h>
# include <stdarg.h>
# include <stdio.h>
# include <pthread.h>
# include <time.h>
# include <sys/time.h>

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
	if (philo->the_struct->print_condition == YES)
		printf("%llu %d %s\n", timefunction(philo->the_struct->epoch), \
			philo->phil_num, str);
}

void	printer(int msg, t_philos *philo)
{
	if (msg == 0)
		printing(philo, "has taken a fork");
	if (msg == 1)
	{
		printing(philo, "is eating");
		philo->prev_mealtime = timefunction(philo->the_struct->epoch);
	}
	if (msg == 2)
		printing(philo, "is sleeping");
	if (msg == 3)
		printing(philo, "is thinking");
	if (msg == 4)
	{
		philo->the_struct->print_condition = NO;
		printf("%llu %d %s\n", timefunction(philo->the_struct->epoch), \
			philo->phil_num, "died");
	}
}

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
	the_struct->philos = malloc(sizeof(t_philos) * (the_struct->n_philos + 1));
	if ((!the_struct->forks) || (!the_struct->philos))
		return (print_error(4), 1);
	init_philos(the_struct);
	return (0);
}

void	eating(t_philos *philo)
{
	printer(0, philo);
	printer(0, philo);
	printer(1, philo);
	sleep_in_bits(philo->the_struct->eat, philo->the_struct);
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
			printer(0, philo);
			break ;
		}
		eating(philo);
		philo->meals--;
		if ((philo->meals == 0) || (philo->the_struct->keep_eating == NO))
			break ;
		sleeping(philo);
	}
	return (NULL);
}

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