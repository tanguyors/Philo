/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_action.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: torsini <torsini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 10:00:38 by torsini           #+#    #+#             */
/*   Updated: 2025/03/13 10:01:16 by torsini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	announce_fork_acquisition(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->write_mutex);
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

void	acquire_forks_odd_philosopher(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->forks[philo->left_fork]);
	announce_fork_acquisition(philo);
	pthread_mutex_lock(&philo->data->forks[philo->right_fork]);
	announce_fork_acquisition(philo);
}

void	release_philosopher_forks(t_philo *philo)
{
	pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
	pthread_mutex_unlock(&philo->data->forks[philo->right_fork]);
	philo->can_eat = 0;
}

void	handle_philosopher_meal(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->meal_mutex);
	philo->last_meal = get_current_time_ms();
	pthread_mutex_unlock(&philo->data->meal_mutex);
	pthread_mutex_lock(&philo->data->write_mutex);
	printf("%lld %d is eating\n", get_current_time_ms()
		- philo->data->start_time, philo->id);
	pthread_mutex_unlock(&philo->data->write_mutex);
	usleep(philo->data->time_to_eat * 1000);
	release_philosopher_forks(philo);
}
