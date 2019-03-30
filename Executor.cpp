#include "Executor.hpp"

void Executor::Run()
{
    try
    {
        double start_time = 0;
        start_time = MPI_Wtime();

        sleep(5);
        printFact();
        printf("SLAVE-%d >>> TIME:%f\n", world_rank, getTime(start_time));
    }
    catch (const exception &exc)
    {
        print(exc.what());
    }
    catch (...)
    {
        print("Error - default\n");
    }
}

long Executor::getFactorial(long n)
{
    long i = 0;
    if (factorials == nullptr)
    {
        throw SystemException(__LINE__, __func__, "Error - nullptr");
    }
    if (n <= 1)
    {
        factorials[n - 1] = i = 1;
    }
    else
    {
        factorials[n - 1] = i = n * getFactorial(n - 1);
    }
    return i;
}

void Executor::factorial(int n)
{
    if (n < 1)
    {
        throw SystemException(__LINE__, __func__, "Error - n < 1");
    }
    factorials = (ulong *)calloc(n, sizeof(ulong));
    if (factorials == nullptr)
    {
        throw SystemException(__LINE__, __func__, "Error - n < 1");
    }

    getFactorial(n);
}

void Executor::printFact()
{
    if (factorials != nullptr)
    {
        for (int i = 0; i < level; i++)
        {
            printf("%ld ", factorials[i]);
        }
        printf("\n");
    }
    else
        print("nullptr - factorials");
}

inline double Executor::getTime(double start)
{
    return MPI_Wtime() - start;
}