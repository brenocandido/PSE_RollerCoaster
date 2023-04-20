# PSE_RollerCoaster

This project intends to provide a solution to the Roller Coaster problem for multiple passengers and cars with multi-threading. This assumes each car has their own track, so all of them may run in parallel.

To compile the code, execute:
```
make all
```

To run it, use (this will compile the code as well):
```
make run
```

If you wish to delete the generated executable, run:
```
make clean
```

Application parameters may be passed as command line arguments to the executable or via the "ARGS" Makefile variable. They will be interpreted in the following order:

1. Number of passengers;
2. Number of roller coaster cars;
3. Roller coaster car capacity.

For example, running:
```
./main 9 2 3
```
or
```
make run ARGS="9 2 3"
```
will both configure the application for 9 passengers and 2 cars with capacity for three people each.

If no arguments are passed or there are invalid values, the program will run with pre-configured amounts.

Threads will run forever, so the any way to stop execution is to terminate the program. Ctrl+C should suffice.
