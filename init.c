#include "philo.h" // qui contient la déclaration de t_data, t_philo, etc.

// Fonction qui renvoie le temps actuel en ms (nécessaire pour start_time, par ex.)
long long get_current_time_ms(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

static int    initialize_simulation_mutexes(t_data *data)
{
    if (pthread_mutex_init(&data->write_mutex, NULL) != 0
        || pthread_mutex_init(&data->meal_mutex, NULL) != 0
        || pthread_mutex_init(&data->finished_mutex, NULL) != 0)
        return (1);
    return (0);
}

static int    initialize_fork_mutexes(t_data *data)
{
    int i;

    data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philos);
    if (!data->forks)
        return (1);
    i = -1;
    while (++i < data->num_philos)
        if (pthread_mutex_init(&data->forks[i], NULL) != 0)
            return (1);
    return (0);
}

static void    setup_philosopher_parameters(t_philo *philo, t_data *data, int i)
{
    philo->id = i + 1;
    philo->meals_eaten = 0;
    philo->last_meal = data->start_time;
    philo->data = data;
    philo->left_fork = i;
    philo->right_fork = (i + 1) % data->num_philos;
    philo->can_eat = 0;
}

static int    allocate_and_initialize_philosophers(t_data *data)
{
    int i;

    data->philos = malloc(sizeof(t_philo) * data->num_philos);
    if (!data->philos)
        return (1);
    i = -1;
    while (++i < data->num_philos)
        setup_philosopher_parameters(&data->philos[i], data, i);
    return (0);
}

int    initialize_simulation_data(t_data *data, int argc, char **argv)
{
    data->num_philos = ft_atoi(argv[1]);
    data->time_to_die = ft_atoi(argv[2]);
    data->time_to_eat = ft_atoi(argv[3]);
    data->time_to_sleep = ft_atoi(argv[4]);
    data->num_must_eat = -1;
    if (argc == 6)
        data->num_must_eat = ft_atoi(argv[5]);
    if (validate_simulation_parameters(data) != 0)
        return (1);
    data->start_time = get_current_time_ms();
    data->finished = 0;
    if (initialize_fork_mutexes(data) != 0
        || initialize_simulation_mutexes(data) != 0
        || allocate_and_initialize_philosophers(data) != 0)
        return (1);
    return (0);
}

