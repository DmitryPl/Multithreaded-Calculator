#include "Calculator.hpp"
#include "System.hpp"
#include <exception>
#include <string>

using std::exception;
using std::string;

int main()
{
    try
    {
        MPI_Init(NULL, NULL);

        int world_size;
        MPI_Comm_size(MPI_COMM_WORLD, &world_size);
        int world_rank;
        MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

        if (world_size < 1)
        {
            throw SystemException(__LINE__, __func__, "Error - world size < 3");
        }

        Calculator calc(world_rank, world_size);
        calc.Run();

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
