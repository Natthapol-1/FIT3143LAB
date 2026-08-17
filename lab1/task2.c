#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

typedef struct
{
    long long start;
    long long end;
    long long *primes;
    long long count;
    long long capacity;
} ThreadArg;

int isPrime(long long num)
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

void *worker(void *arg)
{
    ThreadArg *t = (ThreadArg *)arg;
    t->capacity = 1024;
    t->count = 0;
    t->primes = malloc(t->capacity * sizeof(long long));

    if (!t->primes)
    {
        perror("Failed to allocate memory for thread primes");
        pthread_exit(NULL);
    }

    // Search for primes in the assigned range [start, end]
    for (long long i = t->start; i <= t->end; i++)
    {
        if (isPrime(i))
        {
            if (t->count >= t->capacity)
            {
                t->capacity *= 2;
                long long *temp = realloc(t->primes, t->capacity * sizeof(long long));
                if (!temp)
                {
                    perror("Failed to reallocate memory");
                    break;
                }
                t->primes = temp;
            }
            t->primes[t->count++] = i;
        }
    }
    return NULL;
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
    ThreadArg args[num_threads];

    clock_t start = clock();
    long long count = 0;

    if (limit <= 1000)
    {
        printf("Prime numbers up to %lld:\n", limit);
    }

    long long range_start = 2;
    long long range_end = limit - 1;
    long long total_range = (range_end >= range_start) ? (range_end - range_start + 1) : 0;

    if (total_range <= 0)
    {
        printf("No numbers to check for n = %lld\n", limit);
        return 0;
    }
    long long chunk_size = total_range / num_threads;
    for (int i = 0; i < num_threads; i++)
    {
        args[i].start = range_start + (i * chunk_size);
        args[i].end = (i == num_threads - 1) ? range_end : (args[i].start + chunk_size - 1);

        pthread_create(&threads[i], NULL, worker, &args[i]);
    }

    long long total_primes = 0;
    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], NULL);
        total_primes += args[i].count;
    }

    if (limit <= 1000)
    {
        printf("\n");
    }

    double elapsed = ((double)(clock() - start)) / CLOCKS_PER_SEC;

    printf("Total prime numbers up to %lld: %lld\n", limit, count);
    printf("Execution time: %.6f seconds\n", elapsed);

    return 0;
}