/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_monitor.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: torsini <torsini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 10:07:04 by torsini           #+#    #+#             */
/*   Updated: 2025/03/13 10:07:08 by torsini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_philosopher_death(t_philo *philo)
{
	long long	current_time;
	long long	last_meal_time;

	current_time = get_current_time_ms();
	pthread_mutex_lock(&philo->data->meal_mutex);
	last_meal_time = philo->last_meal;
	pthread_mutex_unlock(&philo->data->meal_mutex);
	if (current_time - last_meal_time > philo->data->time_to_die)
		return (1);
	return (0);
}

void	cleanup_simulation_resources(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->num_philos)
		pthread_mutex_destroy(&data->forks[i]);
	pthread_mutex_destroy(&data->write_mutex);
	pthread_mutex_destroy(&data->meal_mutex);
	pthread_mutex_destroy(&data->finished_mutex);
	free(data->forks);
	free(data->philos);
}

int	start_simulation(t_data *data)
{
	int			i;
	pthread_t	*threads;

	threads = malloc(sizeof(pthread_t) * data->num_philos);
	if (!threads)
		return (1);
	i = -1;
	while (++i < data->num_philos)
		if (pthread_create(&threads[i], NULL, execute_philosopher_routine,
				&data->philos[i]) != 0)
			return (1);
	monitor_philosophers(data);
	i = -1;
	while (++i < data->num_philos)
		pthread_join(threads[i], NULL);
	free(threads);
	return (0);
}

static void	announce_death(t_data *data, int i)
{
	pthread_mutex_lock(&data->write_mutex);
	printf("%lld %d died\n", get_current_time_ms() - data->start_time,
		data->philos[i].id);
	pthread_mutex_lock(&data->finished_mutex);
	data->finished = 1;
	pthread_mutex_unlock(&data->finished_mutex);
	pthread_mutex_unlock(&data->write_mutex);
}

void	monitor_philosophers(t_data *data)
{
	int	i;
	int	should_continue;

	should_continue = 1;
	while (should_continue)
	{
		i = 0;
		while (i < data->num_philos)
		{
			if (check_philosopher_death(&data->philos[i]))
			{
				announce_death(data, i);
				return ;
			}
			i++;
		}
		pthread_mutex_lock(&data->finished_mutex);
		should_continue = !data->finished;
		if (data->all_ate_enough)
			should_continue = 0;
		pthread_mutex_unlock(&data->finished_mutex);
		usleep(1000);
	}
}
