/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_action3.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: torsini <torsini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 11:07:59 by torsini           #+#    #+#             */
/*   Updated: 2025/03/14 11:21:33 by torsini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	handle_philosopher_meal(t_philo *philo)
{
	if (is_simulation_finished(philo))
	{
		release_philosopher_forks(philo);
		return ;
	}
	update_meal_status(philo);
	pthread_mutex_lock(&philo->data->write_mutex);
	if (!is_simulation_finished(philo))
		printf("%lld %d is eating\n", get_current_time_ms()
			- philo->data->start_time, philo->id);
	pthread_mutex_unlock(&philo->data->write_mutex);
	usleep(philo->data->time_to_eat * 1000);
	release_philosopher_forks(philo);
}

void	attempt_to_acquire_forks(t_philo *philo)
{
	if (is_simulation_finished(philo))
		return ;
	if (philo->id % 2 == 0)
		acquire_forks_even_philosopher(philo);
	else
		acquire_forks_odd_philosopher(philo);
	if (is_simulation_finished(philo))
	{
		release_philosopher_forks(philo);
		return ;
	}
	philo->can_eat = 1;
}

void	announce_fork_acquisition(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->write_mutex);
	if (!is_simulation_finished(philo))
		printf("%lld %d has taken a fork\n", get_current_time_ms()
			- philo->data->start_time, philo->id);
	pthread_mutex_unlock(&philo->data->write_mutex);
}

void	acquire_forks_even_philosopher(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->forks[philo->right_fork]);
	announce_fork_acquisition(philo);
	pthread_mutex_lock(&philo->data->forks[philo->left_fork]);
	announce_fork_acquisition(philo);
}
