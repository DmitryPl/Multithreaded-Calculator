#include "System.hpp"

void print(const string &what)
{
	printf("%s\n", what.c_str());
}

void print(const char *what)
{
	printf("%s\n", what);
}

int doNothing()
{
	return success;
}

bool IsItNumber(const string &word)
{
	size_t i = 0;
	if (word[0] == '-')
	{
		i++;
	}
	while (word[i] != '\0')
	{
		if (!isdigit(word[i++]))
			return false;
	}
	return true;
}

void send(Message &msg, int rank, int size)
{
	int *comm = nullptr;
	double *data = nullptr;
	int first = msg.getFirst();
	double second = msg.getSecond();

	if (rank == 0)
	{
		data = (double *)malloc(size * sizeof(double));
		comm = (int *)malloc(size * sizeof(int));
		for (uint i = 0; i < size; i++)
		{
			comm[i] = first;
			data[i] = second;
		}

		if (DEBUG)
		{
			print("Sended commands");
			for (uint i = 0; i < size; i++)
			{
				printf("comm:%d data:%.16lf \n", comm[i], data[i]);
			}
		}
	}

	MPI_Scatter(comm, 1, MPI_INT, &first, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(data, 1, MPI_DOUBLE, &second, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	if (rank == 0)
	{
		free(comm);
		free(data);
	}

	msg.set(first, second);
}

void get(Message &msg, int rank, int size)
{
	int *comm = nullptr;
	double *data = nullptr;
	int first = msg.getFirst();
	double second = msg.getSecond();

	if (DEBUG)
	{
		printf("Sended >> comm:%d data:%.16lf by %d\n", first, second, rank);
	}

	if (rank == 0)
	{
		msg.setData(
			comm = (int *)malloc(size * sizeof(int)),
			data = (double *)malloc(size * sizeof(double)));
	}
	MPI_Gather(&first, 1, MPI_INT, comm, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Gather(&second, 1, MPI_DOUBLE, data, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
}