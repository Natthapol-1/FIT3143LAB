#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

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

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <n>\n", argv[0]);
        return 1;
    }

    // validate input
    char *end = NULL;
    long long limit = strtoll(argv[1], &end, 10);

    if (argv[1][0] == '\0' || *end != '\0' || limit < 2)
    {
        printf("Please enter a valid integer n >= 2.\n");
        return 1;
    }

    double start = omp_get_wtime();

    // initialize an array of all primes
    long long *primes = malloc((limit + 1) * sizeof(long long));

    // Each thread gets its own local array and count
    int max_threads = omp_get_max_threads();

    long long **local_primes = malloc(max_threads * sizeof(long long *));
    long long *local_counts = calloc(max_threads, sizeof(long long));

    if (limit <= 1000)
    {
        printf("Prime numbers up to %lld:\n", limit);
    }

    // Allocate a local array for each thread
    for (int t = 0; t < max_threads; t++)
    {
        local_primes[t] = malloc((limit + 1) * sizeof(long long));
    }

    // Give each thread numbers to check dynamically.
    // Each thread stores its primes in its own local array.
#pragma omp parallel
    {
        int thread_id = omp_get_thread_num();

#pragma omp for schedule(dynamic, 1000)
        for (long long i = 2; i <= limit; i++)
        {
            if (is_prime(i))
            {
                local_primes[thread_id][local_counts[thread_id]++] = i;
            }
        }
    }

    // Combine each thread's primes in thread order.
    long long found_count = 0;

    for (int t = 0; t < max_threads; t++)
    {
        for (long long i = 0; i < local_counts[t]; i++)
        {
            primes[found_count++] = local_primes[t][i];
        }
    }

    if (limit <= 1000)
    {
        for (long long i = 0; i < found_count; i++)
        {
            printf("%lld ", primes[i]);
        }

        printf("\n");
    }
    else
    {
        // print to file
        FILE *file = fopen("primes.txt", "w");

        if (file != NULL)
        {
            for (long long i = 0; i < found_count; i++)
            {
                fprintf(file, "%lld ", primes[i]);
            }

            fclose(file);
        }
    }

    for (int t = 0; t < max_threads; t++)
        free(local_primes[t]);

    free(local_primes);
    free(local_counts);
    free(primes);

    double elapsed = omp_get_wtime() - start;

    printf("Total prime numbers up to %lld: %lld\n", limit, found_count);
    printf("Execution time: %.6f seconds\n", elapsed);

    return 0;
}