#include <stdio.h>
#include <math.h>

int isPrime(int num)
{
    if (num <= 1)
        return 0;
    for (int i = 2; i <= sqrt(num); i++)
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
        printf("Usage: %s <number>\n", argv[0]);
        return 1;
    }

    int k = atoi(argv[1]);
    for (int i = 0; i < k; i++)
    {
        if (isPrime(i))
        {
            if (k <= 100)
            {

                printf("%d ", i);
            }
            else
            {
                FILE *file = fopen("primes.txt", "a");
                if (file != NULL)
                {
                    fprintf(file, "%d ", i);
                    fclose(file);
                }
            }
        }
    }

    return 0;
}