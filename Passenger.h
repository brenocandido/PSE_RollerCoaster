#ifndef PASSENGER_H_
#define PASSENGER_H_

class Passenger
{
public:
    Passenger(int nRuns);

    void thread();

    void board();
    void unboard();

private:
    // Number of runs before passengers leaves the roller coaster
    const int _N_RUNS;
    int runCounter;
};

#endif // PASSENGER_H_