/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_routine.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: torsini <torsini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 10:05:37 by torsini           #+#    #+#             */
/*   Updated: 2025/03/13 10:06:54 by torsini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_simulation_finished(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->finished_mutex);
	if (philo->data->finished)
	{
		pthread_mutex_unlock(&philo->data->finished_mutex);
		return (1);
	}
	pthread_mutex_unlock(&philo->data->finished_mutex);
	return (0);
}

/*static void	announce_philosopher_start(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->write_mutex);
	printf("%d  philo %d has started\n", (int)(get_current_time_ms()
			- philo->data->start_time), philo->id);
	pthread_mutex_unlock(&philo->data->write_mutex);
}*/


void	*execute_philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		usleep(50);

	while (!is_simulation_finished(philo))
	{
		if (is_simulation_finished(philo))
			break;
			
		attempt_to_acquire_forks(philo);
		if (philo->can_eat && !is_simulation_finished(philo))
		{
			handle_philosopher_meal(philo);
			if (!is_simulation_finished(philo))
			{
				sleep_philosopher(philo);
				if (!is_simulation_finished(philo))
					think_philosopher(philo);
			}
		}
		usleep(50);
	}

	if (philo->can_eat)
		release_philosopher_forks(philo);
	return (NULL);
}

int	checkdeath(t_philo *philo)
{
	long long	actual_time;
	long long	time_no_eat;
	long long	last_meal_time;

	actual_time = get_current_time_ms();
	pthread_mutex_lock(&philo->data->meal_mutex);
	last_meal_time = philo->last_meal;
	pthread_mutex_unlock(&philo->data->meal_mutex);
	time_no_eat = actual_time - last_meal_time;

	usleep(100);
	if (time_no_eat > philo->data->time_to_die)
	{
		pthread_mutex_lock(&philo->data->write_mutex);
		printf("%lld %d died\n", get_current_time_ms()
			- philo->data->start_time, philo->id);
		pthread_mutex_lock(&philo->data->finished_mutex);
		philo->data->finished = 1;
		pthread_mutex_unlock(&philo->data->finished_mutex);
		pthread_mutex_unlock(&philo->data->write_mutex);
		return (1);
	}
	return (0);
}
