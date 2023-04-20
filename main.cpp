#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <string>
#include <iostream>
#include <chrono>

#include "RollerCoasterCar.h"
#include "Passenger.h"

void tryArguments(int argc, char *argv[], int &nPassengers, int &nCars, int &carCapacity);

int main(int argc, char *argv[])
{
    int nPassengers;
    int nCars;
    int carCapacity;

    tryArguments(argc, argv, nPassengers, nCars, carCapacity);

    std::vector<Passenger *> passengers;
    std::vector<RollerCoasterCar *> cars;

    std::vector<std::thread> threadPassengers;
    std::vector<std::thread> threadCars;

    std::queue<Passenger *> passengerQueue;
    std::mutex muQueue;
    std::condition_variable cvQueue;

    std::mutex muTerminal;

    for (int i = 0; i < nPassengers; i++)
    {
        Passenger *p = new Passenger(i, passengerQueue, muQueue, cvQueue, muTerminal);
        passengers.push_back(p);
        threadPassengers.push_back(std::thread(&Passenger::thread, p));
    }

    for (int i = 0; i < nCars; i++)
    {
        RollerCoasterCar *car = new RollerCoasterCar(i, carCapacity, passengerQueue, muQueue, cvQueue, muTerminal);
        threadCars.push_back(std::thread(&RollerCoasterCar::thread, car));
        cars.push_back(car);
    }

    for (auto &p : threadPassengers)
    {
        if (p.joinable())
            p.join();
    }

    for (auto &c : threadCars)
    {
        if (c.joinable())
            c.join();
    } 

    // The following deletes are redundant, because the threads will run
    // forever.
    // Allocated memory will still be freed on process termination.

    for (auto &p : passengers)
        delete p;

    for (auto &c : cars)
        delete c;

    return 0;
}

void tryArguments(int argc, char *argv[], int &nPassengers, int &nCars, int &carCapacity)
{
    // .main + 3 arguments
    const int N_ARGS = 4;

    const int DEFAULT_N_PASSENGERS = 10;
    const int DEFAULT_N_CARS = 2;
    const int DEFAULT_CAR_CAPACITY = 3;

    nPassengers = DEFAULT_N_PASSENGERS;
    nCars = DEFAULT_N_CARS;
    carCapacity = DEFAULT_CAR_CAPACITY;

    bool validArgs = false;
    int args[N_ARGS - 1] = {0, 0, 0};

    if (argc == N_ARGS)
    {
        validArgs = true;

        for (int i = 0; i < N_ARGS - 1; i++)
        {
            try
            {
                // Must skip "./main" arg
                int arg = std::stoi(argv[i + 1]);

                if (arg <= 0)
                {
                    validArgs = false;
                    break;
                }

                args[i] = arg;
            }
            catch(const std::exception& e)
            {
                validArgs = false;
                break;
            }
        }
    }

    if (!validArgs)
    {
        std::cout << "Using default parameters!" << std::endl;
    }
    else
    {
        nPassengers = args[0];
        nCars = args[1];
        carCapacity = args[2];
    }

    std::cout   << "Passengers: "   << std::to_string(nPassengers) << std::endl
                << "Cars: "         << std::to_string(nCars) << std::endl
                << "Car capacity: " << std::to_string(carCapacity) << std::endl;

    // Dummy sleep simply so the user can see what was configured.
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}