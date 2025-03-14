/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: torsini <torsini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 10:09:27 by torsini           #+#    #+#             */
/*   Updated: 2025/03/14 11:10:58 by torsini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_data	t_data;

typedef struct s_philo
{
	int					id;
	int					meals_eaten;
	long long			last_meal;
	int					left_fork;
	int					right_fork;
	int					can_eat;
	t_data				*data;
}						t_philo;

typedef struct s_data
{
	int					num_philos;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					num_must_eat;
	int					finished;
	int					all_ate_enough;
	long long			start_time;
	pthread_mutex_t		*forks;
	pthread_mutex_t		write_mutex;
	pthread_mutex_t		meal_mutex;
	pthread_mutex_t		finished_mutex;
	t_philo				*philos;
}						t_data;

/* Fonctions principales */
int						main(int argc, char **argv);
int						validate_arguments_count(int argc);
int						validate_simulation_parameters(t_data *data);

/* Fonctions d'initialisation */
int						initialize_simulation_data(t_data *data, int argc,
							char **argv);
void					cleanup_simulation_resources(t_data *data);

/* Fonctions de temps */
long long				get_current_time_ms(void);

/* Fonctions de routine des philosophes */
void					*execute_philosopher_routine(void *arg);
int						is_simulation_finished(t_philo *philo);
void					attempt_to_acquire_forks(t_philo *philo);
void					handle_philosopher_meal(t_philo *philo);
void					sleep_philosopher(t_philo *philo);
void					think_philosopher(t_philo *philo);
void					update_meal_status(t_philo *philo);

/* Fonctions de gestion des fourchettes */
void					acquire_forks_even_philosopher(t_philo *philo);
void					acquire_forks_odd_philosopher(t_philo *philo);
void					release_philosopher_forks(t_philo *philo);
void					announce_fork_acquisition(t_philo *philo);

/* Fonctions de surveillance */
int						start_simulation(t_data *data);
void					monitor_philosophers(t_data *data);
int						check_philosopher_death(t_philo *philo);

int						check_all_philosophers_ate_enough(t_data *data);

#endif