#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

    clock_t start = clock();
    long long count = 0;

    if (limit <= 1000)
    {
        printf("Prime numbers up to %lld:\n", limit);
    }

    for (long long i = 2; i <= limit; i++)
    {
        if (isPrime(i))
        {
            count++;
            if (limit <= 1000)
            {
                printf("%lld ", i);
            }
        }
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