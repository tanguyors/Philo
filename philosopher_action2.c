/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_action2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: torsini <torsini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 10:00:19 by torsini           #+#    #+#             */
/*   Updated: 2025/03/13 10:03:48 by torsini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	sleep_philosopher(t_philo *philo)
{
	// Affichage du message indiquant que le philosophe dort
	pthread_mutex_lock(&philo->data->write_mutex);
	printf("%lld %d is sleeping\n", get_current_time_ms()
		- philo->data->start_time, philo->id);
	pthread_mutex_unlock(&philo->data->write_mutex);
	// Faire dormir le philosophe pendant time_to_sleep millisecondes
	usleep(philo->data->time_to_sleep * 1000);
}

void	think_philosopher(t_philo *philo)
{
	// Affichage du message indiquant que le philosophe pense
	pthread_mutex_lock(&philo->data->write_mutex);
	printf("%lld %d is thinking\n", get_current_time_ms()
		- philo->data->start_time, philo->id);
	pthread_mutex_unlock(&philo->data->write_mutex);

	// Ici, pas de sleep car le temps de réflexion est indéterminé
	// Le philosophe passera à l'étape suivante (prendre les fourchettes)
	// dès que l'ordonnanceur lui donnera la main
}