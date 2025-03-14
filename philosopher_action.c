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

static int	check_all_philosophers_ate_enough(t_philo *philo)
{
	int	i;
	int	all_ate_enough;

	all_ate_enough = 1;
	i = 0;
	while (i < philo->data->num_philos)
	{
		if (philo->data->philos[i].meals_eaten < philo->data->num_must_eat)
		{
			all_ate_enough = 0;
			break ;
		}
		i++;
	}
	return (all_ate_enough);
}

static void	update_meal_status(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->meal_mutex);
	philo->last_meal = get_current_time_ms();
	philo->meals_eaten++;
	if (philo->data->num_must_eat != -1 
		&& philo->meals_eaten >= philo->data->num_must_eat)
	{
		if (check_all_philosophers_ate_enough(philo))
		{
			pthread_mutex_lock(&philo->data->finished_mutex);
			philo->data->all_ate_enough = 1;
			philo->data->finished = 1;
			pthread_mutex_unlock(&philo->data->finished_mutex);
		}
	}
	pthread_mutex_unlock(&philo->data->meal_mutex);
}

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

void attempt_to_acquire_forks(t_philo *philo)
{
	if (is_simulation_finished(philo))
		return;

	if (philo->id % 2 == 0)
		acquire_forks_even_philosopher(philo);
	else
		acquire_forks_odd_philosopher(philo);

	if (is_simulation_finished(philo))
	{
		release_philosopher_forks(philo);
		return;
	}
	philo->can_eat = 1;
}
