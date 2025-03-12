/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: your_login <your_login@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by your_login        #+#    #+#             */
/*   Updated: 2024/01/01 00:00:00 by your_login       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	validate_arguments_count(int argc)
{
	if (argc < 5 || argc > 6)
	{
		ft_printf("Error: Wrong number of arguments\n");
		ft_printf("Usage: ./philo number_of_philosophers time_to_die ");
		ft_printf("time_to_eat time_to_sleep [number_of_times_each_must_eat]\n");
		return (1);
	}
	return (0);
}

int	validate_simulation_parameters(t_data *data)
{
	if (data->num_philos <= 0 || data->time_to_die <= 0
		|| data->time_to_eat <= 0 || data->time_to_sleep <= 0
		|| (data->num_must_eat <= 0 && data->num_must_eat != -1))
	{
		ft_printf("Error: Invalid arguments values\n");
		return (1);
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (validate_arguments_count(argc))
		return (1);
	if (initialize_simulation_data(&data, argc, argv))
	{
		cleanup_simulation_resources(&data);
		return (1);
	}
	if (start_simulation(&data))
	{
		cleanup_simulation_resources(&data);
		return (1);
	}
	cleanup_simulation_resources(&data);
	return (0);
}

