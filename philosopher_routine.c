#include "philo.h"

static int    is_simulation_finished(t_philo *philo)
{
    pthread_mutex_lock(&philo->data->finished_mutex);
    if (philo->data->finished)
    {
        pthread_mutex_unlock(&philo->data->finished_mutex);
        return (1);
    }
    pthread_mutex_unlock(&philo->data->finished_mutex);
    return (0);
}

static void    announce_philosopher_start(t_philo *philo)
{
    pthread_mutex_lock(&philo->data->write_mutex);
    printf("%d  philo %d has started\n",
        (int)(get_current_time_ms() - philo->data->start_time), philo->id);
    pthread_mutex_unlock(&philo->data->write_mutex);
}

void    attempt_to_acquire_forks(t_philo *philo)
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

void    *execute_philosopher_routine(void *arg)
{
    t_philo *philo;

    philo = (t_philo *)arg;
    if (is_simulation_finished(philo))
        return (NULL);
    announce_philosopher_start(philo);
    if (philo->id % 2 == 0)
        usleep(100);
    while (!is_simulation_finished(philo))
    {
        attempt_to_acquire_forks(philo);
        if (philo->can_eat)
        {
            handle_philosopher_meal(philo);
            // Après avoir mangé, le philosophe dort si la simulation continue
            if (!is_simulation_finished(philo))
            {
                sleep_philosopher(philo);
                think_philosopher(philo);
            }
        }
        usleep(100);
    }
    return (NULL);
}

int    checkdeath(t_philo *philo)
{
    long long actual_time;
    long long time_no_eat;
    long long last_meal_time;

    actual_time = get_current_time_ms();
    pthread_mutex_lock(&philo->data->meal_mutex);
    last_meal_time = philo->last_meal;
    pthread_mutex_unlock(&philo->data->meal_mutex);
    
    time_no_eat = actual_time - last_meal_time;
    if(time_no_eat > philo->data->time_to_die)
    {
        pthread_mutex_lock(&philo->data->write_mutex);
        printf("%lld %d died\n", 
            get_current_time_ms() - philo->data->start_time, philo->id);
        pthread_mutex_unlock(&philo->data->write_mutex);
        return(1);
    }
    return(0);
}

void    print_fork_message(t_philo *philo)
{
    pthread_mutex_lock(&philo->data->write_mutex);
    printf("%lld %d has taken a fork\n", 
        get_current_time_ms() - philo->data->start_time, philo->id);
    pthread_mutex_unlock(&philo->data->write_mutex);
}

void    takeforks(t_philo *philo)
{
    // 1. Vérification rapide de finished
    pthread_mutex_lock(&philo->data->finished_mutex);
    if (philo->data->finished)
    {
        pthread_mutex_unlock(&philo->data->finished_mutex);
        return;
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
        return;
    }
    philo->can_eat = 1;
    pthread_mutex_unlock(&philo->data->finished_mutex);
}

void    release_forks(t_philo *philo)
{
    pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
    pthread_mutex_unlock(&philo->data->forks[philo->right_fork]);
    philo->can_eat = 0;  // Le philosophe ne peut plus manger
}

void eat(t_philo *philo)
{
    // 1. Vérification rapide de l'état
    pthread_mutex_lock(&philo->data->finished_mutex);
    if (philo->data->finished || !philo->can_eat)
    {
        pthread_mutex_unlock(&philo->data->finished_mutex);
        return;
    }
    pthread_mutex_unlock(&philo->data->finished_mutex);

    // 2. Mise à jour du dernier repas
    pthread_mutex_lock(&philo->data->meal_mutex);
    philo->last_meal = get_current_time_ms();
    pthread_mutex_unlock(&philo->data->meal_mutex);

    // 3. Affichage du message
    pthread_mutex_lock(&philo->data->write_mutex);
    printf("%lld %d is eating\n", 
        get_current_time_ms() - philo->data->start_time, philo->id);
    pthread_mutex_unlock(&philo->data->write_mutex);
    
    // 4. Attente et libération des fourchettes
    usleep(philo->data->time_to_eat * 1000);
    release_forks(philo);
}