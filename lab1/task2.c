#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

int is_prime(long long num)
{
    if (num < 2)
        return 0;
    if (num == 2)
        return 1;
    if (num % 2 == 0)
        return 0;

    for (long long i = 3; i * i <= num; i += 2)
    {
        if (num % i == 0)
            return 0;
    }
    return 1;
}

int compare(const void *a, const void *b)
{
    long long int_a = *((const long long *)a);
    long long int_b = *((const long long *)b);
    if (int_a < int_b)
        return -1;
    if (int_a > int_b)
        return 1;
    return 0;
}

// Structure to pass data to each thread
typedef struct
{
    long long start_val;
    long long end_val;
    long long *primes;
    long long *local_count;
    long long *global_offset; // To track where each thread writes in the main array
    pthread_mutex_t *mutex;
    long long limit;
} thread_arg_t;

// Function executed by each thread
void *check_prime(void *arg)
{
    thread_arg_t *data = (thread_arg_t *)arg;

    // Allocate local buffer for this thread's chunk
    long long range_size = data->end_val - data->start_val + 1;
    long long *local_primes = malloc(range_size * sizeof(long long));
    long long local_c = 0;

    for (long long i = data->start_val; i <= data->end_val; i++)
    {
        if (is_prime(i))
        {
            local_primes[local_c++] = i;
        }
    }

    *(data->local_count) = local_c;

    pthread_mutex_lock(data->mutex);
    long long dest_idx = *(data->global_offset);
    *(data->global_offset) += local_c;
    pthread_mutex_unlock(data->mutex);

    memcpy(&data->primes[dest_idx], local_primes, local_c * sizeof(long long));

    free(local_primes);
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <n>\n", argv[0]);
        return 1;
    }

    char *end = NULL;
    long long limit = strtoll(argv[1], &end, 10);

    if (argv[1][0] == '\0' || *end != '\0' || limit < 2)
    {
        printf("Please enter a valid integer n >= 2.\n");
        return 1;
    }

    int num_threads = 4;
    pthread_t threads[num_threads];
    thread_arg_t thread_args[num_threads];
    long long local_counts[num_threads];
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);

    double start = (double)clock() / CLOCKS_PER_SEC;

    long long *primes = malloc((limit + 1) * sizeof(long long));
    long long global_offset = 0;

    if (limit <= 1000)
    {
        printf("Prime numbers up to %lld:\n", limit);
    }

    long long chunk_size = (limit - 1) / num_threads;
    for (int t = 0; t < num_threads; t++)
    {
        thread_args[t].start_val = 2 + t * chunk_size;
        if (t == num_threads - 1)
        {
            thread_args[t].end_val = limit;
        }
        else
        {
            thread_args[t].end_val = thread_args[t].start_val + chunk_size - 1;
        }
        thread_args[t].primes = primes;
        thread_args[t].local_count = &local_counts[t];
        thread_args[t].global_offset = &global_offset;
        thread_args[t].mutex = &mutex;
        thread_args[t].limit = limit;

        pthread_create(&threads[t], NULL, check_prime, &thread_args[t]);
    }

    long long total_count = 0;
    for (int t = 0; t < num_threads; t++)
    {
        pthread_join(threads[t], NULL);
        total_count += local_counts[t];
    }

    pthread_mutex_destroy(&mutex);

    qsort(primes, total_count, sizeof(long long), compare);

    if (limit > 1000)
    {
        FILE *file = fopen("primes.txt", "w");
        if (file != NULL)
        {
            for (long long i = 0; i < total_count; i++)
            {
                fprintf(file, "%lld ", primes[i]);
            }
            fclose(file);
        }
    }
    else
    {
        for (long long i = 0; i < total_count; i++)
            printf("%lld ", primes[i]);
        printf("\n");
    }

    free(primes);

    double elapsed = ((double)clock() / CLOCKS_PER_SEC) - start;

    printf("Total prime numbers up to %lld: %lld\n", limit, total_count);
    printf("Execution time: %.6f seconds\n", elapsed);

    return 0;
}