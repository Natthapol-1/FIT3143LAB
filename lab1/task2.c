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

// Structure to pass data to each thread
typedef struct
{
    long long start_val;    // starting index to [primes] to store in each thread
    long long end_val;      // ending index to [primes] to store in each thread
    long long *primes;      // main array to store all primes
    long long *local_count; // count the number of primes in each thread locally
    long long limit;
} thread_arg_t;

// Function executed by each thread
void *check_prime(void *arg)
{
    thread_arg_t *data = (thread_arg_t *)arg;

    // local primes array for storing primes in this thread (will be added to main array later)
    long long range_size = data->end_val - data->start_val + 1;
    long long *local_primes = malloc(range_size * sizeof(long long));
    long long local_c = 0;

    // loop to check all numbers in the assigned range in this thread
    for (long long i = data->start_val; i <= data->end_val; i++)
    {
        if (is_prime(i))
        {
            local_primes[local_c++] = i;
        }
    }

    *(data->local_count) = local_c;

    // add local_primes back to primes
    memcpy(data->primes, local_primes, local_c * sizeof(long long));

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
    // validate input
    if (argv[1][0] == '\0' || *end != '\0' || limit < 2)
    {
        printf("Please enter a valid integer n >= 2.\n");
        return 1;
    }

    // initialize thread array
    int num_threads = 4;
    pthread_t threads[num_threads];
    thread_arg_t thread_args[num_threads];
    long long local_counts[num_threads];

    struct timespec start, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start);

    long long *primes = malloc((limit + 1) * sizeof(long long));

    if (limit <= 1000)
    {
        printf("Prime numbers up to %lld:\n", limit);
    }

    // distribute whole chunk of numbers into smaller equal chunks to each thread
    long long chunk_size = (limit - 1) / num_threads;

    // Allocate a separate section of the main array for each thread.
    // Each thread writes its primes into its own section so no mutex is needed.
    long long thread_offset = 0;

    for (int t = 0; t < num_threads; t++)
    {
        // last thread gets all the remaining numbers since integer division have remainders
        thread_args[t].start_val = 2 + t * chunk_size;
        if (t == num_threads - 1)
        {
            thread_args[t].end_val = limit;
        }
        else
        {
            // the rest gets the same amount
            thread_args[t].end_val = thread_args[t].start_val + chunk_size - 1;
        }

        // set argument to parse into function called by each thread
        thread_args[t].primes = &primes[thread_offset];
        thread_args[t].local_count = &local_counts[t];
        thread_args[t].limit = limit;

        // Reserve enough space for this thread's entire input range.
        thread_offset += thread_args[t].end_val - thread_args[t].start_val + 1;

        pthread_create(&threads[t], NULL, check_prime, &thread_args[t]);
    }

    long long total_count = 0;

    // wait for all thread to finish
    for (int t = 0; t < num_threads; t++)
    {
        pthread_join(threads[t], NULL);
        total_count += local_counts[t];
    }

    // Move each thread's primes together so that they are stored consecutively.
    // Since the threads process consecutive ranges in increasing order,
    // the final array is already sorted and does not need qsort().
    long long offset = 0;
    long long source_offset = 0;

    for (int t = 0; t < num_threads; t++)
    {
        if (source_offset != offset)
        {
            memmove(&primes[offset],
                    &primes[source_offset],
                    local_counts[t] * sizeof(long long));
        }

        offset += local_counts[t];
        source_offset += thread_args[t].end_val - thread_args[t].start_val + 1;
    }

    if (limit > 1000)
    {
        // print to file
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

    clock_gettime(CLOCK_MONOTONIC, &end_time);

    double elapsed =
        (end_time.tv_sec - start.tv_sec) +
        (end_time.tv_nsec - start.tv_nsec) / 1e9;

    printf("Total prime numbers up to %lld: %lld\n", limit, total_count);
    printf("Execution time: %.6f seconds\n", elapsed);

    return 0;
}