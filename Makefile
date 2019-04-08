MPI_CC = mpic++
MPI_RUNNER = mpirun
MPI_CFLAGS = -std=c++17

TARGET = calc

all: $(TARGET)

$(TARGET): main.cpp
	$(MPI_CC) $< $(MPI_CFLAGS) -o $@

run:
	$(MPI_RUNNER) -np $(n) $(TARGET)

clean:
	$(RM) $(TARGET)
