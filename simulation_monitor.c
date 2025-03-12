#include "philo.h"

int    check_philosopher_death(t_philo *philo)
{
    long long current_time;
    long long last_meal_time;

    current_time = get_current_time_ms();
    pthread_mutex_lock(&philo->data->meal_mutex);
    last_meal_time = philo->last_meal;
    pthread_mutex_unlock(&philo->data->meal_mutex);
    
    if (current_time - last_meal_time > philo->data->time_to_die)
        return (1);
    return (0);
}

void    cleanup_simulation_resources(t_data *data)
{
    int i;

    i = -1;
    while (++i < data->num_philos)
        pthread_mutex_destroy(&data->forks[i]);
    pthread_mutex_destroy(&data->write_mutex);
    pthread_mutex_destroy(&data->meal_mutex);
    pthread_mutex_destroy(&data->finished_mutex);
    free(data->forks);
    free(data->philos);
}

int    start_simulation(t_data *data)
{
    int i;
    pthread_t *threads;

    threads = malloc(sizeof(pthread_t) * data->num_philos);
    if (!threads)
        return (1);
    i = -1;
    while (++i < data->num_philos)
        if (pthread_create(&threads[i], NULL,
            execute_philosopher_routine, &data->philos[i]) != 0)
            return (1);
    monitor_philosophers(data);
    i = -1;
    while (++i < data->num_philos)
        pthread_join(threads[i], NULL);
    free(threads);
    return (0);
}

void    monitor_philosophers(t_data *data)
{
    int i;

    pthread_mutex_lock(&data->finished_mutex);
    while (!data->finished)
    {
        i = 0;
        while (i < data->num_philos)
        {
            pthread_mutex_unlock(&data->finished_mutex);
            if (check_philosopher_death(&data->philos[i]))
            {
                pthread_mutex_lock(&data->write_mutex);
                ft_printf("%d philo %d died\n",
                    (int)(get_current_time_ms() - data->start_time),
                    data->philos[i].id);
                pthread_mutex_lock(&data->finished_mutex);
                data->finished = 1;
                pthread_mutex_unlock(&data->finished_mutex);
                pthread_mutex_unlock(&data->write_mutex);
                return;
            }
            pthread_mutex_lock(&data->finished_mutex);
            i++;
        }
        pthread_mutex_unlock(&data->finished_mutex);
        usleep(1000);
        pthread_mutex_lock(&data->finished_mutex);
    }
    pthread_mutex_unlock(&data->finished_mutex);
}
