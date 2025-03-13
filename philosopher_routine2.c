/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_routine2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: torsini <torsini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 10:06:40 by torsini           #+#    #+#             */
/*   Updated: 2025/03/13 10:06:49 by torsini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_fork_message(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->write_mutex);
	printf("%lld %d has taken a fork\n", get_current_time_ms()
		- philo->data->start_time, philo->id);
	pthread_mutex_unlock(&philo->data->write_mutex);
}

void	takeforks(t_philo *philo)
{
	// 1. Vérification rapide de finished
	pthread_mutex_lock(&philo->data->finished_mutex);
	if (philo->data->finished)
	{
		pthread_mutex_unlock(&philo->data->finished_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->data->finished_mutex);
	// 2. Prise des fourchettes dans un ordre cohérent
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(&philo->data->forks[philo->right_fork]);
		print_fork_message(philo);
		pthread_mutex_lock(&philo->data->forks[philo->left_fork]);
		print_fork_message(philo);
	}
	else
	{
		pthread_mutex_lock(&philo->data->forks[philo->left_fork]);
		print_fork_message(philo);
		pthread_mutex_lock(&philo->data->forks[philo->right_fork]);
		print_fork_message(philo);
	}
	// 3. Vérification finale de finished
	pthread_mutex_lock(&philo->data->finished_mutex);
	if (philo->data->finished)
	{
		pthread_mutex_unlock(&philo->data->finished_mutex);
		pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
		pthread_mutex_unlock(&philo->data->forks[philo->right_fork]);
		return ;
	}
	philo->can_eat = 1;
	pthread_mutex_unlock(&philo->data->finished_mutex);
}

void	release_forks(t_philo *philo)
{
	pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
	pthread_mutex_unlock(&philo->data->forks[philo->right_fork]);
	philo->can_eat = 0; // Le philosophe ne peut plus manger
}

void	eat(t_philo *philo)
{
	// 1. Vérification rapide de l'état
	pthread_mutex_lock(&philo->data->finished_mutex);
	if (philo->data->finished || !philo->can_eat)
	{
		pthread_mutex_unlock(&philo->data->finished_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->data->finished_mutex);

	// 2. Mise à jour du dernier repas
	pthread_mutex_lock(&philo->data->meal_mutex);
	philo->last_meal = get_current_time_ms();
	pthread_mutex_unlock(&philo->data->meal_mutex);

	// 3. Affichage du message
	pthread_mutex_lock(&philo->data->write_mutex);
	printf("%lld %d is eating\n", get_current_time_ms()
		- philo->data->start_time, philo->id);
	pthread_mutex_unlock(&philo->data->write_mutex);

	// 4. Attente et libération des fourchettes
	usleep(philo->data->time_to_eat * 1000);
	release_forks(philo);
}