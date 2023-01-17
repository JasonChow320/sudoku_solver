CC=gcc
CFLAGS=-I.
DEPS = sudoku_solver.h
OBJ = sudoku_solver.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

sudoku_solver: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm *.o sudoku_solver
