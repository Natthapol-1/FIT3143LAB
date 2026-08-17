#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

int isPrime(long long num)
{
    if (num < 2 || num % 2 == 0)
        return 0;
    if (num == 2)
        return 1;

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

    char *end = NULL;
    long long limit = strtoll(argv[1], &end, 10);

    if (argv[1][0] == '\0' || *end != '\0' || limit < 2)
    {
        printf("Please enter a valid integer n >= 2.\n");
        return 1;
    }

    double start = omp_get_wtime();
    long long count = 0;
    long long *primes = malloc(((limit + 1) / 2) * sizeof(long long));
    long long found_count = 0;

    if (limit <= 1000)
    {
        printf("Prime numbers up to %lld:\n", limit);
    }

#pragma omp parallel for reduction(+ : count) schedule(dynamic, 1000)
    for (long long i = 2; i <= limit; i++)
    {
        if (isPrime(i))
        {
            count++;
            if (limit <= 1000)
            {

#pragma omp critical
                {
                    printf("%lld ", i);
                }
            }
            else
            {

#pragma omp critical
                {
                    primes[found_count++] = i;
                }
            }
        }
    }

    FILE *file = fopen("primes.txt", "w");
    if (file != NULL)
    {
        for (long long i = 0; i < found_count; i++)
        {
            fprintf(file, "%lld ", primes[i]);
        }
        fclose(file);
    }

    free(primes);

    if (limit <= 1000)
    {
        printf("\n");
    }

    double elapsed = omp_get_wtime() - start;

    printf("Total prime numbers up to %lld: %lld\n", limit, count);
    printf("Execution time: %.6f seconds\n", elapsed);

    return 0;
}