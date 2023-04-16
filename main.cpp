#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>

#include "RollerCoasterCar.h"
#include "Passenger.h"

int main()
{
    const int N_PASSENGERS = 10;
    const int N_CARS = 2;
    const int CAR_CAPACITY = 3;

    std::vector<Passenger *> passengers;
    std::vector<RollerCoasterCar *> cars;

    std::vector<std::thread> threadPassengers;
    std::vector<std::thread> threadCars;

    std::queue<Passenger *> passengerQueue;
    std::mutex muQueue;
    std::condition_variable cvQueue;

    std::mutex muTerminal;

    for (int i = 0; i < N_PASSENGERS; i++)
    {
        Passenger *p = new Passenger(i, passengerQueue, muQueue, cvQueue, muTerminal);
        passengers.push_back(p);
        threadPassengers.push_back(std::thread(&Passenger::thread, p));
    }

    for (int i = 0; i < CAR_CAPACITY; i++)
    {
        RollerCoasterCar *car = new RollerCoasterCar(i, CAR_CAPACITY, passengerQueue, muQueue, cvQueue, muTerminal);
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

    for (auto &p : passengers)
    {
        delete p;
    }

    for (auto &c : cars)
    {
        delete c;
    }

    return 0;
}