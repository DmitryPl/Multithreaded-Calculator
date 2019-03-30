#include "Calculator.hpp"
#include "Executor.hpp"
#include "System.hpp"
#include <exception>
#include <string>

using std::exception;
using std::string;

void master(int, int);
void slave(int, int);

int main()
{
    try
    {
        MPI_Init(NULL, NULL);

        int world_size;
        MPI_Comm_size(MPI_COMM_WORLD, &world_size);

        if (world_size < 3)
        {
            throw SystemException(__LINE__, __func__, "Error - world size < 3");
        }

        int world_rank;
        MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

        if (world_rank == 0)
        {
            master(world_rank, world_size);
        }

        else
        {
            slave(world_rank, world_size);
        }

        MPI_Finalize();
    }
    catch (const exception &exc)
    {
        print(exc.what());
    }
    catch (...)
    {
        print("Error - default\n");
    }
    return 0;
}

void master(int world_rank, int world_size)
{
    Calculator calc;
    calc.Run(world_rank, world_size);
}

void slave(int world_rank, int world_size)
{
    Executor exec(world_rank, world_size);
    exec.Run();
}
